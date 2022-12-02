#ifndef QNRBFFORMAT_XSNOTE_H
#define QNRBFFORMAT_XSNOTE_H

#include "XSVibrato.h"

QNRBF_BEGIN_NAMESPACE

// SingingTool.Model.XSNoteHeadTag
enum XSNoteHeadTag {
    NoTag,
    SilTag,
    SpTag,
};

// SingingTool.Model.NotePhoneInfo
class QNRBF_API XSNotePhoneInfo {
public:
    XSNotePhoneInfo() : HeadPhoneTimeInSec(0), MidPartOverTailPartRatio(0) {
    }

    /* Properties */
    float HeadPhoneTimeInSec;
    float MidPartOverTailPartRatio;
};

// SingingTool.Model.Note
class QNRBF_API XSNote {
public:
    XSNote() : VibratoPercent(0), startPos(0), widthPos(480), keyIndex(60), headTag(NoTag) {
    }

    /* Properties */
    XSNotePhoneInfo NotePhoneInfo;

    int VibratoPercent;
    XSVibratoStyle Vibrato;
    XSVibratoPercentInfo VibratoPercentInfo;

    /* Members */
    int startPos;
    int widthPos;
    int keyIndex;
    QString lyric;
    QString pronouncing;
    XSNoteHeadTag headTag;
};

QNRBF_END_NAMESPACE

#endif // QNRBFFORMAT_XSNOTE_H
