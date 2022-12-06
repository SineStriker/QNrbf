#include "SvipWriter.h"

#include "Config/SvipConst.h"

#include "Objects/PrimitiveObject.h"
#include "Objects/StringObject.h"
#include "Records/SerializationHeader.h"

QNRBF_BEGIN_NAMESPACE

SvipWriter::SvipWriter(const QNrbf::XSAppModel &model) : appModel(model) {
}

SvipWriter::~SvipWriter() {
}

bool SvipWriter::save() {
    std::list<qint32> ids;

    qint32 idMax = 0;
    qint32 idTmp;

    auto idEnq = [&]() { return (ids.push_back(++idMax), idMax); };               // idMax
    auto idDeq = [&]() { return (idTmp = ids.front(), ids.pop_front(), idTmp); }; // idTmp

    auto writeSerialListMapping = [&]() -> MappingRef {
        QMap<QString, ObjectRef> members;

        // SerialItemList`1+_buf
        members.insert(KEY_NAME_SERIAL_LIST_BUF_1, createReference(idEnq()));

        // _buf
        members.insert(KEY_NAME_SERIAL_LIST_BUF, createReference(idMax)); // no add id

        // Finish
        auto mapping = QSharedPointer<MappingObject>::create();
        mapping->members = std::move(members);

        return mapping;
    };

    auto writeListMapping = [&](qint32 size, qint32 version) -> MappingRef {
        QMap<QString, ObjectRef> members;

        // items
        members.insert(KEY_NAME_LIST_ITEMS, createReference(idEnq()));

        // size
        members.insert(KEY_NAME_LIST_SIZE, createPrimitive(size));

        // version
        members.insert(KEY_NAME_LIST_VERSION, createPrimitive(version));

        // Finish
        auto mapping = QSharedPointer<MappingObject>::create();
        mapping->members = std::move(members);

        return mapping;
    };

    auto writeEnum = [&](qint32 value) -> MappingRef {
        QMap<QString, ObjectRef> members;
        members.insert(KEY_NAME_ENUM_VALUE, createPrimitive(value));

        // Finish
        auto mapping = QSharedPointer<MappingObject>::create();
        mapping->members = std::move(members);

        return mapping;
    };

    auto writeBinaryArray = [&](int size) -> QSharedPointer<ObjectListObject> {
        QList<ObjectRef> res;

        // Add objects
        for (int i = 0; i < size; ++i) {
            res.append(createReference(idEnq()));
        }

        // Finish
        auto listObj = QSharedPointer<ObjectListObject>::create();
        listObj->values = std::move(res);

        return listObj;
    };

    auto encodeParams = [&](const QSharedPointer<XSLineParam> &param) {
        // Encode params
        QByteArray bytes;

        QDataStream in(&bytes, QIODevice::WriteOnly);

        // Write size
        in << param->nodeLinkedList.size();

        // Write values
        for (const auto &node : qAsConst(param->nodeLinkedList)) {
            in << node.Pos;
            in << node.Value;
        }

        QList<quint8> arr;
        arr.reserve(bytes.size());
        for (const auto &byte : bytes) {
            arr.append(byte);
        }

        reg.objectsById.insert(idDeq(), createPrimitiveList(arr, idTmp));
    };

    // Reserved ids
    id_AppModel = ++idMax;
    id_SingingToolModel = ++idMax;
    id_SingingToolLibrary = ++idMax;

    // Write header
    {
        SerializationHeader header;
        header.headerId = -1;
        header.rootId = id_AppModel;
        header.majorVersion = 1;
        header.minorVersion = 0;
        reg.header = QSharedPointer<SerializationHeader>::create(std::move(header));
    }

    // Write libraries
    reg.libraries.insert(id_SingingToolModel, LIBRARY_NAME_SINGING_TOOL_MODEL);
    reg.libraries.insert(id_SingingToolLibrary, LIBRARY_NAME_SINGING_TOOL_LIBRARY);

    /* Hierarchical traverse */

    /* Layer 0 */
    {
        // Write root
        QMap<QString, ObjectRef> members;

        // ActualProjectFilePath <Property> - ignored
        members.insert(NrbfRegistry::toBackingField(KEY_NAME_ACTUAL_PROJECT_FILE_PATH), {});

        // ProjectFilePath <Property>
        members.insert(NrbfRegistry::toBackingField(KEY_NAME_PROJECT_FILE_PATH),
                       createString(appModel.ProjectFilePath, ++idMax));

        // firstNumericalKeyNameAtIndex
        members.insert(KEY_NAME_FIRST_NUMERICAL,
                       createPrimitive(appModel.firstNumericalKeyNameAtIndex));

        // isNumericalKeyName
        members.insert(KEY_NAME_IS_NUMERICAL, createPrimitive(appModel.isNumericalKeyName));

        // isTriplet
        members.insert(KEY_NAME_IS_TRIPLET, createPrimitive(appModel.isTriplet));

        // quantize
        members.insert(KEY_NAME_QUANTIZE, createPrimitive(appModel.quantize));

        // tempoList
        members.insert(KEY_NAME_TEMPO_LIST, createReference(idEnq()));

        // beatList
        members.insert(KEY_NAME_BEAT_LIST, createReference(idEnq()));

        // trackList
        members.insert(KEY_NAME_TRACK_LIST, createReference(idEnq()));

        // Finish
        auto mapping = QSharedPointer<MappingObject>::create();
        mapping->members = std::move(members);

        auto classDef = createClassDef_AppModel(id_AppModel);
        classDef->value = mapping;

        reg.classesById.insert(id_AppModel, classDef);
    }

    /* Deferred items: tempoList, beatList, trackList */

    /* Layer 1 */
    {
        // Write tempoList
        auto classDef = createClassDef_SongTempoList(idDeq());
        classDef->value = writeSerialListMapping();
        reg.classesById.insert(idTmp, classDef);
    }
    {
        // Write beatList
        auto classDef = createClassDef_SongBeatList(idDeq());
        classDef->value = writeSerialListMapping();
        reg.classesById.insert(idTmp, classDef);
    }
    {
        // Write trackList
        auto classDef = createClassDef_TrackList(idDeq());
        classDef->value =
            writeListMapping(appModel.trackList.size(), VALUE_LIST_VERSION_SONG_ITRACK);
        reg.classesById.insert(idTmp, classDef);
    }

    /* Deferred items: tempoListBuf, beatListBuf, trackList contents */

    /* Layer 2 */
    {
        // Write tempoList buf
        auto classDef = createClassDef_SongTempoListBuf(idDeq());
        classDef->value =
            writeListMapping(appModel.tempoList.size(), VALUE_LIST_VERSION_SONG_TEMPO);
        reg.classesById.insert(idTmp, classDef);
    }
    {
        // Write beatList buf
        auto classDef = createClassDef_SongBeatListBuf(idDeq());
        classDef->value = writeListMapping(appModel.beatList.size(), VALUE_LIST_VERSION_SONG_BEAT);
        reg.classesById.insert(idTmp, classDef);
    }
    {
        // Write trackList content
        reg.objectsById.insert(idDeq(), writeBinaryArray(appModel.trackList.size()));
    }

    /* Deferred items: tempoList contents, beatList contents, tracks */

    /* Layer 3 */
    {
        // Write tempoList content
        reg.objectsById.insert(idDeq(), writeBinaryArray(appModel.tempoList.size()));
    }
    {
        // Write beatList content
        reg.objectsById.insert(idDeq(), writeBinaryArray(appModel.beatList.size()));
    }

    auto writeITrack = [&](const QSharedPointer<XSITrack> &track,
                           QMap<QString, ObjectRef> &members) {
        members.insert(KEY_NAME_TRACK_VOLUME, createPrimitive(track->volume));
        members.insert(KEY_NAME_TRACK_PAN, createPrimitive(track->pan));
        members.insert(KEY_NAME_TRACK_NAME, createString(track->name, ++idMax));
        members.insert(KEY_NAME_TRACK_MUTE, createPrimitive(track->mute));
        members.insert(KEY_NAME_TRACK_SOLO, createPrimitive(track->solo));
    };

    std::list<QSharedPointer<XSSingingTrack>> singingTracks;
    {
        // Write tracks
        for (const auto &item : qAsConst(appModel.trackList)) {
            if (item->type() == XSITrack::Singing) {
                auto singingTrack = item.dynamicCast<XSSingingTrack>();

                // Write SingingTrack
                QMap<QString, ObjectRef> members;

                // Base part
                writeITrack(singingTrack, members);

                // AISingerId <Property>
                members.insert(NrbfRegistry::toBackingField(KEY_NAME_AI_SINGER_ID),
                               createString(singingTrack->AISingerId, ++idMax));

                // needRefreshBaseMetadataFlag
                members.insert(KEY_NAME_NEED_REFRESH_FLAG,
                               createPrimitive(singingTrack->needRefreshBaseMetadataFlag));

                // reverbPreset
                {
                    auto reverbPresetClassRef = createClassDef_ReverbPreset(idEnq());
                    reverbPresetClassRef->value = writeEnum(singingTrack->reverbPreset);

                    members.insert(KEY_NAME_REVERB_PRESET, reverbPresetClassRef);
                }

                // Notes
                members.insert(KEY_NAME_NOTE_LIST, createReference(idEnq()));

                // Breath
                members.insert(KEY_NAME_EDITED_BREATH_LINE, singingTrack->editedBreathLine.isNull()
                                                                ? ObjectRef()
                                                                : createReference(idEnq()));

                // Gender
                members.insert(KEY_NAME_EDITED_GENDER_LINE, singingTrack->editedGenderLine.isNull()
                                                                ? ObjectRef()
                                                                : createReference(idEnq()));

                // Pitch
                members.insert(KEY_NAME_EDITED_PITCH_LINE, singingTrack->editedPitchLine.isNull()
                                                               ? ObjectRef()
                                                               : createReference(idEnq()));

                // Volume
                members.insert(KEY_NAME_EDITED_VOLUME_LINE, singingTrack->editedVolumeLine.isNull()
                                                                ? ObjectRef()
                                                                : createReference(idEnq()));

                // Power
                members.insert(KEY_NAME_EDITED_POWER_LINE, singingTrack->editedPowerLine.isNull()
                                                               ? ObjectRef()
                                                               : createReference(idEnq()));

                singingTracks.push_back(singingTrack);

                // Finish
                auto mapping = QSharedPointer<MappingObject>::create();
                mapping->members = std::move(members);

                auto classRef = createClassDef_SingingTrack(idDeq());
                classRef->value = mapping;

                reg.classesById.insert(idTmp, classRef);
            } else {
                auto instrumentTrack = item.dynamicCast<XSInstrumentTrack>();

                // Write InstrumentTrack
                QMap<QString, ObjectRef> members;

                // Base part
                writeITrack(instrumentTrack, members);

                // SampleRate <Property>
                members.insert(NrbfRegistry::toBackingField(KEY_NAME_SAMPLE_RATE),
                               createPrimitive(instrumentTrack->SampleRate));

                // SampleCount <Property>
                members.insert(NrbfRegistry::toBackingField(KEY_NAME_SAMPLE_COUNT),
                               createPrimitive(instrumentTrack->SampleCount));

                // ChannelCount <Property>
                members.insert(NrbfRegistry::toBackingField(KEY_NAME_CHANNEL_COUNT),
                               createPrimitive(instrumentTrack->ChannelCount));

                // OffsetInPos <Property>
                members.insert(NrbfRegistry::toBackingField(KEY_NAME_OFFSET_IN_POS),
                               createPrimitive(instrumentTrack->OffsetInPos));

                // InstrumentFilePath <Property>
                members.insert(NrbfRegistry::toBackingField(KEY_NAME_INSTRUMENT_FILE_PATH),
                               createString(instrumentTrack->InstrumentFilePath, ++idMax));

                // Finish
                auto mapping = QSharedPointer<MappingObject>::create();
                mapping->members = std::move(members);

                auto classRef = createClassDef_InstrumentTrack(idDeq());
                classRef->value = mapping;

                reg.classesById.insert(idTmp, classRef);
            }
        }
    }

    /* Deferred items: tempos, beats, noteLists and editedLines */

    /* Layer 4 */
    std::list<XSBeatSize> beatSizes;
    {
        // Write tempos
        for (const auto &item : qAsConst(appModel.tempoList)) {
            // Write tempo
            QMap<QString, ObjectRef> members;

            // Overlapped <Property>
            members.insert(NrbfRegistry::toBackingField(KEY_NAME_OVERLAPPED),
                           createPrimitive(item.Overlapped));
            // pos
            members.insert(KEY_NAME_TEMPO_POS, createPrimitive(item.pos));

            // tempo
            members.insert(KEY_NAME_TEMPO, createPrimitive(item.tempo));

            // Finish
            auto mapping = QSharedPointer<MappingObject>::create();
            mapping->members = std::move(members);

            auto classRef = createClassDef_SongTempo(idDeq());
            classRef->value = mapping;

            reg.classesById.insert(idTmp, classRef);
        }
    }
    {
        // Write beats
        for (const auto &item : qAsConst(appModel.beatList)) {
            // Write beat
            QMap<QString, ObjectRef> members;

            // Overlapped <Property>
            members.insert(NrbfRegistry::toBackingField(KEY_NAME_OVERLAPPED),
                           createPrimitive(item.Overlapped));

            // barIndex
            members.insert(KEY_NAME_BAR_INDEX, createPrimitive(item.barIndex));

            // beatSize
            members.insert(KEY_NAME_BEAT_SIZE,
                           (beatSizes.push_back(item.beatSize), createReference(idEnq())));

            // Finish
            auto mapping = QSharedPointer<MappingObject>::create();
            mapping->members = std::move(members);

            auto classRef = createClassDef_SongBeat(idDeq());
            classRef->value = mapping;

            reg.classesById.insert(idTmp, classRef);
        }
    }
    {
        // Write noteLists and editedLines
        for (const auto &item : qAsConst(singingTracks)) {
            // Write noteList
            auto classDef = createClassDef_NoteList(idDeq());
            classDef->value = writeSerialListMapping();
            reg.classesById.insert(idTmp, classDef);

            int cnt = 0;
            cnt += !item->editedBreathLine.isNull();
            cnt += !item->editedGenderLine.isNull();
            cnt += !item->editedPitchLine.isNull();
            cnt += !item->editedVolumeLine.isNull();
            cnt += !item->editedPowerLine.isNull();

            for (int i = 0; i < cnt; ++i) {
                // Write lineParam
                QMap<QString, ObjectRef> members;

                members.insert(KEY_NAME_LINE_PARAM, createReference(idEnq()));

                // Finish
                auto mapping = QSharedPointer<MappingObject>::create();
                mapping->members = std::move(members);

                auto classRef = createClassDef_LineParam(idDeq());
                classRef->value = mapping;

                reg.classesById.insert(idTmp, classRef);
            }
        }
    }

    /* Deferred items: beatSizes, noteLists contents and lineParams data */

    /* Layer 5 */
    {
        // Write beatSizes
        for (const auto &item : qAsConst(beatSizes)) {
            // Write beatSize
            QMap<QString, ObjectRef> members;

            // x
            members.insert(KEY_NAME_BEAT_SIZE_X, createPrimitive(item.x));

            // y
            members.insert(KEY_NAME_BEAT_SIZE_Y, createPrimitive(item.y));

            // Finish
            auto mapping = QSharedPointer<MappingObject>::create();
            mapping->members = std::move(members);

            auto classRef = createClassDef_BeatSize(idDeq());
            classRef->value = mapping;

            reg.classesById.insert(idTmp, classRef);
        }
    }
    {
        // Write notesList contents and lineParams data
        for (const auto &item : qAsConst(singingTracks)) {
            // Write noteList
            auto classDef = createClassDef_TrackList(idDeq());
            classDef->value = writeListMapping(item->noteList.size(), VALUE_LIST_VERSION_SONG_NOTE);
            reg.classesById.insert(idTmp, classDef);

            // Write lineParams data
            item->editedBreathLine.isNull() ? void() : encodeParams(item->editedBreathLine);
            item->editedGenderLine.isNull() ? void() : encodeParams(item->editedGenderLine);
            item->editedPitchLine.isNull() ? void() : encodeParams(item->editedPitchLine);
            item->editedVolumeLine.isNull() ? void() : encodeParams(item->editedVolumeLine);
            item->editedPowerLine.isNull() ? void() : encodeParams(item->editedPowerLine);
        }
    }

    /* Deferred items: notes */

    /* Layer 6 */
    {
        for (const auto &track : qAsConst(singingTracks)) {
            for (const auto &note : qAsConst(track->noteList)) {
                // Write note
                QMap<QString, ObjectRef> members;

                // startPos
                members.insert(KEY_NAME_NOTE_START_POS, createPrimitive(note.startPos));

                // widthPos
                members.insert(KEY_NAME_NOTE_WIDTH_POS, createPrimitive(note.widthPos));

                // keyIndex
                members.insert(KEY_NAME_NOTE_KEY_INDEX, createPrimitive(note.keyIndex));

                // lyric
                members.insert(KEY_NAME_NOTE_LYRIC, createString(note.lyric, ++idMax));

                // pronouncing
                members.insert(KEY_NAME_NOTE_PRONOUNCING, createPrimitive(note.pronouncing));

                // Overlapped <Property>
                members.insert(NrbfRegistry::toBackingField(KEY_NAME_OVERLAPPED),
                               createPrimitive(note.Overlapped));

                // VibratoPercent <Property>
                members.insert(NrbfRegistry::toBackingField(KEY_NAME_NOTE_VIBRATO_PERCENT),
                               createPrimitive(note.VibratoPercent));

                // headTag
                {
                    auto headTagClassRef = createClassDef_ReverbPreset(idEnq());
                    headTagClassRef->value = writeEnum(note.headTag);

                    members.insert(KEY_NAME_NOTE_HEAD_TAG, headTagClassRef);
                }

                // NotePhoneInfo <Property>
                members.insert(NrbfRegistry::toBackingField(KEY_NAME_NOTE_PHONEME_INFO),
                               note.NotePhoneInfo.isNull() ? ObjectRef()
                                                           : createReference(idEnq()));

                // Vibrato <Property>
                members.insert(NrbfRegistry::toBackingField(KEY_NAME_NOTE_VIBRATO),
                               note.Vibrato.isNull() ? ObjectRef() : createReference(idEnq()));

                // VibratoPercentInfo <Property>
                members.insert(NrbfRegistry::toBackingField(KEY_NAME_NOTE_VIBRATO_PERCENT_INFO),
                               note.VibratoPercentInfo.isNull() ? ObjectRef()
                                                                : createReference(idEnq()));

                // Finish
                auto mapping = QSharedPointer<MappingObject>::create();
                mapping->members = std::move(members);

                auto classRef = createClassDef_Note(idDeq());
                classRef->value = mapping;

                reg.classesById.insert(idTmp, classRef);
            }
        }
    }

    /* Deferred items: note properties */

    /* Layer 7 */
    std::list<QSharedPointer<XSLineParam>> vibratos;
    {
        for (const auto &track : qAsConst(singingTracks)) {
            for (const auto &note : qAsConst(track->noteList)) {
                // Write NotePhoneInfo
                if (!note.NotePhoneInfo.isNull()) {
                    QMap<QString, ObjectRef> members;

                    // HeadPhoneTimeInSec <Property>
                    members.insert(NrbfRegistry::toBackingField(KEY_NAME_HEAD_PHONEME_TIME),
                                   createPrimitive(note.NotePhoneInfo->HeadPhoneTimeInSec));

                    // MidPartOverTailPartRatio <Property>
                    members.insert(
                        NrbfRegistry::toBackingField(KEY_NAME_MID_PART_OVER_TAIL_PART_RATIO),
                        createPrimitive(note.NotePhoneInfo->MidPartOverTailPartRatio));

                    // Finish
                    auto mapping = QSharedPointer<MappingObject>::create();
                    mapping->members = std::move(members);

                    auto classRef = createClassDef_NotePhoneInfo(idDeq());
                    classRef->value = mapping;

                    reg.classesById.insert(idTmp, classRef);
                }

                // Write Vibrato
                if (!note.Vibrato.isNull()) {
                    QMap<QString, ObjectRef> members;

                    // IsAntiPhase <Property>
                    members.insert(NrbfRegistry::toBackingField(KEY_NAME_VIBRATO_ANTI_PHASE),
                                   createPrimitive(note.Vibrato->IsAntiPhase));

                    // ampLine
                    members.insert(KEY_NAME_VIBRATO_AMP_LINE,
                                   note.Vibrato->ampLine.isNull()
                                       ? ObjectRef()
                                       : (vibratos.push_back(note.Vibrato->ampLine),
                                          createReference(idEnq())));

                    // freqLine
                    members.insert(KEY_NAME_VIBRATO_FREQ_LINE,
                                   note.Vibrato->freqLine.isNull()
                                       ? ObjectRef()
                                       : (vibratos.push_back(note.Vibrato->freqLine),
                                          createReference(idEnq())));

                    auto mapping = QSharedPointer<MappingObject>::create();
                    mapping->members = std::move(members);

                    auto classRef = createClassDef_VibratoStyle(idDeq());
                    classRef->value = mapping;

                    reg.classesById.insert(idTmp, classRef);
                }

                // Write VibratoPercentInfo
                if (!note.VibratoPercentInfo.isNull()) {
                    QMap<QString, ObjectRef> members;

                    // startPercent
                    members.insert(KEY_NAME_VIBRATO_START_PERCENT,
                                   createPrimitive(note.VibratoPercentInfo->startPercent));

                    // endPercent
                    members.insert(KEY_NAME_VIBRATO_END_PERCENT,
                                   createPrimitive(note.VibratoPercentInfo->endPercent));

                    // Finish
                    auto mapping = QSharedPointer<MappingObject>::create();
                    mapping->members = std::move(members);

                    auto classRef = createClassDef_VibratoPercentInfo(idDeq());
                    classRef->value = mapping;

                    reg.classesById.insert(idTmp, classRef);
                }
            }
        }
    }

    /* Deferred items: vibrato properties */

    /* Layer 8 */
    {
        // Write lineParams
        for (const auto &item : vibratos) {
            QMap<QString, ObjectRef> members;

            members.insert(KEY_NAME_LINE_PARAM, createReference(idEnq()));

            // Finish
            auto mapping = QSharedPointer<MappingObject>::create();
            mapping->members = std::move(members);

            auto classRef = createClassDef_LineParam(idDeq());
            classRef->value = mapping;

            reg.classesById.insert(idTmp, classRef);
        }
    }

    /* Deferred items: lineParam data */

    /* Layer 9 */
    {
        // Write lineParams data
        for (const auto &item : vibratos) {
            encodeParams(item);
        }
    }

    return true;
}

