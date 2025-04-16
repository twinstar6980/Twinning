module;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"

#include "third/quickjs_ng/quickjs.h"

#pragma clang diagnostic pop

export module twinning.kernel.third.quickjs_ng;

export namespace Twinning::Kernel::Third::quickjs_ng {

	using $JSRuntime = ::JSRuntime;

	using $JSContext = ::JSContext;

	using $JSValue = ::JSValue;

	using $JSClassID = ::JSClassID;

	using $JSClassDef = ::JSClassDef;

	using $JSPropertyEnum = ::JSPropertyEnum;

	using $JSModuleDef = ::JSModuleDef;

	inline constexpr auto $JS_NewRuntime = ::JS_NewRuntime;

	inline constexpr auto $JS_FreeRuntime = ::JS_FreeRuntime;

	inline constexpr auto $JS_NewContext = ::JS_NewContext;

	inline constexpr auto $JS_FreeContext = ::JS_FreeContext;

	inline constexpr auto $JS_GetRuntime = ::JS_GetRuntime;

	inline constexpr auto $JS_SetModuleLoaderFunc = ::JS_SetModuleLoaderFunc;

	inline constexpr auto $JS_GetImportMeta = ::JS_GetImportMeta;

	inline constexpr auto $JS_IsJobPending = ::JS_IsJobPending;

	inline constexpr auto $JS_ExecutePendingJob = ::JS_ExecutePendingJob;

	inline constexpr auto $JS_GetException = ::JS_GetException;

	inline constexpr auto $JS_GetGlobalObject = ::JS_GetGlobalObject;

	inline constexpr auto $JS_FreeValue = ::JS_FreeValue;

	inline constexpr auto $JS_DupValue = ::JS_DupValue;

	inline constexpr auto $JS_IsUninitialized = ::JS_IsUninitialized;

	inline constexpr auto $JS_IsUndefined = ::JS_IsUndefined;

	inline constexpr auto $JS_IsNull = ::JS_IsNull;

	inline constexpr auto $JS_IsBool = ::JS_IsBool;

	inline constexpr auto $JS_IsBigInt = ::JS_IsBigInt;

	inline constexpr auto $JS_IsNumber = ::JS_IsNumber;

	inline constexpr auto $JS_IsString = ::JS_IsString;

	inline constexpr auto $JS_IsObject = ::JS_IsObject;

	inline constexpr auto $JS_IsException = ::JS_IsException;

	inline constexpr auto $JS_IsFunction = ::JS_IsFunction;

	inline constexpr auto $JS_ToBool = ::JS_ToBool;

	inline constexpr auto $JS_ToFloat64 = ::JS_ToFloat64;

	inline constexpr auto $JS_ToBigInt64 = ::JS_ToBigInt64;

	inline constexpr auto $JS_ToCStringLen = ::JS_ToCStringLen;

	inline constexpr auto $JS_FreeCString = ::JS_FreeCString;

	inline constexpr auto $JS_NewBool = ::JS_NewBool;

	inline constexpr auto $JS_NewBigInt64 = ::JS_NewBigInt64;

	inline constexpr auto $JS_NewFloat64 = ::JS_NewFloat64;

	inline constexpr auto $JS_NewStringLen = ::JS_NewStringLen;

	inline constexpr auto $JS_NewObjectClass = ::JS_NewObjectClass;

	inline constexpr auto $JS_NewObject = ::JS_NewObject;

	inline constexpr auto $JS_NewArray = ::JS_NewArray;

	inline constexpr auto $JS_NewCFunction2 = ::JS_NewCFunction2;

	inline constexpr auto $JS_SetOpaque = ::JS_SetOpaque;

	inline constexpr auto $JS_GetOpaque = ::JS_GetOpaque;

	inline constexpr auto $JS_GetPrototype = ::JS_GetPrototype;

	inline constexpr auto $JS_SetPrototype = ::JS_SetPrototype;

	inline constexpr auto $JS_DefinePropertyValue = ::JS_DefinePropertyValue;

	inline constexpr auto $JS_DefinePropertyGetSet = ::JS_DefinePropertyGetSet;

	inline constexpr auto $JS_DeleteProperty = ::JS_DeleteProperty;

	inline constexpr auto $JS_HasProperty = ::JS_HasProperty;

	inline constexpr auto $JS_GetProperty = ::JS_GetProperty;

	inline constexpr auto $JS_SetProperty = ::JS_SetProperty;

	inline constexpr auto $JS_GetOwnPropertyNames = ::JS_GetOwnPropertyNames;

	inline constexpr auto $JS_FreePropertyEnum = ::JS_FreePropertyEnum;

	inline constexpr auto $JS_NewAtomLen = ::JS_NewAtomLen;

	inline constexpr auto $JS_NewAtomUInt32 = ::JS_NewAtomUInt32;

	inline constexpr auto $JS_FreeAtom = ::JS_FreeAtom;

	inline constexpr auto $JS_AtomToValue = ::JS_AtomToValue;

	inline constexpr auto $JS_AtomToCString = ::JS_AtomToCString;

	inline constexpr auto $JS_NewClassID = ::JS_NewClassID;

	inline constexpr auto $JS_NewClass = ::JS_NewClass;

	inline constexpr auto $JS_GetClassProto = ::JS_GetClassProto;

	inline constexpr auto $JS_SetClassProto = ::JS_SetClassProto;

	inline constexpr auto $JS_NewArrayBuffer = ::JS_NewArrayBuffer;

	inline constexpr auto $JS_NewArrayBufferCopy = ::JS_NewArrayBufferCopy;

	inline constexpr auto $JS_GetArrayBuffer = ::JS_GetArrayBuffer;

	inline constexpr auto $JS_Call = ::JS_Call;

	inline constexpr auto $JS_Eval = ::JS_Eval;

	inline constexpr auto $JS_SetConstructor = ::JS_SetConstructor;

	inline constexpr auto $JS_Throw = ::JS_Throw;

	inline constexpr auto $JS_ThrowReferenceError = ::JS_ThrowReferenceError;

	inline constexpr auto $JS_PROP_THROW = JS_PROP_THROW;

	inline constexpr auto $JS_PROP_C_W_E = JS_PROP_C_W_E;

	inline constexpr auto $JS_CFUNC_generic = ::JS_CFUNC_generic;

	inline constexpr auto $JS_CFUNC_constructor = ::JS_CFUNC_constructor;

	inline constexpr auto $JS_GPN_STRING_MASK = JS_GPN_STRING_MASK;

	inline constexpr auto $JS_EVAL_TYPE_GLOBAL = JS_EVAL_TYPE_GLOBAL;

	inline constexpr auto $JS_EVAL_TYPE_MODULE = JS_EVAL_TYPE_MODULE;

	inline constexpr auto $JS_EVAL_FLAG_STRICT = JS_EVAL_FLAG_STRICT;

	inline constexpr auto $JS_EVAL_FLAG_COMPILE_ONLY = JS_EVAL_FLAG_COMPILE_ONLY;

	inline auto const $JS_UNINITIALIZED = JS_UNINITIALIZED;

	inline auto const $JS_UNDEFINED = JS_UNDEFINED;

	inline auto const $JS_NULL = JS_NULL;

	inline auto const $JS_EXCEPTION = JS_EXCEPTION;

	inline auto $JS_VALUE_GET_PTR (
		JSValue v
	) -> void * {
		return JS_VALUE_GET_PTR(v);
	}

}
