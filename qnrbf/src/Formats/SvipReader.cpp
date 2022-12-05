#include "SvipReader.h"

#include <QDebug>

#include "Objects/ClassMemberObject.h"
#include "Objects/PrimitiveListObject.h"
#include "Objects/StringListObject.h"

#include "Config/SvipConst.h"

QNRBF_BEGIN_NAMESPACE

#define ERROR_ON_PROPERTY_NOT_FOUND(PROPERTY)                                                      \
    qDebug().noquote() << QString("Svip: Property \"%1\" not found").arg(PROPERTY);                \
    return false;

#define ERROR_ON_MEMBER_NOT_FOUND(MEMBER)                                                          \
    qDebug().noquote() << QString("Svip: Member variable \"%1\" not found").arg(MEMBER);           \
    return false;

#define ERROR_ON_BAD_CLASS_NAME(NAME)                                                              \
    qDebug().noquote() << QString("Svip: \"%1\" class name not correct").arg(NAME);                \
    return false;

#define CHECK_IF_OBJECT_NULL(OBJECT, NAME)                                                         \
    if (OBJECT.isNull()) {                                                                         \
        qDebug().noquote() << QString("Svip: \"%1\" cannot be null").arg(NAME);                    \
        return false;                                                                              \
    }

#define CHECK_CLASS_NAME(MAPPING, LONG_NAME, NAME)                                                 \
    if (MAPPING->typeName != LONG_NAME) {                                                          \
        ERROR_ON_BAD_CLASS_NAME(NAME);                                                             \
    }

#define CHECK_OBJECT_TYPE(OBJECT, TYPE, NAME)                                                      \
    if (OBJECT->type() != AbstractObject::TYPE) {                                                  \
        qDebug().noquote() << QString("Svip: \"%1\" object type not match").arg(NAME);             \
        return false;                                                                              \
    }

#define CHECK_NONNULL_CLASS_NAME(MAPPING, LONG_NAME, NAME)                                         \
    CHECK_IF_OBJECT_NULL(MAPPING, NAME);                                                           \
    CHECK_CLASS_NAME(MAPPING, LONG_NAME, NAME);

#define CHECK_NONNULL_OBJECT_TYPE(OBJECT, TYPE, NAME)                                              \
    CHECK_IF_OBJECT_NULL(OBJECT, NAME)                                                             \
    CHECK_OBJECT_TYPE(OBJECT, TYPE, NAME)

// Config of checking assembly
#ifndef QNRBF_CHECK_ASSEMBLY
#undef CHECK_CLASS_NAME
#define CHECK_CLASS_NAME(MAPPING, LONG_NAME, NAME)

#undef CHECK_OBJECT_TYPE
#define CHECK_OBJECT_TYPE(OBJECT, TYPE, NAME)
#endif

SvipReader::SvipReader(const NrbfRegistry &reg) : reg(reg) {
}

SvipReader::~SvipReader() {
}

