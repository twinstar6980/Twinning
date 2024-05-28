#pragma once

#if defined M_compiler_msvc
#pragma warning(push)
#pragma warning(disable:4191)
#pragma warning(disable:4365)
#pragma warning(disable:5039)
#endif
#if defined M_compiler_clang
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wold-style-cast"
#pragma clang diagnostic ignored "-Wreserved-macro-identifier"
#pragma clang diagnostic ignored "-Wdouble-promotion"
#pragma clang diagnostic ignored "-Wsign-conversion"
#pragma clang diagnostic ignored "-Wshorten-64-to-32"
#pragma clang diagnostic ignored "-Wreserved-identifier"
#pragma clang diagnostic ignored "-Wzero-as-null-pointer-constant"
#pragma clang diagnostic ignored "-Wcast-function-type"
#pragma clang diagnostic ignored "-Wnewline-eof"
#pragma clang diagnostic ignored "-Wimplicit-int-conversion"
#pragma clang diagnostic ignored "-Wimplicit-int-float-conversion"
#endif

#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone
#include "third/quickjs/cutils.h"
#include "third/quickjs/quickjs.h"
#endif
#if defined M_system_windows
#include "third/quickjs.c_smile/cutils.h"
#include "third/quickjs.c_smile/quickjs.h"
#endif

#if defined M_compiler_msvc
#pragma warning(pop)
#endif
#if defined M_compiler_clang
#pragma clang diagnostic pop
#endif

namespace Twinning::Kernel::Third::quickjs {

	using BOOL = ::BOOL;

	using JSRuntime = ::JSRuntime;

	using JSContext = ::JSContext;

	using JSValue = ::JSValue;

	using JSClassID = ::JSClassID;

	using JSClassDef = ::JSClassDef;

	using JSPropertyEnum = ::JSPropertyEnum;

	using JSModuleDef = ::JSModuleDef;

	inline constexpr auto JS_NewRuntime = ::JS_NewRuntime;

	inline constexpr auto JS_FreeRuntime = ::JS_FreeRuntime;

	inline constexpr auto JS_NewContext = ::JS_NewContext;

	inline constexpr auto JS_FreeContext = ::JS_FreeContext;

	inline constexpr auto JS_GetRuntime = ::JS_GetRuntime;

	inline constexpr auto JS_SetHostPromiseRejectionTracker = ::JS_SetHostPromiseRejectionTracker;

	inline constexpr auto JS_SetModuleLoaderFunc = ::JS_SetModuleLoaderFunc;

	inline constexpr auto JS_GetImportMeta = ::JS_GetImportMeta;

	inline constexpr auto JS_IsJobPending = ::JS_IsJobPending;

	inline constexpr auto JS_ExecutePendingJob = ::JS_ExecutePendingJob;

	inline constexpr auto JS_GetException = ::JS_GetException;

	inline constexpr auto JS_GetGlobalObject = ::JS_GetGlobalObject;

	inline constexpr auto JS_FreeValue = ::JS_FreeValue;

	inline constexpr auto JS_DupValue = ::JS_DupValue;

	inline constexpr auto JS_IsUninitialized = ::JS_IsUninitialized;

	inline constexpr auto JS_IsUndefined = ::JS_IsUndefined;

	inline constexpr auto JS_IsNull = ::JS_IsNull;

	inline constexpr auto JS_IsBool = ::JS_IsBool;

	inline constexpr auto JS_IsBigInt = ::JS_IsBigInt;

	inline constexpr auto JS_IsNumber = ::JS_IsNumber;

	inline constexpr auto JS_IsString = ::JS_IsString;

	inline constexpr auto JS_IsObject = ::JS_IsObject;

	inline constexpr auto JS_IsException = ::JS_IsException;

	inline constexpr auto JS_IsFunction = ::JS_IsFunction;

	inline constexpr auto JS_ToBool = ::JS_ToBool;

	inline constexpr auto JS_ToFloat64 = ::JS_ToFloat64;

	inline constexpr auto JS_ToBigInt64 = ::JS_ToBigInt64;

	inline constexpr auto JS_ToCStringLen = ::JS_ToCStringLen;

	inline constexpr auto JS_FreeCString = ::JS_FreeCString;

	inline constexpr auto JS_NewBool = ::JS_NewBool;

	inline constexpr auto JS_NewBigInt64 = ::JS_NewBigInt64;

