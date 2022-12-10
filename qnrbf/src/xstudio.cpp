#include "xstudio.h"

#include <memory>

#include "QNrbfStream.h"

static void free_node_list(xs_node *list, void(Cleaner)(void *)) {
    auto p = list;
    while (p) {
        // Free data
        if (p->data) {
            Cleaner(p->data);
        }

        // Free node
        auto node = p;
        p = p->next;
        free(node);
    }
}

static void copy_string(const QByteArray &src, xs_string *dst) {
    int size = src.size();
    dst->size = size;
    if (size == 0) {
        dst->str = nullptr;
    } else {
        dst->str = (char *) qnrbf_malloc(size);
        qnrbf_memcpy(dst->str, src.data(), size);
    }
}

static void copy_string(const QString &src, xs_string *dst) {
    return copy_string(src.toUtf8(), dst);
}

inline static QByteArray from_xs_string_bytes(const xs_string &str) {
    return QByteArray(str.str, str.size);
}

inline static QString from_xs_string(const xs_string &str) {
    return QString::fromUtf8(str.str, str.size);
}

static void free_string(xs_string *str) {
    if (!str->str) {
        return;
    }
    qnrbf_free(str->str);
    str->size = 0;
    str->str = nullptr;
}

static void free_model(xs_app_model *model) {
    // Free ProjectFilePath
    if (model->ProjectFilePath.str) {
        free(model->ProjectFilePath.str);
    }

    // Free tempoList
    free_node_list(model->tempoList, free);

    // Free beatList
    free_node_list(model->beatList, free);

    auto track_cleaner = [](void *data) {
        auto track = reinterpret_cast<xs_track *>(data);
        if (track->track_type == SINGING) {
            // Free singingTrack
            auto singingTrack = reinterpret_cast<xs_singing_track *>(track);

            auto note_cleaner = [](void *data) {
                auto note = reinterpret_cast<xs_note *>(data);
                if (note->NotePhoneInfo) {
                    free(note->NotePhoneInfo);
                }
                if (note->Vibrato) {
                    // Free vibrato params
                    free_node_list(note->Vibrato->ampLine, free);
                    free_node_list(note->Vibrato->freqLine, free);
                    free(note->Vibrato);
                }
                if (note->VibratoPercentInfo) {
                    free(note->VibratoPercentInfo);
                }
                if (note->lyric.str) {
                    free(note->lyric.str);
                }
                if (note->pronouncing.str) {
                    free(note->pronouncing.str);
                }

                // Free note
                free(note);
            };

            // Free noteList
            free_node_list(singingTrack->noteList, note_cleaner);

            // Free track params
            free_node_list(singingTrack->editedPitchLine, free);
            free_node_list(singingTrack->editedVolumeLine, free);
            free_node_list(singingTrack->editedBreathLine, free);
            free_node_list(singingTrack->editedGenderLine, free);
            free_node_list(singingTrack->editedPowerLine, free);
        } else {
            // Free instrumentTrack
            auto instrumentTrack = reinterpret_cast<xs_instrument_track *>(track);
            if (instrumentTrack->InstrumentFilePath.str) {
                free(instrumentTrack->InstrumentFilePath.str);
            }
        }

        // Free track base
        if (track->name.str) {
            free(track->name.str);
        }

        // Free track
        free(track);
    };

    // Free trackList
    free_node_list(model->trackList, track_cleaner);

    // Free app model
    free(model);
}

void *qnrbf_malloc(int size) {
    auto ptr = malloc(size);
    memset(ptr, 0, size);
    return ptr;
}

void qnrbf_free(void *data) {
    free(data);
}

void qnrbf_memcpy(void *dst, const void *src, int count) {
    memcpy(dst, src, count);
}

void qnrbf_memset(void *dst, int value, int count) {
    memset(dst, value, count);
}

qnrbf_xstudio_context *qnrbf_xstudio_alloc_context() {
    auto ctx = reinterpret_cast<qnrbf_xstudio_context *>(malloc(sizeof(qnrbf_xstudio_context)));
    ctx->status = OK;
    ctx->error = {nullptr, 0};
    ctx->buf = {nullptr, 0};
    ctx->model = nullptr;
    return ctx;
}

