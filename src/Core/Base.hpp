#pragma once
#include "MessageHandler.hpp"

// Define macro assertion call for VS
#define SK_DEBUGBREAK() __debugbreak ()
#ifdef SK_ENABLE_ASSERTS
#define SK_ASSERT(x, ...)                                                     \
    {                                                                         \
        if (!(x))                                                             \
        {                                                                     \
            qCritical ("Assertion Failed: " __VA_ARGS__);                     \
            SK_DEBUGBREAK ();                                                 \
            qFatal ("Assertion Failed: " __VA_ARGS__);                        \
        }                                                                     \
    }
#else
#define SK_ASSERT(x, ...)
#endif
