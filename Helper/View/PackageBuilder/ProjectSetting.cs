#pragma warning disable 0,
// ReSharper disable CheckNamespace MemberHidesStaticFromOuterClass

using Helper;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;

namespace Helper.View.PackageBuilder {

	#region type

	[JsonObject(ItemRequired = Required.AllowNull)]
	public class PackageVersion {
		public Integer Number                              = default!;
		public Integer ExtendedTextureInformationForPvz2Cn = default!;
	}

	[JsonObject(ItemRequired = Required.AllowNull)]
	public class PackageCompression {
		public Boolean      General = default!;
		public Boolean      Texture = default!;
		public List<String> Filter  = default!;
	}

	[JsonObject(ItemRequired = Required.AllowNull)]
	public class PackageCategory {
		public List<Integer> Resolution = default!;
		public List<String>  Locale     = default!;
	}

	public enum PackageManifestType {
		Internal,
		ExternalRtonWithArrayPath,
		ExternalRtonWithStringPath,
		ExternalNewton,
	}

	[JsonObject(ItemRequired = Required.AllowNull)]
	public class PackageManifest {
		public PackageManifestType Type   = default!;
		public String              Suffix = default!;
	}

	[JsonObject(ItemRequired = Required.AllowNull)]
	public class Variable {
		public String Name  = default!;
		public String Value = default!;
	}

	public enum ResourceType {
		Dummy,
		General,
		Texture,
		SpecialRton,
		SpecialPtx,
		SpecialPam,
		SpecialWem,
	}

	[JsonObject(ItemRequired = Required.AllowNull)]
	public class ResourceCategory {
		public Integer? Resolution = default!;
		public String?  Locale     = default!;
	}

	[JsonObject(ItemRequired = Required.AllowNull)]
	public class DummyResourceProperty {
	}

	[JsonObject(ItemRequired = Required.AllowNull)]
	public class GeneralResourceProperty {
		public String Path = default!;
		public String Type = default!;
	}

	[JsonObject(ItemRequired = Required.AllowNull)]
	public class TextureResourcePropertySpriteResource {
		public String        Id       = default!;
		public String        Path     = default!;
		public List<Integer> Position = default!;
		public List<Integer> Size     = default!;
		public List<Integer> Offset   = default!;
		public List<Integer> Separate = default!;
	}

	[JsonObject(ItemRequired = Required.AllowNull)]
	public class TextureResourceProperty {
		public String                                      Path                = default!;
		public Integer                                     Format              = default!;
		public Integer                                     Pitch               = default!;
		public Integer                                     AdditionalByteCount = default!;
		public List<Integer>                               Size                = default!;
		public List<TextureResourcePropertySpriteResource> Sprite              = default!;
	}

	[JsonObject(ItemRequired = Required.AllowNull)]
	public class SpecialRtonResourceProperty {
		public String Conversion = default!;
		public String Path       = default!;
	}

	[JsonObject(ItemRequired = Required.AllowNull)]
	public class SpecialPtxResourcePropertySpriteResource {
		public String        Source   = default!;
		public String        Id       = default!;
		public String        Path     = default!;
		public List<Integer> Offset   = default!;
		public List<Integer> Separate = default!;
	}

	[JsonObject(ItemRequired = Required.AllowNull)]
	public class SpecialPtxResourceProperty {
		public String                                         Conversion = default!;
		public String                                         Path       = default!;
		public Integer                                        Resolution = default!;
		public List<SpecialPtxResourcePropertySpriteResource> Sprite     = default!;
	}

	[JsonObject(ItemRequired = Required.AllowNull)]
	public class SpecialPamResourceProperty {
		public String Conversion = default!;
		public String Path       = default!;
	}

	[JsonObject(ItemRequired = Required.AllowNull)]
	public class SpecialPopfxResourceProperty {
		public String Conversion = default!;
		public String Path       = default!;
	}

	[JsonObject(ItemRequired = Required.AllowNull)]
	public class SpecialWemResourceProperty {
		public String Conversion = default!;
		public String Path       = default!;
	}

	[JsonObject(ItemRequired = Required.AllowNull)]
	public class SpecialBnkResourceProperty {
		public String Conversion = default!;
		public String Path       = default!;
	}

	[JsonObject(ItemRequired = Required.AllowNull)]
	public class ResourceSetting {
		public ResourceCategory Category = default!;
		public ResourceType     Type     = default!;
		public JToken           Property = default!;
		public List<Variable>   Variable = default!;
	}

	[JsonObject(ItemRequired = Required.AllowNull)]
	public class GroupSetting {
		public List<Variable> Variable = default!;
	}

	[JsonObject(ItemRequired = Required.AllowNull)]
	public class PartSetting {
		public List<Variable> Variable = default!;
	}

	[JsonObject(ItemRequired = Required.AllowNull)]
	public class PackageConversionItem {
		public String Name = default!;

		[JsonExtensionData()]
		public IDictionary<String, JToken> AdditionalData = default!;
	}

	[JsonObject(ItemRequired = Required.AllowNull)]
	public class PackageConversion {
		public List<PackageConversionItem> Rton = default!;
		public List<PackageConversionItem> Ptx  = default!;
		public List<PackageConversionItem> Pam  = default!;
		public List<PackageConversionItem> Wem  = default!;
	}

	[JsonObject(ItemRequired = Required.AllowNull)]
	public class PackageSetting {
		public String             Name        = default!;
		public List<String>       Part        = default!;
		public PackageVersion     Version     = default!;
		public PackageCompression Compression = default!;
		public PackageManifest    Manifest    = default!;
		public PackageCategory    Category    = default!;
		public PackageConversion  Conversion  = default!;
		public List<Variable>     Variable    = default!;
	}

	[JsonObject(ItemRequired = Required.AllowNull)]
	public class ProjectSetting {
		public List<PackageSetting> Package = default!;
	}

	#endregion

}