QSharedPointer<UserClassTypeObject> SvipWriter::createClassDef_ReverbPreset(qint32 objectId) {
    ClassInfo classInfo;
    classInfo.objectId = objectId;
    classInfo.name = ASSEMBLY_NAME_REVERB_PRESET;
    classInfo.memberCount = 1;
    classInfo.memberNames = QStringList({KEY_NAME_ENUM_VALUE});

    MemberTypeInfo memberTypeInfo;
    memberTypeInfo.binaryTypeEnums = {BinaryTypeEnumeration::Primitive};
    memberTypeInfo.additionalInfos = {PrimitiveTypeEnumeration::Int32};

    auto obj = QSharedPointer<UserClassTypeObject>::create();
    obj->classInfo = std::move(classInfo);
    obj->memberTypeInfo = std::move(memberTypeInfo);
    obj->libraryId = id_SingingToolLibrary;
    return obj;
}

QSharedPointer<UserClassTypeObject> SvipWriter::createClassDef_NoteHeadTag(qint32 objectId) {
    ClassInfo classInfo;
    classInfo.objectId = objectId;
    classInfo.name = ASSEMBLY_NAME_NOTE_HEAD_TAG;
    classInfo.memberCount = 1;
    classInfo.memberNames = QStringList({KEY_NAME_ENUM_VALUE});

    MemberTypeInfo memberTypeInfo;
    memberTypeInfo.binaryTypeEnums = {BinaryTypeEnumeration::Primitive};
    memberTypeInfo.additionalInfos = {PrimitiveTypeEnumeration::Int32};

    auto obj = QSharedPointer<UserClassTypeObject>::create();
    obj->classInfo = std::move(classInfo);
    obj->memberTypeInfo = std::move(memberTypeInfo);
    obj->libraryId = id_SingingToolModel;
    return obj;
}

