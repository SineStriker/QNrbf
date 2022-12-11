#ifndef __QNRBF_GLOBAL_H__
#define __QNRBF_GLOBAL_H__

#ifdef __cplusplus
#  include <QDataStream>
#  include <QIODevice>
#else
#  ifdef _WIN32
#    define Q_DECL_EXPORT __declspec(dllexport)
#    define Q_DECL_IMPORT __declspec(dllimport)
#  else
#    define Q_DECL_EXPORT
#    define Q_DECL_IMPORT
#  endif
#endif

#ifndef QNRBF_EXPORT
#  ifdef QNRBF_STATIC
#    define QNRBF_EXPORT
#  else
#    ifdef QNRBF_LIBRARY
#      define QNRBF_EXPORT Q_DECL_EXPORT
#    else
#      define QNRBF_EXPORT Q_DECL_IMPORT
#    endif
#  endif
#endif

#ifdef QNRBF_ONLY_C_STYLE
#  define QNRBF_API
#  define QNRBF_C_API QNRBF_EXPORT
#else
#  define QNRBF_API QNRBF_EXPORT
#  define QNRBF_C_API QNRBF_EXPORT
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
#  define QNRBF_EXTERN_C_BEGIN extern "C" {
#  define QNRBF_EXTERN_C_END };
#else
#  define QNRBF_EXTERN_C_BEGIN
#  define QNRBF_EXTERN_C_END
#endif

#endif // __QNRBF_GLOBAL_H__