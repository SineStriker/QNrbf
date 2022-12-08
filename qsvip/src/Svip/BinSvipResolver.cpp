#include "Svip/BinSvipResolver.h"

#include "SvipUtils.h"

#include "Utils/ListUtils.h"
#include "Utils/TimeSynchronizer.h"

using Tempo = QSvipModel::SongTempo;
using TimeSig = QSvipModel::TimeSignature;

static double DEFAULT_TEMPO = 120;

bool BinSvipResolver::load(const QNrbf::XSAppModel &in, QSvipModel &out,
                           const QVariantMap &options) {
    auto decodeParam = [](const QNrbf::XSLineParam &param,
                          int(op)(int) = nullptr) -> QSvipModel::ParamCurve {
        QSvipModel::ParamCurve res;
        for (const auto &item : param.nodeLinkedList) {
            res.PointList.append(qMakePair(item.Pos, op ? op(item.Value) : item.Value));
        }
        return res;
    };

    // Find version
    QString version;
    {
        auto it = options.find("version");
        if (it == options.end() || it.value().type() != QVariant::String) {
            qDebug().noquote() << "BinSvipResolver: Missing version information";
            return false;
        }
        version = it.value().toString();
    }

    QSvipModel proj;
    proj.Version = version;

    // Convert tempos
    for (const auto &item : in.tempoList) {
        proj.SongTempoList.append(Tempo{item.pos, double(item.tempo) / 100});
    }

    // Convert beats
    for (const auto &item : in.beatList) {
        proj.TimeSignatureList.append(TimeSig{item.barIndex, item.beatSize.x, item.beatSize.y});
    }

    // Convert tracks
    for (const auto &item : in.trackList) {
        if (item.isNull()) {
            continue;
        }

        QSharedPointer<QSvipModel::Track> resTrack;
        if (item->type() == QNrbf::XSITrack::Singing) {
            // Convert singing tracks
            auto singingTrack = item.dynamicCast<QNrbf::XSSingingTrack>();

            QSvipModel::SingingTrack track;
            track.AISingerName =
                (singingTrack->AISingerId.isEmpty())
                    ? QString()
                    : QString("$(%1)").arg(singingTrack->AISingerId); // Use escape syntax
            track.ReverbPreset = SvipUtils::reverbPresets_index2Name(singingTrack->reverbPreset);

            for (const auto &noteItem : qAsConst(singingTrack->noteList)) {
                // Convert notes
                QSvipModel::Note note;

                note.StartPos = noteItem.startPos;
                note.Length = noteItem.widthPos;
                note.KeyNumber = noteItem.keyIndex - 12;
                note.HeadTag = SvipUtils::noteHeadTag_index2Name(noteItem.headTag);
                note.Lyric = noteItem.lyric;
                note.Pronunciation = noteItem.pronouncing;

                if (!noteItem.NotePhoneInfo.isNull()) {
                    // Convert phoneme info
                    QSvipModel::Phones ph;

                    ph.HeadLengthInSecs = noteItem.NotePhoneInfo->HeadPhoneTimeInSec;
                    ph.MidRatioOverTail = noteItem.NotePhoneInfo->MidPartOverTailPartRatio;

                    note.EditedPhones = QSharedPointer<QSvipModel::Phones>::create(std::move(ph));
                }

                if (!noteItem.Vibrato.isNull()) {
                    // Convert vibrato
                    QSvipModel::Vibrato vibrato;

                    if (!noteItem.VibratoPercentInfo.isNull()) {
                        vibrato.StartPercent = noteItem.VibratoPercentInfo->startPercent;
                        vibrato.EndPercent = noteItem.VibratoPercentInfo->endPercent;
                    } else if (noteItem.VibratoPercent > 0) {
                        vibrato.StartPercent = 1.0 - double(noteItem.VibratoPercent) / 100;
                        vibrato.EndPercent = 1.0;
                    }

                    vibrato.IsAntiPhase = noteItem.Vibrato->IsAntiPhase;
                    if (!noteItem.Vibrato->ampLine.isNull()) {
                        vibrato.Amplitude = decodeParam(*noteItem.Vibrato->ampLine);
                    }
                    if (!noteItem.Vibrato->freqLine.isNull()) {
                        vibrato.Frequency = decodeParam(*noteItem.Vibrato->freqLine);
                    }

                    note.Vibrato = QSharedPointer<QSvipModel::Vibrato>::create(std::move(vibrato));
                }

                track.NoteList.append(note);
            }

            // Convert track params
            if (!singingTrack->editedBreathLine.isNull()) {
                track.EditedParams.Breath = decodeParam(*singingTrack->editedBreathLine);
            }
            if (!singingTrack->editedGenderLine.isNull()) {
                track.EditedParams.Gender = decodeParam(*singingTrack->editedGenderLine);
            }
            if (!singingTrack->editedPitchLine.isNull()) {
                auto op = [](int x) -> int { return x > 1050 ? x - 1150 : -100; };
                track.EditedParams.Pitch = decodeParam(*singingTrack->editedPitchLine, op);
            }
            if (!singingTrack->editedVolumeLine.isNull()) {
                track.EditedParams.Volume = decodeParam(*singingTrack->editedVolumeLine);
            }
            if (!singingTrack->editedPowerLine.isNull()) {
                track.EditedParams.Strength = decodeParam(*singingTrack->editedPowerLine);
            }

            resTrack = QSharedPointer<QSvipModel::SingingTrack>::create(std::move(track));
        } else {
            // Convert instrument tracks
            auto instrumentTrack = dynamic_cast<QNrbf::XSInstrumentTrack *>(item.data());

            QSvipModel::InstrumentalTrack track;
            track.AudioFilePath = instrumentTrack->InstrumentFilePath;
            track.Offset = instrumentTrack->OffsetInPos;

            resTrack = QSharedPointer<QSvipModel::InstrumentalTrack>::create(std::move(track));
        }

        // Convert base information
        resTrack->Title = item->name;
        resTrack->Mute = item->mute;
        resTrack->Solo = item->solo;
        resTrack->Volume = item->volume;
        resTrack->Pan = item->pan;

        proj.TrackList.append(resTrack);
    }

    out = proj;

    return true;
}

bool BinSvipResolver::save(const QSvipModel &in, QNrbf::XSAppModel &out,
                           const QVariantMap &options) {

    if (in.TimeSignatureList.isEmpty()) {
        qDebug() << "SvipUtils: Empty time signature list";
        return false;
    }

    const TimeSig &firstTimeSig = in.TimeSignatureList.front();

    qint32 firstBarTick =
        qRound(1920.0 * double(firstTimeSig.Numerator) / firstTimeSig.Denominator);
    QList<Tempo> firstBarTempo = ListUtils::Where(
        in.SongTempoList, [&](const Tempo &tempo) { return tempo.Position < firstBarTick; });
    bool isAbsoluteTimeMode = ListUtils::Any(
        in.SongTempoList, [&](const Tempo &tempo) { return tempo.BPM < 20 || tempo.BPM > 300; });

    TimeSynchronizer sync(in.SongTempoList, firstBarTick, isAbsoluteTimeMode, DEFAULT_TEMPO);

    QNrbf::XSAppModel model;

    return false;
}
