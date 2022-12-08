#include "Svip/SvipUtils.h"

#include "Svip/Config/QSvipConst.h"

QNrbf::XSReverbPreset SvipUtils::reverbPresets_name2Index(const QString &name) {
    return QNrbf::DEFAULT;
}

QString SvipUtils::reverbPresets_index2Name(QNrbf::XSReverbPreset index) {
    QString res;
    switch (index) {
        case QNrbf::XSReverbPreset::NONE:
            res = Q_FROM_UNICODE(ReverbPreset_Dry);
            break;
        case QNrbf::XSReverbPreset::DEFAULT:
            res = Q_FROM_UNICODE(ReverbPreset_FloatingLight);
            break;
        case QNrbf::XSReverbPreset::SMALLHALL1:
            res = Q_FROM_UNICODE(ReverbPreset_Afternoon);
            break;
        case QNrbf::XSReverbPreset::MEDIUMHALL1:
            res = Q_FROM_UNICODE(ReverbPreset_Moonlight);
            break;
        case QNrbf::XSReverbPreset::LARGEHALL1:
            res = Q_FROM_UNICODE(ReverbPreset_Cristal);
            break;
        case QNrbf::XSReverbPreset::SMALLROOM1:
            res = Q_FROM_UNICODE(ReverbPreset_Soda);
            break;
        case QNrbf::XSReverbPreset::MEDIUMROOM1:
            res = Q_FROM_UNICODE(ReverbPreset_Nightingale);
            break;
        case QNrbf::XSReverbPreset::LONGREVERB2:
            res = Q_FROM_UNICODE(ReverbPreset_BigDream);
            break;
        default:
            break;
    }
    return res;
}

QNrbf::XSNoteHeadTag SvipUtils::noteHeadTag_name2Index(const QString &name) {
    QNrbf::XSNoteHeadTag res = QNrbf::XSNoteHeadTag::NoTag;
    if (name == "0") {
        res = QNrbf::XSNoteHeadTag::SilTag;
    } else if (name == "V") {
        res = QNrbf::XSNoteHeadTag::SpTag;
    }
    return res;
}

QString SvipUtils::noteHeadTag_index2Name(QNrbf::XSNoteHeadTag index) {
    QString res;
    switch (index) {
        case QNrbf::XSNoteHeadTag::SilTag:
            res = "0";
            break;
        case QNrbf::XSNoteHeadTag::SpTag:
            res = "V";
            break;
        default:
            break;
    }
    return res;
}
