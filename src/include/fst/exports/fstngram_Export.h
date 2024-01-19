
#ifndef fstngram_EXPORT_H
#define fstngram_EXPORT_H

#ifdef fstngram_BUILT_AS_STATIC
#  define fstngram_EXPORT
#  define FSTNGRAM_NO_EXPORT
#else
#  ifndef fstngram_EXPORT
#    ifdef fstngram_EXPORTS
        /* We are building this library */
#      define fstngram_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define fstngram_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef FSTNGRAM_NO_EXPORT
#    define FSTNGRAM_NO_EXPORT 
#  endif
#endif

#ifndef FSTNGRAM_DEPRECATED
#  define FSTNGRAM_DEPRECATED __declspec(deprecated)
#endif

#ifndef FSTNGRAM_DEPRECATED_EXPORT
#  define FSTNGRAM_DEPRECATED_EXPORT fstngram_EXPORT FSTNGRAM_DEPRECATED
#endif

#ifndef FSTNGRAM_DEPRECATED_NO_EXPORT
#  define FSTNGRAM_DEPRECATED_NO_EXPORT FSTNGRAM_NO_EXPORT FSTNGRAM_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef FSTNGRAM_NO_DEPRECATED
#    define FSTNGRAM_NO_DEPRECATED
#  endif
#endif

#endif /* fstngram_EXPORT_H */