bool SvipReader::load() {
    auto root = reg.findReference(reg.header->rootId);

    // Check not null
    CHECK_NONNULL_OBJECT_TYPE(root, Mapping, "AppModel");

    auto mapping = root.dynamicCast<MappingObject>();
    CHECK_CLASS_NAME(mapping, ASSEMBLY_NAME_APP_MODEL, "AppModel");

    // Start reading
    XSAppModel svip;
    const auto &rootMembers = mapping->members;

    // Read basic information
    {
        // ProjectFilePath <Property>
        if (!reg.findString(rootMembers, NrbfRegistry::toBackingField(KEY_NAME_PROJECT_FILE_PATH),
                            svip.ProjectFilePath)) {
            ERROR_ON_PROPERTY_NOT_FOUND(KEY_NAME_PROJECT_FILE_PATH);
        }
        // quantize
        if (!reg.findPrimitive(rootMembers, KEY_NAME_QUANTIZE, PrimitiveTypeEnumeration::Int32,
                               svip.quantize)) {
            ERROR_ON_MEMBER_NOT_FOUND(KEY_NAME_QUANTIZE);
        }
        // isTriplet
        if (!reg.findPrimitive(rootMembers, KEY_NAME_IS_TRIPLET, PrimitiveTypeEnumeration::Boolean,
                               svip.isTriplet)) {
            ERROR_ON_MEMBER_NOT_FOUND(KEY_NAME_IS_TRIPLET);
        }
        // isNumericalKeyName
        if (!reg.findPrimitive(rootMembers, KEY_NAME_IS_NUMERICAL,
                               PrimitiveTypeEnumeration::Boolean, svip.isNumericalKeyName)) {
            ERROR_ON_MEMBER_NOT_FOUND(KEY_NAME_IS_NUMERICAL);
        }
        // firstNumericalKeyNameAtIndex
        if (!reg.findPrimitive(rootMembers, KEY_NAME_FIRST_NUMERICAL,
                               PrimitiveTypeEnumeration::Int32,
                               svip.firstNumericalKeyNameAtIndex)) {
            ERROR_ON_MEMBER_NOT_FOUND(KEY_NAME_FIRST_NUMERICAL);
        }
    }

    // Read beats
    {
        QList<ObjectRef> beatObjects;
        if (!readSerialObjectList(rootMembers, KEY_NAME_BEAT_LIST, ASSEMBLY_NAME_BEAT_LIST,
                                  "SerialOverlapableItemList<SongBeat>",
                                  ASSEMBLY_NAME_BEAT_LIST_BUF, "List<SongBeat>", beatObjects)) {
            return false;
        }
        for (auto obj : qAsConst(beatObjects)) {
            obj = reg.resolveReference(obj);
            if (obj.isNull()) {
                continue;
            }
            CHECK_OBJECT_TYPE(obj, Mapping, "SongBeat");

            auto beatMapping = obj.dynamicCast<MappingObject>();
            CHECK_CLASS_NAME(beatMapping, ASSEMBLY_NAME_SONG_BEAT, "SongBeat");

            XSSongBeat beat;
            if (!readBeat(beatMapping->members, beat)) {
                return false;
            }

            svip.beatList.append(beat);
        }
    }

    // Read tempos
    {
        QList<ObjectRef> tempoObjects;
        if (!readSerialObjectList(rootMembers, KEY_NAME_TEMPO_LIST, ASSEMBLY_NAME_TEMPO_LIST,
                                  "SerialOverlapableItemList<SongTempo>",
                                  ASSEMBLY_NAME_TEMPO_LIST_BUF, "List<SongTempo>", tempoObjects)) {
            return false;
        }
        for (auto obj : qAsConst(tempoObjects)) {
            obj = reg.resolveReference(obj);
            if (obj.isNull()) {
                continue;
            }
            CHECK_OBJECT_TYPE(obj, Mapping, "SongTempo");

            auto tempoMapping = obj.dynamicCast<MappingObject>();
            CHECK_CLASS_NAME(tempoMapping, ASSEMBLY_NAME_SONG_TEMPO, "SongTempo");

            XSSongTempo tempo;
            if (!readTempo(tempoMapping->members, tempo)) {
                return false;
            }
            svip.tempoList.append(tempo);
        }
    }

    // Read tracks
    {
        MappingRef trackList;
        if (!reg.findObject(rootMembers, KEY_NAME_TRACK_LIST, trackList)) {
            ERROR_ON_MEMBER_NOT_FOUND(KEY_NAME_TRACK_LIST);
        }

        // Track list cannot be null
        CHECK_NONNULL_CLASS_NAME(trackList, ASSEMBLY_NAME_TRACK_LIST, "List<ITrack>");

        QList<ObjectRef> trackObjects;
        if (!reg.findObjectList(trackList->members, KEY_NAME_LIST_ITEMS, trackObjects)) {
            ERROR_ON_MEMBER_NOT_FOUND(KEY_NAME_LIST_ITEMS);
        }

        for (auto obj : qAsConst(trackObjects)) {
            obj = reg.resolveReference(obj);
            if (obj.isNull()) {
                continue;
            }
            CHECK_OBJECT_TYPE(obj, Mapping, "ITrack");

            auto trackMapping = obj.dynamicCast<MappingObject>();
            if (trackMapping->typeName == ASSEMBLY_NAME_SINGING_TRACK) {
                // Read singing track
                XSSingingTrack track;
                const auto &trackMembers = trackMapping->members;

                // Base part
                if (!readITrack(trackMembers, track)) {
                    return false;
                }

                // AISingerId <Property>
                if (!reg.findString(trackMembers,
                                    NrbfRegistry::toBackingField(KEY_NAME_AI_SINGER_ID),
                                    track.AISingerId)) {
                    ERROR_ON_PROPERTY_NOT_FOUND(KEY_NAME_AI_SINGER_ID);
                }
                // needRefreshBaseMetadataFlag
                if (!reg.findPrimitive(trackMembers, KEY_NAME_NEED_REFRESH_FLAG,
                                       PrimitiveTypeEnumeration::Boolean,
                                       track.needRefreshBaseMetadataFlag)) {
                    ERROR_ON_MEMBER_NOT_FOUND(KEY_NAME_NEED_REFRESH_FLAG);
                }
                // reverbPreset
                {
                    qint32 val = XSReverbPreset::NONE;
                    if (!readEnum(trackMembers, KEY_NAME_REVERB_PRESET, ASSEMBLY_NAME_REVERB_PRESET,
                                  "ReverbPreset", val)) {
                        return false;
                    }
                    if (val < XSReverbPreset::NONE || val > XSReverbPreset::LONGREVERB2) {
                        qDebug().noquote() << QString("SVip: Invalid ReverbPreset value %1")
                                                  .arg(QString::number(val));
                        return false;
                    }
                    track.reverbPreset = static_cast<XSReverbPreset>(val);
                }

                // Breath
                {
                    MappingRef breath;
                    if (!reg.findObject(trackMembers, KEY_NAME_EDITED_BREATH_LINE, breath)) {
                        ERROR_ON_MEMBER_NOT_FOUND(KEY_NAME_EDITED_BREATH_LINE);
                    }
                    if (!breath.isNull()) {
                        CHECK_CLASS_NAME(breath, ASSEMBLY_NAME_LINE_PARAM, "LineParam");

                        XSLineParam param;
                        if (!readLineParam(breath->members, param)) {
                            return false;
                        }
                        track.editedBreathLine =
                            QSharedPointer<XSLineParam>::create(std::move(param));
                    }
                }
                // Gender
                {
                    MappingRef gender;
                    if (!reg.findObject(trackMembers, KEY_NAME_EDITED_GENDER_LINE, gender)) {
                        ERROR_ON_MEMBER_NOT_FOUND(KEY_NAME_EDITED_GENDER_LINE);
                    }
                    if (!gender.isNull()) {
                        CHECK_CLASS_NAME(gender, ASSEMBLY_NAME_LINE_PARAM, "LineParam");

                        XSLineParam param;
                        if (!readLineParam(gender->members, param)) {
                            return false;
                        }
                        track.editedGenderLine =
                            QSharedPointer<XSLineParam>::create(std::move(param));
                    }
                }
                // Pitch
                {
                    MappingRef pitch;
                    if (!reg.findObject(trackMembers, KEY_NAME_EDITED_PITCH_LINE, pitch)) {
                        ERROR_ON_MEMBER_NOT_FOUND(KEY_NAME_EDITED_PITCH_LINE);
                    }
                    if (!pitch.isNull()) {
                        CHECK_CLASS_NAME(pitch, ASSEMBLY_NAME_LINE_PARAM, "LineParam");

                        XSLineParam param;
                        if (!readLineParam(pitch->members, param)) {
                            return false;
                        }
                        track.editedPitchLine =
                            QSharedPointer<XSLineParam>::create(std::move(param));
                    }
                }
                // Volume
                {
                    MappingRef volume;
                    if (!reg.findObject(trackMembers, KEY_NAME_EDITED_VOLUME_LINE, volume)) {
                        ERROR_ON_MEMBER_NOT_FOUND(KEY_NAME_EDITED_VOLUME_LINE);
                    }
                    if (!volume.isNull()) {
                        CHECK_CLASS_NAME(volume, ASSEMBLY_NAME_LINE_PARAM, "LineParam");

                        XSLineParam param;
                        if (!readLineParam(volume->members, param)) {
                            return false;
                        }
                        track.editedVolumeLine =
                            QSharedPointer<XSLineParam>::create(std::move(param));
                    }
                }
                // Power
                {
                    MappingRef power;
                    if (!reg.findObject(trackMembers, KEY_NAME_EDITED_POWER_LINE, power)) {
                        ERROR_ON_MEMBER_NOT_FOUND(KEY_NAME_EDITED_POWER_LINE);
                    }
                    if (!power.isNull()) {
                        CHECK_CLASS_NAME(power, ASSEMBLY_NAME_LINE_PARAM, "LineParam");

                        XSLineParam param;
                        if (!readLineParam(power->members, param)) {
                            return false;
                        }
                        track.editedPowerLine =
                            QSharedPointer<XSLineParam>::create(std::move(param));
                    }
                }

                // Notes
                {
                    QList<ObjectRef> noteList;
                    if (!readSerialObjectList(
                            trackMembers, KEY_NAME_NOTE_LIST, ASSEMBLY_NAME_NOTE_LIST,
                            "SerialOverlapableItemList<Note>", ASSEMBLY_NAME_NOTE_LIST_BUF,
                            "List<Note>", noteList)) {
                        return false;
                    }

                    for (auto noteObj : qAsConst(noteList)) {
                        noteObj = reg.resolveReference(noteObj);
                        if (noteObj.isNull()) {
                            continue;
                        }
                        CHECK_OBJECT_TYPE(noteObj, Mapping, "Note");

                        auto noteMapping = noteObj.dynamicCast<MappingObject>();
                        CHECK_CLASS_NAME(noteMapping, ASSEMBLY_NAME_NOTE, "Note");

                        XSNote note;
                        if (!readNote(noteMapping->members, note)) {
                            return false;
                        }
                        track.noteList.append(note);
                    }
                }

                svip.trackList.append(QSharedPointer<XSSingingTrack>::create(std::move(track)));
            } else if (trackMapping->typeName == ASSEMBLY_NAME_INSTRUMENT_TRACK) {
                // Read instrument track
                XSInstrumentTrack track;
                const auto &trackMembers = trackMapping->members;

                // Base part
                if (!readITrack(trackMembers, track)) {
                    return false;
                }

                // SampleRate <Property>
                if (!reg.findPrimitive(trackMembers,
                                       NrbfRegistry::toBackingField(KEY_NAME_SAMPLE_RATE),
                                       PrimitiveTypeEnumeration::Double, track.SampleRate)) {
                    ERROR_ON_PROPERTY_NOT_FOUND(KEY_NAME_SAMPLE_RATE);
                }
                // SampleCount <Property>
                if (!reg.findPrimitive(trackMembers,
                                       NrbfRegistry::toBackingField(KEY_NAME_SAMPLE_COUNT),
                                       PrimitiveTypeEnumeration::Int32, track.SampleCount)) {
                    ERROR_ON_PROPERTY_NOT_FOUND(KEY_NAME_SAMPLE_COUNT);
                }
                // ChannelCount <Property>
                if (!reg.findPrimitive(trackMembers,
                                       NrbfRegistry::toBackingField(KEY_NAME_CHANNEL_COUNT),
                                       PrimitiveTypeEnumeration::Int32, track.ChannelCount)) {
                    ERROR_ON_PROPERTY_NOT_FOUND(KEY_NAME_CHANNEL_COUNT);
                }
                // OffsetInPos <Property>
                if (!reg.findPrimitive(trackMembers,
                                       NrbfRegistry::toBackingField(KEY_NAME_OFFSET_IN_POS),
                                       PrimitiveTypeEnumeration::Int32, track.OffsetInPos)) {
                    ERROR_ON_PROPERTY_NOT_FOUND(KEY_NAME_OFFSET_IN_POS);
                }
                // InstrumentFilePath <Property>
                if (!reg.findString(trackMembers,
                                    NrbfRegistry::toBackingField(KEY_NAME_INSTRUMENT_FILE_PATH),
                                    track.InstrumentFilePath)) {
                    ERROR_ON_PROPERTY_NOT_FOUND(KEY_NAME_INSTRUMENT_FILE_PATH);
                }
                svip.trackList.append(QSharedPointer<XSInstrumentTrack>::create(std::move(track)));
            } else {
                ERROR_ON_BAD_CLASS_NAME("ITrack");
            }
        }
    }

    appModel = std::move(svip);
    return true;
}

