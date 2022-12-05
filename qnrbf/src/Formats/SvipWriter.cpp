#include "SvipWriter.h"

#include "Config/SvipConst.h"

QNRBF_BEGIN_NAMESPACE

SvipWriter::SvipWriter(const QNrbf::XSAppModel &model) : appModel(model) {
}

SvipWriter::~SvipWriter() {
}

bool SvipWriter::save() {
    return false;
}

QNRBF_END_NAMESPACE