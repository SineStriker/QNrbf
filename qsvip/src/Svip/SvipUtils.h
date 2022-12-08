#ifndef QNRBFFORMAT_SVIPUTILS_H
#define QNRBFFORMAT_SVIPUTILS_H

#include "Svip/QSvipModel.h"
#include "XStudio/XSAppModel.h"

class SvipUtils {
public:
    static QNrbf::XSReverbPreset reverbPresets_name2Index(const QString &name);
    static QString reverbPresets_index2Name(QNrbf::XSReverbPreset index);

    static QNrbf::XSNoteHeadTag noteHeadTag_name2Index(const QString &name);
    static QString noteHeadTag_index2Name(QNrbf::XSNoteHeadTag index);
};


#endif // QNRBFFORMAT_SVIPUTILS_H