bool SvipReader::readBeat(const QMap<QString, ObjectRef> &members, XSSongBeat &out) {
    // Overlapped <Property>
    if (!reg.findPrimitive(members, NrbfRegistry::toBackingField(KEY_NAME_OVERLAPPED),
                           PrimitiveTypeEnumeration::Boolean, out.Overlapped)) {
        ERROR_ON_PROPERTY_NOT_FOUND(KEY_NAME_OVERLAPPED);
    }
    // barIndex
    if (!reg.findPrimitive(members, KEY_NAME_BAR_INDEX, PrimitiveTypeEnumeration::Int32,
                           out.barIndex)) {
        ERROR_ON_MEMBER_NOT_FOUND(KEY_NAME_BAR_INDEX);
    }

    // beatSize
    MappingRef beatSizeObj;
    if (!reg.findObject(members, KEY_NAME_BEAT_SIZE, beatSizeObj)) {
        ERROR_ON_MEMBER_NOT_FOUND(KEY_NAME_BEAT_SIZE);
    }
    if (!beatSizeObj.isNull()) {
        CHECK_CLASS_NAME(beatSizeObj, ASSEMBLY_NAME_BEAT_SIZE, "BeatSize");
        {
            const auto &beatSizeMembers = beatSizeObj->members;
            // x
            if (!reg.findPrimitive(beatSizeMembers, KEY_NAME_BEAT_SIZE_X,
                                   PrimitiveTypeEnumeration::Int32, out.beatSize.x)) {
                ERROR_ON_MEMBER_NOT_FOUND(KEY_NAME_BEAT_SIZE_X);
            }
            // y
            if (!reg.findPrimitive(beatSizeMembers, KEY_NAME_BEAT_SIZE_Y,
                                   PrimitiveTypeEnumeration::Int32, out.beatSize.y)) {
                ERROR_ON_MEMBER_NOT_FOUND(KEY_NAME_BEAT_SIZE_Y);
            }
        }
    }
    return true;
}

