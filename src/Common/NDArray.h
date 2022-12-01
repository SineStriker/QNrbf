#ifndef QNRBFFORMAT_NDARRAY_H
#define QNRBFFORMAT_NDARRAY_H

#include <QList>
#include <QSharedDataPointer>

#include "qnrbf_global.h"

QNRBF_BEGIN_NAMESPACE

class NDArrayData;

class QNRBF_INTERNAL NDArray {
public:
    NDArray(int blockSize, const QList<int> &lengths);
    NDArray(int blockSize, const QList<int> &lengths, const QList<int> &lowerBounds);
    ~NDArray();

protected:
    QSharedDataPointer<NDArrayData> d;
};

QNRBF_END_NAMESPACE


#endif // QNRBFFORMAT_NDARRAY_H
