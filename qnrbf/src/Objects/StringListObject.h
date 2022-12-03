#ifndef QNRBFFORMAT_STRINGLISTOBJECT_H
#define QNRBFFORMAT_STRINGLISTOBJECT_H

#include "AbstractObject.h"

QNRBF_BEGIN_NAMESPACE

class QNRBF_INTERNAL StringListObject : public AbstractObject {
public:
   explicit StringListObject(const QStringList &arr);

   QStringList values;
   QList<int> lengths;
   QList<int> lowerBounds;
};

QNRBF_END_NAMESPACE


#endif // QNRBFFORMAT_STRINGLISTOBJECT_H
