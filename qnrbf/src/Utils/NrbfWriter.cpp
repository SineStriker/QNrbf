#include "NrbfWriter.h"

QNRBF_BEGIN_NAMESPACE

NrbfWriter::NrbfWriter(QDataStream *stream) : stream(stream) {
}

NrbfWriter::NrbfWriter(QDataStream *stream, const NrbfRegistry &reg) : reg(reg), stream(stream) {
}

NrbfWriter::~NrbfWriter() {
}

void NrbfWriter::setRegistry(const NrbfRegistry &reg) {
    this->reg = reg;
}

bool NrbfWriter::write() {
    if (reg.header.isNull()) {
        return false;
    }

    QDataStream &out = *stream;

    // Write header
    if (!reg.header->write(out)) {
        qDebug() << "NrbfWriter: Failed to write header";
        return false;
    }

    // Write libraries
    for (auto it = reg.libraries.begin(); it != reg.libraries.end(); ++it) {
        BinaryLibrary record;
        record.libraryId = it.key();
        record.libraryName = it.value();

        if (!record.write(out)) {
            qDebug() << "NrbfWriter: Failed to write header";
            return false;
        }
    }

    // Write objects


    return false;
}

QNRBF_END_NAMESPACE