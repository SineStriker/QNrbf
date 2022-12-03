#ifndef __QSVIP_H__
#define __QSVIP_H__

#include "qsvip_global.h"

extern "C" {

QSVIP_API void qsvip_reader_init(char *buf, int size);

QSVIP_API void qsvip_reader_free();

QSVIP_API bool qsvip_reader_load();

QSVIP_API int qsvip_reader_alloc_output();

QSVIP_API int qsvip_reader_alloc_error();

QSVIP_API char *qsvip_reader_get_output();

QSVIP_API char *qsvip_reader_get_error();
}

#endif // __QSVIP_H__