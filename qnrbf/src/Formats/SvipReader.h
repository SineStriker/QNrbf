#ifndef QNRBFFORMAT_SVIPREADER_H
#define QNRBFFORMAT_SVIPREADER_H

#include "Objects/MappingObject.h"
#include "XStudio/XSAppModel.h"

#include "Utils/NrbfRegistry.h"

QNRBF_BEGIN_NAMESPACE

class QNRBF_INTERNAL SvipReader {
public:
    explicit SvipReader(const NrbfRegistry &reg);

    bool load();

    XSAppModel appModel;

protected:
    NrbfRegistry reg;

    bool readBeat(const QMap<QString, ObjectRef> &members, XSSongBeat &out);
    bool readTempo(const QMap<QString, ObjectRef> &members, XSSongTempo &out);
    bool readITrack(const QMap<QString, ObjectRef> &members, XSITrack &out);
    bool readLineParam(const QMap<QString, ObjectRef> &members, XSLineParam &out);
    bool readNote(const QMap<QString, ObjectRef> &members, XSNote &out);
    bool readSerialObjectList(const QMap<QString, ObjectRef> &members, const QString &key,
                              const QString &assembly1, const QString &name1,
                              const QString &assembly2, const QString &name2,
                              QList<ObjectRef> &out);
    bool readEnum(const QMap<QString, ObjectRef> &members, const QString &key,
                  const QString &assembly, const QString &name, qint32 &out);
};

QNRBF_END_NAMESPACE


#endif // QNRBFFORMAT_SVIPREADER_H
