
#ifndef fstfarscript_EXPORT_H
#define fstfarscript_EXPORT_H

#ifdef fstfar_BUILT_AS_STATIC
#  define fstfarscript_EXPORT
#  define FSTFARSCRIPT_NO_EXPORT
#else
#  ifndef fstfarscript_EXPORT
#    ifdef fstfarscript_EXPORTS
        /* We are building this library */
#      define fstfarscript_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define fstfarscript_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef FSTFARSCRIPT_NO_EXPORT
#    define FSTFARSCRIPT_NO_EXPORT 
#  endif
#endif

#ifndef FSTFARSCRIPT_DEPRECATED
#  define FSTFARSCRIPT_DEPRECATED __declspec(deprecated)
#endif

#ifndef FSTFARSCRIPT_DEPRECATED_EXPORT
#  define FSTFARSCRIPT_DEPRECATED_EXPORT fstfarscript_EXPORT FSTFARSCRIPT_DEPRECATED
#endif

#ifndef FSTFARSCRIPT_DEPRECATED_NO_EXPORT
#  define FSTFARSCRIPT_DEPRECATED_NO_EXPORT FSTFARSCRIPT_NO_EXPORT FSTFARSCRIPT_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef FSTFARSCRIPT_NO_DEPRECATED
#    define FSTFARSCRIPT_NO_DEPRECATED
#  endif
#endif

#endif /* fstfarscript_EXPORT_H */
