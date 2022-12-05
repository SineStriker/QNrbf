#ifndef QNRBFFORMAT_SVIPCONST_H
#define QNRBFFORMAT_SVIPCONST_H

#include "qnrbf_global.h"

QNRBF_BEGIN_NAMESPACE

// Class type names
const char ASSEMBLY_NAME_APP_MODEL[] = "SingingTool.Model.AppModel";
const char ASSEMBLY_NAME_BEAT_LIST[] =
    "SingingTool.Library.SerialOverlapableItemList`1[[SingingTool.Model.SingingGeneralConcept."
    "SongBeat, SingingTool.Model, Version=1.0.0.0, Culture=neutral, PublicKeyToken=null]]";
const char ASSEMBLY_NAME_BEAT_LIST_BUF[] =
    "System.Collections.Generic.List`1[[SingingTool.Model.SingingGeneralConcept.SongBeat, "
    "SingingTool.Model, Version=1.0.0.0, Culture=neutral, PublicKeyToken=null]]";
const char ASSEMBLY_NAME_TEMPO_LIST[] =
    "SingingTool.Library.SerialOverlapableItemList`1[[SingingTool.Model.SingingGeneralConcept."
    "SongTempo, SingingTool.Model, Version=1.0.0.0, Culture=neutral, PublicKeyToken=null]]";
const char ASSEMBLY_NAME_TEMPO_LIST_BUF[] =
    "System.Collections.Generic.List`1[[SingingTool.Model.SingingGeneralConcept.SongTempo, "
    "SingingTool.Model, Version=1.0.0.0, Culture=neutral, PublicKeyToken=null]]";
const char ASSEMBLY_NAME_TRACK_LIST[] =
    "System.Collections.Generic.List`1[[SingingTool.Model.ITrack, SingingTool.Model, "
    "Version=1.0.0.0, Culture=neutral, PublicKeyToken=null]]";
const char ASSEMBLY_NAME_NOTE_LIST[] =
    "SingingTool.Library.SerialOverlapableItemList`1[[SingingTool.Model.Note, SingingTool.Model, "
    "Version=1.0.0.0, Culture=neutral, PublicKeyToken=null]]";
const char ASSEMBLY_NAME_NOTE_LIST_BUF[] =
    "System.Collections.Generic.List`1[[SingingTool.Model.Note, SingingTool.Model, "
    "Version=1.0.0.0, Culture=neutral, PublicKeyToken=null]]";
const char ASSEMBLY_NAME_SONG_BEAT[] = "SingingTool.Model.SingingGeneralConcept.SongBeat";
const char ASSEMBLY_NAME_BEAT_SIZE[] = "SingingTool.Model.SingingGeneralConcept.BeatSize";
const char ASSEMBLY_NAME_SONG_TEMPO[] = "SingingTool.Model.SingingGeneralConcept.SongTempo";
const char ASSEMBLY_NAME_SINGING_TRACK[] = "SingingTool.Model.SingingTrack";
const char ASSEMBLY_NAME_INSTRUMENT_TRACK[] = "SingingTool.Model.InstrumentTrack";
const char ASSEMBLY_NAME_LINE_PARAM[] = "SingingTool.Model.Line.LineParam";
const char ASSEMBLY_NAME_REVERB_PRESET[] = "SingingTool.Library.Audio.ReverbPreset";
const char ASSEMBLY_NAME_NOTE[] = "SingingTool.Model.Note";
const char ASSEMBLY_NAME_NOTE_PHONEME_INFO[] = "SingingTool.Model.NotePhoneInfo";
const char ASSEMBLY_NAME_NOTE_HEAD_TAG[] = "SingingTool.Model.NoteHeadTag";
const char ASSEMBLY_NAME_VIBRATO_PERCENT_INFO[] = "SingingTool.Model.VibratoPercentInfo";
const char ASSEMBLY_NAME_VIBRATO_STYLE[] = "SingingTool.Model.VibratoStyle";

// AppModel keys
const char KEY_NAME_PROJECT_FILE_PATH[] = "ProjectFilePath";
const char KEY_NAME_QUANTIZE[] = "_quantize";
const char KEY_NAME_IS_TRIPLET[] = "_isTriplet";
const char KEY_NAME_IS_NUMERICAL[] = "_isNumerialKeyName";
const char KEY_NAME_TEMPO_LIST[] = "_tempoList";
const char KEY_NAME_BEAT_LIST[] = "_beatList";
const char KEY_NAME_TRACK_LIST[] = "_trackList";
const char KEY_NAME_FIRST_NUMERICAL[] = "_firstNumerialKeyNameAtIndex";

