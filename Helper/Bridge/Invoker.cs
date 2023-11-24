#pragma warning disable 0,
// ReSharper disable InconsistentNaming VariableHidesOuterVariable AccessToModifiedClosure

using Helper;
using Helper.Utility;

namespace Helper.Bridge {

	public static unsafe class Invoker {

		#region function

		public static String Execute (
			Library                          library,
			Func<List<String>, List<String>> callback,
			String                           script,
			List<String>                     argument
		) {
			var callbackResultHandler = default(Interface.StringList*);
			var callbackExceptionHandler = default(Interface.String*);
			var callbackProxy = Interface.String* (
				Interface.StringList** argument,
				Interface.StringList** result
			) => {
				Converter.DestructStringList(ref *callbackResultHandler);
				Converter.DestructString(ref *callbackExceptionHandler);
				*result = callbackResultHandler;
				try {
					var argumentValue = Converter.ParseStringList(ref **argument);
					var resultValue = callback(argumentValue);
					Converter.ConstructStringList(ref *callbackResultHandler, resultValue);
					return null;
				}
				catch (Exception e) {
					var exceptionValue = e.ToString();
					Converter.ConstructString(ref *callbackExceptionHandler, exceptionValue);
					return callbackExceptionHandler;
				}
			};
			var callbackProxyGuard = GCHandle.Alloc(callbackProxy);
			{
				callbackResultHandler = MemoryHelper.Allocate<Interface.StringList>();
				callbackExceptionHandler = MemoryHelper.Allocate<Interface.String>();
				Converter.ConstructStringList(ref *callbackResultHandler, new List<String>());
				Converter.ConstructString(ref *callbackExceptionHandler, "");
			}
			var callbackPointer = default(Interface.Callback*);
			var scriptPointer = default(Interface.String*);
			var argumentPointer = default(Interface.StringList*);
			var resultPointer = default(Interface.String*);
			{
				callbackPointer = MemoryHelper.Allocate<Interface.Callback>();
				scriptPointer = MemoryHelper.Allocate<Interface.String>();
				argumentPointer = MemoryHelper.Allocate<Interface.StringList>();
				Converter.ConstructCallback(ref *callbackPointer, (delegate* <Interface.String*, Interface.StringList**, Interface.StringList**>)Marshal.GetFunctionPointerForDelegate(callbackProxy));
				Converter.ConstructString(ref *scriptPointer, script);
				Converter.ConstructStringList(ref *argumentPointer, argument);
			}
			var exceptionPointer = library.Execute(&callbackPointer, &scriptPointer, &argumentPointer, &resultPointer);
			{
				Converter.DestructCallback(ref *callbackPointer);
				Converter.DestructString(ref *scriptPointer);
				Converter.DestructStringList(ref *argumentPointer);
				MemoryHelper.Free(callbackPointer);
				MemoryHelper.Free(scriptPointer);
				MemoryHelper.Free(argumentPointer);
			}
			var result = Converter.ParseString(ref *resultPointer);
			{
				Converter.DestructStringList(ref *callbackResultHandler);
				Converter.DestructString(ref *callbackExceptionHandler);
				MemoryHelper.Free(callbackResultHandler);
				MemoryHelper.Free(callbackExceptionHandler);
			}
			callbackProxyGuard.Free();
			if (exceptionPointer is not null) {
				var exception = Converter.ParseString(ref *exceptionPointer);
				throw new Exception(exception);
			}
			return result;
		}

		#endregion

	}

}
