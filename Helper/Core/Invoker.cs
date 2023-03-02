#pragma warning disable 0,
// ReSharper disable InconsistentNaming VariableHidesOuterVariable AccessToModifiedClosure

using Helper;

namespace Helper.Core {

	public static unsafe class Invoker {

		#region function

		public static UInt64 Version (
			Library library
		) {
			var result = library.Version();
			return Converter.ParseSize(ref *result);
		}

		public static String? Execute (
			Library                          library,
			Func<List<String>, List<String>> callback,
			String                           script,
			List<String>                     argument
		) {
			var callbackResult = (Interface.StringList*)null;
			var callbackProxy = (
				Interface.StringList* argument
			) => {
				var resultValue = new List<String>();
				try {
					resultValue.Clear();
					resultValue.Add("");
					resultValue.AddRange(callback(Converter.ParseStringList(ref *argument)));
				} catch (Exception e) {
					resultValue.Clear();
					resultValue.Add(e.ToString());
				}
				*callbackResult = Converter.CreateStringList(resultValue);
				return callbackResult;
			};
			fixed (Interface.StringList* pointer = new Interface.StringList[1]) {
				callbackResult = pointer;
				*callbackResult = Converter.CreateStringList(new List<String>());
			}
			var callbackStructure = Converter.CreateCallback((delegate* <Interface.StringList*, Interface.StringList*>)Marshal.GetFunctionPointerForDelegate(callbackProxy));
			var scriptStructure = Converter.CreateString(script);
			var argumentStructure = Converter.CreateStringList(argument);
			var resultStructure = library.Execute(&callbackStructure, &scriptStructure, &argumentStructure);
			return resultStructure == null ? null : Converter.ParseString(ref *resultStructure);
		}

		public static String? Prepare (
			Library library
		) {
			var resultStructure = library.Prepare();
			return resultStructure == null ? null : Converter.ParseString(ref *resultStructure);
		}

		#endregion

	}

}