void qnrbf_xstudio_free_context(qnrbf_xstudio_context *ctx) {
    if (ctx->error.str) {
        free(ctx->error.str);
    }
    if (ctx->buf.str) {
        free(ctx->buf.str);
    }
    if (ctx->model) {
        free_model(ctx->model);
    }

    // Free context
    free(ctx);
}

struct MessageHandler {
    QByteArray error;
};

static MessageHandler *hError = nullptr;

static void customMessageHandler(QtMsgType type, const QMessageLogContext &context,
                                 const QString &msg) {
    hError->error.append(msg.toUtf8());
}

void qnrbf_xstudio_read(qnrbf_xstudio_context *ctx) {
    // Filter debug output
    hError = new MessageHandler();
    auto orgHandler = qInstallMessageHandler(customMessageHandler);

    // Get buf
    QByteArray data(ctx->buf.str, ctx->buf.size);

    // Remove buf
    free_string(&ctx->buf);
    free_string(&ctx->error);
    if (ctx->model) {
        free_model(ctx->model);
        ctx->model = nullptr;
    }

    // Read
    QNrbfStream in(&data, QIODevice::ReadOnly);
    QNrbf::XSAppModel model;
    in >> model;

    // Restore debug output
    qInstallMessageHandler(orgHandler);

    // Set status
    ctx->status = (qnrbf_stream_status) in.status();

    if (in.status() != QDataStream::Ok) {
        // Set error message
        copy_string(hError->error, &ctx->error);
    } else {
        // Set output data
        auto resModel = (xs_app_model *) qnrbf_malloc(sizeof(xs_app_model));

        copy_string(model.ProjectFilePath, &resModel->ProjectFilePath);
        resModel->quantize = model.quantize;
        resModel->isTriplet = model.isTriplet;
        resModel->isNumericalKeyName = model.isNumericalKeyName;
        resModel->firstNumericalKeyNameAtIndex = model.firstNumericalKeyNameAtIndex;

        // Copy tempo list
        {
            auto &head = resModel->tempoList;
            head = nullptr;
            auto p = head;
            for (const auto &item : qAsConst(model.tempoList)) {
                auto tempo = (xs_song_tempo *) qnrbf_malloc(sizeof(xs_song_tempo));
                tempo->base.Overlapped = item.Overlapped;
                tempo->pos = item.pos;
                tempo->tempo = item.tempo;

                auto node = (xs_node *) qnrbf_malloc(sizeof(xs_node));
                node->data = tempo;
                node->next = nullptr;

                // Create node
                if (p) {
                    p->next = node;
                } else {
                    head = node;
                }
                p = node;
            }
        }

        // Copy beat list
        {
            auto &head = resModel->beatList;
            head = nullptr;
            auto p = head;
            for (const auto &item : qAsConst(model.beatList)) {
                auto beat = (xs_song_beat *) qnrbf_malloc(sizeof(xs_song_beat));
                beat->base.Overlapped = item.Overlapped;
                beat->barIndex = item.barIndex;
                beat->beatSize.x = item.beatSize.x;
                beat->beatSize.y = item.beatSize.y;

                auto node = (xs_node *) qnrbf_malloc(sizeof(xs_node));
                node->data = beat;
                node->next = nullptr;

                // Create node
                if (p) {
                    p->next = node;
                } else {
                    head = node;
                }
                p = node;
            }
        }

        // Copy track list
        {
            auto copy_track_base = [](const QNrbf::XSITrack &in, xs_track *out) {
                out->volume = in.volume;
                out->pan = in.pan;
                out->mute = in.mute;
                out->solo = in.solo;
                copy_string(in.name, &out->name);
            };

            auto create_line_param_list = [](const QNrbf::XSLineParam &lineParam) -> xs_node * {
                xs_node *head = nullptr;
                auto p = head;

                for (const auto &item : lineParam.nodeLinkedList) {
                    auto param = (xs_line_param *) qnrbf_malloc(sizeof(xs_line_param));
                    param->Pos = item.Pos;
                    param->Value = item.Value;

                    // Create node
                    auto node = (xs_node *) qnrbf_malloc(sizeof(xs_node));
                    node->data = param;
                    node->next = nullptr;

                    if (p) {
                        p->next = node;
                    } else {
                        head = node;
                    }
                    p = node;
                }

                return head;
            };

            auto &head = resModel->trackList;
            head = nullptr;
            auto p = head;
            for (const auto &item : qAsConst(model.trackList)) {
                void *trackPtr;
                if (item->type() == QNrbf::XSITrack::Singing) {
                    auto singingTrack = dynamic_cast<QNrbf::XSSingingTrack *>(item.data());

                    auto track = (xs_singing_track *) qnrbf_malloc(sizeof(xs_singing_track));

                    // Copy base
                    track->base.track_type = SINGING;
                    copy_track_base(*singingTrack, &track->base);

                    copy_string(singingTrack->AISingerId, &track->AISingerId);
                    track->needRefreshBaseMetadataFlag = singingTrack->needRefreshBaseMetadataFlag;
                    track->reverbPreset = singingTrack->reverbPreset;

                    // Copy track params
                    track->editedPitchLine =
                        singingTrack->editedPitchLine.isNull()
                            ? nullptr
                            : create_line_param_list(*singingTrack->editedPitchLine.data());
                    track->editedVolumeLine =
                        singingTrack->editedVolumeLine.isNull()
                            ? nullptr
                            : create_line_param_list(*singingTrack->editedVolumeLine.data());
                    track->editedBreathLine =
                        singingTrack->editedBreathLine.isNull()
                            ? nullptr
                            : create_line_param_list(*singingTrack->editedBreathLine.data());
                    track->editedGenderLine =
                        singingTrack->editedGenderLine.isNull()
                            ? nullptr
                            : create_line_param_list(*singingTrack->editedGenderLine.data());
                    track->editedPowerLine =
                        singingTrack->editedPowerLine.isNull()
                            ? nullptr
                            : create_line_param_list(*singingTrack->editedPowerLine.data());

                    // Copy note list
                    {
                        auto &head2 = track->noteList = nullptr;
                        auto p2 = head2;
                        for (const auto &noteItem : qAsConst(singingTrack->noteList)) {
                            auto note = (xs_note *) qnrbf_malloc(sizeof(xs_note));
                            note->base.Overlapped = noteItem.Overlapped;
                            note->VibratoPercent = noteItem.VibratoPercent;
                            note->startPos = noteItem.startPos;
                            note->widthPos = noteItem.widthPos;
                            note->keyIndex = noteItem.keyIndex;
                            copy_string(noteItem.lyric, &note->lyric);
                            copy_string(noteItem.pronouncing, &note->pronouncing);
                            note->headTag = (xs_note_head_tag) noteItem.headTag;

                            // Copy NotePhoneInfo
                            if (!noteItem.NotePhoneInfo.isNull()) {
                                const auto &org = *noteItem.NotePhoneInfo.data();
                                auto info =
                                    (xs_note_phone_info *) qnrbf_malloc(sizeof(xs_note_phone_info));

                                info->HeadPhoneTimeInSec = org.HeadPhoneTimeInSec;
                                info->MidPartOverTailPartRatio = org.MidPartOverTailPartRatio;

                                note->NotePhoneInfo = info;
                            } else {
                                note->NotePhoneInfo = nullptr;
                            }

                            // Copy VibratoStyle
                            if (!noteItem.Vibrato.isNull()) {
                                const auto &org = *noteItem.Vibrato.data();
                                auto vibrato =
                                    (xs_vibrato_style *) qnrbf_malloc(sizeof(xs_vibrato_style));

                                vibrato->IsAntiPhase = org.IsAntiPhase;
                                vibrato->ampLine =
                                    org.ampLine.isNull()
                                        ? nullptr
                                        : create_line_param_list(*org.ampLine.data());
                                vibrato->freqLine =
                                    org.freqLine.isNull()
                                        ? nullptr
                                        : create_line_param_list(*org.freqLine.data());

                                note->Vibrato = vibrato;
                            } else {
                                note->Vibrato = nullptr;
                            }

                            // Copy VibratoPercentInfo
                            if (!noteItem.VibratoPercentInfo.isNull()) {
                                const auto &org = *noteItem.VibratoPercentInfo.data();
                                auto info = (xs_vibrato_percent_info *) qnrbf_malloc(
                                    sizeof(xs_vibrato_percent_info));

                                info->startPercent = org.startPercent;
                                info->endPercent = org.endPercent;

                                note->VibratoPercentInfo = info;
                            } else {
                                note->VibratoPercentInfo = nullptr;
                            }

                            // Create node
                            auto node = (xs_node *) qnrbf_malloc(sizeof(xs_node));
                            node->data = note;
                            node->next = nullptr;

                            if (p2) {
                                p2->next = node;
                            } else {
                                head2 = node;
                            }
                            p2 = node;
                        }
                    }

                    trackPtr = track;
                } else {
                    auto instrumentTrack = dynamic_cast<QNrbf::XSInstrumentTrack *>(item.data());

                    auto track = (xs_instrument_track *) qnrbf_malloc(sizeof(xs_instrument_track));

                    // Copy base
                    track->base.track_type = INSTRUMENT;
                    copy_track_base(*instrumentTrack, &track->base);

                    track->SampleRate = instrumentTrack->SampleRate;
                    track->SampleCount = instrumentTrack->SampleCount;
                    track->ChannelCount = instrumentTrack->ChannelCount;
                    track->OffsetInPos = instrumentTrack->OffsetInPos;
                    copy_string(instrumentTrack->InstrumentFilePath, &track->InstrumentFilePath);

                    trackPtr = track;
                }

                // Create node
                auto node = (xs_node *) qnrbf_malloc(sizeof(xs_node));
                node->data = trackPtr;
                node->next = nullptr;

                if (p) {
                    p->next = node;
                } else {
                    head = node;
                }
                p = node;
            }
        }

        ctx->model = resModel;
    }

    delete hError;
    hError = nullptr;
}

