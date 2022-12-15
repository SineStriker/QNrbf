#include "xstudio.h"

#include <QMutex>
#include <QThread>

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

static void free_model(xs_app_model **model_ptr) {
    if (*model_ptr == nullptr) {
        return;
    }
    auto &model = *model_ptr;

    // Free ProjectFilePath
    free_string(&model->ProjectFilePath);

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
                    auto vibrato = note->Vibrato;

                    // Free vibrato params
                    free_node_list(vibrato->ampLine, free);
                    free_node_list(vibrato->freqLine, free);

                    // Free vibrato
                    free(vibrato);
                }
                if (note->VibratoPercentInfo) {
                    free(note->VibratoPercentInfo);
                }
                free_string(&note->lyric);
                free_string(&note->pronouncing);

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
            free_string(&instrumentTrack->InstrumentFilePath);
        }

        // Free track base
        free_string(&track->name);

        // Free track
        free(track);
    };

    // Free trackList
    free_node_list(model->trackList, track_cleaner);

    // Free app model
    free(model);

    model = nullptr;
}

static void free_context_content(qnrbf_xstudio_context *ctx) {
    free_string(&ctx->error);
    free_string(&ctx->buf);
    free_model(&ctx->model);
}

struct MessageHandler {
private:
    QHash<void *, QByteArray> errors;
    QMutex lock;
    QtMessageHandler orgHandler;

    static MessageHandler *global;

    static void Handler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
        QMutexLocker guard(&global->lock);
        global->errors[QThread::currentThreadId()].append(msg.toUtf8());
    }

    MessageHandler() : orgHandler(nullptr) {
        orgHandler = qInstallMessageHandler(MessageHandler::Handler);
    }

    ~MessageHandler() {
        qInstallMessageHandler(orgHandler);
    }

public:
    static void create() {
        if (global)
            return;
        global = new MessageHandler();
    }

    static void destroy() {
        if (!global)
            return;
        delete global;
        global = nullptr;
    }

    static void addLogger() {
        // In case that the user didn't call `init`
        create();

        QMutexLocker guard(&global->lock);
        global->errors.insert(QThread::currentThreadId(), {});
    }

    static void removeLogger() {
        QMutexLocker guard(&global->lock);
        global->errors.remove(QThread::currentThreadId());
    }

    static QByteArray getLog() {
        return global->errors[QThread::currentThreadId()];
    }
};

MessageHandler *MessageHandler::global = nullptr;

void qnrbf_dll_init() {
    MessageHandler::create();
}

void qnrbf_dll_exit() {
    MessageHandler::destroy();
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
    // Remove content
    free_context_content(ctx);

    // Free context
    free(ctx);
}

