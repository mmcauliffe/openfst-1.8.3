
#ifndef fstcompressscript_EXPORT_H
#define fstcompressscript_EXPORT_H

#ifdef fstcompressscript_BUILT_AS_STATIC
#  define fstcompressscript_EXPORT
#  define FSTCOMPRESSSCRIPT_NO_EXPORT
#else
#  ifndef fstcompressscript_EXPORT
#    ifdef fstcompressscript_EXPORTS
        /* We are building this library */
#      define fstcompressscript_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define fstcompressscript_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef FSTCOMPRESSSCRIPT_NO_EXPORT
#    define FSTCOMPRESSSCRIPT_NO_EXPORT 
#  endif
#endif

#ifndef FSTCOMPRESSSCRIPT_DEPRECATED
#  define FSTCOMPRESSSCRIPT_DEPRECATED __declspec(deprecated)
#endif

#ifndef FSTCOMPRESSSCRIPT_DEPRECATED_EXPORT
#  define FSTCOMPRESSSCRIPT_DEPRECATED_EXPORT fstcompressscript_EXPORT FSTCOMPRESSSCRIPT_DEPRECATED
#endif

#ifndef FSTCOMPRESSSCRIPT_DEPRECATED_NO_EXPORT
#  define FSTCOMPRESSSCRIPT_DEPRECATED_NO_EXPORT FSTCOMPRESSSCRIPT_NO_EXPORT FSTCOMPRESSSCRIPT_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef FSTCOMPRESSSCRIPT_NO_DEPRECATED
#    define FSTCOMPRESSSCRIPT_NO_DEPRECATED
#  endif
#endif

#endif /* fstcompressscript_EXPORT_H */
