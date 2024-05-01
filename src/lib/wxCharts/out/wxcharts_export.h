
#ifndef WXCHARTS_EXPORT_H
#define WXCHARTS_EXPORT_H

#ifdef WXCHARTS_STATIC_DEFINE
#  define WXCHARTS_EXPORT
#  define WXCHARTS_NO_EXPORT
#else
#  ifndef WXCHARTS_EXPORT
#    ifdef wxcharts_EXPORTS
        /* We are building this library */
#      define WXCHARTS_EXPORT 
#    else
        /* We are using this library */
#      define WXCHARTS_EXPORT 
#    endif
#  endif

#  ifndef WXCHARTS_NO_EXPORT
#    define WXCHARTS_NO_EXPORT 
#  endif
#endif

#ifndef WXCHARTS_DEPRECATED
#  define WXCHARTS_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef WXCHARTS_DEPRECATED_EXPORT
#  define WXCHARTS_DEPRECATED_EXPORT WXCHARTS_EXPORT WXCHARTS_DEPRECATED
#endif

#ifndef WXCHARTS_DEPRECATED_NO_EXPORT
#  define WXCHARTS_DEPRECATED_NO_EXPORT WXCHARTS_NO_EXPORT WXCHARTS_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef WXCHARTS_NO_DEPRECATED
#    define WXCHARTS_NO_DEPRECATED
#  endif
#endif

#endif /* WXCHARTS_EXPORT_H */
