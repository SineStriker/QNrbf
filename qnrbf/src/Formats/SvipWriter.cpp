#include "SvipWriter.h"

#include <QtMath>

#include "Config/SvipConst.h"

#include "Objects/PrimitiveObject.h"
#include "Objects/StringObject.h"
#include "Records/SerializationHeader.h"

#include "Utils/NrbfHelper.h"

#ifdef QNRBF_ENABLE_EXTRA_PARAMS
#define WRITE_POWER_PARAMS
#endif

QNRBF_BEGIN_NAMESPACE

SvipWriter::SvipWriter(const QNrbf::XSAppModel &model) : appModel(model) {
    classDef_ReverbPreset = 0;       // -24
    classDef_NoteHeadTag = 0;        // -169
    classDef_AppModel = 0;           // 1
    classDef_SongTempoList = 0;      // 5
    classDef_SongBeatList = 0;       // 6
    classDef_TrackList = 0;          // 7
    classDef_SongTempoListBuf = 0;   // 8
    classDef_SongBeatListBuf = 0;    // 9
    classDef_SingingTrack = 0;       // 13
    classDef_InstrumentTrack = 0;    // 14
    classDef_SongTempo = 0;          // 15
    classDef_SongBeat = 0;           // 17
    classDef_NoteList = 0;           // 19
    classDef_LineParam = 0;          // 20
    classDef_BeatSize = 0;           // 29
    classDef_NoteListBuf = 0;        // 31
    classDef_Note = 0;               // 37
    classDef_NotePhoneInfo = 0;      // 170
    classDef_VibratoStyle = 0;       // 352
    classDef_VibratoPercentInfo = 0; // 353
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

    auto writeBinaryArray = [&](int size, const QString &typeName, qint32 libraryId) {
        QList<ObjectRef> res;

        // Add objects
        for (int i = 0; i < size; ++i) {
            res.append(createReference(idEnq()));
        }

        // Finish
        auto listObj = QSharedPointer<ObjectListObject>::create();
        listObj->values = std::move(res);
        listObj->hasShapeInfo = true;

        auto objId = idDeq();

        // Write binary array
        {
            BinaryArray record;
            record.objectId = objId;
            record.binaryArrayTypeEnum = BinaryArrayTypeEnumeration::Single;
            record.rank = 1;
            record.lengths = {size};
            record.binaryTypeEnum = BinaryTypeEnumeration::Class;
            record.additionInfo = ClassTypeInfo(typeName, libraryId);

            listObj->shapeInfo = std::move(record);
        }

        reg.objectsById.insert(objId, listObj);
    };

    auto encodeParams = [&](const QSharedPointer<XSLineParam> &param) {
        // Encode params
        QByteArray bytes;

        QDataStream in(&bytes, QIODevice::WriteOnly);
#if Q_BYTE_ORDER == Q_LITTLE_ENDIAN
        in.setByteOrder(QDataStream::LittleEndian);
#else
        in.setByteOrder(QDataStream::BigEndian);
#endif

        // Write size
        in << qint32(param->nodeLinkedList.size());

        // Write values
        for (const auto &node : qAsConst(param->nodeLinkedList)) {
            in << node.Pos;
            in << node.Value;
        }

        // Add bytes to pow of 2
        qint32 expectedSize = bytes.size() / 4;
        expectedSize =
            (expectedSize < 64) ? 64 : qint32(qPow(2, std::ceil(std::log2(expectedSize))));
        expectedSize *= 4;

        if (bytes.size() < expectedSize) {
            bytes.append(expectedSize - bytes.size(), 0);
        }

        auto objId = idDeq();
        reg.objectsById.insert(
            objId,
            createPrimitiveList(PrimitiveValueArray(bytes, PrimitiveTypeEnumeration::Byte), objId));
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
        members.insert(Helper::toBackingField(KEY_NAME_ACTUAL_PROJECT_FILE_PATH), {});

        // ProjectFilePath <Property>
        members.insert(Helper::toBackingField(KEY_NAME_PROJECT_FILE_PATH),
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

        auto objId = id_AppModel;
        mapping->id = objId;
        if (classDef_AppModel == 0) {
            auto classDef = createClassDef_AppModel(objId);
            classDef->id = objId;
            classDef->value = mapping;
            reg.classesById.insert(objId, classDef);

            classDef_AppModel = objId;
        } else {
            reg.objectsById.insert(objId, mapping);
        }
        mapping->classId = objId;
    }

    /* Deferred items: tempoList, beatList, trackList */

    /* Layer 1 */
    {
        // Write tempoList
        auto mapping = writeSerialListMapping(); // idEnq
        auto objId = idDeq();
        mapping->id = objId;
        if (classDef_SongTempoList == 0) {
            auto classDef = createClassDef_SongTempoList(objId);
            classDef->id = objId;
            classDef->value = mapping;
            reg.classesById.insert(objId, classDef);

            classDef_SongTempoList = objId;
        } else {
            reg.objectsById.insert(objId, mapping);
        }
    }
    {
        // Write beatList
        auto mapping = writeSerialListMapping(); // idEnq
        auto objId = idDeq();
        mapping->id = objId;
        if (classDef_SongBeatList == 0) {
            auto classDef = createClassDef_SongBeatList(objId);
            classDef->id = objId;
            classDef->value = mapping;
            reg.classesById.insert(objId, classDef);

            classDef_SongBeatList = objId;
        } else {
            reg.objectsById.insert(objId, mapping);
        }
        mapping->classId = classDef_SongBeatList;
    }
    {
        // Write trackList
        auto mapping =
            writeListMapping(appModel.trackList.size(), VALUE_LIST_VERSION_SONG_ITRACK); // idEnq
        auto objId = idDeq();
        mapping->id = objId;
        if (classDef_TrackList == 0) {
            auto classDef = createClassDef_TrackList(objId);
            classDef->id = objId;
            classDef->value = mapping;
            reg.classesById.insert(objId, classDef);

            classDef_TrackList = objId;
        } else {
            reg.objectsById.insert(objId, mapping);
        }
        mapping->classId = classDef_TrackList;
    }

    /* Deferred items: tempoListBuf, beatListBuf, trackList contents */

    /* Layer 2 */
    {
        // Write tempoList buf
        auto mapping =
            writeListMapping(appModel.tempoList.size(), VALUE_LIST_VERSION_SONG_TEMPO); // idEnq
        auto objId = idDeq();
        mapping->id = objId;
        if (classDef_SongTempoListBuf == 0) {
            auto classDef = createClassDef_SongTempoListBuf(objId);
            classDef->id = objId;
            classDef->value = mapping;
            reg.classesById.insert(objId, classDef);

            classDef_SongTempoListBuf = objId;
        } else {
            reg.objectsById.insert(objId, mapping);
        }
        mapping->classId = classDef_SongTempoListBuf;
    }
    {
        // Write beatList buf
        auto mapping =
            writeListMapping(appModel.beatList.size(), VALUE_LIST_VERSION_SONG_BEAT); // idEnq
        auto objId = idDeq();
        mapping->id = objId;
        if (classDef_SongBeatListBuf == 0) {
            auto classDef = createClassDef_SongBeatListBuf(objId);
            classDef->id = objId;
            classDef->value = mapping;
            reg.classesById.insert(objId, classDef);

            classDef_SongBeatListBuf = objId;
        } else {
            reg.objectsById.insert(objId, mapping);
        }
        mapping->classId = classDef_SongBeatListBuf;
    }
    {
        // Write trackList contents
        writeBinaryArray(appModel.trackList.size(), ASSEMBLY_NAME_ITRACK, id_SingingToolModel);
    }

    /* Deferred items: tempoList contents, beatList contents, tracks */

    /* Layer 3 */
    {
        // Write tempoList contents
        writeBinaryArray(appModel.tempoList.size(), ASSEMBLY_NAME_SONG_TEMPO, id_SingingToolModel);
    }
    {
        // Write beatList contents
        writeBinaryArray(appModel.beatList.size(), ASSEMBLY_NAME_SONG_BEAT, id_SingingToolModel);
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

                // needRefreshBaseMetadataFlag
                members.insert(KEY_NAME_NEED_REFRESH_FLAG,
                               createPrimitive(singingTrack->needRefreshBaseMetadataFlag));

                // Notes
                members.insert(KEY_NAME_NOTE_LIST, createReference(idEnq()));

                // Pitch
                members.insert(KEY_NAME_EDITED_PITCH_LINE, singingTrack->editedPitchLine.isNull()
                                                               ? ObjectRef()
                                                               : createReference(idEnq()));

                // Volume
                members.insert(KEY_NAME_EDITED_VOLUME_LINE, singingTrack->editedVolumeLine.isNull()
                                                                ? ObjectRef()
                                                                : createReference(idEnq()));

                // Breath
                members.insert(KEY_NAME_EDITED_BREATH_LINE, singingTrack->editedBreathLine.isNull()
                                                                ? ObjectRef()
                                                                : createReference(idEnq()));

                // Gender
                members.insert(KEY_NAME_EDITED_GENDER_LINE, singingTrack->editedGenderLine.isNull()
                                                                ? ObjectRef()
                                                                : createReference(idEnq()));

#ifdef WRITE_POWER_PARAMS
                members.insert(KEY_NAME_EDITED_POWER_LINE, singingTrack->editedPowerLine.isNull()
                                                               ? ObjectRef()
                                                               : createReference(idEnq()));
#endif

                // reverbPreset
                {
                    auto mapping = writeEnum(singingTrack->reverbPreset);
                    auto objId = -(++idMax);
                    mapping->id = objId;
                    if (classDef_ReverbPreset == 0) {
                        auto reverbPresetClassDef = createClassDef_ReverbPreset(objId);
                        reverbPresetClassDef->id = objId;
                        reverbPresetClassDef->value = mapping;
                        members.insert(KEY_NAME_REVERB_PRESET, reverbPresetClassDef);

                        reg.classesById.insert(objId, reverbPresetClassDef);

                        classDef_ReverbPreset = objId;
                    } else {
                        members.insert(KEY_NAME_REVERB_PRESET, mapping);

                        reg.objectsById.insert(objId, mapping);
                    }
                    mapping->classId = classDef_ReverbPreset;
                }

                // Base part
                writeITrack(singingTrack, members);

                // AISingerId <Property>
                members.insert(Helper::toBackingField(KEY_NAME_AI_SINGER_ID),
                               createString(singingTrack->AISingerId, ++idMax));

                singingTracks.push_back(singingTrack);

                // Finish
                auto mapping = QSharedPointer<MappingObject>::create();
                mapping->members = std::move(members);

                auto objId = idDeq();
                mapping->id = objId;
                if (classDef_SingingTrack == 0) {
                    auto classDef = createClassDef_SingingTrack(objId);
                    classDef->id = objId;
                    classDef->value = mapping;
                    reg.classesById.insert(objId, classDef);

                    classDef_SingingTrack = objId;
                } else {
                    reg.objectsById.insert(objId, mapping);
                }
                mapping->classId = classDef_SingingTrack;
            } else {
                auto instrumentTrack = item.dynamicCast<XSInstrumentTrack>();

                // Write InstrumentTrack
                QMap<QString, ObjectRef> members;

                // Base part
                writeITrack(instrumentTrack, members);

                // SampleRate <Property>
                members.insert(Helper::toBackingField(KEY_NAME_SAMPLE_RATE),
                               createPrimitive(instrumentTrack->SampleRate));

                // SampleCount <Property>
                members.insert(Helper::toBackingField(KEY_NAME_SAMPLE_COUNT),
                               createPrimitive(instrumentTrack->SampleCount));

                // ChannelCount <Property>
                members.insert(Helper::toBackingField(KEY_NAME_CHANNEL_COUNT),
                               createPrimitive(instrumentTrack->ChannelCount));

                // OffsetInPos <Property>
                members.insert(Helper::toBackingField(KEY_NAME_OFFSET_IN_POS),
                               createPrimitive(instrumentTrack->OffsetInPos));

                // InstrumentFilePath <Property>
                members.insert(Helper::toBackingField(KEY_NAME_INSTRUMENT_FILE_PATH),
                               createString(instrumentTrack->InstrumentFilePath, ++idMax));

                // Finish
                auto mapping = QSharedPointer<MappingObject>::create();
                mapping->members = std::move(members);

                auto objId = idDeq();
                mapping->id = objId;
                if (classDef_InstrumentTrack == 0) {
                    auto classDef = createClassDef_InstrumentTrack(objId);
                    classDef->id = objId;
                    classDef->value = mapping;
                    reg.classesById.insert(objId, classDef);

                    classDef_InstrumentTrack = objId;
                } else {
                    reg.objectsById.insert(objId, mapping);
                }
                mapping->classId = classDef_InstrumentTrack;
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
            members.insert(Helper::toBackingField(KEY_NAME_OVERLAPPED),
                           createPrimitive(item.Overlapped));
            // pos
            members.insert(KEY_NAME_TEMPO_POS, createPrimitive(item.pos));

            // tempo
            members.insert(KEY_NAME_TEMPO, createPrimitive(item.tempo));

            // Finish
            auto mapping = QSharedPointer<MappingObject>::create();
            mapping->members = std::move(members);

            auto objId = idDeq();
            mapping->id = objId;
            if (classDef_SongTempo == 0) {
                auto classDef = createClassDef_SongTempo(objId);
                classDef->id = objId;
                classDef->value = mapping;
                reg.classesById.insert(objId, classDef);

                classDef_SongTempo = objId;
            } else {
                reg.objectsById.insert(objId, mapping);
            }
            mapping->classId = classDef_SongTempo;
        }
    }
    {
        // Write beats
        for (const auto &item : qAsConst(appModel.beatList)) {
            // Write beat
            QMap<QString, ObjectRef> members;

            // Overlapped <Property>
            members.insert(Helper::toBackingField(KEY_NAME_OVERLAPPED),
                           createPrimitive(item.Overlapped));

            // barIndex
            members.insert(KEY_NAME_BAR_INDEX, createPrimitive(item.barIndex));

            // beatSize
            members.insert(KEY_NAME_BEAT_SIZE,
                           (beatSizes.push_back(item.beatSize), createReference(idEnq())));

            // Finish
            auto mapping = QSharedPointer<MappingObject>::create();
            mapping->members = std::move(members);

            auto objId = idDeq();
            mapping->id = objId;
            if (classDef_SongBeat == 0) {
                auto classDef = createClassDef_SongBeat(objId);
                classDef->id = objId;
                classDef->value = mapping;
                reg.classesById.insert(objId, classDef);

                classDef_SongBeat = objId;
            } else {
                reg.objectsById.insert(objId, mapping);
            }
            mapping->classId = classDef_SongBeat;
        }
    }
    {
        // Write noteLists and editedLines
        for (const auto &item : qAsConst(singingTracks)) {
            // Write noteList
            {
                auto mapping = writeSerialListMapping();
                auto objId = idDeq();
                mapping->id = objId;
                if (classDef_NoteList == 0) {
                    auto classDef = createClassDef_NoteList(objId);
                    classDef->id = objId;
                    classDef->value = mapping;
                    reg.classesById.insert(objId, classDef);

                    classDef_NoteList = objId;
                } else {
                    reg.objectsById.insert(objId, mapping);
                }
                mapping->classId = classDef_NoteList;
            }

            int cnt = 0;
            cnt += !item->editedPitchLine.isNull();
            cnt += !item->editedVolumeLine.isNull();
            cnt += !item->editedBreathLine.isNull();
            cnt += !item->editedGenderLine.isNull();
#ifdef WRITE_POWER_PARAMS
            cnt += !item->editedPowerLine.isNull();
#endif
            for (int i = 0; i < cnt; ++i) {
                // Write lineParam
                QMap<QString, ObjectRef> members;

                members.insert(KEY_NAME_LINE_PARAM, createReference(idEnq()));

                // Finish
                auto mapping = QSharedPointer<MappingObject>::create();
                mapping->members = std::move(members);

                auto objId = idDeq();
                mapping->id = objId;
                if (classDef_LineParam == 0) {
                    auto classDef = createClassDef_LineParam(objId);
                    classDef->id = objId;
                    classDef->value = mapping;
                    reg.classesById.insert(objId, classDef);

                    classDef_LineParam = objId;
                } else {
                    reg.objectsById.insert(objId, mapping);
                }
                mapping->classId = classDef_LineParam;
            }
        }
    }

    /* Deferred items: beatSizes, noteLists buf and lineParams data */

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

            auto objId = idDeq();
            mapping->id = objId;
            if (classDef_BeatSize == 0) {
                auto classDef = createClassDef_BeatSize(objId);
                classDef->id = objId;
                classDef->value = mapping;
                reg.classesById.insert(objId, classDef);

                classDef_BeatSize = objId;
            } else {
                reg.objectsById.insert(objId, mapping);
            }
            mapping->classId = classDef_BeatSize;
        }
    }
    {
        // Write notesList buf and lineParams data
        for (const auto &item : qAsConst(singingTracks)) {
            // Write noteList contents
            {
                auto mapping =
                    writeListMapping(item->noteList.size(), VALUE_LIST_VERSION_SONG_NOTE);
                auto objId = idDeq();
                mapping->id = objId;
                if (classDef_NoteListBuf == 0) {
                    auto classDef = createClassDef_NoteListBuf(objId);
                    classDef->id = objId;
                    classDef->value = mapping;
                    reg.classesById.insert(objId, classDef);
                } else {
                    reg.objectsById.insert(objId, mapping);
                }
            }

            // Write lineParams data
            item->editedPitchLine.isNull() ? void() : encodeParams(item->editedPitchLine);
            item->editedVolumeLine.isNull() ? void() : encodeParams(item->editedVolumeLine);
            item->editedBreathLine.isNull() ? void() : encodeParams(item->editedBreathLine);
            item->editedGenderLine.isNull() ? void() : encodeParams(item->editedGenderLine);
#ifdef WRITE_POWER_PARAMS
            item->editedPowerLine.isNull() ? void() : encodeParams(item->editedPowerLine);
#endif
        }
    }

    /* Deferred items: noteLists contents */

    /* Layer 6 */
    {
        for (const auto &track : qAsConst(singingTracks)) {
            writeBinaryArray(track->noteList.size(), ASSEMBLY_NAME_NOTE, id_SingingToolModel);
        }
    }

    /* Deferred items: notes */

    /* Layer 7 */
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
                members.insert(KEY_NAME_NOTE_PRONOUNCING, createString(note.pronouncing, ++idMax));

                // Overlapped <Property>
                members.insert(Helper::toBackingField(KEY_NAME_OVERLAPPED),
                               createPrimitive(note.Overlapped));

                // VibratoPercent <Property>
                members.insert(Helper::toBackingField(KEY_NAME_NOTE_VIBRATO_PERCENT),
                               createPrimitive(note.VibratoPercent));

                // headTag
                {
                    auto mapping = writeEnum(note.headTag);
                    auto objId = -(++idMax);
                    mapping->id = objId;
                    if (classDef_NoteHeadTag == 0) {
                        auto headTagClassDef = createClassDef_NoteHeadTag(objId);
                        headTagClassDef->id = objId;
                        headTagClassDef->value = mapping;
                        members.insert(KEY_NAME_NOTE_HEAD_TAG, headTagClassDef);

                        reg.classesById.insert(objId, headTagClassDef);

                        classDef_NoteHeadTag = objId;
                    } else {
                        members.insert(KEY_NAME_NOTE_HEAD_TAG, mapping);

                        reg.objectsById.insert(objId, mapping);
                    }
                    mapping->classId = classDef_NoteHeadTag;
                }

                // NotePhoneInfo <Property>
                members.insert(Helper::toBackingField(KEY_NAME_NOTE_PHONEME_INFO),
                               note.NotePhoneInfo.isNull() ? ObjectRef()
                                                           : createReference(idEnq()));

                // Vibrato <Property>
                members.insert(Helper::toBackingField(KEY_NAME_NOTE_VIBRATO),
                               note.Vibrato.isNull() ? ObjectRef() : createReference(idEnq()));

                // VibratoPercentInfo <Property>
                members.insert(Helper::toBackingField(KEY_NAME_NOTE_VIBRATO_PERCENT_INFO),
                               note.VibratoPercentInfo.isNull() ? ObjectRef()
                                                                : createReference(idEnq()));

                // Finish
                auto mapping = QSharedPointer<MappingObject>::create();
                mapping->members = std::move(members);

                auto objId = idDeq();
                mapping->id = objId;
                if (classDef_Note == 0) {
                    auto classDef = createClassDef_Note(objId);
                    classDef->id = objId;
                    classDef->value = mapping;
                    reg.classesById.insert(objId, classDef);

                    classDef_Note = objId;
                } else {
                    reg.objectsById.insert(objId, mapping);
                }
                mapping->classId = classDef_Note;
            }
        }
    }

    /* Deferred items: note properties */

    /* Layer 8 */
    std::list<QSharedPointer<XSLineParam>> vibratos;
    {
        for (const auto &track : qAsConst(singingTracks)) {
            for (const auto &note : qAsConst(track->noteList)) {
                // Write NotePhoneInfo
                if (!note.NotePhoneInfo.isNull()) {
                    QMap<QString, ObjectRef> members;

                    // HeadPhoneTimeInSec <Property>
                    members.insert(Helper::toBackingField(KEY_NAME_HEAD_PHONEME_TIME),
                                   createPrimitive(note.NotePhoneInfo->HeadPhoneTimeInSec));

                    // MidPartOverTailPartRatio <Property>
                    members.insert(Helper::toBackingField(KEY_NAME_MID_PART_OVER_TAIL_PART_RATIO),
                                   createPrimitive(note.NotePhoneInfo->MidPartOverTailPartRatio));

                    // Finish
                    auto mapping = QSharedPointer<MappingObject>::create();
                    mapping->members = std::move(members);

                    auto objId = idDeq();
                    mapping->id = objId;
                    if (classDef_NotePhoneInfo == 0) {
                        auto classDef = createClassDef_NotePhoneInfo(objId);
                        classDef->id = objId;
                        classDef->value = mapping;
                        reg.classesById.insert(objId, classDef);

                        classDef_NotePhoneInfo = objId;
                    } else {
                        reg.objectsById.insert(objId, mapping);
                    }
                    mapping->classId = classDef_NotePhoneInfo;
                }

                // Write Vibrato
                if (!note.Vibrato.isNull()) {
                    QMap<QString, ObjectRef> members;

                    // IsAntiPhase <Property>
                    members.insert(Helper::toBackingField(KEY_NAME_VIBRATO_ANTI_PHASE),
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

                    auto objId = idDeq();
                    mapping->id = objId;
                    if (classDef_VibratoStyle == 0) {
                        auto classDef = createClassDef_VibratoStyle(objId);
                        classDef->id = objId;
                        classDef->value = mapping;
                        reg.classesById.insert(objId, classDef);

                        classDef_VibratoStyle = objId;
                    } else {
                        reg.objectsById.insert(objId, mapping);
                    }
                    mapping->classId = classDef_VibratoStyle;
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

                    auto objId = idDeq();
                    mapping->id = objId;
                    if (classDef_VibratoPercentInfo == 0) {
                        auto classDef = createClassDef_VibratoPercentInfo(objId);
                        classDef->id = objId;
                        classDef->value = mapping;
                        reg.classesById.insert(objId, classDef);

                        classDef_VibratoPercentInfo = objId;
                    } else {
                        reg.objectsById.insert(objId, mapping);
                    }
                    mapping->classId = classDef_VibratoPercentInfo;
                }
            }
        }
    }

    /* Deferred items: vibrato properties */

    /* Layer 9 */
    {
        // Write lineParams
        for (const auto &item : vibratos) {
            QMap<QString, ObjectRef> members;

            members.insert(KEY_NAME_LINE_PARAM, createReference(idEnq()));

            // Finish
            auto mapping = QSharedPointer<MappingObject>::create();
            mapping->members = std::move(members);

            auto objId = idDeq();
            mapping->id = objId;
            if (classDef_LineParam == 0) {
                auto classDef = createClassDef_LineParam(objId);
                classDef->id = objId;
                classDef->value = mapping;
                reg.classesById.insert(objId, classDef);

                classDef_LineParam = objId;
            } else {
                reg.objectsById.insert(objId, mapping);
            }
            mapping->classId = classDef_LineParam;
        }
    }

    /* Deferred items: lineParam data */

    /* Layer 10 */
    {
        // Write lineParams data
        for (const auto &item : vibratos) {
            encodeParams(item);
        }
    }

    return true;
}

