#ifndef QNRBF_DATARECORD_H
#define QNRBF_DATARECORD_H

#include "AbstractObject.h"
#include "Common/PrimitiveValue.h"

QNRBF_BEGIN_NAMESPACE

class QNRBF_INTERNAL DataObject : public AbstractObject {
public:
    DataObject();
    explicit DataObject(const PrimitiveValue &data);

public:
    PrimitiveValue data;
};

QNRBF_END_NAMESPACE

#endif // QNRBF_DATARECORD_H
