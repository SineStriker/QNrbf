#ifndef QNRBFFORMAT_QNRBFVALUE_H
#define QNRBFFORMAT_QNRBFVALUE_H

#include <QExplicitlySharedDataPointer>
#include <QJsonObject>
#include <QSet>
#include <QVariant>

#include "Primitive/DateTime.h"
#include "Primitive/Decimal.h"
#include "Primitive/TimeSpan.h"

class QNrbfObject;
class QNrbfContainerPrivate;

class QNRBF_API QNrbfValue {
public:
    QNrbfValue();
    QNrbfValue(const QNrbfValue &other);
    QNrbfValue &operator=(const QNrbfValue &other);
    ~QNrbfValue();

    enum Type {
        Invalid,
        Null,
        Boolean,
        Byte,
        Char,
        Decimal,
        Double,
        Int16,
        Int32,
        Int64,
        SByte,
        Single,
        TimeSpan,
        DateTime,
        UInt16,
        UInt32,
        UInt64,
        String,
        Object,
    };

    QNrbfValue(Type type);
    QNrbfValue(bool b);
    QNrbfValue(quint8 uc);
    QNrbfValue(const QChar &ch);
    QNrbfValue(const QNrbf::Decimal &dec);
    QNrbfValue(double d);
    QNrbfValue(qint16 s);
    QNrbfValue(qint32 i);
    QNrbfValue(qint64 l);
    QNrbfValue(qint8 c);
    QNrbfValue(float f);
    QNrbfValue(const QNrbf::TimeSpan &ts);
    QNrbfValue(const QNrbf::DateTime &dt);
    QNrbfValue(quint16 us);
    QNrbfValue(quint32 u);
    QNrbfValue(quint64 ul);
    QNrbfValue(const QString &str);
    QNrbfValue(const QNrbfObject &obj);

    QNrbfValue(Type type, int size);
    QNrbfValue(const QList<bool> &bs);
    QNrbfValue(const QList<quint8> &ucs);
    QNrbfValue(const QList<QChar> &chs);
    QNrbfValue(const QList<QNrbf::Decimal> &decs);
    QNrbfValue(const QList<double> &ds);
    QNrbfValue(const QList<qint16> &ss);
    QNrbfValue(const QList<qint32> &is);
    QNrbfValue(const QList<qint64> &ls);
    QNrbfValue(const QList<qint8> &cs);
    QNrbfValue(const QList<float> &fs);
    QNrbfValue(const QList<QNrbf::TimeSpan> &tss);
    QNrbfValue(const QList<QNrbf::DateTime> &dts);
    QNrbfValue(const QList<quint16> &uss);
    QNrbfValue(const QList<quint32> &us);
    QNrbfValue(const QList<quint64> &uls);
    QNrbfValue(const QStringList &strs);
    QNrbfValue(const QList<QNrbfObject> &objs);

    Type type() const;
    bool isArray() const;

    template <class T>
    T value() const {
        return *reinterpret_cast<T *>(constData());
    }

    template <class T>
    void setValue(const T &val) {
        *this = QNrbfValue(&val);
    }

    template <class T>
    QNrbfValue fromVariant(const T &val) {
        QNrbfValue res;
        res.setValue<T>(val);
        return res;
    }

    // Not suggested to use
    void *data();
    const void *constData() const;
    inline const void *data() const {
        return constData();
    }

protected:
    QExplicitlySharedDataPointer<QNrbfContainerPrivate> d;
};

#endif // QNRBFFORMAT_QNRBFVALUE_H
