
#ifndef fstlinearscript_EXPORT_H
#define fstlinearscript_EXPORT_H

#ifdef fstlinearscript_BUILT_AS_STATIC
#  define fstlinearscript_EXPORT
#  define FSTLINEARSCRIPT_NO_EXPORT
#else
#  ifndef fstlinearscript_EXPORT
#    ifdef fstlinearscript_EXPORTS
        /* We are building this library */
#      define fstlinearscript_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define fstlinearscript_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef FSTLINEARSCRIPT_NO_EXPORT
#    define FSTLINEARSCRIPT_NO_EXPORT 
#  endif
#endif

#ifndef FSTLINEARSCRIPT_DEPRECATED
#  define FSTLINEARSCRIPT_DEPRECATED __declspec(deprecated)
#endif

#ifndef FSTLINEARSCRIPT_DEPRECATED_EXPORT
#  define FSTLINEARSCRIPT_DEPRECATED_EXPORT fstlinearscript_EXPORT FSTLINEARSCRIPT_DEPRECATED
#endif

#ifndef FSTLINEARSCRIPT_DEPRECATED_NO_EXPORT
#  define FSTLINEARSCRIPT_DEPRECATED_NO_EXPORT FSTLINEARSCRIPT_NO_EXPORT FSTLINEARSCRIPT_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef FSTLINEARSCRIPT_NO_DEPRECATED
#    define FSTLINEARSCRIPT_NO_DEPRECATED
#  endif
#endif

#endif /* fstlinearscript_EXPORT_H */
