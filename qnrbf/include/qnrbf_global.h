#ifndef __QNRBF_GLOBAL_H__
#define __QNRBF_GLOBAL_H__

#ifdef __cplusplus
#  include <QDataStream>
#  include <QIODevice>
#endif

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

#ifndef QNRBF_INTERNAL
#  if defined(QNRBF_LIBRARY) || defined(QNRBF_STATIC)
#    define QNRBF_INTERNAL
#  endif
#endif

#ifdef __cplusplus
#  define QNRBF_BEGIN_NAMESPACE namespace QNrbf {
#  define QNRBF_END_NAMESPACE };
#  define QNRBF_USING_NAMESPACE using namespace QNrbf;
#endif

#endif // __QNRBF_GLOBAL_H__