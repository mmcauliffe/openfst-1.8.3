
#ifndef fst_EXPORT_H
#define fst_EXPORT_H

#ifdef fst_BUILT_AS_STATIC
#  define fst_EXPORT
#  define FST_NO_EXPORT
#else
#  ifndef fst_EXPORT
#    ifdef fst_EXPORTS
        /* We are building this library */
#      define fst_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define fst_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef FST_NO_EXPORT
#    define FST_NO_EXPORT 
#  endif
#endif

#ifndef FST_DEPRECATED
#  define FST_DEPRECATED __declspec(deprecated)
#endif

#ifndef FST_DEPRECATED_EXPORT
#  define FST_DEPRECATED_EXPORT fst_EXPORT FST_DEPRECATED
#endif

#ifndef FST_DEPRECATED_NO_EXPORT
#  define FST_DEPRECATED_NO_EXPORT FST_NO_EXPORT FST_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef FST_NO_DEPRECATED
#    define FST_NO_DEPRECATED
#  endif
#endif

#endif /* fst_EXPORT_H */
