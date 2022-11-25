#include "qnrbfstream.h"

#include <QDebug>

#include "Enums/RecordTypeEnumeration.h"
#include "Primitive/LengthPrefixedString.h"

#include "Records/ArraySingleObject.h"
#include "Records/ArraySinglePrimitive.h"
#include "Records/ArraySingleString.h"
#include "Records/BinaryArray.h"
#include "Records/BinaryLibrary.h"
#include "Records/BinaryMethodCall.h"
#include "Records/BinaryMethodReturn.h"
#include "Records/BinaryObjectString.h"
#include "Records/ClassWithId.h"
#include "Records/ClassWithMembers.h"
#include "Records/ClassWithMembersAndTypes.h"
#include "Records/MemberPrimitiveTyped.h"
#include "Records/MemberReference.h"
#include "Records/ObjectNullMultiple.h"
#include "Records/SerializationHeader.h"
#include "Records/SystemClassWithMembers.h"
#include "Records/SystemClassWithMembersAndTypes.h"

QNRBF_USING_NAMESPACE

static void init(QNrbfStream *stream) {
    stream->setByteOrder(QDataStream::LittleEndian);
}

static QString posToStr(qint64 pos) {
    static const int print_base = 16;
    return "0x" + QString::number(pos, print_base).toUpper();
}

QNrbfStream::QNrbfStream() {
    init(this);
}

QNrbfStream::QNrbfStream(QIODevice *dev) : QDataStream(dev) {
    init(this);
}

QNrbfStream::QNrbfStream(QByteArray *in, QIODevice::OpenMode flags) : QDataStream(in, flags) {
    init(this);
}

QNrbfStream::QNrbfStream(const QByteArray &in) : QDataStream(in) {
    init(this);
}

QNrbfStream::~QNrbfStream() {
}

QDataStream &QNrbfStream::operator>>(QString &str) {
    QString res;
    if (!Parser::readString(res, *this)) {
        str.clear();
    } else {
        str = std::move(res);
    }
    return *this;
}

