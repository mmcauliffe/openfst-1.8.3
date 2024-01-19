
#ifndef fstspecial_EXPORT_H
#define fstspecial_EXPORT_H

#ifdef fstspecial_BUILT_AS_STATIC
#  define fstspecial_EXPORT
#  define FSTSPECIAL_NO_EXPORT
#else
#  ifndef fstspecial_EXPORT
#    ifdef fstspecial_EXPORTS
        /* We are building this library */
#      define fstspecial_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define fstspecial_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef FSTSPECIAL_NO_EXPORT
#    define FSTSPECIAL_NO_EXPORT 
#  endif
#endif

#ifndef FSTSPECIAL_DEPRECATED
#  define FSTSPECIAL_DEPRECATED __declspec(deprecated)
#endif

#ifndef FSTSPECIAL_DEPRECATED_EXPORT
#  define FSTSPECIAL_DEPRECATED_EXPORT fstspecial_EXPORT FSTSPECIAL_DEPRECATED
#endif

#ifndef FSTSPECIAL_DEPRECATED_NO_EXPORT
#  define FSTSPECIAL_DEPRECATED_NO_EXPORT FSTSPECIAL_NO_EXPORT FSTSPECIAL_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef FSTSPECIAL_NO_DEPRECATED
#    define FSTSPECIAL_NO_DEPRECATED
#  endif
#endif

#endif /* fstspecial_EXPORT_H */