const char KEY_NAME_SERIAL_LIST_BUF[] = "_buf";
const char KEY_NAME_LIST_ITEMS[] = "_items";

// IOverlappable keys
const char KEY_NAME_OVERLAPPED[] = "Overlaped";

// SongBeat keys
const char KEY_NAME_BAR_INDEX[] = "_barIndex";
const char KEY_NAME_BEAT_SIZE[] = "_beatSize";

// BeatSize keys
const char KEY_NAME_BEAT_SIZE_X[] = "_x";
const char KEY_NAME_BEAT_SIZE_Y[] = "_y";

// SongTempo keys
const char KEY_NAME_TEMPO_POS[] = "_pos";
const char KEY_NAME_TEMPO[] = "_tempo";

// Track keys
const char KEY_NAME_TRACK_MUTE[] = "_mute";
const char KEY_NAME_TRACK_NAME[] = "_name";
const char KEY_NAME_TRACK_PAN[] = "_pan";
const char KEY_NAME_TRACK_SOLO[] = "_solo";
const char KEY_NAME_TRACK_VOLUME[] = "_volume";

// InstrumentTrack keys
const char KEY_NAME_CHANNEL_COUNT[] = "ChannelCount";
const char KEY_NAME_OFFSET_IN_POS[] = "OffsetInPos";
const char KEY_NAME_SAMPLE_COUNT[] = "SampleCount";
const char KEY_NAME_SAMPLE_RATE[] = "SampleRate";
const char KEY_NAME_INSTRUMENT_FILE_PATH[] = "InstrumentFilePath";

// SingingTrack keys
const char KEY_NAME_AI_SINGER_ID[] = "AISingerId";
const char KEY_NAME_NOTE_LIST[] = "_noteList";
const char KEY_NAME_NEED_REFRESH_FLAG[] = "_needRefreshBaseMetadataFlag";
const char KEY_NAME_EDITED_BREATH_LINE[] = "_editedBreathLine";
const char KEY_NAME_EDITED_GENDER_LINE[] = "_editedGenderLine";
const char KEY_NAME_EDITED_PITCH_LINE[] = "_editedPitchLine";
const char KEY_NAME_EDITED_VOLUME_LINE[] = "_editedVolumeLine";
const char KEY_NAME_EDITED_POWER_LINE[] = "_editedPowerLine";
const char KEY_NAME_REVERB_PRESET[] = "_reverbPreset";

// LineParam keys
const char KEY_NAME_LINE_PARAM[] = "LineParam";

// Enumeration keys
const char KEY_NAME_ENUM_VALUE[] = "value__";

// NotePhoneInfo keys
const char KEY_NAME_HEAD_PHONEME_TIME[] = "HeadPhoneTimeInSec";
const char KEY_NAME_MID_PART_OVER_TAIL_PART_RATIO[] = "MidPartOverTailPartRatio";

// VibratoPercentInfo keys
const char KEY_NAME_VIBRATO_START_PERCENT[] = "_startPercent";
const char KEY_NAME_VIBRATO_END_PERCENT[] = "_endPercent";

// VibratoStyle keys
const char KEY_NAME_VIBRATO_ANTI_PHASE[] = "IsAntiPhase";
const char KEY_NAME_VIBRATO_AMP_LINE[] = "_ampLine";
const char KEY_NAME_VIBRATO_FREQ_LINE[] = "_freqLine";

// Note keys
const char KEY_NAME_NOTE_PHONEME_INFO[] = "NotePhoneInfo";
const char KEY_NAME_NOTE_VIBRATO[] = "Vibrato";
const char KEY_NAME_NOTE_VIBRATO_PERCENT[] = "VibratoPercent";
const char KEY_NAME_NOTE_VIBRATO_PERCENT_INFO[] = "VibratoPercentInfo";
const char KEY_NAME_NOTE_HEAD_TAG[] = "_headTag";
const char KEY_NAME_NOTE_KEY_INDEX[] = "_keyIndex";
const char KEY_NAME_NOTE_LYRIC[] = "_lyric";
const char KEY_NAME_NOTE_PRONOUNCING[] = "_pronouncing";
const char KEY_NAME_NOTE_START_POS[] = "_startPos";
const char KEY_NAME_NOTE_WIDTH_POS[] = "_widthPos";

QNRBF_END_NAMESPACE


#endif // QNRBFFORMAT_SVIPCONST_H