QDataStream &QNrbfStream::operator>>(QNrbfObject &obj) {
    QDataStream &in = *this;
    bool failed = false;
    bool hasHead = false;
    bool reachEnd = false;

    while (!atEnd() && !reachEnd) {
        qint64 startPos = in.device()->pos();

        quint8 recordType;
        in >> recordType;

        switch (recordType) {
            case (quint8) QNrbf::RecordTypeEnumeration::SerializedStreamHeader: {
                if (!hasHead) {
                    QNrbf::SerializationHeader record;
                    if (!record.read(in)) {
                        qDebug().noquote()
                            << QString("QNrbfStream: read SerializedStreamHeader error "
                                       "at %1, start from %2")
                                   .arg(posToStr(device()->pos()), posToStr(startPos));
                        failed = true;
                    } else {
                        // res = WRAPP(record);
                    }
                    hasHead = true;
                } else {
                    qDebug() << "QNrbfStream: multiple stream header";
                    failed = true;
                }
                break;
            }
            case (quint8) QNrbf::RecordTypeEnumeration::ClassWithId: {
                QNrbf::ClassWithId record;
                if (!record.read(in)) {
                    qDebug().noquote()
                        << QString("QNrbfStream: read ClassWithId error at %1, start from %2")
                               .arg(posToStr(device()->pos()), posToStr(startPos));
                    failed = true;
                } else {
                    // res = WRAPP(record);
                }
                break;
            }
            case (quint8) QNrbf::RecordTypeEnumeration::SystemClassWithMembers: {
                QNrbf::SystemClassWithMembers record;
                if (!record.read(in)) {
                    qDebug().noquote() << QString("QNrbfStream: read System error "
                                                  "at %1, start from %2")
                                              .arg(posToStr(device()->pos()), posToStr(startPos));
                    failed = true;
                } else {
                    // res = WRAPP(record);
                }
                break;
            }
            case (quint8) QNrbf::RecordTypeEnumeration::ClassWithMembers: {
                QNrbf::ClassWithMembers record;
                if (!record.read(in)) {
                    qDebug().noquote().noquote()
                        << QString("QNrbfStream: read User error at %1, start from %2")
                               .arg(posToStr(device()->pos()), posToStr(startPos));
                    failed = true;
                } else {
                    // res = WRAPP(record);
                }
                break;
            }
            case (quint8) QNrbf::RecordTypeEnumeration::SystemClassWithMembersAndTypes: {
                QNrbf::SystemClassWithMembersAndTypes record;
                if (!record.read(in)) {
                    qDebug().noquote() << QString("QNrbfStream: read SystemWithTypes "
                                                  "error at %1, start from %2")
                                              .arg(posToStr(device()->pos()), posToStr(startPos));
                    failed = true;
                } else {
                    // res = WRAPP(record);
                }
                break;
            }
            case (quint8) QNrbf::RecordTypeEnumeration::ClassWithMembersAndTypes: {
                QNrbf::ClassWithMembersAndTypes record;
                if (!record.read(in)) {
                    qDebug().noquote() << QString("QNrbfStream: read UserWithTypes "
                                                  "error at %1, start from %2")
                                              .arg(posToStr(device()->pos()), posToStr(startPos));
                    failed = true;
                } else {
                    // res = WRAPP(record);
                }
                break;
            }
            case (quint8) QNrbf::RecordTypeEnumeration::BinaryObjectString: {
                QNrbf::BinaryObjectString record;
                if (!record.read(in)) {
                    qDebug().noquote()
                        << QString(
                               "QNrbfStream: read BinaryObjectString error at %1, start from %2")
                               .arg(posToStr(device()->pos()), posToStr(startPos));
                    failed = true;
                } else {
                    // res = WRAPP(record);
                }
                break;
            }
            case (quint8) QNrbf::RecordTypeEnumeration::BinaryArray: {
                QNrbf::BinaryArray record;
                if (!record.read(in)) {
                    qDebug().noquote()
                        << QString("QNrbfStream: read BinaryArray error at %1, start from %2")
                               .arg(posToStr(device()->pos()), posToStr(startPos));
                    failed = true;
                } else {
                    // res = WRAPP(record);
                }
                break;
            }
            case (quint8) QNrbf::RecordTypeEnumeration::MemberPrimitiveTyped: {
                QNrbf::MemberPrimitiveTyped record;
                if (!record.read(in)) {
                    qDebug().noquote()
                        << QString(
                               "QNrbfStream: read MemberPrimitiveTyped error at %1, start from %2")
                               .arg(posToStr(device()->pos()), posToStr(startPos));
                    failed = true;
                } else {
                    // res = WRAPP(record);
                }
                break;
            }
            case (quint8) QNrbf::RecordTypeEnumeration::MemberReference: {
                QNrbf::MemberReference record;
                if (!record.read(in)) {
                    qDebug().noquote()
                        << QString("QNrbfStream: read MemberReference error at %1, start from %2")
                               .arg(posToStr(device()->pos()), posToStr(startPos));
                    failed = true;
                } else {
                    // res = WRAPP(record);
                }
                break;
            }
            case (quint8) QNrbf::RecordTypeEnumeration::ObjectNull: {
                //            QNrbf::ObjectNull record;
                //            record.placeholder = 0;
                //            // res = WRAPP(record);
                break;
            }
            case (quint8) QNrbf::RecordTypeEnumeration::MessageEnd: {
                reachEnd = true;
                break;
            }
            case (quint8) QNrbf::RecordTypeEnumeration::BinaryLibrary: {
                QNrbf::BinaryLibrary record;
                if (!record.read(in)) {
                    qDebug().noquote()
                        << QString("QNrbfStream: read readBinaryLibrary error at %1, start from %2")
                               .arg(posToStr(device()->pos()), posToStr(startPos));
                    failed = true;
                } else {
                    // res = WRAPP(record);
                }
                break;
            }
            case (quint8) QNrbf::RecordTypeEnumeration::ObjectNullMultiple256: {
                QNrbf::ObjectNullMultiple record;
                if (!record.read(in, true)) {
                    qDebug().noquote()
                        << QString(
                               "QNrbfStream: read ObjectNullMultiple256 error at %1, start from %2")
                               .arg(posToStr(device()->pos()), posToStr(startPos));
                    failed = true;
                } else {
                    // res = WRAPP(record);
                }
                break;
            }
            case (quint8) QNrbf::RecordTypeEnumeration::ObjectNullMultiple: {
                QNrbf::ObjectNullMultiple record;
                if (!record.read(in, false)) {
                    qDebug().noquote()
                        << QString(
                               "QNrbfStream: read ObjectNullMultiple error at %1, start from %2")
                               .arg(posToStr(device()->pos()), posToStr(startPos));
                    failed = true;
                } else {
                    // res = WRAPP(record);
                }
                break;
            }
            case (quint8) QNrbf::RecordTypeEnumeration::ArraySinglePrimitive: {
                QNrbf::ArraySinglePrimitive record;
                if (!record.read(in)) {
                    qDebug().noquote()
                        << QString(
                               "QNrbfStream: read ArraySinglePrimitive error at %1, start from %2")
                               .arg(posToStr(device()->pos()), posToStr(startPos));
                    failed = true;
                } else {
                    // res = WRAPP(record);
                }
                break;
            }
            case (quint8) QNrbf::RecordTypeEnumeration::ArraySingleObject: {
                QNrbf::ArraySingleObject record;
                if (!record.read(in)) {
                    qDebug().noquote()
                        << QString("QNrbfStream: read ArraySingleObject error at %1, start from %2")
                               .arg(posToStr(device()->pos()), posToStr(startPos));
                    failed = true;
                } else {
                    // res = WRAPP(record);
                }
                break;
            }
            case (quint8) QNrbf::RecordTypeEnumeration::ArraySingleString: {
                QNrbf::ArraySingleString record;
                if (!record.read(in)) {
                    qDebug().noquote()
                        << QString("QNrbfStream: read ArraySingleString error at %1, start from %2")
                               .arg(posToStr(device()->pos()), posToStr(startPos));
                    failed = true;
                } else {
                    // res = WRAPP(record);
                }
                break;
            }
            case (quint8) QNrbf::RecordTypeEnumeration::MethodCall: {
                QNrbf::BinaryMethodCall record;
                if (!record.read(in)) {
                    qDebug().noquote()
                        << QString("QNrbfStream: read BinaryMethodCall error at %1, start from %2")
                               .arg(posToStr(device()->pos()), posToStr(startPos));
                    failed = true;
                } else {
                    // res = WRAPP(record);
                }
                break;
            }
            case (quint8) QNrbf::RecordTypeEnumeration::MethodReturn: {
                QNrbf::BinaryMethodReturn record;
                if (!record.read(in)) {
                    qDebug().noquote()
                        << QString(
                               "QNrbfStream: read BinaryMethodReturn error at %1, start from %2")
                               .arg(posToStr(device()->pos()), posToStr(startPos));
                    failed = true;
                } else {
                    // res = WRAPP(record);
                }
                break;
            }
            default:
                break;
        }
        if (failed) {
            if (status() == QDataStream::Ok) {
                setStatus(QDataStream::ReadCorruptData);
            }
            break;
        }
    }

    return *this;
}
