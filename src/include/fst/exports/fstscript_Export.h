
#ifndef fstscript_EXPORT_H
#define fstscript_EXPORT_H

#ifdef fstscript_BUILT_AS_STATIC
#  define fstscript_EXPORT
#  define FSTSCRIPT_NO_EXPORT
#else
#  ifndef fstscript_EXPORT
#    ifdef fstscript_EXPORTS
        /* We are building this library */
#      define fstscript_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define fstscript_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef FSTSCRIPT_NO_EXPORT
#    define FSTSCRIPT_NO_EXPORT 
#  endif
#endif

#ifndef FSTSCRIPT_DEPRECATED
#  define FSTSCRIPT_DEPRECATED __declspec(deprecated)
#endif

#ifndef FSTSCRIPT_DEPRECATED_EXPORT
#  define FSTSCRIPT_DEPRECATED_EXPORT fstscript_EXPORT FSTSCRIPT_DEPRECATED
#endif

#ifndef FSTSCRIPT_DEPRECATED_NO_EXPORT
#  define FSTSCRIPT_DEPRECATED_NO_EXPORT FSTSCRIPT_NO_EXPORT FSTSCRIPT_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef FSTSCRIPT_NO_DEPRECATED
#    define FSTSCRIPT_NO_DEPRECATED
#  endif
#endif

#endif /* fstscript_EXPORT_H */
