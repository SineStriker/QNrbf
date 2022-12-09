#include "xstudio.h"

#include <memory>

static void free_node_list(xs_node *list, void(Cleaner)(void *)) {
    auto p = list;
    while (p) {
        // Free data
        if (p->data)
            Cleaner(p->data);

        // Free node
        auto node = p;
        p = p->next;
        free(node);
    }
}

void *qnrbf_alloc_memory(int size) {
    return malloc(size);
}

void qnrbf_free_memory(void *data) {
    free(data);
}

qnrbf_xstudio_context *qnrbf_xstudio_alloc_context() {
    auto ctx = reinterpret_cast<qnrbf_xstudio_context *>(malloc(sizeof(qnrbf_xstudio_context)));
    ctx->status = OK;
    ctx->error = {nullptr, 0};
    ctx->buf = {nullptr, 0};
    ctx->model = nullptr;
}

void qnrbf_xstudio_free_context(qnrbf_xstudio_context *ctx) {
    if (ctx->error.str) {
        free(ctx->error.str);
    }
    if (ctx->buf.str) {
        free(ctx->buf.str);
    }
    if (!ctx->model) {
        return;
    }

    // Free app model
    xs_app_model *model = ctx->model;

    // Free ProjectFilePath
    if (model->ProjectFilePath.str) {
        free(model->ProjectFilePath.str);
    }

    // Free tempoList
    free_node_list(model->tempoList, free);

    // Free beatList
    free_node_list(model->beatList, free);

    // Free trackList
    {
        auto cleaner = [](void *data) {
            auto track = reinterpret_cast<xs_track *>(data);
            if (track->track_type == SINGING) {
                auto singingTrack = reinterpret_cast<xs_singing_track *>(track);

                auto note_cleaner = [](void *data) {
                    auto note = reinterpret_cast<xs_note *>(data);
                    if (note->NotePhoneInfo) {
                        free(note->NotePhoneInfo);
                    }
                    if (note->Vibrato) {
                        free_node_list(note->Vibrato->ampLine, free);
                        free_node_list(note->Vibrato->freqLine, free);
                        free(note->Vibrato);
                    }
                    if (note->VibratoPercentInfo) {
                        free(note->VibratoPercentInfo);
                    }
                    if (note->lyric.str){
                        free(note->lyric.str);
                    }
                    if (note->pronouncing.str){
                        free(note->pronouncing.str);
                    }
                };

                free_node_list(singingTrack->noteList, note_cleaner);

                free_node_list(singingTrack->editedPitchLine, free);
                free_node_list(singingTrack->editedVolumeLine, free);
                free_node_list(singingTrack->editedBreathLine, free);
                free_node_list(singingTrack->editedGenderLine, free);
                free_node_list(singingTrack->editedPowerLine, free);
            } else {
                auto instrumentTrack = reinterpret_cast<xs_instrument_track *>(track);
                if (instrumentTrack->InstrumentFilePath.str) {
                    free(instrumentTrack->InstrumentFilePath.str);
                }
            }
            if (track->name.str) {
                free(track->name.str);
            }
            free(track);
        };

        free_node_list(model->trackList, cleaner);
    }
}

void nrbf_xstudio_read() {
}

void nrbf_xstudio_write() {
}