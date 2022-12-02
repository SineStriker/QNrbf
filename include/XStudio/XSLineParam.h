#ifndef QNRBFFORMAT_XSLINEPARAM_H
#define QNRBFFORMAT_XSLINEPARAM_H

#include <list>

#include "qnrbf_global.h"

QNRBF_BEGIN_NAMESPACE

class QNRBF_API XSLineParamNode {
public:
    XSLineParamNode() : Pos(0), Value(0) {
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

    std::list<XSLineParamNode> _nodeLinkedList;
};

QNRBF_END_NAMESPACE


#endif // QNRBFFORMAT_XSLINEPARAM_H
