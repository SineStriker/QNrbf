#ifndef QNRBFFORMAT_SVIPWRITER_H
#define QNRBFFORMAT_SVIPWRITER_H

#include "Objects/MappingObject.h"
#include "Objects/SystemClassTypeObject.h"
#include "Objects/UserClassTypeObject.h"

#include "XStudio/XSAppModel.h"

#include "Records/BinaryLibrary.h"
#include "Records/ClassWithMembersAndTypes.h"
#include "Records/SystemClassWithMembersAndTypes.h"

#include "Objects/DeferredReferenceObject.h"
#include "Objects/ObjectListObject.h"
#include "Objects/PrimitiveListObject.h"
#include "Objects/PrimitiveObject.h"
#include "Objects/StringObject.h"
#include "Utils/NrbfRegistry.h"

QNRBF_BEGIN_NAMESPACE

class QNRBF_INTERNAL SvipWriter {
public:
    explicit SvipWriter(const XSAppModel &model);
    ~SvipWriter();

    bool save();

    NrbfRegistry reg;

protected:
    qint32 classDef_ReverbPreset;       // -24
    qint32 classDef_NoteHeadTag;        // -169
    qint32 classDef_AppModel;           // 1
    qint32 classDef_SongTempoList;      // 5
    qint32 classDef_SongBeatList;       // 6
    qint32 classDef_TrackList;          // 7
    qint32 classDef_SongTempoListBuf;   // 8
    qint32 classDef_SongBeatListBuf;    // 9
    qint32 classDef_SingingTrack;       // 13
    qint32 classDef_InstrumentTrack;    // 14
    qint32 classDef_SongTempo;          // 15
    qint32 classDef_SongBeat;           // 17
    qint32 classDef_NoteList;           // 19
    qint32 classDef_LineParam;          // 20
    qint32 classDef_BeatSize;           // 29
    qint32 classDef_NoteListBuf;        // 31
    qint32 classDef_Note;               // 37
    qint32 classDef_NotePhoneInfo;      // 170
    qint32 classDef_VibratoStyle;       // 352
    qint32 classDef_VibratoPercentInfo; // 353

    QSharedPointer<UserClassTypeObject> createClassDef_ReverbPreset(qint32 objectId) const;
    QSharedPointer<UserClassTypeObject> createClassDef_NoteHeadTag(qint32 objectId) const;
    QSharedPointer<UserClassTypeObject> createClassDef_AppModel(qint32 objectId) const;
    QSharedPointer<UserClassTypeObject> createClassDef_SongTempoList(qint32 objectId) const;
    QSharedPointer<UserClassTypeObject> createClassDef_SongBeatList(qint32 objectId) const;
    QSharedPointer<SystemClassTypeObject> createClassDef_TrackList(qint32 objectId) const;
    QSharedPointer<SystemClassTypeObject> createClassDef_SongTempoListBuf(qint32 objectId) const;
    QSharedPointer<SystemClassTypeObject> createClassDef_SongBeatListBuf(qint32 objectId) const;
    QSharedPointer<UserClassTypeObject> createClassDef_SingingTrack(qint32 objectId) const;
    QSharedPointer<UserClassTypeObject> createClassDef_InstrumentTrack(qint32 objectId) const;
    QSharedPointer<UserClassTypeObject> createClassDef_SongTempo(qint32 objectId) const;
    QSharedPointer<UserClassTypeObject> createClassDef_SongBeat(qint32 objectId) const;
    QSharedPointer<UserClassTypeObject> createClassDef_NoteList(qint32 objectId) const;
    QSharedPointer<UserClassTypeObject> createClassDef_LineParam(qint32 objectId) const;
    QSharedPointer<UserClassTypeObject> createClassDef_BeatSize(qint32 objectId) const;
    QSharedPointer<SystemClassTypeObject> createClassDef_NoteListBuf(qint32 objectId) const;
    QSharedPointer<UserClassTypeObject> createClassDef_Note(qint32 objectId);
    QSharedPointer<UserClassTypeObject> createClassDef_NotePhoneInfo(qint32 objectId) const;
    QSharedPointer<UserClassTypeObject> createClassDef_VibratoStyle(qint32 objectId) const;
    QSharedPointer<UserClassTypeObject> createClassDef_VibratoPercentInfo(qint32 objectId) const;

    static QSharedPointer<DeferredReferenceObject> createReference(qint32 id);
    static QSharedPointer<PrimitiveObject> createPrimitive(const PrimitiveValue &value);

    QSharedPointer<StringObject> createString(const QString &value, qint32 objectId);
    QSharedPointer<PrimitiveListObject> createPrimitiveList(const PrimitiveValueArray &values,
                                                            qint32 objectId);

    XSAppModel appModel;

    qint32 id_AppModel;
    qint32 id_SingingToolModel;
    qint32 id_SingingToolLibrary;
};

QNRBF_END_NAMESPACE


#endif // QNRBFFORMAT_SVIPWRITER_H
