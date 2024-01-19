
#ifndef fstcompact_EXPORT_H
#define fstcompact_EXPORT_H

#ifdef fstcompact_BUILT_AS_STATIC
#  define fstcompact_EXPORT
#  define FSTCOMPACT_NO_EXPORT
#else
#  ifndef fstcompact_EXPORT
#    ifdef fstcompact_EXPORTS
        /* We are building this library */
#      define fstcompact_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define fstcompact_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef FSTCOMPACT_NO_EXPORT
#    define FSTCOMPACT_NO_EXPORT 
#  endif
#endif

#ifndef FSTCOMPACT_DEPRECATED
#  define FSTCOMPACT_DEPRECATED __declspec(deprecated)
#endif

#ifndef FSTCOMPACT_DEPRECATED_EXPORT
#  define FSTCOMPACT_DEPRECATED_EXPORT fstcompact_EXPORT FSTCOMPACT_DEPRECATED
#endif

#ifndef FSTCOMPACT_DEPRECATED_NO_EXPORT
#  define FSTCOMPACT_DEPRECATED_NO_EXPORT FSTCOMPACT_NO_EXPORT FSTCOMPACT_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef FSTCOMPACT_NO_DEPRECATED
#    define FSTCOMPACT_NO_DEPRECATED
#  endif
#endif

#endif /* fstcompact_EXPORT_H */
