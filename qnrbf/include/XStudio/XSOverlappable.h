#ifndef QNRBFFORMAT_XSOVERLAPPABLE_H
#define QNRBFFORMAT_XSOVERLAPPABLE_H

#include "qnrbf_global.h"

QNRBF_BEGIN_NAMESPACE

// SingingTool.Library.Overlapable
class QNRBF_API XSIOverlappable {
public:
    XSIOverlappable() : Overlapped(false) {
    }

    /* Properties */
    bool Overlapped; // The word was misspelled in the original code
};

QNRBF_END_NAMESPACE


#endif // QNRBFFORMAT_XSOVERLAPPABLE_H
