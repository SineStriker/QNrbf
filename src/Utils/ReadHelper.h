#ifndef QNRBF_READHELPER_H
#define QNRBF_READHELPER_H

// Private part

#include <QDataStream>
#include <QVariant>

#include "qnrbf_global.h"

#include "Common/BinaryObject.h"

#include "Records/BinaryArray.h"
#include "Records/BinaryObjectString.h"
#include "Records/ClassWithId.h"
#include "Records/ClassWithMembers.h"
#include "Records/ClassWithMembersAndTypes.h"
#include "Records/SystemClassWithMembers.h"
#include "Records/SystemClassWithMembersAndTypes.h"

QNRBF_BEGIN_NAMESPACE

class ReadHelper {
public:
    explicit ReadHelper(QDataStream *stream);
    ~ReadHelper();

    bool onClassWithId(ClassWithId &in, QSharedPointer<AbstractObject> &out);

    bool onSystemClassWithMembers(SystemClassWithMembers &in, QSharedPointer<AbstractObject> &out);

    bool onClassWithMembers(ClassWithMembers &in, QSharedPointer<AbstractObject> &out);

    bool onSystemClassWithMembersAndTypes(SystemClassWithMembersAndTypes &in,
                                          QSharedPointer<AbstractObject> &out);
    bool onClassWithMembersAndTypes(ClassWithMembersAndTypes &in,
                                    QSharedPointer<AbstractObject> &out);
    bool onBinaryObjectString(BinaryObjectString &in, QSharedPointer<AbstractObject> &out);

    bool onBinaryArray(BinaryArray &in, QSharedPointer<AbstractObject> &out);

    QHash<qint32, QSharedPointer<AbstractObject>> objects;
    QHash<qint32, QString> libraries;

protected:
    QDataStream *stream;
};

QNRBF_END_NAMESPACE

#endif // QNRBF_READHELPER_H
