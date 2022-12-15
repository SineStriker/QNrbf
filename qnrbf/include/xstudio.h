#ifndef QNRBFFORMAT_XSTUDIO_H
#define QNRBFFORMAT_XSTUDIO_H

#include "qnrbf_global.h"

QNRBF_EXTERN_C_BEGIN

enum xs_note_head_tag {
    NO_TAG,
    SPL_TAG,
    SP_TAG,
};

struct xs_string {
    char *str;
    int size;
};

struct xs_node {
    void *data;
    struct xs_node *next;
};

struct xs_overlappable {
    bool Overlapped;
};

struct xs_beat_size {
    int x;
    int y;
};

struct xs_song_tempo {
    struct xs_overlappable base;

    int pos;
    int tempo;
};

struct xs_song_beat {
    struct xs_overlappable base;

    int barIndex;
    struct xs_beat_size beatSize;
};

struct xs_line_param {
    int Pos;
    int Value;
};

struct xs_vibrato_style {
    bool IsAntiPhase;

    // Linked list
    struct xs_node *ampLine;
    struct xs_node *freqLine;
};

struct xs_vibrato_percent_info {
    float startPercent;
    float endPercent;
};

struct xs_note_phone_info {
    float HeadPhoneTimeInSec;
    float MidPartOverTailPartRatio;
};

struct xs_note {
    struct xs_overlappable base;

    struct xs_note_phone_info *NotePhoneInfo;

    int VibratoPercent;
    struct xs_vibrato_style *Vibrato;
    struct xs_vibrato_percent_info *VibratoPercentInfo;

    int startPos;
    int widthPos;
    int keyIndex;
    struct xs_string lyric;
    struct xs_string pronouncing;
    xs_note_head_tag headTag;
};

enum xs_track_type {
    SINGING,
    INSTRUMENT,
};

struct xs_track {
    double volume;
    double pan;
    struct xs_string name;
    bool mute;
    bool solo;

    xs_track_type track_type;
};

struct xs_singing_track {
    struct xs_track base;

    struct xs_string AISingerId;

    // Linked list
    struct xs_node *noteList;

    bool needRefreshBaseMetadataFlag;

    // Linked list
    struct xs_node *editedPitchLine;
    struct xs_node *editedVolumeLine;
    struct xs_node *editedBreathLine;
    struct xs_node *editedGenderLine;
    struct xs_node *editedPowerLine;

    int reverbPreset;
};

struct xs_instrument_track {
    struct xs_track base;

    double SampleRate;
    int SampleCount;
    int ChannelCount;
    int OffsetInPos;
    struct xs_string InstrumentFilePath;
};

struct xs_app_model {
    struct xs_string ProjectFilePath;

    // Linked list
    struct xs_node *tempoList;
    struct xs_node *beatList;
    struct xs_node *trackList;

    int quantize;
    bool isTriplet;
    bool isNumericalKeyName;
    int firstNumericalKeyNameAtIndex;
};

enum qnrbf_stream_status {
    OK,
    READ_PAST_END,
    READ_CORRUPT_DATA,
    WRITE_FAILED,
};

struct qnrbf_xstudio_context {
    qnrbf_stream_status status;
    struct xs_string error;
    struct xs_string buf;
    struct xs_app_model *model;
};

/* You must call the following functions when loading or freeing this library,
 * to make it thread-safe for your application.
 */
QNRBF_C_API void qnrbf_dll_init();

QNRBF_C_API void qnrbf_dll_exit();

/* Allocator */
QNRBF_C_API void *qnrbf_malloc(int size);

QNRBF_C_API void qnrbf_free(void *data);

QNRBF_C_API void qnrbf_memcpy(void *dst, const void *src, int count);

QNRBF_C_API void qnrbf_memset(void *dst, int value, int count);

/* Context */
QNRBF_C_API qnrbf_xstudio_context *qnrbf_xstudio_alloc_context();

QNRBF_C_API void qnrbf_xstudio_free_context(qnrbf_xstudio_context *ctx);

QNRBF_C_API void qnrbf_xstudio_read(qnrbf_xstudio_context *src);

QNRBF_C_API void qnrbf_xstudio_write(qnrbf_xstudio_context *ctx);

QNRBF_EXTERN_C_END

#endif // QNRBFFORMAT_XSTUDIO_H
