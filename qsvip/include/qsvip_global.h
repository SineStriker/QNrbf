#ifndef __QSVIP_GLOBAL_H__
#define __QSVIP_GLOBAL_H__

#include <QtGlobal>

#ifndef QSVIP_API
#  ifdef QSVIP_STATIC
#    define QSVIP_API
#  else
#    ifdef QSVIP_LIBRARY
#      define QSVIP_API Q_DECL_EXPORT
#    else
#      define QSVIP_API Q_DECL_IMPORT
#    endif
#  endif
#endif

#endif // __QSVIP_GLOBAL_H__