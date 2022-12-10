#ifndef QNRBFFORMAT_XSLINEPARAM_H
#define QNRBFFORMAT_XSLINEPARAM_H

#include <list>

#include "qnrbf_global.h"

QNRBF_BEGIN_NAMESPACE

// SingingTool.Model.Line.LineParamNode
class QNRBF_API XSLineParamNode {
public:
    XSLineParamNode() : XSLineParamNode(0, 0) {
    }
    XSLineParamNode(int pos, int value) : Pos(pos), Value(value) {
    }

    /* Properties */
    int Pos;
    int Value;
};

// SingingTool.Model.Line.LineParam
class QNRBF_API XSLineParam {
public:
    /* This class is derived from System.Runtime.Serialization.ISerializable
     * Using custom functions to pack or unpack data
     */

    std::list<XSLineParamNode> nodeLinkedList;

    /* LineParam property of a singing track cannot be null or empty,
     * it should at least contain the following two magic point.
     */
    inline void setDefault() {
        nodeLinkedList = {{-192000, 0}, {1073741823, 0}};
    }
};

QNRBF_END_NAMESPACE


#endif // QNRBFFORMAT_XSLINEPARAM_H
