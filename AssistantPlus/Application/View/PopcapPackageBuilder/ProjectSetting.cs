#pragma warning disable 0,
// ReSharper disable

using Twinning.AssistantPlus;
using Twinning.AssistantPlus.Utility;
using System.Text.Json.Nodes;
using System.Text.Json.Serialization;

namespace Twinning.AssistantPlus.View.PopcapPackageBuilder {

	public record PackageVersion {
		public Integer Number                              { get; set; } = default!;
		public Integer ExtendedTextureInformationForPvz2Cn { get; set; } = default!;
	}

	public record PackageCompression {
		public Boolean      General { get; set; } = default!;
		public Boolean      Texture { get; set; } = default!;
		public List<String> Filter  { get; set; } = default!;
	}

	public record PackageCategory {
		public List<Integer> Resolution { get; set; } = default!;
		public List<String>  Locale     { get; set; } = default!;
	}

	public enum PackageManifestType {
		Internal,
		ExternalRtonWithArrayPath,
		ExternalRtonWithStringPath,
		ExternalNewton,
	}

	public record PackageManifest {
		public PackageManifestType Type   { get; set; } = default!;
		public String              Suffix { get; set; } = default!;
	}

	public record Variable {
		public String Name  { get; set; } = default!;
		public String Value { get; set; } = default!;
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
		public Integer? Resolution { get; set; } = default!;
		public String?  Locale     { get; set; } = default!;
	}

	public record DummyResourceProperty {
	}

	public record GeneralResourceProperty {
		public String Path { get; set; } = default!;
		public String Type { get; set; } = default!;
	}

	public record TextureResourcePropertySpriteResource {
		public String        Identifier { get; set; } = default!;
		public String        Path       { get; set; } = default!;
		public List<Integer> Position   { get; set; } = default!;
		public List<Integer> Size       { get; set; } = default!;
		public List<Integer> Offset     { get; set; } = default!;
		public List<Integer> Separate   { get; set; } = default!;
	}

	public record TextureResourceProperty {
		public String                                      Path                { get; set; } = default!;
		public Integer                                     Format              { get; set; } = default!;
		public Integer                                     Pitch               { get; set; } = default!;
		public Integer                                     AdditionalByteCount { get; set; } = default!;
		public List<Integer>                               Size                { get; set; } = default!;
		public List<TextureResourcePropertySpriteResource> Sprite              { get; set; } = default!;
	}

	public record SpecialRtonResourceProperty {
		public String Conversion { get; set; } = default!;
		public String Path       { get; set; } = default!;
	}

	public record SpecialPtxResourcePropertySpriteResource {
		public String        Source     { get; set; } = default!;
		public String        Identifier { get; set; } = default!;
		public String        Path       { get; set; } = default!;
		public List<Integer> Offset     { get; set; } = default!;
		public List<Integer> Separate   { get; set; } = default!;
	}

	public record SpecialPtxResourceProperty {
		public String                                         Conversion { get; set; } = default!;
		public String                                         Path       { get; set; } = default!;
		public Integer                                        Resolution { get; set; } = default!;
		public List<SpecialPtxResourcePropertySpriteResource> Sprite     { get; set; } = default!;
	}

	public record SpecialPamResourceProperty {
		public String Conversion { get; set; } = default!;
		public String Path       { get; set; } = default!;
	}

	public record SpecialPopfxResourceProperty {
		public String Conversion { get; set; } = default!;
		public String Path       { get; set; } = default!;
	}

	public record SpecialWemResourceProperty {
		public String Conversion { get; set; } = default!;
		public String Path       { get; set; } = default!;
	}

	public record SpecialBnkResourceProperty {
		public String Conversion { get; set; } = default!;
		public String Path       { get; set; } = default!;
	}

	public record ResourceSetting {
		public ResourceCategory Category { get; set; } = default!;
		public ResourceType     Type     { get; set; } = default!;
		public JsonNode         Property { get; set; } = default!;
		public List<Variable>   Variable { get; set; } = default!;
	}

	public record GroupSetting {
		public List<Variable> Variable { get; set; } = default!;
	}

	public record PartSetting {
		public List<Variable> Variable { get; set; } = default!;
	}

	public record PackageConversionItem {
		public String Name { get; set; } = default!;
		[JsonExtensionData()]
		public Dictionary<String, Object> AdditionalData { get; set; } = default!;
	}

	public record PackageConversion {
		public List<PackageConversionItem> Rton { get; set; } = default!;
		public List<PackageConversionItem> Ptx  { get; set; } = default!;
		public List<PackageConversionItem> Pam  { get; set; } = default!;
		public List<PackageConversionItem> Wem  { get; set; } = default!;
	}

	public record PackageSetting {
		public String             Name        { get; set; } = default!;
		public List<String>       Part        { get; set; } = default!;
		public PackageVersion     Version     { get; set; } = default!;
		public PackageCompression Compression { get; set; } = default!;
		public PackageManifest    Manifest    { get; set; } = default!;
		public PackageCategory    Category    { get; set; } = default!;
		public PackageConversion  Conversion  { get; set; } = default!;
		public List<Variable>     Variable    { get; set; } = default!;
	}

	public record ProjectSetting {
		public List<PackageSetting> Package { get; set; } = default!;
	}

}