QSharedPointer<UserClassTypeObject> SvipWriter::createClassDef_AppModel(qint32 objectId) {
    ClassInfo classInfo;
    classInfo.objectId = objectId;
    classInfo.name = ASSEMBLY_NAME_APP_MODEL;
    classInfo.memberCount = 9;
    classInfo.memberNames = QStringList({
        NrbfRegistry::toBackingField(KEY_NAME_PROJECT_FILE_PATH),
        KEY_NAME_TEMPO_LIST,
        KEY_NAME_BEAT_LIST,
        KEY_NAME_TRACK_LIST,
        KEY_NAME_QUANTIZE,
        KEY_NAME_IS_TRIPLET,
        KEY_NAME_IS_NUMERICAL,
        KEY_NAME_FIRST_NUMERICAL,
        NrbfRegistry::toBackingField(KEY_NAME_ACTUAL_PROJECT_FILE_PATH),
    });

    MemberTypeInfo memberTypeInfo;
    memberTypeInfo.binaryTypeEnums = {
        BinaryTypeEnumeration::String,    BinaryTypeEnumeration::Class,
        BinaryTypeEnumeration::Class,     BinaryTypeEnumeration::SystemClass,
        BinaryTypeEnumeration::Primitive, BinaryTypeEnumeration::Primitive,
        BinaryTypeEnumeration::Primitive, BinaryTypeEnumeration::Primitive,
        BinaryTypeEnumeration::String,
    };
    memberTypeInfo.additionalInfos = {
        {},
        ClassTypeInfo(ASSEMBLY_NAME_TEMPO_LIST, id_SingingToolLibrary),
        ClassTypeInfo(ASSEMBLY_NAME_BEAT_LIST, id_SingingToolLibrary),
        PrimitiveTypeEnumeration::Int32,
        PrimitiveTypeEnumeration::Boolean,
        PrimitiveTypeEnumeration::Boolean,
        PrimitiveTypeEnumeration::Int32,
        {},
    };

    auto obj = QSharedPointer<UserClassTypeObject>::create();
    obj->classInfo = std::move(classInfo);
    obj->memberTypeInfo = std::move(memberTypeInfo);
    obj->libraryId = id_SingingToolModel;
    return obj;
}

