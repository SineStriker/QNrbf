#ifndef __QNRBF_GLOBAL_H__
#define __QNRBF_GLOBAL_H__

#include <QtGlobal>

#ifndef QNRBF_API
#  ifdef QNRBF_STATIC
#    define QNRBF_API
#  else
#    ifdef QNRBF_LIBRARY
#      define QNRBF_API Q_DECL_EXPORT
#    else
#      define QNRBF_API Q_DECL_IMPORT
#    endif
#  endif
#endif

#ifdef QNRBF_LIBRARY
#  define QNRBF_INTERNAL
#endif

#define QNRBF_BEGIN_NAMESPACE namespace QNrbf {
#define QNRBF_END_NAMESPACE };
#define QNRBF_USING_NAMESPACE using namespace QNrbf;

#endif // __QNRBF_GLOBAL_H__