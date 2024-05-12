#pragma warning disable 0,
// ReSharper disable CheckNamespace

using AssistantPlus;
using AssistantPlus.Utility;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;

namespace AssistantPlus {

	public static class GameReflectionModel {

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

		[JsonConverter(typeof(GameReflectionHelper.CompositeTypeJsonConverter))]
		public record CompositeType {
			public PrimitiveType  Primitive = default!;
			public CompositeType? Element   = default!;
			public String?        Class     = default!;
		}

		// ----------------

		[JsonObject(ItemRequired = Required.AllowNull)]
		public record FixedValue {
			public System.Object Value = default!;
		}

		[JsonObject(ItemRequired = Required.AllowNull)]
		public record NamedValue : FixedValue {
			public String Name = default!;
		}

		[JsonObject(ItemRequired = Required.AllowNull)]
		public record ExtraValue : NamedValue {
			public CompositeType Type = default!;
		}

		// ----------------

		[JsonObject(ItemRequired = Required.AllowNull)]
		public record FixedObject {
			public List<List<FixedValue?>> Property      = default!;
			public List<ExtraValue>        ExtraProperty = default!;
		}

		[JsonObject(ItemRequired = Required.AllowNull)]
		public record AddressedFixedObject : FixedObject {
			public String? Alias = default!;
			public String  Class = default!;
		}

		// ----------------

		[JsonObject(ItemRequired = Required.AllowNull)]
		public record ValueDescriptor {
			public String Value       = default!;
			public String Description = default!;
		}

		[JsonObject(ItemRequired = Required.AllowNull)]
		public record EnumerationDescriptor {
			public String                Name        = default!;
			public List<ValueDescriptor> Value       = default!;
			public String                Description = default!;
		}

		[JsonObject(ItemRequired = Required.AllowNull)]
		public record PropertyDescriptor {
			public String        Name        = default!;
			public CompositeType Type        = default!;
			public String        Description = default!;
		}

		[JsonObject(ItemRequired = Required.AllowNull)]
		public record ObjectDescriptor {
			public String                   Name        = default!;
			public List<PropertyDescriptor> Property    = default!;
			public List<ObjectDescriptor>   Derived     = default!;
			public String                   Description = default!;
		}

		// ----------------

		[JsonObject(ItemRequired = Required.AllowNull)]
		public record DescriptorArchive {
			public List<EnumerationDescriptor> Enumeration = default!;
			public List<ObjectDescriptor>      Object      = default!;
		}

		[JsonObject(ItemRequired = Required.AllowNull)]
		public record DescriptorMap {
			public Dictionary<String, EnumerationDescriptor>  Enumeration = default!;
			public Dictionary<String, List<ObjectDescriptor>> Object      = default!;
		}

		// ----------------

		[JsonObject(ItemRequired = Required.Default)]
		public record DataObject {
			public String?       uid      = default!;
			public List<String>? aliases  = default!;
			public String        objclass = default!;
			public JObject       objdata  = default!;
		}

		[JsonObject(ItemRequired = Required.Default)]
		public record DataArchive {
			public Integer          version = default!;
			public List<DataObject> objects = default!;
		}

		#endregion

	}

}
