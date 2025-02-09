#pragma warning disable 0,
// ReSharper disable CheckNamespace MemberHidesStaticFromOuterClass

using AssistantPlus;
using AssistantPlus.Utility;
using System.Text.Json.Nodes;
using System.Text.Json.Serialization;

namespace AssistantPlus.View.PackageBuilder {

	public record PackageVersion {
		public Integer Number                              = default!;
		public Integer ExtendedTextureInformationForPvz2Cn = default!;
	}

	public record PackageCompression {
		public Boolean      General = default!;
		public Boolean      Texture = default!;
		public List<String> Filter  = default!;
	}

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

	public record PackageManifest {
		public PackageManifestType Type   = default!;
		public String              Suffix = default!;
	}

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

	public record ResourceCategory {
		public Integer? Resolution = default!;
		public String?  Locale     = default!;
	}

	public record DummyResourceProperty {
	}

	public record GeneralResourceProperty {
		public String Path = default!;
		public String Type = default!;
	}

	public record TextureResourcePropertySpriteResource {
		public String        Id       = default!;
		public String        Path     = default!;
		public List<Integer> Position = default!;
		public List<Integer> Size     = default!;
		public List<Integer> Offset   = default!;
		public List<Integer> Separate = default!;
	}

	public record TextureResourceProperty {
		public String                                      Path                = default!;
		public Integer                                     Format              = default!;
		public Integer                                     Pitch               = default!;
		public Integer                                     AdditionalByteCount = default!;
		public List<Integer>                               Size                = default!;
		public List<TextureResourcePropertySpriteResource> Sprite              = default!;
	}

	public record SpecialRtonResourceProperty {
		public String Conversion = default!;
		public String Path       = default!;
	}

	public record SpecialPtxResourcePropertySpriteResource {
		public String        Source   = default!;
		public String        Id       = default!;
		public String        Path     = default!;
		public List<Integer> Offset   = default!;
		public List<Integer> Separate = default!;
	}

	public record SpecialPtxResourceProperty {
		public String                                         Conversion = default!;
		public String                                         Path       = default!;
		public Integer                                        Resolution = default!;
		public List<SpecialPtxResourcePropertySpriteResource> Sprite     = default!;
	}

	public record SpecialPamResourceProperty {
		public String Conversion = default!;
		public String Path       = default!;
	}

	public record SpecialPopfxResourceProperty {
		public String Conversion = default!;
		public String Path       = default!;
	}

	public record SpecialWemResourceProperty {
		public String Conversion = default!;
		public String Path       = default!;
	}

	public record SpecialBnkResourceProperty {
		public String Conversion = default!;
		public String Path       = default!;
	}

	public record ResourceSetting {
		public ResourceCategory Category = default!;
		public ResourceType     Type     = default!;
		public JsonNode         Property = default!;
		public List<Variable>   Variable = default!;
	}

	public record GroupSetting {
		public List<Variable> Variable = default!;
	}

	public record PartSetting {
		public List<Variable> Variable = default!;
	}

	public record PackageConversionItem {
		public String Name = default!;
		[JsonExtensionData()]
		public Dictionary<String, Object> AdditionalData = default!;
	}

	public record PackageConversion {
		public List<PackageConversionItem> Rton = default!;
		public List<PackageConversionItem> Ptx  = default!;
		public List<PackageConversionItem> Pam  = default!;
		public List<PackageConversionItem> Wem  = default!;
	}

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

	public record ProjectSetting {
		public List<PackageSetting> Package = default!;
	}

}
