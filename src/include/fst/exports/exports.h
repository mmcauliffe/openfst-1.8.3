
#ifndef exports_H
#define exports_H

#ifdef _WIN32
    #include <fst/exports/fst_Export.h>
    #include <fst/exports/fstcompact_Export.h>
    #include <fst/exports/fstcompressscript_Export.h>
    #include <fst/exports/fstconst_Export.h>
    #include <fst/exports/fstfarscript_Export.h>
    #include <fst/exports/fstfar_Export.h>
    #include <fst/exports/fstlinearscript_Export.h>
    #include <fst/exports/fstmpdtscript_Export.h>
    #include <fst/exports/fstngram_Export.h>
    #include <fst/exports/fstpdtscript_Export.h>
    #include <fst/exports/fstscript_Export.h>
    #include <fst/exports/fstspecial_Export.h>
#else

    #define fst_EXPORT
    #define fstcompact_EXPORT
    #define fstcompressscript_EXPORT
    #define fstconst_EXPORT
    #define fstconst_EXPORT
    #define fstfar_EXPORT
    #define fstfarscript_EXPORT
    #define fstlinearscript_EXPORT
    #define fstmpdtscript_EXPORT
    #define fstngram_EXPORT
    #define fstpdtscript_EXPORT
    #define fstscript_EXPORT
    #define fstspecial_EXPORT
#endif // _WIN32
#endif