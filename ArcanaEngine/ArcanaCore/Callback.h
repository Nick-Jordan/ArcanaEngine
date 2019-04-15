#ifndef CALLBACK_H_
#define CALLBACK_H_

#include "BaseCallback.h"

/** \def REGISTER_CALLBACK(CallbackName, ...)
 *
 *  Creates a typedef of a callback with specified argument types.
 *  The callback's function returns a void.
 */

#define REGISTER_CALLBACK(CallbackName, ...) typedef BaseCallback<void, __VA_ARGS__> CallbackName

/** \def REGISTER_CALLBACK_RETURN_TYPE(CallbackName, ReturnType, ...)
 *
 *  Creates a typedef of a callback with specified return type and argument types.
 */

#define REGISTER_CALLBACK_RETURN_TYPE(CallbackName, ReturnType, ...) typedef BaseCallback<ReturnType, __VA_ARGS__> CallbackName

#endif