bool SvipReader::readTempo(const QMap<QString, ObjectRef> &members, XSSongTempo &out) {
    // Overlapped <Property>
    if (!reg.findPrimitive(members, NrbfRegistry::toBackingField(KEY_NAME_OVERLAPPED),
                           PrimitiveTypeEnumeration::Boolean, out.Overlapped)) {
        ERROR_ON_PROPERTY_NOT_FOUND(KEY_NAME_OVERLAPPED);
    }
    // pos
    if (!reg.findPrimitive(members, KEY_NAME_TEMPO_POS, PrimitiveTypeEnumeration::Int32, out.pos)) {
        ERROR_ON_MEMBER_NOT_FOUND(KEY_NAME_TEMPO_POS);
    }
    // tempo
    if (!reg.findPrimitive(members, KEY_NAME_TEMPO, PrimitiveTypeEnumeration::Int32, out.tempo)) {
        ERROR_ON_MEMBER_NOT_FOUND(KEY_NAME_TEMPO);
    }
    return true;
}

bool SvipReader::readITrack(const QMap<QString, ObjectRef> &members, XSITrack &out) {
    // volume
    if (!reg.findPrimitive(members, KEY_NAME_TRACK_VOLUME, PrimitiveTypeEnumeration::Double,
                           out.volume)) {
        ERROR_ON_MEMBER_NOT_FOUND(KEY_NAME_TRACK_VOLUME);
    }
    // pan
    if (!reg.findPrimitive(members, KEY_NAME_TRACK_PAN, PrimitiveTypeEnumeration::Double,
                           out.pan)) {
        ERROR_ON_MEMBER_NOT_FOUND(KEY_NAME_TRACK_PAN);
    }
    // name
    if (!reg.findString(members, KEY_NAME_TRACK_NAME, out.name)) {
        ERROR_ON_MEMBER_NOT_FOUND(KEY_NAME_INSTRUMENT_FILE_PATH);
    }
    // mute
    if (!reg.findPrimitive(members, KEY_NAME_TRACK_MUTE, PrimitiveTypeEnumeration::Boolean,
                           out.mute)) {
        ERROR_ON_MEMBER_NOT_FOUND(KEY_NAME_TRACK_MUTE);
    }
    // solo
    if (!reg.findPrimitive(members, KEY_NAME_TRACK_SOLO, PrimitiveTypeEnumeration::Boolean,
                           out.solo)) {
        ERROR_ON_MEMBER_NOT_FOUND(KEY_NAME_TRACK_SOLO);
    }
    return true;
}

