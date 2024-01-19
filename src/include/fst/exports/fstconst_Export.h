
#ifndef fstconst_EXPORT_H
#define fstconst_EXPORT_H

#ifdef fstconst_BUILT_AS_STATIC
#  define fstconst_EXPORT
#  define FSTCONST_NO_EXPORT
#else
#  ifndef fstconst_EXPORT
#    ifdef fstconst_EXPORTS
        /* We are building this library */
#      define fstconst_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define fstconst_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef FSTCONST_NO_EXPORT
#    define FSTCONST_NO_EXPORT 
#  endif
#endif

#ifndef FSTCONST_DEPRECATED
#  define FSTCONST_DEPRECATED __declspec(deprecated)
#endif

#ifndef FSTCONST_DEPRECATED_EXPORT
#  define FSTCONST_DEPRECATED_EXPORT fstconst_EXPORT FSTCONST_DEPRECATED
#endif

#ifndef FSTCONST_DEPRECATED_NO_EXPORT
#  define FSTCONST_DEPRECATED_NO_EXPORT FSTCONST_NO_EXPORT FSTCONST_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef FSTCONST_NO_DEPRECATED
#    define FSTCONST_NO_DEPRECATED
#  endif
#endif

#endif /* fstconst_EXPORT_H */
