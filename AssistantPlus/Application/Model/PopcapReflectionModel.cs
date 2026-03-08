#pragma warning disable 0,
// ReSharper disable CheckNamespace

using Twinning.AssistantPlus;
using Twinning.AssistantPlus.Utility;
using System.Text.Json.Nodes;
using JsonOptionalAttribute = Twinning.AssistantPlus.Utility.JsonHelper.JsonOptionalAttribute;

namespace Twinning.AssistantPlus {

	public static class PopcapReflectionModel {

		#region type

		public enum PrimitiveType {
			Boolean,
			Integer,
			Floater,
			String,
			Enumeration,
			Reference,
			List,
			Map,
			Object,
		}

		public record CompositeType {
			public PrimitiveType  Primitive { get; set; } = default!;
			public CompositeType? Element   { get; set; } = default!;
			public String?        Class     { get; set; } = default!;
		}

		// ----------------

		public record FixedValue {
			public Object Value { get; set; } = default!;
		}

		public record NamedValue : FixedValue {
			public String Name { get; set; } = default!;
		}

		public record ExtraValue : NamedValue {
			public CompositeType Type { get; set; } = default!;
		}

		// ----------------

		public record FixedObject {
			public List<List<FixedValue?>> Property      { get; set; } = default!;
			public List<ExtraValue>        ExtraProperty { get; set; } = default!;
		}

		public record AddressedFixedObject : FixedObject {
			public String? Alias { get; set; } = default!;
			public String  Class { get; set; } = default!;
		}

		// ----------------

		public record ValueDescriptor {
			public String Value       { get; set; } = default!;
			public String Description { get; set; } = default!;
		}

		public record EnumerationDescriptor {
			public String                Name        { get; set; } = default!;
			public List<ValueDescriptor> Value       { get; set; } = default!;
			public String                Description { get; set; } = default!;
		}

		public record PropertyDescriptor {
			public String        Name        { get; set; } = default!;
			public CompositeType Type        { get; set; } = default!;
			public String        Description { get; set; } = default!;
		}

		public record ObjectDescriptor {
			public String                   Name        { get; set; } = default!;
			public List<PropertyDescriptor> Property    { get; set; } = default!;
			public List<ObjectDescriptor>   Derived     { get; set; } = default!;
			public String                   Description { get; set; } = default!;
		}

		// ----------------

		public record DescriptorArchive {
			public List<EnumerationDescriptor> Enumeration { get; set; } = default!;
			public List<ObjectDescriptor>      Object      { get; set; } = default!;
		}

		public record DescriptorMap {
			public Dictionary<String, EnumerationDescriptor>  Enumeration { get; set; } = default!;
			public Dictionary<String, List<ObjectDescriptor>> Object      { get; set; } = default!;
		}

		// ----------------

		// ReSharper disable InconsistentNaming

		// @formatter:off

		public record DataObject {
			[JsonOptional()]
			public String?       uid      { get; set; } = default!;
			[JsonOptional()]
			public List<String>? aliases  { get; set; } = default!;
			public String        objclass { get; set; } = default!;
			public JsonObject    objdata  { get; set; } = default!;
		}

		public record DataArchive {
			[JsonOptional()]
			public Integer?         version { get; set; } = default!;
			public List<DataObject> objects { get; set; } = default!;
		}

		// @formatter:on

		// ReSharper restore InconsistentNaming

		#endregion

	}

}
