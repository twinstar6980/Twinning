#pragma warning disable 0,
// ReSharper disable

using Helper;
using System.Text;

namespace Helper.Core {

	public static unsafe class Converter {

		#region utility

		public static IntegerU64 ParseSize (
			ref Interface.Size structure
		) {
			return structure.value;
		}

		public static Interface.Size CreateSize (
			IntegerU64 value
		) {
			return new Interface.Size {
				value = value,
			};
		}

		// ----------------

		public static String ParseString (
			ref Interface.String structure
		) {
			return structure.data == null ? "" : Encoding.UTF8.GetString((Byte*)structure.data, (Size)Converter.ParseSize(ref structure.size));
		}

		public static Interface.String CreateString (
			String value
		) {
			var data = Encoding.UTF8.GetBytes(value.ToCharArray());
			var structure = new Interface.String();
			fixed (Byte* pointer = data) {
				structure.data = (Interface.Character*)pointer;
			}
			structure.size = Converter.CreateSize((IntegerU64)data.Length);
			structure.capacity = Converter.CreateSize((IntegerU64)data.Length);
			return structure;
		}

		// ----------------

		public static List<String> ParseStringList (
			ref Interface.StringList structure
		) {
			var value = new List<String> {
				Capacity = (Size)Converter.ParseSize(ref structure.size),
			};
			for (var index = 0; index < (Size)Converter.ParseSize(ref structure.size); ++index) {
				value.Add(Converter.ParseString(ref structure.data[index]));
			}
			return value;
		}

		public static Interface.StringList CreateStringList (
			List<String> value
		) {
			var structure = new Interface.StringList();
			fixed (Interface.String* pointer = new Interface.String[value.Count]) {
				structure.data = pointer;
			}
			structure.size = Converter.CreateSize((IntegerU64)value.Count);
			structure.capacity = Converter.CreateSize((IntegerU64)value.Count);
			for (var index = 0; index < value.Count; ++index) {
				structure.data[index] = Converter.CreateString(value[index]);
			}
			return structure;
		}

		// ----------------

		public static delegate* <Interface.String*, Interface.StringList**, Interface.StringList**> ParseCallback (
			ref Interface.Callback structure
		) {
			return structure.value;
		}

		public static Interface.Callback CreateCallback (
			delegate* <Interface.String*, Interface.StringList**, Interface.StringList**> value
		) {
			return new Interface.Callback {
				value = value,
			};
		}

		#endregion

	}

}
