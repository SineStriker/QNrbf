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
    int idMax = 0;

    // Reserved ids
    const int id_AppModel = ++idMax;
    const int id_SingingToolModel = ++idMax;
    const int id_SingingToolLibrary = ++idMax;

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

    // Write root
    {
        // Write root members
        QMap<QString, ObjectRef> rootMembers;

        // ActualProjectFilePath <Property> - ignored
        rootMembers.insert(NrbfRegistry::toBackingField(KEY_NAME_ACTUAL_PROJECT_FILE_PATH), {});

        // ProjectFilePath <Property>
        rootMembers.insert(NrbfRegistry::toBackingField(KEY_NAME_PROJECT_FILE_PATH),
                           createString(appModel.ProjectFilePath, ++idMax));

        // firstNumericalKeyNameAtIndex
        rootMembers.insert(KEY_NAME_FIRST_NUMERICAL,
                           createPrimitive(appModel.firstNumericalKeyNameAtIndex));

        // isNumericalKeyName
        rootMembers.insert(KEY_NAME_IS_NUMERICAL, createPrimitive(appModel.isNumericalKeyName));

        // isTriplet
        rootMembers.insert(KEY_NAME_IS_TRIPLET, createPrimitive(appModel.isTriplet));

        // quantize
        rootMembers.insert(KEY_NAME_QUANTIZE, createPrimitive(appModel.quantize));

        // tempoList
        MappingRef tempoListRef;
        { auto tempoList = createClassDef_SongTempoList(id_SingingToolModel, ++idMax); }
    }

    return false;
}

QSharedPointer<UserClassTypeObject> SvipWriter::createClassDef_ReverbPreset(qint32 libraryId,
                                                                            qint32 objectId) {
    return QSharedPointer<UserClassTypeObject>();
}
QSharedPointer<UserClassTypeObject> SvipWriter::createClassDef_NoteHeadTag(qint32 libraryId,
                                                                           qint32 objectId) {
    return QSharedPointer<UserClassTypeObject>();
}
QSharedPointer<UserClassTypeObject> SvipWriter::createClassDef_AppModel(qint32 libraryId,
                                                                        qint32 objectId) {
    return QSharedPointer<UserClassTypeObject>();
}
QSharedPointer<UserClassTypeObject> SvipWriter::createClassDef_SongTempoList(qint32 libraryId,
                                                                             qint32 objectId) {
    return QSharedPointer<UserClassTypeObject>();
}
QSharedPointer<UserClassTypeObject> SvipWriter::createClassDef_SongBeatList(qint32 libraryId,
                                                                            qint32 objectId) {
    return QSharedPointer<UserClassTypeObject>();
}
QSharedPointer<SystemClassTypeObject> SvipWriter::createClassDef_TrackList(qint32 libraryId,
                                                                           qint32 objectId) {
    return QSharedPointer<SystemClassTypeObject>();
}
QSharedPointer<SystemClassTypeObject> SvipWriter::createClassDef_SongTempoListBuf(qint32 libraryId,
                                                                                  qint32 objectId) {
    return QSharedPointer<SystemClassTypeObject>();
}
QSharedPointer<SystemClassTypeObject> SvipWriter::createClassDef_SongBeatListBuf(qint32 libraryId,
                                                                                 qint32 objectId) {
    return QSharedPointer<SystemClassTypeObject>();
}
QSharedPointer<UserClassTypeObject> SvipWriter::createClassDef_SingingTrack(qint32 libraryId,
                                                                            qint32 objectId) {
    return QSharedPointer<UserClassTypeObject>();
}
QSharedPointer<UserClassTypeObject> SvipWriter::createClassDef_InstrumentTrack(qint32 libraryId,
                                                                               qint32 objectId) {
    return QSharedPointer<UserClassTypeObject>();
}
QSharedPointer<UserClassTypeObject> SvipWriter::createClassDef_SongTempo(qint32 libraryId,
                                                                         qint32 objectId) {
    return QSharedPointer<UserClassTypeObject>();
}
QSharedPointer<UserClassTypeObject> SvipWriter::createClassDef_SongBeat(qint32 libraryId,
                                                                        qint32 objectId) {
    return QSharedPointer<UserClassTypeObject>();
}
QSharedPointer<UserClassTypeObject> SvipWriter::createClassDef_NoteList(qint32 libraryId,
                                                                        qint32 objectId) {
    return QSharedPointer<UserClassTypeObject>();
}
QSharedPointer<UserClassTypeObject> SvipWriter::createClassDef_LineParam(qint32 libraryId,
                                                                         qint32 objectId) {
    return QSharedPointer<UserClassTypeObject>();
}
QSharedPointer<UserClassTypeObject> SvipWriter::createClassDef_BeatSize(qint32 libraryId,
                                                                        qint32 objectId) {
    return QSharedPointer<UserClassTypeObject>();
}
QSharedPointer<SystemClassTypeObject> SvipWriter::createClassDef_NoteListBuf(qint32 libraryId,
                                                                             qint32 objectId) {
    return QSharedPointer<SystemClassTypeObject>();
}
QSharedPointer<UserClassTypeObject> SvipWriter::createClassDef_Note(qint32 libraryId,
                                                                    qint32 objectId) {
    return QSharedPointer<UserClassTypeObject>();
}
QSharedPointer<UserClassTypeObject> SvipWriter::createClassDef_NotePhoneInfo(qint32 libraryId,
                                                                             qint32 objectId) {
    return QSharedPointer<UserClassTypeObject>();
}
QSharedPointer<UserClassTypeObject> SvipWriter::createClassDef_VibratoStyle(qint32 libraryId,
                                                                            qint32 objectId) {
    return QSharedPointer<UserClassTypeObject>();
}
QSharedPointer<UserClassTypeObject> SvipWriter::createClassDef_VibratoPercentInfo(qint32 libraryId,
                                                                                  qint32 objectId) {
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