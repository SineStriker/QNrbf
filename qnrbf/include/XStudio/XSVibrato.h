#ifndef QNRBFFORMAT_XSVIBRATO_H
#define QNRBFFORMAT_XSVIBRATO_H

#include <QSharedPointer>

#include "XSLineParam.h"

QNRBF_BEGIN_NAMESPACE

// SingingTool.Model.VibratoStyle
class QNRBF_API XSVibratoStyle {
public:
    XSVibratoStyle() : IsAntiPhase(false) {
    }

    /* Properties */
    bool IsAntiPhase;

    /* Members */
    QSharedPointer<XSLineParam> ampLine;
    QSharedPointer<XSLineParam> freqLine;
};

// SingingTool.Model.VibratoPercentInfo
class QNRBF_API XSVibratoPercentInfo {
public:
    XSVibratoPercentInfo() : XSVibratoPercentInfo(0, 100) {
    }

    XSVibratoPercentInfo(float start, float end) : startPercent(start), endPercent(end) {
    }

    /* Members */
    float startPercent;
    float endPercent;
};

QNRBF_END_NAMESPACE

#endif // QNRBFFORMAT_XSVIBRATO_H
