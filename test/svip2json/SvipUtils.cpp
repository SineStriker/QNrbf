#include "SvipUtils.h"

bool SvipUtils::bin2Json(const QNrbf::XSAppModel &in, const QString &version, QSvipModel &out) {
    auto decodeParam = [](const QNrbf::XSLineParam &param,
                          int(op)(int) = nullptr) -> QSvipModel::ParamCurve {
        QSvipModel::ParamCurve res;
        for (const auto &item : param._nodeLinkedList) {
            res.PointList.append(qMakePair(item.Pos, op ? op(item.Value) : item.Value));
        }
        return res;
    };

    QSvipModel proj;
    proj.Version = version;

    // Convert tempos
    for (const auto &item : in.tempoList) {
        proj.SongTempoList.append(QSvipModel::SongTempo{item.pos, double(item.tempo) / 100});
    }

    // Convert beats
    for (const auto &item : in.beatList) {
        proj.TimeSignatureList.append(
            QSvipModel::TimeSignature{item.barIndex, item.beatSize.x, item.beatSize.y});
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
            track.AISingerName = singingTrack->AISingerId;
            track.ReverbPreset = reverbPresets_index2Name(singingTrack->reverbPreset);

            for (const auto &noteItem : qAsConst(singingTrack->noteList)) {
                // Convert notes
                QSvipModel::Note note;

                note.StartPos = noteItem.startPos;
                note.Length = noteItem.widthPos;
                note.KeyNumber = noteItem.keyIndex - 12;
                note.HeadTag = noteHeadTag_index2Name(noteItem.headTag);
                note.Lyric = noteItem.lyric;
                note.Pronunciation = noteItem.pronouncing;

                if (!noteItem.NotePhoneInfo.isNull()) {
                    // Convert phoneme info
                    QSvipModel::Phones ph;

                    ph.HeadLengthInSecs = noteItem.NotePhoneInfo->HeadPhoneTimeInSec;
                    ph.MidRatioOverTail = noteItem.NotePhoneInfo->MidPartOverTailPartRatio;

                    note.EditedPhones = QSharedPointer<QSvipModel::Phones>::create(ph);
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
                        vibrato.Amplitude = QSharedPointer<QSvipModel::ParamCurve>::create(
                            decodeParam(*noteItem.Vibrato->ampLine));
                    }
                    if (!noteItem.Vibrato->freqLine.isNull()) {
                        vibrato.Frequency = QSharedPointer<QSvipModel::ParamCurve>::create(
                            decodeParam(*noteItem.Vibrato->freqLine));
                    }

                    note.Vibrato = QSharedPointer<QSvipModel::Vibrato>::create(std::move(vibrato));
                }

                track.NoteList.append(note);
            }

            // Convert track params
            if (!singingTrack->editedBreathLine.isNull()) {
                track.EditedParams.Breath = QSharedPointer<QSvipModel::ParamCurve>::create(
                    decodeParam(*singingTrack->editedBreathLine));
            }
            if (!singingTrack->editedGenderLine.isNull()) {
                track.EditedParams.Gender = QSharedPointer<QSvipModel::ParamCurve>::create(
                    decodeParam(*singingTrack->editedGenderLine));
            }
            if (!singingTrack->editedPitchLine.isNull()) {
                auto op = [](int x) -> int { return x > 1050 ? x - 1150 : -100; };
                track.EditedParams.Pitch = QSharedPointer<QSvipModel::ParamCurve>::create(
                    decodeParam(*singingTrack->editedPitchLine, op));
            }
            if (!singingTrack->editedVolumeLine.isNull()) {
                track.EditedParams.Volume = QSharedPointer<QSvipModel::ParamCurve>::create(
                    decodeParam(*singingTrack->editedVolumeLine));
            }
            if (!singingTrack->editedPowerLine.isNull()) {
                track.EditedParams.Strength = QSharedPointer<QSvipModel::ParamCurve>::create(
                    decodeParam(*singingTrack->editedPowerLine));
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

bool SvipUtils::json2Bin(const QSvipModel &in, QNrbf::XSAppModel &out) {
    return false;
}

QNrbf::XSReverbPreset SvipUtils::reverbPresets_name2Index(const QString &name) {
    return QNrbf::DEFAULT;
}

QString SvipUtils::reverbPresets_index2Name(QNrbf::XSReverbPreset index) {
    QString res;
    switch (index) {
        case QNrbf::XSReverbPreset::NONE:
            res = "None";
            break;
        case QNrbf::XSReverbPreset::DEFAULT:
            res = "FloatingLight";
            break;
        case QNrbf::XSReverbPreset::SMALLHALL1:
            res = "Afternoon";
            break;
        case QNrbf::XSReverbPreset::MEDIUMHALL1:
            res = "Moonlight";
            break;
        case QNrbf::XSReverbPreset::LARGEHALL1:
            res = "Cristal";
            break;
        case QNrbf::XSReverbPreset::SMALLROOM1:
            res = "Soda";
            break;
        case QNrbf::XSReverbPreset::MEDIUMROOM1:
            res = "Nightingale";
            break;
        case QNrbf::XSReverbPreset::LONGREVERB2:
            res = "BigDream";
            break;
        default:
            break;
    }
    return res;
}

QNrbf::XSNoteHeadTag SvipUtils::noteHeadTag_name2Index(const QString &name) {
    QNrbf::XSNoteHeadTag res = QNrbf::XSNoteHeadTag::NoTag;
    if (name == "0") {
        res = QNrbf::XSNoteHeadTag::SilTag;
    } else if (name == "V") {
        res = QNrbf::XSNoteHeadTag::SpTag;
    }
    return res;
}

QString SvipUtils::noteHeadTag_index2Name(QNrbf::XSNoteHeadTag index) {
    QString res;
    switch (index) {
        case QNrbf::XSNoteHeadTag::SilTag:
            res = "0";
            break;
        case QNrbf::XSNoteHeadTag::SpTag:
            res = "V";
            break;
        default:
            break;
    }
    return res;
}
