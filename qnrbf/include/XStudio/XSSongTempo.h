#ifndef QNRBFFORMAT_XSSONGTEMPO_H
#define QNRBFFORMAT_XSSONGTEMPO_H

#include "XSOverlappable.h"

QNRBF_BEGIN_NAMESPACE

// SingingTool.Model.SingingGeneralConcept.SongTempo
class QNRBF_API XSSongTempo : public XSIOverlappable {
public:
    XSSongTempo() : pos(0), tempo(120) {
    }

    XSSongTempo(int pos, int tempo) : pos(pos), tempo(tempo) {
    }

    /* Members */
    int pos;
    int tempo;
};

QNRBF_END_NAMESPACE

#endif // QNRBFFORMAT_XSSONGTEMPO_H