QSharedPointer<UserClassTypeObject> SvipWriter::createClassDef_SongTempoList(qint32 objectId) {
    ClassInfo classInfo;
    classInfo.objectId = objectId;
    classInfo.name = ASSEMBLY_NAME_TEMPO_LIST;
    classInfo.memberCount = 2;
    classInfo.memberNames = QStringList({
        KEY_NAME_SERIAL_LIST_BUF,
        KEY_NAME_SERIAL_LIST_BUF_1,
    });

    MemberTypeInfo memberTypeInfo;
    memberTypeInfo.binaryTypeEnums = {
        BinaryTypeEnumeration::SystemClass,
        BinaryTypeEnumeration::SystemClass,
    };
    memberTypeInfo.additionalInfos = {
        QString(ASSEMBLY_NAME_TEMPO_LIST_BUF),
        QString(ASSEMBLY_NAME_TEMPO_LIST_BUF),
    };

    auto obj = QSharedPointer<UserClassTypeObject>::create();
    obj->classInfo = std::move(classInfo);
    obj->memberTypeInfo = std::move(memberTypeInfo);
    obj->libraryId = id_SingingToolLibrary;
    return obj;
}

QSharedPointer<UserClassTypeObject> SvipWriter::createClassDef_SongBeatList(qint32 objectId) {
    ClassInfo classInfo;
    classInfo.objectId = objectId;
    classInfo.name = ASSEMBLY_NAME_BEAT_LIST;
    classInfo.memberCount = 2;
    classInfo.memberNames = QStringList({
        KEY_NAME_SERIAL_LIST_BUF,
        KEY_NAME_SERIAL_LIST_BUF_1,
    });

    MemberTypeInfo memberTypeInfo;
    memberTypeInfo.binaryTypeEnums = {
        BinaryTypeEnumeration::SystemClass,
        BinaryTypeEnumeration::SystemClass,
    };
    memberTypeInfo.additionalInfos = {
        QString(ASSEMBLY_NAME_BEAT_LIST_BUF),
        QString(ASSEMBLY_NAME_BEAT_LIST_BUF),
    };

    auto obj = QSharedPointer<UserClassTypeObject>::create();
    obj->classInfo = std::move(classInfo);
    obj->memberTypeInfo = std::move(memberTypeInfo);
    obj->libraryId = id_SingingToolLibrary;
    return obj;
}

