#pragma warning disable 0,
// ReSharper disable

using Helper;
using Helper.Utility;

namespace Helper.Bridge {

	public static unsafe class Converter {

		#region utility

		public static IntegerU64 ParseSize (
			ref Interface.Size structure
		) {
			return structure.value;
		}

		public static void ConstructSize (
			ref Interface.Size structure,
			IntegerU64         value
		) {
			structure.value = value;
			return;
		}

		public static void DestructSize (
			ref Interface.Size structure
		) {
			structure.value = 0;
			return;
		}

		// ----------------

		public static String ParseString (
			ref Interface.String structure
		) {
			return structure.data is null ? String.Empty : Encoding.UTF8.GetString((Byte*)structure.data, (Size)Converter.ParseSize(ref structure.size));
		}

		public static void ConstructString (
			ref Interface.String structure,
			String               value
		) {
			var data = Encoding.UTF8.GetBytes(value.ToCharArray());
			var size = data.Length;
			structure.data = (Interface.Character*)MemoryHelper.Alloc(sizeof(Interface.Character) * size);
			fixed (Byte* dataPointer = data) {
				MemoryHelper.Copy(dataPointer, structure.data, size);
			}
			Converter.ConstructSize(ref structure.size, (IntegerU64)size);
			Converter.ConstructSize(ref structure.capacity, (IntegerU64)size);
			return;
		}

		public static void DestructString (
			ref Interface.String structure
		) {
			MemoryHelper.Free(structure.data);
			structure.data = null;
			Converter.DestructSize(ref structure.size);
			Converter.DestructSize(ref structure.capacity);
			return;
		}

		// ----------------

		public static List<String> ParseStringList (
			ref Interface.StringList structure
		) {
			var size = (Size)Converter.ParseSize(ref structure.size);
			var value = new List<String>(size);
			for (var index = 0; index < size; ++index) {
				value.Add(Converter.ParseString(ref structure.data[index]));
			}
			return value;
		}

		public static void ConstructStringList (
			ref Interface.StringList structure,
			List<String>             value
		) {
			var size = value.Count;
			structure.data = (Interface.String*)MemoryHelper.Alloc(sizeof(Interface.String) * size);
			for (var index = 0; index < size; ++index) {
				Converter.ConstructString(ref structure.data[index], value[index]);
			}
			Converter.ConstructSize(ref structure.size, (IntegerU64)size);
			Converter.ConstructSize(ref structure.capacity, (IntegerU64)size);
			return;
		}

		public static void DestructStringList (
			ref Interface.StringList structure
		) {
			var size = (Size)Converter.ParseSize(ref structure.size);
			for (var index = 0; index < size; ++index) {
				Converter.DestructString(ref structure.data[index]);
			}
			MemoryHelper.Free(structure.data);
			structure.data = null;
			Converter.DestructSize(ref structure.size);
			Converter.DestructSize(ref structure.capacity);
			return;
		}

		// ----------------

		public static delegate* <Interface.String*, Interface.StringList**, Interface.StringList**> ParseCallback (
			ref Interface.Callback structure
		) {
			return structure.value;
		}

		public static void ConstructCallback (
			ref Interface.Callback                                                        structure,
			delegate* <Interface.String*, Interface.StringList**, Interface.StringList**> value
		) {
			structure.value = value;
			return;
		}

		public static void DestructCallback (
			ref Interface.Callback structure
		) {
			structure.value = null;
			return;
		}

		#endregion

	}

}
