#ifndef QNRBFFORMAT_SVIPWRITER_H
#define QNRBFFORMAT_SVIPWRITER_H

#include "Objects/MappingObject.h"
#include "XStudio/XSAppModel.h"

#include "Utils/NrbfRegistry.h"

QNRBF_BEGIN_NAMESPACE

class QNRBF_API SvipWriter {
public:
    explicit SvipWriter(const XSAppModel &model);
    ~SvipWriter();

    bool save();

    NrbfRegistry reg;

protected:
    XSAppModel appModel;
};

QNRBF_END_NAMESPACE


#endif // QNRBFFORMAT_SVIPWRITER_H
