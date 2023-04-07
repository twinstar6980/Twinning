#pragma warning disable 0,
// ReSharper disable InconsistentNaming VariableHidesOuterVariable AccessToModifiedClosure

using Helper;

namespace Helper.Core {

	public static unsafe class Invoker {

		#region function

		public static IntegerU64 Version (
			Library library
		) {
			var numberPointer = (Interface.Size*)null;
			{
				fixed (Interface.Size* pointer = new Interface.Size[1]) {
					*pointer = Converter.CreateSize(0);
					numberPointer = pointer;
				}
			}
			var exceptionPointer = library.Version(&numberPointer);
			var number = Converter.ParseSize(ref *numberPointer);
			if (exceptionPointer != null) {
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
			var callbackExceptionHandler = (Interface.String*)null;
			var callbackResultHandler = (Interface.StringList*)null;
			var callbackProxy = Interface.String* (
				Interface.StringList** argument,
				Interface.StringList** result
			) => {
				*result = callbackResultHandler;
				try {
					var argumentValue = Converter.ParseStringList(ref **argument);
					var resultValue = callback(argumentValue);
					*callbackResultHandler = Converter.CreateStringList(resultValue);
					return null;
				} catch (Exception e) {
					var exceptionValue = e.ToString();
					*callbackExceptionHandler = Converter.CreateString(exceptionValue);
					return callbackExceptionHandler;
				}
			};
			fixed (Interface.String* pointer = new Interface.String[1]) {
				callbackExceptionHandler = pointer;
				*callbackExceptionHandler = Converter.CreateString("");
			}
			fixed (Interface.StringList* pointer = new Interface.StringList[1]) {
				callbackResultHandler = pointer;
				*callbackResultHandler = Converter.CreateStringList(new List<String>());
			}
			var callbackPointer = (Interface.Callback*)null;
			var scriptPointer = (Interface.String*)null;
			var argumentPointer = (Interface.StringList*)null;
			var resultPointer = (Interface.String*)null;
			{
				fixed (Interface.Callback* pointer = new Interface.Callback[1]) {
					*pointer = Converter.CreateCallback((delegate*<Interface.String*, Interface.StringList**, Interface.StringList**>)Marshal.GetFunctionPointerForDelegate(callbackProxy));
					callbackPointer = pointer;
				}
				fixed (Interface.String* pointer = new Interface.String[1]) {
					*pointer = Converter.CreateString(script);
					scriptPointer = pointer;
				}
				fixed (Interface.StringList* pointer = new Interface.StringList[1]) {
					*pointer = Converter.CreateStringList(argument);
					argumentPointer = pointer;
				}
			}
			var exceptionPointer = library.Execute(&callbackPointer, &scriptPointer, &argumentPointer, &resultPointer);
			var result = Converter.ParseString(ref *resultPointer);
			if (exceptionPointer != null) {
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
			if (exceptionPointer != null) {
				var exception = Converter.ParseString(ref *exceptionPointer);
				throw new Exception(exception);
			}
			return;
		}

		#endregion

	}

}
