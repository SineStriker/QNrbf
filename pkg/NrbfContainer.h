#ifndef QNRBFFORMAT_NRBFCONTAINER_H
#define QNRBFFORMAT_NRBFCONTAINER_H

#include "Common/private/PrimitiveValueData.h"

class QNRBF_INTERNAL QNrbfContainerPrivate : public QNrbf::PrimitiveValueData {
public:
    QNrbfContainerPrivate();
    QNrbfContainerPrivate(const QNrbfContainerPrivate &other);
    QNrbfContainerPrivate(QNrbfContainerPrivate &&other) noexcept;
    ~QNrbfContainerPrivate();

    void zeroMem();
    void clone(const QNrbfContainerPrivate &other);
    void swap(QNrbfContainerPrivate &other);
    void clean();
};

#endif // QNRBFFORMAT_NRBFCONTAINER_H
