#pragma once

// clang-format off
#if defined(_MSC_VER)
    //  Microsoft
    #define _EXPORT __declspec(dllexport)
    #define _IMPORT __declspec(dllimport)
#elif defined(__GNUC__)
    //  GCC
    #define _EXPORT __attribute__((visibility("default")))
    #define _IMPORT
#else
    //  do nothing and hope for the best?
    #define _EXPORT
    #define _IMPORT
    #pragma warning Unknown dynamic link import / export semantics.
#endif
