#pragma warning disable 0,
// ReSharper disable CheckNamespace MemberHidesStaticFromOuterClass

using AssistantPlus;
using AssistantPlus.Utility;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;

namespace AssistantPlus.View.PackageBuilder {

	[JsonObject(ItemRequired = Required.AllowNull)]
	public record PackageVersion {
		public Integer Number                              = default!;
		public Integer ExtendedTextureInformationForPvz2Cn = default!;
	}

	[JsonObject(ItemRequired = Required.AllowNull)]
	public record PackageCompression {
		public Boolean      General = default!;
		public Boolean      Texture = default!;
		public List<String> Filter  = default!;
	}

	[JsonObject(ItemRequired = Required.AllowNull)]
	public record PackageCategory {
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
	public record PackageManifest {
		public PackageManifestType Type   = default!;
		public String              Suffix = default!;
	}

	[JsonObject(ItemRequired = Required.AllowNull)]
	public record Variable {
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
	public record ResourceCategory {
		public Integer? Resolution = default!;
		public String?  Locale     = default!;
	}

	[JsonObject(ItemRequired = Required.AllowNull)]
	public record DummyResourceProperty {
	}

	[JsonObject(ItemRequired = Required.AllowNull)]
	public record GeneralResourceProperty {
		public String Path = default!;
		public String Type = default!;
	}

	[JsonObject(ItemRequired = Required.AllowNull)]
	public record TextureResourcePropertySpriteResource {
		public String        Id       = default!;
		public String        Path     = default!;
		public List<Integer> Position = default!;
		public List<Integer> Size     = default!;
		public List<Integer> Offset   = default!;
		public List<Integer> Separate = default!;
	}

	[JsonObject(ItemRequired = Required.AllowNull)]
	public record TextureResourceProperty {
		public String                                      Path                = default!;
		public Integer                                     Format              = default!;
		public Integer                                     Pitch               = default!;
		public Integer                                     AdditionalByteCount = default!;
		public List<Integer>                               Size                = default!;
		public List<TextureResourcePropertySpriteResource> Sprite              = default!;
	}

	[JsonObject(ItemRequired = Required.AllowNull)]
	public record SpecialRtonResourceProperty {
		public String Conversion = default!;
		public String Path       = default!;
	}

	[JsonObject(ItemRequired = Required.AllowNull)]
	public record SpecialPtxResourcePropertySpriteResource {
		public String        Source   = default!;
		public String        Id       = default!;
		public String        Path     = default!;
		public List<Integer> Offset   = default!;
		public List<Integer> Separate = default!;
	}

	[JsonObject(ItemRequired = Required.AllowNull)]
	public record SpecialPtxResourceProperty {
		public String                                         Conversion = default!;
		public String                                         Path       = default!;
		public Integer                                        Resolution = default!;
		public List<SpecialPtxResourcePropertySpriteResource> Sprite     = default!;
	}

	[JsonObject(ItemRequired = Required.AllowNull)]
	public record SpecialPamResourceProperty {
		public String Conversion = default!;
		public String Path       = default!;
	}

	[JsonObject(ItemRequired = Required.AllowNull)]
	public record SpecialPopfxResourceProperty {
		public String Conversion = default!;
		public String Path       = default!;
	}

	[JsonObject(ItemRequired = Required.AllowNull)]
	public record SpecialWemResourceProperty {
		public String Conversion = default!;
		public String Path       = default!;
	}

	[JsonObject(ItemRequired = Required.AllowNull)]
	public record SpecialBnkResourceProperty {
		public String Conversion = default!;
		public String Path       = default!;
	}

	[JsonObject(ItemRequired = Required.AllowNull)]
	public record ResourceSetting {
		public ResourceCategory Category = default!;
		public ResourceType     Type     = default!;
		public JToken           Property = default!;
		public List<Variable>   Variable = default!;
	}

	[JsonObject(ItemRequired = Required.AllowNull)]
	public record GroupSetting {
		public List<Variable> Variable = default!;
	}

	[JsonObject(ItemRequired = Required.AllowNull)]
	public record PartSetting {
		public List<Variable> Variable = default!;
	}

	[JsonObject(ItemRequired = Required.AllowNull)]
	public record PackageConversionItem {
		public String Name = default!;
		[JsonExtensionData()]
		public IDictionary<String, JToken> AdditionalData = default!;
	}

	[JsonObject(ItemRequired = Required.AllowNull)]
	public record PackageConversion {
		public List<PackageConversionItem> Rton = default!;
		public List<PackageConversionItem> Ptx  = default!;
		public List<PackageConversionItem> Pam  = default!;
		public List<PackageConversionItem> Wem  = default!;
	}

	[JsonObject(ItemRequired = Required.AllowNull)]
	public record PackageSetting {
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
	public record ProjectSetting {
		public List<PackageSetting> Package = default!;
	}

}
