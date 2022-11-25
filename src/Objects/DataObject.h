#ifndef QNRBF_DATARECORD_H
#define QNRBF_DATARECORD_H

#include "AbstractObject.h"

#include <QVariant>

QNRBF_BEGIN_NAMESPACE

class QNRBF_API DataObject : public AbstractObject {
public:
    DataObject(const QVariant &data) : data(data) {
    }
    QVariant data;
};

QNRBF_END_NAMESPACE

#endif // QNRBF_DATARECORD_H
