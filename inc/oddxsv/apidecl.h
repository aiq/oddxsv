#ifndef ODDXSV_APIDECL_H
#define ODDXSV_APIDECL_H

#ifdef ODDXSV_STATIC_DEFINE
#  define ODDXSV_API
#  define ODDXSV_NO_EXPORT
#else
#  ifdef _WIN32
#    ifndef ODDXSV_API
#      ifdef ODDXSV_EXPORTS
#        define ODDXSV_API __declspec(dllexport)
#      else
#        define ODDXSV_API __declspec(dllimport)
#      endif
#    endif
#  else
#    define ODDXSV_API
#    define ODDXSV_NO_EXPORT
#  endif
#
#  ifndef ODDXSV_NO_EXPORT
#    define ODDXSV_NO_EXPORT 
#  endif
#endif

#endif