	inline constexpr auto JS_NewFloat64 = ::JS_NewFloat64;

	inline constexpr auto JS_NewStringLen = ::JS_NewStringLen;

	inline constexpr auto JS_NewObjectClass = ::JS_NewObjectClass;

	inline constexpr auto JS_NewObject = ::JS_NewObject;

	inline constexpr auto JS_NewArray = ::JS_NewArray;

	inline constexpr auto JS_NewCFunction2 = ::JS_NewCFunction2;

	inline constexpr auto JS_SetOpaque = ::JS_SetOpaque;

	inline constexpr auto JS_GetOpaque = ::JS_GetOpaque;

	inline constexpr auto JS_GetPrototype = ::JS_GetPrototype;

	inline constexpr auto JS_SetPrototype = ::JS_SetPrototype;

	inline constexpr auto JS_DefinePropertyValue = ::JS_DefinePropertyValue;

	inline constexpr auto JS_DefinePropertyGetSet = ::JS_DefinePropertyGetSet;

	inline constexpr auto JS_DeleteProperty = ::JS_DeleteProperty;

	inline constexpr auto JS_HasProperty = ::JS_HasProperty;

	inline constexpr auto JS_GetProperty = ::JS_GetProperty;

	inline constexpr auto JS_SetProperty = ::JS_SetProperty;

	inline constexpr auto JS_GetOwnPropertyNames = ::JS_GetOwnPropertyNames;

	inline constexpr auto js_free_prop_enum = ::js_free_prop_enum;

	inline constexpr auto JS_NewAtomLen = ::JS_NewAtomLen;

	inline constexpr auto JS_NewAtomUInt32 = ::JS_NewAtomUInt32;

	inline constexpr auto JS_FreeAtom = ::JS_FreeAtom;

	inline constexpr auto JS_AtomToValue = ::JS_AtomToValue;

	inline constexpr auto JS_AtomToCString = ::JS_AtomToCString;

	inline constexpr auto JS_NewClassID = ::JS_NewClassID;

	inline constexpr auto JS_NewClass = ::JS_NewClass;

	inline constexpr auto JS_GetClassProto = ::JS_GetClassProto;

	inline constexpr auto JS_SetClassProto = ::JS_SetClassProto;

	inline constexpr auto JS_NewArrayBuffer = ::JS_NewArrayBuffer;

	inline constexpr auto JS_NewArrayBufferCopy = ::JS_NewArrayBufferCopy;

	inline constexpr auto JS_GetArrayBuffer = ::JS_GetArrayBuffer;

	inline constexpr auto JS_Call = ::JS_Call;

	inline constexpr auto JS_Eval = ::JS_Eval;

	inline constexpr auto JS_SetConstructor = ::JS_SetConstructor;

	inline constexpr auto JS_Throw = ::JS_Throw;

	inline constexpr auto JS_ThrowReferenceError = ::JS_ThrowReferenceError;

	inline constexpr auto JS_UNINITIALIZED_ = ::JSValue{JS_UNINITIALIZED};

	inline constexpr auto JS_UNDEFINED_ = ::JSValue{JS_UNDEFINED};

	inline constexpr auto JS_NULL_ = ::JSValue{JS_NULL};

	inline constexpr auto JS_EXCEPTION_ = ::JSValue{JS_EXCEPTION};

	inline constexpr auto JS_PROP_THROW_ = int{JS_PROP_THROW};

	inline constexpr auto JS_PROP_C_W_E_ = int{JS_PROP_C_W_E};

	inline constexpr auto JS_CFUNC_generic_ = ::JSCFunctionEnum{JS_CFUNC_generic};

	inline constexpr auto JS_CFUNC_constructor_ = ::JSCFunctionEnum{JS_CFUNC_constructor};

	inline constexpr auto JS_GPN_STRING_MASK_ = int{JS_GPN_STRING_MASK};

	inline constexpr auto JS_EVAL_TYPE_GLOBAL_ = int{JS_EVAL_TYPE_GLOBAL};

	inline constexpr auto JS_EVAL_TYPE_MODULE_ = int{JS_EVAL_TYPE_MODULE};

	inline constexpr auto JS_EVAL_FLAG_STRICT_ = int{JS_EVAL_FLAG_STRICT};

	inline constexpr auto JS_EVAL_FLAG_COMPILE_ONLY_ = int{JS_EVAL_FLAG_COMPILE_ONLY};

}