void qnrbf_xstudio_write(qnrbf_xstudio_context *ctx) {
    // Get input model
    QNrbf::XSAppModel model;

    const auto &inModel = ctx->model;
    model.ProjectFilePath = from_xs_string(ctx->model->ProjectFilePath);
    model.quantize = inModel->quantize;
    model.isTriplet = inModel->isTriplet;
    model.isNumericalKeyName = inModel->isNumericalKeyName;
    model.firstNumericalKeyNameAtIndex = inModel->firstNumericalKeyNameAtIndex;

    // Copy tempo list
    {
        auto node = inModel->tempoList;
        while (node) {
            auto inTempo = (xs_song_tempo *) node->data;

            QNrbf::XSSongTempo tempo;
            tempo.Overlapped = inTempo->base.Overlapped;
            tempo.pos = inTempo->pos;
            tempo.tempo = inTempo->tempo;

            model.tempoList.append(tempo);

            node = node->next;
        }
    }

    // Copy beat list
    {
        auto node = inModel->beatList;
        while (node) {
            auto inBeat = (xs_song_beat *) node->data;

            QNrbf::XSSongBeat beat;
            beat.Overlapped = inBeat->base.Overlapped;
            beat.barIndex = inBeat->barIndex;
            beat.beatSize = QNrbf::XSBeatSize(inBeat->beatSize.x, inBeat->beatSize.y);

            model.beatList.append(beat);

            node = node->next;
        }
    }

    // Copy track list
    {
        auto createLineParam = [](xs_node *lineParams) -> QNrbf::XSLineParam {
            QNrbf::XSLineParam res;

            auto node = lineParams;
            while (node) {
                auto inParam = (xs_line_param *) node->data;
                res.nodeLinkedList.emplace_back(inParam->Pos, inParam->Value);
                node = node->next;
            };

            return res;
        };

        auto node = inModel->trackList;
        while (node) {
            QSharedPointer<QNrbf::XSITrack> track;
            auto baseTrack = (xs_track *) node->data;

            if (baseTrack->track_type == SINGING) {
                auto inTrack = (xs_singing_track *) baseTrack;
                auto singingTrack = QSharedPointer<QNrbf::XSSingingTrack>::create();
                singingTrack->AISingerId = from_xs_string(inTrack->AISingerId);
                singingTrack->needRefreshBaseMetadataFlag = inTrack->needRefreshBaseMetadataFlag;
                singingTrack->reverbPreset = (QNrbf::XSReverbPreset) inTrack->reverbPreset;

                // Copy params
                if (inTrack->editedPitchLine) {
                    singingTrack->editedPitchLine = QSharedPointer<QNrbf::XSLineParam>::create(
                        createLineParam(inTrack->editedPitchLine));
                }
                if (inTrack->editedVolumeLine) {
                    singingTrack->editedVolumeLine = QSharedPointer<QNrbf::XSLineParam>::create(
                        createLineParam(inTrack->editedVolumeLine));
                }
                if (inTrack->editedBreathLine) {
                    singingTrack->editedBreathLine = QSharedPointer<QNrbf::XSLineParam>::create(
                        createLineParam(inTrack->editedBreathLine));
                }
                if (inTrack->editedGenderLine) {
                    singingTrack->editedGenderLine = QSharedPointer<QNrbf::XSLineParam>::create(
                        createLineParam(inTrack->editedGenderLine));
                }
                if (inTrack->editedPowerLine) {
                    singingTrack->editedPowerLine = QSharedPointer<QNrbf::XSLineParam>::create(
                        createLineParam(inTrack->editedPowerLine));
                }

                // Copy notes
                {
                    auto noteNode = inTrack->noteList;
                    while (noteNode) {
                        auto inNote = (xs_note *) noteNode->data;

                        QNrbf::XSNote note;
                        note.Overlapped = inNote->base.Overlapped;
                        note.VibratoPercent = inNote->VibratoPercent;
                        note.startPos = inNote->startPos;
                        note.widthPos = inNote->widthPos;
                        note.keyIndex = inNote->keyIndex;
                        note.lyric = from_xs_string(inNote->lyric);
                        note.pronouncing = from_xs_string(inNote->pronouncing);
                        note.headTag = (QNrbf::XSNoteHeadTag) inNote->headTag;

                        // Copy NotePhoneInfo
                        if (inNote->NotePhoneInfo) {
                            note.NotePhoneInfo = QSharedPointer<QNrbf::XSNotePhoneInfo>::create(
                                inNote->NotePhoneInfo->HeadPhoneTimeInSec,
                                inNote->NotePhoneInfo->MidPartOverTailPartRatio);
                        }

                        // Copy Vibrato
                        if (inNote->Vibrato) {
                            const auto &inVibrato = inNote->Vibrato;

                            QNrbf::XSVibratoStyle vibrato;
                            vibrato.IsAntiPhase = inVibrato->IsAntiPhase;
                            if (inVibrato->ampLine)
                                vibrato.ampLine = QSharedPointer<QNrbf::XSLineParam>::create(
                                    createLineParam(inVibrato->ampLine));
                            if (inVibrato->freqLine)
                                vibrato.freqLine = QSharedPointer<QNrbf::XSLineParam>::create(
                                    createLineParam(inVibrato->freqLine));
                        }

                        // Copy VibratoPercentInfo
                        if (inNote->VibratoPercentInfo) {
                            note.VibratoPercentInfo =
                                QSharedPointer<QNrbf::XSVibratoPercentInfo>::create(
                                    inNote->VibratoPercentInfo->startPercent,
                                    inNote->VibratoPercentInfo->endPercent);
                        }

                        singingTrack->noteList.append(note);

                        noteNode = noteNode->next;
                    }
                }

                track = singingTrack;
            } else {
                auto inTrack = (xs_instrument_track *) baseTrack;
                auto instrumentTrack = QSharedPointer<QNrbf::XSInstrumentTrack>::create();

                instrumentTrack->SampleRate = inTrack->SampleRate;
                instrumentTrack->SampleCount = inTrack->SampleCount;
                instrumentTrack->ChannelCount = inTrack->ChannelCount;
                instrumentTrack->OffsetInPos = inTrack->OffsetInPos;
                instrumentTrack->InstrumentFilePath = from_xs_string(inTrack->InstrumentFilePath);

                track = instrumentTrack;
            }

            // Copy track base
            track->volume = baseTrack->volume;
            track->pan = baseTrack->pan;
            track->name = from_xs_string(baseTrack->name);
            track->mute = baseTrack->mute;
            track->solo = baseTrack->solo;

            model.trackList.append(track);

            node = node->next;
        }
    }

    // Filter debug output
    hError = new MessageHandler();
    auto orgHandler = qInstallMessageHandler(customMessageHandler);

    // Remove buf
    free_string(&ctx->buf);
    free_string(&ctx->error);
    free_model(ctx->model);
    ctx->model = nullptr;

    // Write
    QByteArray data;
    QNrbfStream out(&data, QIODevice::WriteOnly);
    out << model;

    // Restore debug output
    qInstallMessageHandler(orgHandler);

    // Set status
    ctx->status = (qnrbf_stream_status) out.status();

    if (out.status() != QDataStream::Ok) {
        // Set error message
        copy_string(hError->error, &ctx->error);
    } else {
        // Set output data
        copy_string(data, &ctx->buf);
    }

    delete hError;
    hError = nullptr;
}