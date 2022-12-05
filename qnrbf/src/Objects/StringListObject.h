#ifndef QNRBFFORMAT_STRINGLISTOBJECT_H
#define QNRBFFORMAT_STRINGLISTOBJECT_H

#include "ListObject.h"

QNRBF_BEGIN_NAMESPACE

class QNRBF_INTERNAL StringListObject : public ListObject {
public:
   explicit StringListObject(const QStringList &arr);

   QStringList values;
};

QNRBF_END_NAMESPACE


#endif // QNRBFFORMAT_STRINGLISTOBJECT_H