QSharedPointer<SystemClassTypeObject> SvipWriter::createClassDef_TrackList(qint32 objectId) {
    ClassInfo classInfo;
    classInfo.objectId = objectId;
    classInfo.name = ASSEMBLY_NAME_TRACK_LIST;
    classInfo.memberCount = 3;
    classInfo.memberNames = QStringList({
        KEY_NAME_LIST_ITEMS,
        KEY_NAME_LIST_SIZE,
        KEY_NAME_LIST_VERSION,
    });

    MemberTypeInfo memberTypeInfo;
    memberTypeInfo.binaryTypeEnums = {
        BinaryTypeEnumeration::Class,
        BinaryTypeEnumeration::Primitive,
        BinaryTypeEnumeration::Primitive,
    };
    memberTypeInfo.additionalInfos = {
        ClassTypeInfo(ASSEMBLY_NAME_ITRACK_ARRAY, id_SingingToolModel),
        PrimitiveTypeEnumeration::Int32,
        PrimitiveTypeEnumeration::Int32,
    };

    auto obj = QSharedPointer<SystemClassTypeObject>::create();
    obj->classInfo = std::move(classInfo);
    obj->memberTypeInfo = std::move(memberTypeInfo);
    return obj;
}

QSharedPointer<SystemClassTypeObject> SvipWriter::createClassDef_SongTempoListBuf(qint32 objectId) {
    ClassInfo classInfo;
    classInfo.objectId = objectId;
    classInfo.name = ASSEMBLY_NAME_TEMPO_LIST_BUF;
    classInfo.memberCount = 3;
    classInfo.memberNames = QStringList({
        KEY_NAME_LIST_ITEMS,
        KEY_NAME_LIST_SIZE,
        KEY_NAME_LIST_VERSION,
    });

    MemberTypeInfo memberTypeInfo;
    memberTypeInfo.binaryTypeEnums = {
        BinaryTypeEnumeration::Class,
        BinaryTypeEnumeration::Primitive,
        BinaryTypeEnumeration::Primitive,
    };
    memberTypeInfo.additionalInfos = {
        ClassTypeInfo(ASSEMBLY_NAME_SONG_TEMPO_ARRAY, id_SingingToolModel),
        PrimitiveTypeEnumeration::Int32,
        PrimitiveTypeEnumeration::Int32,
    };

    auto obj = QSharedPointer<SystemClassTypeObject>::create();
    obj->classInfo = std::move(classInfo);
    obj->memberTypeInfo = std::move(memberTypeInfo);
    return obj;
}

