#pragma warning disable 0,
// ReSharper disable CheckNamespace

using AssistantPlus;
using AssistantPlus.Utility;
using System.Text.Json.Nodes;
using JsonOptionalAttribute = AssistantPlus.Utility.JsonHelper.JsonOptionalAttribute;

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

		public record CompositeType {
			public PrimitiveType  Primitive = default!;
			public CompositeType? Element   = default!;
			public String?        Class     = default!;
		}

		// ----------------

		public record FixedValue {
			public Object Value = default!;
		}

		public record NamedValue : FixedValue {
			public String Name = default!;
		}

		public record ExtraValue : NamedValue {
			public CompositeType Type = default!;
		}

		// ----------------

		public record FixedObject {
			public List<List<FixedValue?>> Property      = default!;
			public List<ExtraValue>        ExtraProperty = default!;
		}

		public record AddressedFixedObject : FixedObject {
			public String? Alias = default!;
			public String  Class = default!;
		}

		// ----------------

		public record ValueDescriptor {
			public String Value       = default!;
			public String Description = default!;
		}

		public record EnumerationDescriptor {
			public String                Name        = default!;
			public List<ValueDescriptor> Value       = default!;
			public String                Description = default!;
		}

		public record PropertyDescriptor {
			public String        Name        = default!;
			public CompositeType Type        = default!;
			public String        Description = default!;
		}

		public record ObjectDescriptor {
			public String                   Name        = default!;
			public List<PropertyDescriptor> Property    = default!;
			public List<ObjectDescriptor>   Derived     = default!;
			public String                   Description = default!;
		}

		// ----------------

		public record DescriptorArchive {
			public List<EnumerationDescriptor> Enumeration = default!;
			public List<ObjectDescriptor>      Object      = default!;
		}

		public record DescriptorMap {
			public Dictionary<String, EnumerationDescriptor>  Enumeration = default!;
			public Dictionary<String, List<ObjectDescriptor>> Object      = default!;
		}

		// ----------------

		public record DataObject {
			[JsonOptional()]
			public String? uid = default!;
			[JsonOptional()]
			public List<String>? aliases = default!;
			public String     objclass = default!;
			public JsonObject objdata  = default!;
		}

		public record DataArchive {
			[JsonOptional()]
			public Integer? version = default!;
			public List<DataObject> objects = default!;
		}

		#endregion

	}

}
