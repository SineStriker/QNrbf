#ifndef QNRBFFORMAT_SVIPFORMAT_H
#define QNRBFFORMAT_SVIPFORMAT_H

#include "qnrbf_global.h"

#include "Objects/MappingObject.h"
#include "XStudio/XSAppModel.h"

QNRBF_BEGIN_NAMESPACE

class QNRBF_INTERNAL SvipFormat {
public:
    bool load(const ObjectRef &in);
    bool save(ObjectRef &out);

    XSAppModel appModel;

protected:
    static bool readBeat(const QMap<QString, ObjectRef> &members, XSSongBeat &out);
    static bool readTempo(const QMap<QString, ObjectRef> &members, XSSongTempo &out);
    static bool readITrack(const QMap<QString, ObjectRef> &members, XSITrack &out);
    static bool readLineParam(const QMap<QString, ObjectRef> &members, XSLineParam &out);
    static bool readNote(const QMap<QString, ObjectRef> &members, XSNote &out);
    static bool readSerialObjectList(const QMap<QString, ObjectRef> &members, const QString &key,
                                     const QString &assembly1, const QString &name1,
                                     const QString &assembly2, const QString &name2,
                                     QList<ObjectRef> &out);
    static bool readEnum(const QMap<QString, ObjectRef> &members, const QString &key,
                         const QString &assembly, const QString &name, qint32 &out);
};

QNRBF_END_NAMESPACE


#endif // QNRBFFORMAT_SVIPFORMAT_H
