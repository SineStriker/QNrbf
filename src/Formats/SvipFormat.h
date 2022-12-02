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
};

QNRBF_END_NAMESPACE


#endif // QNRBFFORMAT_SVIPFORMAT_H
