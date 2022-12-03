#ifndef QNRBFFORMAT_XSTRACK_H
#define QNRBFFORMAT_XSTRACK_H

#include <QDataStream>

#include "XSLineParam.h"
#include "XSNote.h"
#include "XSReverbPreset.h"

QNRBF_BEGIN_NAMESPACE

class QNRBF_API XSITrack {
public:
    XSITrack() : XSITrack(1) {
    }
    explicit XSITrack(double volume) : volume(volume), pan(0), mute(false), solo(false) {
    }

    enum Type {
        Singing,
        Instrument,
    };

    virtual Type type() const = 0;

    /* Members */
    double volume;
    double pan;
    QString name;
    bool mute;
    bool solo;
};

// SingingTool.Model.SingingTrack
class QNRBF_API XSSingingTrack : public XSITrack {
public:
    XSSingingTrack() : XSITrack(0.7), needRefreshBaseMetadataFlag(false), reverbPreset(NONE) {
    }

    inline Type type() const override {
        return Singing;
    }

    /* Properties */
    QString AISingerId;

    /* Members */
    QList<XSNote> noteList;

    bool needRefreshBaseMetadataFlag;

    QSharedPointer<XSLineParam> editedPitchLine;
    QSharedPointer<XSLineParam> editedVolumeLine;
    QSharedPointer<XSLineParam> editedBreathLine;
    QSharedPointer<XSLineParam> editedGenderLine;
    QSharedPointer<XSLineParam> editedPowerLine;

    XSReverbPreset reverbPreset;
};

// SingingTool.Model.InstrumentTrack
class QNRBF_API XSInstrumentTrack : public XSITrack {
public:
    XSInstrumentTrack()
        : XSITrack(0.3), SampleRate(0), SampleCount(0), ChannelCount(0), OffsetInPos(0) {
    }

    inline Type type() const override {
        return Instrument;
    }

    /* Properties */
    double SampleRate;
    int SampleCount;
    int ChannelCount;
    int OffsetInPos;
    QString InstrumentFilePath;
};

QNRBF_END_NAMESPACE

#endif // QNRBFFORMAT_XSTRACK_H