bool SvipReader::readLineParam(const QMap<QString, ObjectRef> &members, XSLineParam &out) {
    /* Use custom deserialization strategy */

    QList<qint8> bytes;
    if (!reg.findPrimitiveList(members, KEY_NAME_LINE_PARAM, PrimitiveTypeEnumeration::Byte,
                               bytes)) {
        ERROR_ON_MEMBER_NOT_FOUND(KEY_NAME_LINE_PARAM);
    }

    // Skip if empty
    if (bytes.isEmpty()) {
        return true;
    }

    QByteArray data;
    data.resize(bytes.size());
    std::copy(bytes.begin(), bytes.end(), data.begin());

    QDataStream stream(&data, QIODevice::ReadOnly);
    stream.setByteOrder(QDataStream::LittleEndian);

    // Read size
    qint32 size;
    stream >> size;
    if (stream.status() != QDataStream::Ok) {
        goto failed;
    }

    // Read items
    for (int i = 0; i < size; i++) {
        int pos, val;
        stream >> pos;
        stream >> val;
        if (stream.status() != QDataStream::Ok) {
            goto failed;
        }
        out._nodeLinkedList.emplace_back(pos, val);
    }

    return true;

failed:
    qDebug() << "Svip: LineParam deserialize failed";
    return false;
}

