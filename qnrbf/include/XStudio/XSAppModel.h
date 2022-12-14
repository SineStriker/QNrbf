#ifndef QNRBFFORMAT_XSAPPMODEL_H
#define QNRBFFORMAT_XSAPPMODEL_H

#include "XSSongBeat.h"
#include "XSSongTempo.h"
#include "XSTrack.h"

QNRBF_BEGIN_NAMESPACE

// SingingTool.Model.AppModel
class QNRBF_API XSAppModel {
public:
    XSAppModel()
        : quantize(8), isTriplet(false), isNumericalKeyName(true), firstNumericalKeyNameAtIndex(0) {
    }

    /* Properties */
    QString ProjectFilePath;

    /* Members */
    QList<XSSongTempo> tempoList;
    QList<XSSongBeat> beatList;

    QList<QSharedPointer<XSITrack>> trackList;

    int quantize;
    bool isTriplet;
    bool isNumericalKeyName;          // The word was misspelled in the original code
    int firstNumericalKeyNameAtIndex; // The word was misspelled in the original code
};

QNRBF_END_NAMESPACE

#endif // QNRBFFORMAT_XSAPPMODEL_H
