#ifndef QNRBFFORMAT_NRBFWRITER_H
#define QNRBFFORMAT_NRBFWRITER_H

#include "Records/ArraySingleObject.h"
#include "Records/ArraySinglePrimitive.h"
#include "Records/ArraySingleString.h"
#include "Records/BinaryArray.h"
#include "Records/BinaryLibrary.h"
#include "Records/BinaryObjectString.h"
#include "Records/ClassWithId.h"
#include "Records/ClassWithMembers.h"
#include "Records/ClassWithMembersAndTypes.h"
#include "Records/MemberPrimitiveTyped.h"
#include "Records/MemberReference.h"
#include "Records/ObjectNullMultiple.h"
#include "Records/SerializationHeader.h"
#include "Records/SystemClassWithMembers.h"
#include "Records/SystemClassWithMembersAndTypes.h"

#include "NrbfRegistry.h"

QNRBF_BEGIN_NAMESPACE

class QNRBF_INTERNAL NrbfWriter {
public:
    explicit NrbfWriter(QDataStream *stream);
    NrbfWriter(QDataStream *stream, const NrbfRegistry &reg);
    ~NrbfWriter();

    void setRegistry(const NrbfRegistry &reg);

    bool write();

protected:
    bool writeObject(const ObjectRef &objRef);

    // Registry
    NrbfRegistry reg;

    QSet<qint32> writtenObjects;

    // Properties
    QDataStream *stream;

    bool writeMembers(const MappingRef &mapping, const QStringList &memberNames,
                      const MemberTypeInfo &memberTypeInfo);
};

QNRBF_END_NAMESPACE


#endif // QNRBFFORMAT_NRBFWRITER_H
