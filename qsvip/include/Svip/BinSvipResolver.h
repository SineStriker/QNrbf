#ifndef QNRBFFORMAT_BINSVIPRESOLVER_H
#define QNRBFFORMAT_BINSVIPRESOLVER_H

#include "QSvipModel.h"
#include "XStudio/XSAppModel.h"

class QSVIP_API BinSvipResolver {
public:
    bool load(const QNrbf::XSAppModel &in, QSvipModel &out, const QVariantMap &options);
    bool save(const QSvipModel &in, QNrbf::XSAppModel &out, const QVariantMap &options);
};

#endif // QNRBFFORMAT_BINSVIPRESOLVER_H
