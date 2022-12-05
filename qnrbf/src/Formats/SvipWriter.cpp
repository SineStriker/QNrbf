#include "SvipWriter.h"

#include "Config/SvipConst.h"

#include "Records/SerializationHeader.h"

QNRBF_BEGIN_NAMESPACE

SvipWriter::SvipWriter(const QNrbf::XSAppModel &model) : appModel(model) {
}

SvipWriter::~SvipWriter() {
}

bool SvipWriter::save() {
    int idMax = 1;

    // Write header
    SerializationHeader header;
    header.headerId = -1;
    header.rootId = idMax;
    header.majorVersion = 1;
    header.minorVersion = 0;
    reg.header = QSharedPointer<SerializationHeader>::create(std::move(header));

    // Root object


    return false;
}

QNRBF_END_NAMESPACE