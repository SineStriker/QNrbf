#include "NDArray.h"
#include "private/NDArrayData.h"

QNRBF_BEGIN_NAMESPACE

NDArray::NDArray(int blockSize, const QList<int> &lengths) : d(new NDArrayData()) {
}

NDArray::NDArray(int blockSize, const QList<int> &lengths, const QList<int> &lowerBounds)
    : d(new NDArrayData()) {
}

NDArray::~NDArray() {
}

QNRBF_END_NAMESPACE
