
#ifndef fstmpdtscript_EXPORT_H
#define fstmpdtscript_EXPORT_H

#ifdef fstmpdtscript_BUILT_AS_STATIC
#  define fstmpdtscript_EXPORT
#  define FSTMPDTSCRIPT_NO_EXPORT
#else
#  ifndef fstmpdtscript_EXPORT
#    ifdef fstmpdtscript_EXPORTS
        /* We are building this library */
#      define fstmpdtscript_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define fstmpdtscript_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef FSTMPDTSCRIPT_NO_EXPORT
#    define FSTMPDTSCRIPT_NO_EXPORT 
#  endif
#endif

#ifndef FSTMPDTSCRIPT_DEPRECATED
#  define FSTMPDTSCRIPT_DEPRECATED __declspec(deprecated)
#endif

#ifndef FSTMPDTSCRIPT_DEPRECATED_EXPORT
#  define FSTMPDTSCRIPT_DEPRECATED_EXPORT fstmpdtscript_EXPORT FSTMPDTSCRIPT_DEPRECATED
#endif

#ifndef FSTMPDTSCRIPT_DEPRECATED_NO_EXPORT
#  define FSTMPDTSCRIPT_DEPRECATED_NO_EXPORT FSTMPDTSCRIPT_NO_EXPORT FSTMPDTSCRIPT_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef FSTMPDTSCRIPT_NO_DEPRECATED
#    define FSTMPDTSCRIPT_NO_DEPRECATED
#  endif
#endif

#endif /* fstmpdtscript_EXPORT_H */
