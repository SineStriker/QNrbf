#ifndef QNRBFFORMAT_QSVIPCONST_H
#define QNRBFFORMAT_QSVIPCONST_H

#include <QString>

#define Q_FROM_UNICODE(NAME) QString::fromRawData(NAME, sizeof(NAME) / sizeof(QChar))

const QChar ReverbPreset_Dry[] = {0x5e72, 0x58f0};
const QChar ReverbPreset_FloatingLight[] = {0x6d6e, 0x5149};
const QChar ReverbPreset_Afternoon[] = {0x5348, 0x540e};
const QChar ReverbPreset_Moonlight[] = {0x6708, 0x5149};
const QChar ReverbPreset_Cristal[] = {0x6c34, 0x6676};
const QChar ReverbPreset_Soda[] = {0x6c7d, 0x6c34};
const QChar ReverbPreset_Nightingale[] = {0x591c, 0x83ba};
const QChar ReverbPreset_BigDream[] = {0x5927, 0x68a6};

#endif // QNRBFFORMAT_QSVIPCONST_H
