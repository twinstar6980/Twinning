#pragma warning disable 0, 8618,
// ReSharper disable InconsistentNaming

using Helper;

namespace Helper.Core {

	#region type

	public record SymbolTable {
		public required Interface.version version;
		public required Interface.execute execute;
		public required Interface.prepare prepare;
	}

	public class SymbolNameTable {
		public const String version = "?version@Interface@Core@TwinStar@@YAPEBUSize@123@XZ";
		public const String execute = "?execute@Interface@Core@TwinStar@@YAPEBUString@123@PEBUCallback@123@PEBU4123@PEBUStringList@123@@Z";
		public const String prepare = "?prepare@Interface@Core@TwinStar@@YAPEBUString@123@XZ";
	}

	#endregion

}