bool SvipReader::readNote(const QMap<QString, ObjectRef> &members, XSNote &out) {
    // startPos
    if (!reg.findPrimitive(members, KEY_NAME_NOTE_START_POS, PrimitiveTypeEnumeration::Int32,
                           out.startPos)) {
        ERROR_ON_MEMBER_NOT_FOUND(KEY_NAME_NOTE_KEY_INDEX);
    }
    // widthPos
    if (!reg.findPrimitive(members, KEY_NAME_NOTE_WIDTH_POS, PrimitiveTypeEnumeration::Int32,
                           out.widthPos)) {
        ERROR_ON_MEMBER_NOT_FOUND(KEY_NAME_NOTE_KEY_INDEX);
    }
    // keyIndex
    if (!reg.findPrimitive(members, KEY_NAME_NOTE_KEY_INDEX, PrimitiveTypeEnumeration::Int32,
                           out.keyIndex)) {
        ERROR_ON_MEMBER_NOT_FOUND(KEY_NAME_NOTE_KEY_INDEX);
    }
    // lyric
    if (!reg.findString(members, KEY_NAME_NOTE_LYRIC, out.lyric)) {
        ERROR_ON_MEMBER_NOT_FOUND(KEY_NAME_NOTE_LYRIC);
    }
    // pronouncing
    if (!reg.findString(members, KEY_NAME_NOTE_PRONOUNCING, out.pronouncing)) {
        ERROR_ON_MEMBER_NOT_FOUND(KEY_NAME_NOTE_PRONOUNCING);
    }
    // Overlapped <Property>
    if (!reg.findPrimitive(members, NrbfRegistry::toBackingField(KEY_NAME_OVERLAPPED),
                           PrimitiveTypeEnumeration::Boolean, out.Overlapped)) {
        ERROR_ON_PROPERTY_NOT_FOUND(KEY_NAME_OVERLAPPED);
    }
    // VibratoPercent <Property>
    if (!reg.findPrimitive(members, NrbfRegistry::toBackingField(KEY_NAME_NOTE_VIBRATO_PERCENT),
                           PrimitiveTypeEnumeration::Int32, out.VibratoPercent)) {
        ERROR_ON_PROPERTY_NOT_FOUND(KEY_NAME_NOTE_VIBRATO_PERCENT);
    }

    // headTag
    {
        qint32 val = XSNoteHeadTag::NoTag;
        if (!readEnum(members, KEY_NAME_NOTE_HEAD_TAG, ASSEMBLY_NAME_NOTE_HEAD_TAG, "NoteHeadTag",
                      val)) {
            return false;
        }
        if (val < XSNoteHeadTag::NoTag || val > XSNoteHeadTag::SpTag) {
            qDebug().noquote()
                << QString("SVip: Invalid NoteHeadTag value %1").arg(QString::number(val));
            return false;
        }
    }

    // VibratoPercentInfo <Property>
    {
        MappingRef infoObj;
        if (!reg.findObject(members,
                            NrbfRegistry::toBackingField(KEY_NAME_NOTE_VIBRATO_PERCENT_INFO),
                            infoObj)) {
            ERROR_ON_PROPERTY_NOT_FOUND(KEY_NAME_NOTE_VIBRATO_PERCENT_INFO);
        }
        if (!infoObj.isNull()) {
            CHECK_CLASS_NAME(infoObj, ASSEMBLY_NAME_VIBRATO_PERCENT_INFO, "VibratoPercentInfo");

            out.VibratoPercentInfo = QSharedPointer<XSVibratoPercentInfo>::create();
            const auto &vibratoMembers = infoObj->members;
            if (!reg.findPrimitive(vibratoMembers, KEY_NAME_VIBRATO_START_PERCENT,
                                   PrimitiveTypeEnumeration::Single,
                                   out.VibratoPercentInfo->startPercent)) {
                ERROR_ON_MEMBER_NOT_FOUND(KEY_NAME_HEAD_PHONEME_TIME);
            }
            if (!reg.findPrimitive(vibratoMembers, KEY_NAME_VIBRATO_END_PERCENT,
                                   PrimitiveTypeEnumeration::Single,
                                   out.VibratoPercentInfo->endPercent)) {
                ERROR_ON_MEMBER_NOT_FOUND(KEY_NAME_MID_PART_OVER_TAIL_PART_RATIO);
            }
        }
    }

    // NotePhoneInfo <Property>
    {
        MappingRef phonemeObj;
        if (!reg.findObject(members, NrbfRegistry::toBackingField(KEY_NAME_NOTE_PHONEME_INFO),
                            phonemeObj)) {
            ERROR_ON_PROPERTY_NOT_FOUND(KEY_NAME_NOTE_PHONEME_INFO);
        }
        if (!phonemeObj.isNull()) {
            CHECK_CLASS_NAME(phonemeObj, ASSEMBLY_NAME_NOTE_PHONEME_INFO, "NotePhoneInfo");

            out.NotePhoneInfo = QSharedPointer<XSNotePhoneInfo>::create();
            const auto &phonemeMembers = phonemeObj->members;

            // HeadPhoneTimeInSec
            if (!reg.findPrimitive(
                    phonemeMembers, NrbfRegistry::toBackingField(KEY_NAME_HEAD_PHONEME_TIME),
                    PrimitiveTypeEnumeration::Single, out.NotePhoneInfo->HeadPhoneTimeInSec)) {
                ERROR_ON_PROPERTY_NOT_FOUND(KEY_NAME_HEAD_PHONEME_TIME);
            }
            // MidPartOverTailPartRatio
            if (!reg.findPrimitive(
                    phonemeMembers,
                    NrbfRegistry::toBackingField(KEY_NAME_MID_PART_OVER_TAIL_PART_RATIO),
                    PrimitiveTypeEnumeration::Single,
                    out.NotePhoneInfo->MidPartOverTailPartRatio)) {
                ERROR_ON_PROPERTY_NOT_FOUND(KEY_NAME_MID_PART_OVER_TAIL_PART_RATIO);
            }
        }
    }

    // Vibrato <Property>
    {
        MappingRef styleObj;
        if (!reg.findObject(members, NrbfRegistry::toBackingField(KEY_NAME_NOTE_VIBRATO),
                            styleObj)) {
            ERROR_ON_PROPERTY_NOT_FOUND(KEY_NAME_NOTE_VIBRATO);
        }
        if (!styleObj.isNull()) {
            CHECK_CLASS_NAME(styleObj, ASSEMBLY_NAME_VIBRATO_STYLE, "VibratoStyle");

            out.Vibrato = QSharedPointer<XSVibratoStyle>::create();
            const auto &styleMembers = styleObj->members;

            // IsAntiPhase <Property>
            if (!reg.findPrimitive(styleMembers,
                                   NrbfRegistry::toBackingField(KEY_NAME_VIBRATO_ANTI_PHASE),
                                   PrimitiveTypeEnumeration::Boolean, out.Vibrato->IsAntiPhase)) {
                ERROR_ON_PROPERTY_NOT_FOUND(KEY_NAME_VIBRATO_ANTI_PHASE);
            }

            // ampLine
            {
                MappingRef amp;
                if (!reg.findObject(styleMembers, KEY_NAME_VIBRATO_AMP_LINE, amp)) {
                    ERROR_ON_MEMBER_NOT_FOUND(KEY_NAME_VIBRATO_AMP_LINE);
                }
                if (!amp.isNull()) {
                    CHECK_CLASS_NAME(amp, ASSEMBLY_NAME_LINE_PARAM, "LineParam");

                    out.Vibrato->ampLine = QSharedPointer<XSLineParam>::create();
                    if (!readLineParam(amp->members, *out.Vibrato->ampLine)) {
                        return false;
                    }
                }
            }

            // freqLine
            {
                MappingRef freq;
                if (!reg.findObject(styleMembers, KEY_NAME_VIBRATO_FREQ_LINE, freq)) {
                    ERROR_ON_MEMBER_NOT_FOUND(KEY_NAME_VIBRATO_FREQ_LINE);
                }
                if (!freq.isNull()) {
                    CHECK_CLASS_NAME(freq, ASSEMBLY_NAME_LINE_PARAM, "LineParam");

                    out.Vibrato->freqLine = QSharedPointer<XSLineParam>::create();
                    if (!readLineParam(freq->members, *out.Vibrato->freqLine)) {
                        return false;
                    }
                }
            }
        }
    }

    return true;
}

