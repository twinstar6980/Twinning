#pragma warning disable 0,
// ReSharper disable InconsistentNaming VariableHidesOuterVariable AccessToModifiedClosure

using Helper;
using Helper.Utility;

namespace Helper.Bridge {

	public static unsafe class Invoker {

		#region function

		public static IntegerU64 Version (
			Library library
		) {
			var numberPointer = default(Interface.Size*);
			{
			}
			var exceptionPointer = library.Version(&numberPointer);
			{
			}
			var number = Converter.ParseSize(ref *numberPointer);
			if (exceptionPointer is not null) {
				var exception = Converter.ParseString(ref *exceptionPointer);
				throw new Exception(exception);
			}
			return number;
		}

		public static String Execute (
			Library                          library,
			Func<List<String>, List<String>> callback,
			String                           script,
			List<String>                     argument
		) {
			var callbackExceptionHandler = default(Interface.String*);
			var callbackResultHandler = default(Interface.StringList*);
			var callbackProxy = Interface.String* (
				Interface.StringList** argument,
				Interface.StringList** result
			) => {
				Converter.DestructString(ref *callbackExceptionHandler);
				Converter.DestructStringList(ref *callbackResultHandler);
				*result = callbackResultHandler;
				try {
					var argumentValue = Converter.ParseStringList(ref **argument);
					var resultValue = callback(argumentValue);
					Converter.ConstructStringList(ref *callbackResultHandler, resultValue);
					return null;
				} catch (Exception e) {
					var exceptionValue = e.ToString();
					Converter.ConstructString(ref *callbackExceptionHandler, exceptionValue);
					return callbackExceptionHandler;
				}
			};
			var callbackProxyGuard = GCHandle.Alloc(callbackProxy);
			{
				callbackExceptionHandler = (Interface.String*)MemoryHelper.Alloc(sizeof(Interface.String));
				callbackResultHandler = (Interface.StringList*)MemoryHelper.Alloc(sizeof(Interface.StringList));
				Converter.ConstructString(ref *callbackExceptionHandler, "");
				Converter.ConstructStringList(ref *callbackResultHandler, new List<String>());
			}
			var callbackPointer = default(Interface.Callback*);
			var scriptPointer = default(Interface.String*);
			var argumentPointer = default(Interface.StringList*);
			var resultPointer = default(Interface.String*);
			{
				callbackPointer = (Interface.Callback*)MemoryHelper.Alloc(sizeof(Interface.Callback));
				scriptPointer = (Interface.String*)MemoryHelper.Alloc(sizeof(Interface.String));
				argumentPointer = (Interface.StringList*)MemoryHelper.Alloc(sizeof(Interface.StringList));
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
				Converter.DestructString(ref *callbackExceptionHandler);
				Converter.DestructStringList(ref *callbackResultHandler);
				MemoryHelper.Free(callbackExceptionHandler);
				MemoryHelper.Free(callbackResultHandler);
			}
			callbackProxyGuard.Free();
			if (exceptionPointer is not null) {
				var exception = Converter.ParseString(ref *exceptionPointer);
				throw new Exception(exception);
			}
			return result;
		}

		public static void Prepare (
			Library library
		) {
			{
			}
			var exceptionPointer = library.Prepare();
			{
			}
			if (exceptionPointer is not null) {
				var exception = Converter.ParseString(ref *exceptionPointer);
				throw new Exception(exception);
			}
			return;
		}

		#endregion

	}

}
