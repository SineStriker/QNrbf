#ifndef QNRBFFORMAT_XSTRACK_H
#define QNRBFFORMAT_XSTRACK_H

#include <QDataStream>

#include "XSLineParam.h"
#include "XSNote.h"
#include "XSReverbPreset.h"

QNRBF_BEGIN_NAMESPACE

// SingingTool.Model.SingingTrack
class QNRBF_API XSSingingTrack {
public:
    XSSingingTrack()
        : needRefreshBaseMetadataFlag(false), reverbPreset(NONE), volume(0.7), pan(0.7),
          mute(false), solo(false) {
    }

    /* Properties */
    QString AISingerId;

    /* Members */
    QList<XSNote> noteList;

    bool needRefreshBaseMetadataFlag;

    XSLineParam editedPitchLine;
    XSLineParam editedVolumeLine;
    XSLineParam editedBreathLine;
    XSLineParam editedGenderLine;

    XSReverbPreset reverbPreset;

    double volume;
    double pan;
    QString name;
    bool mute;
    bool solo;
};

// SingingTool.Model.InstrumentTrack
class QNRBF_API XSInstrumentTrack {
public:
    XSInstrumentTrack()
        : SampleRate(0), SampleCount(0), ChannelCount(0), OffsetInPos(0), volume(0.3), pan(0),
          mute(false), solo(false) {
    }

    /* Properties */
    double SampleRate;
    int SampleCount;
    int ChannelCount;
    int OffsetInPos;
    QString InstrumentFilePath;

    /* Members */
    double volume;
    double pan;
    QString name;
    bool mute;
    bool solo;
};

QNRBF_END_NAMESPACE

#endif // QNRBFFORMAT_XSTRACK_H
