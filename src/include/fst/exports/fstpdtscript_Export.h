
#ifndef fstpdtscript_EXPORT_H
#define fstpdtscript_EXPORT_H

#ifdef fstpdtscript_BUILT_AS_STATIC
#  define fstpdtscript_EXPORT
#  define FSTPDTSCRIPT_NO_EXPORT
#else
#  ifndef fstpdtscript_EXPORT
#    ifdef fstpdtscript_EXPORTS
        /* We are building this library */
#      define fstpdtscript_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define fstpdtscript_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef FSTPDTSCRIPT_NO_EXPORT
#    define FSTPDTSCRIPT_NO_EXPORT 
#  endif
#endif

#ifndef FSTPDTSCRIPT_DEPRECATED
#  define FSTPDTSCRIPT_DEPRECATED __declspec(deprecated)
#endif

#ifndef FSTPDTSCRIPT_DEPRECATED_EXPORT
#  define FSTPDTSCRIPT_DEPRECATED_EXPORT fstpdtscript_EXPORT FSTPDTSCRIPT_DEPRECATED
#endif

#ifndef FSTPDTSCRIPT_DEPRECATED_NO_EXPORT
#  define FSTPDTSCRIPT_DEPRECATED_NO_EXPORT FSTPDTSCRIPT_NO_EXPORT FSTPDTSCRIPT_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef FSTPDTSCRIPT_NO_DEPRECATED
#    define FSTPDTSCRIPT_NO_DEPRECATED
#  endif
#endif

#endif /* fstpdtscript_EXPORT_H */
