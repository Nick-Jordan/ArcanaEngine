#ifndef CALLBACK_H_
#define CALLBACK_H_

#include "BaseCallback.h"

#define REGISTER_CALLBACK(CallbackName, ...) typedef BaseCallback<void, __VA_ARGS__> CallbackName;

#define REGISTER_CALLBACK_RETURN_TYPE(CallbackName, ReturnType, ...) typedef BaseCallback<ReturnType, __VA_ARGS__> CallbackName;

#endif