void qnrbf_xstudio_read(qnrbf_xstudio_context *ctx) {
    // Get buf
    QByteArray data(ctx->buf.str, ctx->buf.size);

    // Remove content
    free_context_content(ctx);

    // Register debug output
    MessageHandler::addLogger();

    // Read
    QNrbfStream in(&data, QIODevice::ReadOnly);
    QNrbf::XSAppModel model;
    in >> model;

    // Get error message
    QByteArray errorMessage = MessageHandler::getLog();

    // Unregister debug output
    MessageHandler::removeLogger();

    // Set status
    ctx->status = (qnrbf_stream_status) in.status();

    if (in.status() != QDataStream::Ok) {
        // Set error message
        copy_string(errorMessage, &ctx->error);
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
                xs_track *trackPtr;
                if (item->type() == QNrbf::XSITrack::Singing) {
                    auto singingTrack = dynamic_cast<QNrbf::XSSingingTrack *>(item.data());

                    auto track = (xs_singing_track *) qnrbf_malloc(sizeof(xs_singing_track));
                    track->base.track_type = SINGING;

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

                                // Save one
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

                                // Save one
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

                                // Save one
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

                    // Save one
                    trackPtr = (xs_track *) track;
                } else {
                    auto instrumentTrack = dynamic_cast<QNrbf::XSInstrumentTrack *>(item.data());

                    auto track = (xs_instrument_track *) qnrbf_malloc(sizeof(xs_instrument_track));
                    track->base.track_type = INSTRUMENT;

                    track->SampleRate = instrumentTrack->SampleRate;
                    track->SampleCount = instrumentTrack->SampleCount;
                    track->ChannelCount = instrumentTrack->ChannelCount;
                    track->OffsetInPos = instrumentTrack->OffsetInPos;
                    copy_string(instrumentTrack->InstrumentFilePath, &track->InstrumentFilePath);

                    // Save one
                    trackPtr = (xs_track *) track;
                }

                // Copy base
                trackPtr->volume = item->volume;
                trackPtr->pan = item->pan;
                trackPtr->mute = item->mute;
                trackPtr->solo = item->solo;
                copy_string(item->name, &trackPtr->name);

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

            // Save one
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

            // Save one
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

                // Save one
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
                            const auto &org = inNote->NotePhoneInfo;

                            // Save one
                            note.NotePhoneInfo = QSharedPointer<QNrbf::XSNotePhoneInfo>::create(
                                org->HeadPhoneTimeInSec, org->MidPartOverTailPartRatio);
                        }

                        // Copy Vibrato
                        if (inNote->Vibrato) {
                            const auto &org = inNote->Vibrato;

                            QNrbf::XSVibratoStyle vibrato;
                            vibrato.IsAntiPhase = org->IsAntiPhase;
                            if (org->ampLine)
                                vibrato.ampLine = QSharedPointer<QNrbf::XSLineParam>::create(
                                    createLineParam(org->ampLine));
                            if (org->freqLine)
                                vibrato.freqLine = QSharedPointer<QNrbf::XSLineParam>::create(
                                    createLineParam(org->freqLine));

                            // Save one
                            note.Vibrato = QSharedPointer<QNrbf::XSVibratoStyle>::create(vibrato);
                        }

                        // Copy VibratoPercentInfo
                        if (inNote->VibratoPercentInfo) {
                            const auto &org = inNote->VibratoPercentInfo;

                            // Save one
                            note.VibratoPercentInfo =
                                QSharedPointer<QNrbf::XSVibratoPercentInfo>::create(
                                    org->startPercent, org->endPercent);
                        }

                        // Save one
                        singingTrack->noteList.append(note);

                        noteNode = noteNode->next;
                    }
                }

                // Save one
                track = singingTrack;
            } else {
                auto inTrack = (xs_instrument_track *) baseTrack;
                auto instrumentTrack = QSharedPointer<QNrbf::XSInstrumentTrack>::create();

                instrumentTrack->SampleRate = inTrack->SampleRate;
                instrumentTrack->SampleCount = inTrack->SampleCount;
                instrumentTrack->ChannelCount = inTrack->ChannelCount;
                instrumentTrack->OffsetInPos = inTrack->OffsetInPos;
                instrumentTrack->InstrumentFilePath = from_xs_string(inTrack->InstrumentFilePath);

                // Save one
                track = instrumentTrack;
            }

            // Copy track base
            track->volume = baseTrack->volume;
            track->pan = baseTrack->pan;
            track->name = from_xs_string(baseTrack->name);
            track->mute = baseTrack->mute;
            track->solo = baseTrack->solo;

            // Save one
            model.trackList.append(track);

            node = node->next;
        }
    }

    // Remove content
    free_context_content(ctx);

    // Register debug output
    MessageHandler::addLogger();

    // Write
    QByteArray data;
    QNrbfStream out(&data, QIODevice::WriteOnly);
    out << model;

    // Get error message
    QByteArray errorMessage = MessageHandler::getLog();

    // Unregister debug output
    MessageHandler::removeLogger();

    // Set status
    ctx->status = (qnrbf_stream_status) out.status();

    if (out.status() != QDataStream::Ok) {
        // Set error message
        copy_string(errorMessage, &ctx->error);
    } else {
        // Set output data
        copy_string(data, &ctx->buf);
    }
}