QSharedPointer<SystemClassTypeObject> SvipWriter::createClassDef_SongBeatListBuf(qint32 objectId) {
    ClassInfo classInfo;
    classInfo.objectId = objectId;
    classInfo.name = ASSEMBLY_NAME_BEAT_LIST_BUF;
    classInfo.memberCount = 3;
    classInfo.memberNames = QStringList({
        KEY_NAME_LIST_ITEMS,
        KEY_NAME_LIST_SIZE,
        KEY_NAME_LIST_VERSION,
    });

    MemberTypeInfo memberTypeInfo;
    memberTypeInfo.binaryTypeEnums = {
        BinaryTypeEnumeration::Class,
        BinaryTypeEnumeration::Primitive,
        BinaryTypeEnumeration::Primitive,
    };
    memberTypeInfo.additionalInfos = {
        ClassTypeInfo(ASSEMBLY_NAME_SONG_BEAT_ARRAY, id_SingingToolModel),
        PrimitiveTypeEnumeration::Int32,
        PrimitiveTypeEnumeration::Int32,
    };

    auto obj = QSharedPointer<SystemClassTypeObject>::create();
    obj->classInfo = std::move(classInfo);
    obj->memberTypeInfo = std::move(memberTypeInfo);
    return obj;
}

QSharedPointer<UserClassTypeObject> SvipWriter::createClassDef_SingingTrack(qint32 objectId) {
    return QSharedPointer<UserClassTypeObject>();
}
QSharedPointer<UserClassTypeObject> SvipWriter::createClassDef_InstrumentTrack(qint32 objectId) {
    return QSharedPointer<UserClassTypeObject>();
}
QSharedPointer<UserClassTypeObject> SvipWriter::createClassDef_SongTempo(qint32 objectId) {
    return QSharedPointer<UserClassTypeObject>();
}
QSharedPointer<UserClassTypeObject> SvipWriter::createClassDef_SongBeat(qint32 objectId) {
    return QSharedPointer<UserClassTypeObject>();
}
QSharedPointer<UserClassTypeObject> SvipWriter::createClassDef_NoteList(qint32 objectId) {
    return QSharedPointer<UserClassTypeObject>();
}
QSharedPointer<UserClassTypeObject> SvipWriter::createClassDef_LineParam(qint32 objectId) {
    return QSharedPointer<UserClassTypeObject>();
}
QSharedPointer<UserClassTypeObject> SvipWriter::createClassDef_BeatSize(qint32 objectId) {
    return QSharedPointer<UserClassTypeObject>();
}
QSharedPointer<SystemClassTypeObject> SvipWriter::createClassDef_NoteListBuf(qint32 objectId) {
    return QSharedPointer<SystemClassTypeObject>();
}
QSharedPointer<UserClassTypeObject> SvipWriter::createClassDef_Note(qint32 objectId) {
    return QSharedPointer<UserClassTypeObject>();
}
QSharedPointer<UserClassTypeObject> SvipWriter::createClassDef_NotePhoneInfo(qint32 objectId) {
    return QSharedPointer<UserClassTypeObject>();
}
QSharedPointer<UserClassTypeObject> SvipWriter::createClassDef_VibratoStyle(qint32 objectId) {
    return QSharedPointer<UserClassTypeObject>();
}
QSharedPointer<UserClassTypeObject> SvipWriter::createClassDef_VibratoPercentInfo(qint32 objectId) {
    return QSharedPointer<UserClassTypeObject>();
}

QSharedPointer<DeferredReferenceObject> SvipWriter::createReference(qint32 id) {
    return QSharedPointer<DeferredReferenceObject>::create(id);
}

QSharedPointer<StringObject> SvipWriter::createString(const QString &value, qint32 objectId) {
    auto obj = QSharedPointer<StringObject>();
    obj->id = objectId;
    return obj;
}

QSharedPointer<PrimitiveObject> SvipWriter::createPrimitive(const PrimitiveValue &value) {
    return QSharedPointer<PrimitiveObject>::create(value);
}

QSharedPointer<PrimitiveListObject>
    SvipWriter::createPrimitiveList(const PrimitiveValueArray &values, qint32 objectId) {
    auto obj = QSharedPointer<PrimitiveListObject>::create(values);
    obj->id = objectId;
    return obj;
}

QSharedPointer<ObjectListObject> SvipWriter::createObjectList(const QList<ObjectRef> &values,
                                                              qint32 objectId) {
    auto obj = QSharedPointer<ObjectListObject>::create(values);
    obj->id = objectId;
    return obj;
}

QNRBF_END_NAMESPACE