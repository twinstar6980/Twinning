#pragma warning disable 0,
// ReSharper disable InconsistentNaming

using Helper;

namespace Helper.Bridge {

	#region type

	public record SymbolTable {
		public required Interface.version version;
		public required Interface.execute execute;
		public required Interface.prepare prepare;
	}

	public class SymbolNameTable {
		public const String version = "?version@Interface@Kernel@TwinStar@@YAPEAUString@123@PEAPEAUSize@123@@Z";
		public const String execute = "?execute@Interface@Kernel@TwinStar@@YAPEAUString@123@PEAPEAUCallback@123@PEAPEAU4123@PEAPEAUStringList@123@1@Z";
		public const String prepare = "?prepare@Interface@Kernel@TwinStar@@YAPEAUString@123@XZ";
	}

	#endregion

}