QSharedPointer<UserClassTypeObject> SvipWriter::createClassDef_ReverbPreset(qint32 objectId) const {
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

QSharedPointer<UserClassTypeObject> SvipWriter::createClassDef_NoteHeadTag(qint32 objectId) const {
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

QSharedPointer<UserClassTypeObject> SvipWriter::createClassDef_AppModel(qint32 objectId) const {
    ClassInfo classInfo;
    classInfo.objectId = objectId;
    classInfo.name = ASSEMBLY_NAME_APP_MODEL;
    classInfo.memberCount = 9;
    classInfo.memberNames = QStringList({
        Helper::toBackingField(KEY_NAME_PROJECT_FILE_PATH),
        KEY_NAME_TEMPO_LIST,
        KEY_NAME_BEAT_LIST,
        KEY_NAME_TRACK_LIST,
        KEY_NAME_QUANTIZE,
        KEY_NAME_IS_TRIPLET,
        KEY_NAME_IS_NUMERICAL,
        KEY_NAME_FIRST_NUMERICAL,
        Helper::toBackingField(KEY_NAME_ACTUAL_PROJECT_FILE_PATH),
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
        QString(ASSEMBLY_NAME_TRACK_LIST),
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

QSharedPointer<UserClassTypeObject>
    SvipWriter::createClassDef_SongTempoList(qint32 objectId) const {
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

QSharedPointer<UserClassTypeObject> SvipWriter::createClassDef_SongBeatList(qint32 objectId) const {
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

QSharedPointer<SystemClassTypeObject> SvipWriter::createClassDef_TrackList(qint32 objectId) const {
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

QSharedPointer<SystemClassTypeObject>
    SvipWriter::createClassDef_SongTempoListBuf(qint32 objectId) const {
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

QSharedPointer<SystemClassTypeObject>
    SvipWriter::createClassDef_SongBeatListBuf(qint32 objectId) const {
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

QSharedPointer<UserClassTypeObject> SvipWriter::createClassDef_SingingTrack(qint32 objectId) const {
    ClassInfo classInfo;
    classInfo.objectId = objectId;
    classInfo.name = ASSEMBLY_NAME_SINGING_TRACK;
#ifdef WRITE_POWER_PARAMS
    classInfo.memberCount = 14;
#else
    classInfo.memberCount = 13;
#endif
    classInfo.memberNames = QStringList({
        KEY_NAME_NOTE_LIST,
        KEY_NAME_NEED_REFRESH_FLAG,
        KEY_NAME_EDITED_PITCH_LINE,
        KEY_NAME_EDITED_VOLUME_LINE,
        KEY_NAME_EDITED_BREATH_LINE,
        KEY_NAME_EDITED_GENDER_LINE,
#ifdef WRITE_POWER_PARAMS
        KEY_NAME_EDITED_POWER_LINE,
#endif
        KEY_NAME_REVERB_PRESET,
        KEY_NAME_TRACK_VOLUME,
        KEY_NAME_TRACK_PAN,
        KEY_NAME_TRACK_NAME,
        KEY_NAME_TRACK_MUTE,
        KEY_NAME_TRACK_SOLO,
        Helper::toBackingField(KEY_NAME_AI_SINGER_ID),
    });

    MemberTypeInfo memberTypeInfo;
    memberTypeInfo.binaryTypeEnums = {
        BinaryTypeEnumeration::Class,     BinaryTypeEnumeration::Primitive,
        BinaryTypeEnumeration::Class,     BinaryTypeEnumeration::Class,
        BinaryTypeEnumeration::Class,     BinaryTypeEnumeration::Class,
#ifdef WRITE_POWER_PARAMS
        BinaryTypeEnumeration::Class,
#endif
        BinaryTypeEnumeration::Class,     BinaryTypeEnumeration::Primitive,
        BinaryTypeEnumeration::Primitive, BinaryTypeEnumeration::String,
        BinaryTypeEnumeration::Primitive, BinaryTypeEnumeration::Primitive,
        BinaryTypeEnumeration::String,
    };
    memberTypeInfo.additionalInfos = {
        ClassTypeInfo(ASSEMBLY_NAME_NOTE_LIST, id_SingingToolLibrary),
        PrimitiveTypeEnumeration::Boolean,
        ClassTypeInfo(ASSEMBLY_NAME_LINE_PARAM, id_SingingToolModel),
        ClassTypeInfo(ASSEMBLY_NAME_LINE_PARAM, id_SingingToolModel),
        ClassTypeInfo(ASSEMBLY_NAME_LINE_PARAM, id_SingingToolModel),
        ClassTypeInfo(ASSEMBLY_NAME_LINE_PARAM, id_SingingToolModel),
#ifdef WRITE_POWER_PARAMS
        ClassTypeInfo(ASSEMBLY_NAME_LINE_PARAM, id_SingingToolModel),
#endif
        ClassTypeInfo(ASSEMBLY_NAME_REVERB_PRESET, id_SingingToolLibrary),
        PrimitiveTypeEnumeration::Double,
        PrimitiveTypeEnumeration::Double,
        {},
        PrimitiveTypeEnumeration::Boolean,
        PrimitiveTypeEnumeration::Boolean,
        {},
    };

    auto obj = QSharedPointer<UserClassTypeObject>::create();
    obj->classInfo = std::move(classInfo);
    obj->memberTypeInfo = std::move(memberTypeInfo);
    obj->libraryId = id_SingingToolModel;
    return obj;
}

QSharedPointer<UserClassTypeObject>
    SvipWriter::createClassDef_InstrumentTrack(qint32 objectId) const {
    ClassInfo classInfo;
    classInfo.objectId = objectId;
    classInfo.name = ASSEMBLY_NAME_INSTRUMENT_TRACK;
    classInfo.memberCount = 10;
    classInfo.memberNames = QStringList({
        KEY_NAME_TRACK_VOLUME,
        KEY_NAME_TRACK_PAN,
        KEY_NAME_TRACK_NAME,
        KEY_NAME_TRACK_MUTE,
        KEY_NAME_TRACK_SOLO,
        Helper::toBackingField(KEY_NAME_SAMPLE_RATE),
        Helper::toBackingField(KEY_NAME_SAMPLE_COUNT),
        Helper::toBackingField(KEY_NAME_CHANNEL_COUNT),
        Helper::toBackingField(KEY_NAME_OFFSET_IN_POS),
        Helper::toBackingField(KEY_NAME_INSTRUMENT_FILE_PATH),
    });

    MemberTypeInfo memberTypeInfo;
    memberTypeInfo.binaryTypeEnums = {
        BinaryTypeEnumeration::Primitive, BinaryTypeEnumeration::Primitive,
        BinaryTypeEnumeration::String,    BinaryTypeEnumeration::Primitive,
        BinaryTypeEnumeration::Primitive, BinaryTypeEnumeration::Primitive,
        BinaryTypeEnumeration::Primitive, BinaryTypeEnumeration::Primitive,
        BinaryTypeEnumeration::Primitive, BinaryTypeEnumeration::String,
    };
    memberTypeInfo.additionalInfos = {
        PrimitiveTypeEnumeration::Double,
        PrimitiveTypeEnumeration::Double,
        {},
        PrimitiveTypeEnumeration::Boolean,
        PrimitiveTypeEnumeration::Boolean,
        PrimitiveTypeEnumeration::Double,
        PrimitiveTypeEnumeration::Int32,
        PrimitiveTypeEnumeration::Int32,
        PrimitiveTypeEnumeration::Int32,
        {},
    };

    auto obj = QSharedPointer<UserClassTypeObject>::create();
    obj->classInfo = std::move(classInfo);
    obj->memberTypeInfo = std::move(memberTypeInfo);
    obj->libraryId = id_SingingToolModel;
    return obj;
}

QSharedPointer<UserClassTypeObject> SvipWriter::createClassDef_SongTempo(qint32 objectId) const {
    ClassInfo classInfo;
    classInfo.objectId = objectId;
    classInfo.name = ASSEMBLY_NAME_SONG_TEMPO;
    classInfo.memberCount = 3;
    classInfo.memberNames = QStringList({
        KEY_NAME_TEMPO_POS,
        KEY_NAME_TEMPO,
        Helper::toBackingField(KEY_NAME_OVERLAPPED),
    });

    MemberTypeInfo memberTypeInfo;
    memberTypeInfo.binaryTypeEnums = {
        BinaryTypeEnumeration::Primitive,
        BinaryTypeEnumeration::Primitive,
        BinaryTypeEnumeration::Primitive,
    };
    memberTypeInfo.additionalInfos = {
        PrimitiveTypeEnumeration::Int32,
        PrimitiveTypeEnumeration::Int32,
        PrimitiveTypeEnumeration::Boolean,
    };

    auto obj = QSharedPointer<UserClassTypeObject>::create();
    obj->classInfo = std::move(classInfo);
    obj->memberTypeInfo = std::move(memberTypeInfo);
    obj->libraryId = id_SingingToolModel;
    return obj;
}

QSharedPointer<UserClassTypeObject> SvipWriter::createClassDef_SongBeat(qint32 objectId) const {
    ClassInfo classInfo;
    classInfo.objectId = objectId;
    classInfo.name = ASSEMBLY_NAME_SONG_BEAT;
    classInfo.memberCount = 3;
    classInfo.memberNames = QStringList({
        KEY_NAME_BAR_INDEX,
        KEY_NAME_BEAT_SIZE,
        Helper::toBackingField(KEY_NAME_OVERLAPPED),
    });

    MemberTypeInfo memberTypeInfo;
    memberTypeInfo.binaryTypeEnums = {
        BinaryTypeEnumeration::Primitive,
        BinaryTypeEnumeration::Class,
        BinaryTypeEnumeration::Primitive,
    };
    memberTypeInfo.additionalInfos = {
        PrimitiveTypeEnumeration::Int32,
        ClassTypeInfo(ASSEMBLY_NAME_BEAT_SIZE, id_SingingToolModel),
        PrimitiveTypeEnumeration::Boolean,
    };

    auto obj = QSharedPointer<UserClassTypeObject>::create();
    obj->classInfo = std::move(classInfo);
    obj->memberTypeInfo = std::move(memberTypeInfo);
    obj->libraryId = id_SingingToolModel;
    return obj;
}

QSharedPointer<UserClassTypeObject> SvipWriter::createClassDef_NoteList(qint32 objectId) const {
    ClassInfo classInfo;
    classInfo.objectId = objectId;
    classInfo.name = ASSEMBLY_NAME_NOTE_LIST;
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

QSharedPointer<UserClassTypeObject> SvipWriter::createClassDef_LineParam(qint32 objectId) const {
    ClassInfo classInfo;
    classInfo.objectId = objectId;
    classInfo.name = ASSEMBLY_NAME_LINE_PARAM;
    classInfo.memberCount = 1;
    classInfo.memberNames = QStringList({KEY_NAME_LINE_PARAM});

    MemberTypeInfo memberTypeInfo;
    memberTypeInfo.binaryTypeEnums = {BinaryTypeEnumeration::PrimitiveArray};
    memberTypeInfo.additionalInfos = {PrimitiveTypeEnumeration::Byte};

    auto obj = QSharedPointer<UserClassTypeObject>::create();
    obj->classInfo = std::move(classInfo);
    obj->memberTypeInfo = std::move(memberTypeInfo);
    obj->libraryId = id_SingingToolModel;
    return obj;
}

QSharedPointer<UserClassTypeObject> SvipWriter::createClassDef_BeatSize(qint32 objectId) const {
    ClassInfo classInfo;
    classInfo.objectId = objectId;
    classInfo.name = ASSEMBLY_NAME_BEAT_SIZE;
    classInfo.memberCount = 2;
    classInfo.memberNames = QStringList({
        KEY_NAME_BEAT_SIZE_X,
        KEY_NAME_BEAT_SIZE_Y,
    });

    MemberTypeInfo memberTypeInfo;
    memberTypeInfo.binaryTypeEnums = {
        BinaryTypeEnumeration::Primitive,
        BinaryTypeEnumeration::Primitive,
    };
    memberTypeInfo.additionalInfos = {
        PrimitiveTypeEnumeration::Int32,
        PrimitiveTypeEnumeration::Int32,
    };

    auto obj = QSharedPointer<UserClassTypeObject>::create();
    obj->classInfo = std::move(classInfo);
    obj->memberTypeInfo = std::move(memberTypeInfo);
    obj->libraryId = id_SingingToolModel;
    return obj;
}

QSharedPointer<SystemClassTypeObject>
    SvipWriter::createClassDef_NoteListBuf(qint32 objectId) const {
    ClassInfo classInfo;
    classInfo.objectId = objectId;
    classInfo.name = ASSEMBLY_NAME_NOTE_LIST_BUF;
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
        ClassTypeInfo(ASSEMBLY_NAME_NOTE_ARRAY, id_SingingToolModel),
        PrimitiveTypeEnumeration::Int32,
        PrimitiveTypeEnumeration::Int32,
    };

    auto obj = QSharedPointer<SystemClassTypeObject>::create();
    obj->classInfo = std::move(classInfo);
    obj->memberTypeInfo = std::move(memberTypeInfo);
    return obj;
}

QSharedPointer<UserClassTypeObject> SvipWriter::createClassDef_Note(qint32 objectId) {
    ClassInfo classInfo;
    classInfo.objectId = objectId;
    classInfo.name = ASSEMBLY_NAME_NOTE;
    classInfo.memberCount = 11;
    classInfo.memberNames = QStringList({
        KEY_NAME_NOTE_START_POS,
        KEY_NAME_NOTE_WIDTH_POS,
        KEY_NAME_NOTE_KEY_INDEX,
        KEY_NAME_NOTE_LYRIC,
        KEY_NAME_NOTE_PRONOUNCING,
        KEY_NAME_NOTE_HEAD_TAG,
        Helper::toBackingField(KEY_NAME_OVERLAPPED),
        Helper::toBackingField(KEY_NAME_NOTE_PHONEME_INFO),
        Helper::toBackingField(KEY_NAME_NOTE_VIBRATO_PERCENT),
        Helper::toBackingField(KEY_NAME_NOTE_VIBRATO),
        Helper::toBackingField(KEY_NAME_NOTE_VIBRATO_PERCENT_INFO),
    });

    MemberTypeInfo memberTypeInfo;
    memberTypeInfo.binaryTypeEnums = {
        BinaryTypeEnumeration::Primitive, BinaryTypeEnumeration::Primitive,
        BinaryTypeEnumeration::Primitive, BinaryTypeEnumeration::String,
        BinaryTypeEnumeration::String,    BinaryTypeEnumeration::Class,
        BinaryTypeEnumeration::Primitive, BinaryTypeEnumeration::Class,
        BinaryTypeEnumeration::Primitive, BinaryTypeEnumeration::Class,
        BinaryTypeEnumeration::Class,
    };
    memberTypeInfo.additionalInfos = {
        PrimitiveTypeEnumeration::Int32,
        PrimitiveTypeEnumeration::Int32,
        PrimitiveTypeEnumeration::Int32,
        {},
        {},
        ClassTypeInfo(ASSEMBLY_NAME_NOTE_HEAD_TAG, id_SingingToolModel),
        PrimitiveTypeEnumeration::Boolean,
        ClassTypeInfo(ASSEMBLY_NAME_NOTE_PHONEME_INFO, id_SingingToolModel),
        PrimitiveTypeEnumeration::Int32,
        ClassTypeInfo(ASSEMBLY_NAME_VIBRATO_STYLE, id_SingingToolModel),
        ClassTypeInfo(ASSEMBLY_NAME_VIBRATO_PERCENT_INFO, id_SingingToolModel),
    };

    auto obj = QSharedPointer<UserClassTypeObject>::create();
    obj->classInfo = std::move(classInfo);
    obj->memberTypeInfo = std::move(memberTypeInfo);
    obj->libraryId = id_SingingToolModel;
    return obj;
}

QSharedPointer<UserClassTypeObject>
    SvipWriter::createClassDef_NotePhoneInfo(qint32 objectId) const {
    ClassInfo classInfo;
    classInfo.objectId = objectId;
    classInfo.name = ASSEMBLY_NAME_NOTE_PHONEME_INFO;
    classInfo.memberCount = 2;
    classInfo.memberNames = QStringList({
        Helper::toBackingField(KEY_NAME_HEAD_PHONEME_TIME),
        Helper::toBackingField(KEY_NAME_MID_PART_OVER_TAIL_PART_RATIO),
    });

    MemberTypeInfo memberTypeInfo;
    memberTypeInfo.binaryTypeEnums = {
        BinaryTypeEnumeration::Primitive,
        BinaryTypeEnumeration::Primitive,
    };
    memberTypeInfo.additionalInfos = {
        PrimitiveTypeEnumeration::Single,
        PrimitiveTypeEnumeration::Single,
    };

    auto obj = QSharedPointer<UserClassTypeObject>::create();
    obj->classInfo = std::move(classInfo);
    obj->memberTypeInfo = std::move(memberTypeInfo);
    obj->libraryId = id_SingingToolModel;
    return obj;
}

QSharedPointer<UserClassTypeObject> SvipWriter::createClassDef_VibratoStyle(qint32 objectId) const {
    ClassInfo classInfo;
    classInfo.objectId = objectId;
    classInfo.name = ASSEMBLY_NAME_VIBRATO_STYLE;
    classInfo.memberCount = 3;
    classInfo.memberNames = QStringList({
        KEY_NAME_VIBRATO_AMP_LINE,
        KEY_NAME_VIBRATO_FREQ_LINE,
        Helper::toBackingField(KEY_NAME_VIBRATO_ANTI_PHASE),
    });

    MemberTypeInfo memberTypeInfo;
    memberTypeInfo.binaryTypeEnums = {
        BinaryTypeEnumeration::Class,
        BinaryTypeEnumeration::Class,
        BinaryTypeEnumeration::Primitive,
    };
    memberTypeInfo.additionalInfos = {
        ClassTypeInfo(ASSEMBLY_NAME_LINE_PARAM, id_SingingToolModel),
        ClassTypeInfo(ASSEMBLY_NAME_LINE_PARAM, id_SingingToolModel),
        PrimitiveTypeEnumeration::Boolean,
    };

    auto obj = QSharedPointer<UserClassTypeObject>::create();
    obj->classInfo = std::move(classInfo);
    obj->memberTypeInfo = std::move(memberTypeInfo);
    obj->libraryId = id_SingingToolModel;
    return obj;
}

QSharedPointer<UserClassTypeObject>
    SvipWriter::createClassDef_VibratoPercentInfo(qint32 objectId) const {
    ClassInfo classInfo;
    classInfo.objectId = objectId;
    classInfo.name = ASSEMBLY_NAME_VIBRATO_PERCENT_INFO;
    classInfo.memberCount = 2;
    classInfo.memberNames = QStringList({
        KEY_NAME_VIBRATO_START_PERCENT,
        KEY_NAME_VIBRATO_END_PERCENT,
    });

    MemberTypeInfo memberTypeInfo;
    memberTypeInfo.binaryTypeEnums = {
        BinaryTypeEnumeration::Primitive,
        BinaryTypeEnumeration::Primitive,
    };
    memberTypeInfo.additionalInfos = {
        PrimitiveTypeEnumeration::Single,
        PrimitiveTypeEnumeration::Single,
    };

    auto obj = QSharedPointer<UserClassTypeObject>::create();
    obj->classInfo = std::move(classInfo);
    obj->memberTypeInfo = std::move(memberTypeInfo);
    obj->libraryId = id_SingingToolModel;
    return obj;
}

QSharedPointer<DeferredReferenceObject> SvipWriter::createReference(qint32 id) {
    return QSharedPointer<DeferredReferenceObject>::create(id);
}

QSharedPointer<PrimitiveObject> SvipWriter::createPrimitive(const PrimitiveValue &value) {
    return QSharedPointer<PrimitiveObject>::create(value);
}

QSharedPointer<StringObject> SvipWriter::createString(const QString &value, qint32 objectId) {
    auto obj = QSharedPointer<StringObject>::create(value);
    obj->id = objectId;
    reg.objectsById.insert(objectId, obj);
    return obj;
}

QSharedPointer<PrimitiveListObject>
    SvipWriter::createPrimitiveList(const PrimitiveValueArray &values, qint32 objectId) {
    auto obj = QSharedPointer<PrimitiveListObject>::create(values);
    obj->id = objectId;
    reg.objectsById.insert(objectId, obj);
    return obj;
}

QNRBF_END_NAMESPACE