bool SvipReader::readSerialObjectList(const QMap<QString, ObjectRef> &members, const QString &key,
                                      const QString &assembly1, const QString &name1,
                                      const QString &assembly2, const QString &name2,
                                      QList<ObjectRef> &out) {
    MappingRef list;
    if (!reg.findObject(members, key, list)) {
        ERROR_ON_MEMBER_NOT_FOUND(key);
    }
    if (list.isNull()) {
        return true;
    }
    CHECK_CLASS_NAME(list, assembly1, name1);

    MappingRef buf;
    if (!reg.findObject(list->members, KEY_NAME_SERIAL_LIST_BUF, buf)) {
        ERROR_ON_MEMBER_NOT_FOUND(KEY_NAME_SERIAL_LIST_BUF);
    }
    CHECK_NONNULL_CLASS_NAME(buf, assembly2, name2);

    if (!reg.findObjectList(buf->members, KEY_NAME_LIST_ITEMS, out)) {
        ERROR_ON_MEMBER_NOT_FOUND(KEY_NAME_LIST_ITEMS);
    }

    return true;
}

bool SvipReader::readEnum(const QMap<QString, ObjectRef> &members, const QString &key,
                          const QString &assembly, const QString &name, qint32 &out) {
    MappingRef obj;
    if (!reg.findObject(members, key, obj)) {
        ERROR_ON_MEMBER_NOT_FOUND(key);
    }
    if (obj.isNull()) {
        return true;
    }
    CHECK_CLASS_NAME(obj, assembly, name);
    if (!reg.findPrimitive(obj->members, KEY_NAME_ENUM_VALUE, PrimitiveTypeEnumeration::Int32,
                           out)) {
        ERROR_ON_MEMBER_NOT_FOUND(KEY_NAME_ENUM_VALUE);
    }
    return true;
}

QNRBF_END_NAMESPACE