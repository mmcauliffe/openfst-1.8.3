
#ifndef fstfar_EXPORT_H
#define fstfar_EXPORT_H

#ifdef fstfar_BUILT_AS_STATIC
#  define fstfar_EXPORT
#  define FSTFAR_NO_EXPORT
#else
#  ifndef fstfar_EXPORT
#    ifdef fstfar_EXPORTS
        /* We are building this library */
#      define fstfar_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define fstfar_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef FSTFAR_NO_EXPORT
#    define FSTFAR_NO_EXPORT 
#  endif
#endif

#ifndef FSTFAR_DEPRECATED
#  define FSTFAR_DEPRECATED __declspec(deprecated)
#endif

#ifndef FSTFAR_DEPRECATED_EXPORT
#  define FSTFAR_DEPRECATED_EXPORT fstfar_EXPORT FSTFAR_DEPRECATED
#endif

#ifndef FSTFAR_DEPRECATED_NO_EXPORT
#  define FSTFAR_DEPRECATED_NO_EXPORT FSTFAR_NO_EXPORT FSTFAR_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef FSTFAR_NO_DEPRECATED
#    define FSTFAR_NO_DEPRECATED
#  endif
#endif

#endif /* fstfar_EXPORT_H */
