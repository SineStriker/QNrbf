#ifndef QNRBFFORMAT_XSSONGBEAT_H
#define QNRBFFORMAT_XSSONGBEAT_H

#include "XSOverlappable.h"

QNRBF_BEGIN_NAMESPACE

// SingingTool.Model.SingingGeneralConcept.BeatSize
class QNRBF_API XSBeatSize {
public:
    XSBeatSize() : x(0), y(0){};
    XSBeatSize(int x, int y) : x(x), y(y){};

    /* Members */
    int x;
    int y;
};

// SingingTool.Model.SingingGeneralConcept.SongBeat
class QNRBF_API XSSongBeat : public XSOverlappable {
public:
    XSSongBeat() : barIndex(0) {
    }

    XSSongBeat(int barIndex, int x, int y) : barIndex(barIndex), beatSize(x, y) {
    }

    /* Members */
    int barIndex;
    XSBeatSize beatSize;
};

QNRBF_END_NAMESPACE


#endif // QNRBFFORMAT_XSSONGBEAT_H