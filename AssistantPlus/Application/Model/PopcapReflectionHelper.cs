#pragma warning disable 0,
// ReSharper disable CheckNamespace

using Twinning.AssistantPlus;
using Twinning.AssistantPlus.Utility;
using System.Text.Json;
using System.Text.Json.Nodes;
using System.Text.Json.Serialization;

namespace Twinning.AssistantPlus {

	public static class PopcapReflectionHelper {

		#region utility

		public class CompositeTypeJsonConverter : JsonConverter<PopcapReflectionModel.CompositeType> {

			public override Boolean CanConvert (
				Type objectType
			) {
				return objectType == typeof(PopcapReflectionModel.CompositeType);
			}

			public override void Write (
				Utf8JsonWriter                      writer,
				PopcapReflectionModel.CompositeType value,
				JsonSerializerOptions               options
			) {
				throw new NotImplementedException();
			}

			[UnconditionalSuppressMessage("Trimming", "IL2026")]
			[UnconditionalSuppressMessage("AOT", "IL3050")]
			public override PopcapReflectionModel.CompositeType Read (
				ref Utf8JsonReader    reader,
				Type                  typeToConvert,
				JsonSerializerOptions options
			) {
				return PopcapReflectionHelper.ParseTypeExpression(JsonSerializer.Deserialize<String>(ref reader, options).AsNotNull());
			}

		}

		// ----------------

		public static PopcapReflectionModel.CompositeType ParseTypeExpression (
			List<String> ripe
		) {
			var raw = new PopcapReflectionModel.CompositeType() {
				Primitive = default!,
				Element = null,
				Class = null,
			};
			switch (ripe[0]) {
				case nameof(PopcapReflectionModel.PrimitiveType.Boolean): {
					AssertTest(ripe.Count == 1);
					raw.Primitive = PopcapReflectionModel.PrimitiveType.Boolean;
					break;
				}
				case nameof(PopcapReflectionModel.PrimitiveType.Integer): {
					AssertTest(ripe.Count == 1);
					raw.Primitive = PopcapReflectionModel.PrimitiveType.Integer;
					break;
				}
				case nameof(PopcapReflectionModel.PrimitiveType.Floater): {
					AssertTest(ripe.Count == 1);
					raw.Primitive = PopcapReflectionModel.PrimitiveType.Floater;
					break;
				}
				case nameof(PopcapReflectionModel.PrimitiveType.String): {
					AssertTest(ripe.Count == 1);
					raw.Primitive = PopcapReflectionModel.PrimitiveType.String;
					break;
				}
				case nameof(PopcapReflectionModel.PrimitiveType.Enumeration): {
					AssertTest(ripe.Count == 2);
					raw.Primitive = PopcapReflectionModel.PrimitiveType.Enumeration;
					raw.Class = ripe[1];
					break;
				}
				case nameof(PopcapReflectionModel.PrimitiveType.Reference): {
					AssertTest(ripe.Count == 2);
					raw.Primitive = PopcapReflectionModel.PrimitiveType.Reference;
					raw.Class = ripe[1];
					break;
				}
				case nameof(PopcapReflectionModel.PrimitiveType.List): {
					AssertTest(ripe.Count >= 2);
					raw.Primitive = PopcapReflectionModel.PrimitiveType.List;
					raw.Element = PopcapReflectionHelper.ParseTypeExpression(ripe[1..]);
					break;
				}
				case nameof(PopcapReflectionModel.PrimitiveType.Map): {
					AssertTest(ripe.Count >= 2);
					raw.Primitive = PopcapReflectionModel.PrimitiveType.Map;
					raw.Element = PopcapReflectionHelper.ParseTypeExpression(ripe[1..]);
					break;
				}
				case nameof(PopcapReflectionModel.PrimitiveType.Object): {
					AssertTest(ripe.Count == 2);
					raw.Primitive = PopcapReflectionModel.PrimitiveType.Object;
					raw.Class = ripe[1];
					break;
				}
				default: throw new ();
			}
			return raw;
		}

		public static PopcapReflectionModel.CompositeType ParseTypeExpression (
			String ripe
		) {
			return PopcapReflectionHelper.ParseTypeExpression(ripe.Split(".").ToList());
		}

		public static String MakeTypeName (
			PopcapReflectionModel.CompositeType ripe
		) {
			return ripe.Primitive switch {
				PopcapReflectionModel.PrimitiveType.Boolean     => $"{ripe.Primitive}",
				PopcapReflectionModel.PrimitiveType.Integer     => $"{ripe.Primitive}",
				PopcapReflectionModel.PrimitiveType.Floater     => $"{ripe.Primitive}",
				PopcapReflectionModel.PrimitiveType.String      => $"{ripe.Primitive}",
				PopcapReflectionModel.PrimitiveType.Enumeration => $"{ripe.Primitive} • {ripe.Class.AsNotNull()}",
				PopcapReflectionModel.PrimitiveType.Reference   => $"{ripe.Primitive} • {ripe.Class.AsNotNull()}",
				PopcapReflectionModel.PrimitiveType.List        => $"{ripe.Primitive} • {PopcapReflectionHelper.MakeTypeName(ripe.Element.AsNotNull())}",
				PopcapReflectionModel.PrimitiveType.Map         => $"{ripe.Primitive} • {PopcapReflectionHelper.MakeTypeName(ripe.Element.AsNotNull())}",
				PopcapReflectionModel.PrimitiveType.Object      => $"{ripe.Primitive} • {ripe.Class.AsNotNull()}",
				_                                               => throw new UnreachableException(),
			};
		}

		// ----------------

		public static String MakeEnumerationExpression (
			Tuple<String> ripe
		) {
			return ripe.Item1;
		}

		public static Tuple<String> ParseEnumerationExpression (
			String raw
		) {
			var ripe = new Tuple<String>(raw);
			return ripe;
		}

		// ----------------

		public static String MakeReferenceExpression (
			Tuple<String, String> ripe
		) {
			return $"RTID({(ripe.Item1.Length == 0 && ripe.Item2.Length == 0 ? "0" : $"{ripe.Item2}@{ripe.Item1}")})";
		}

		public static Tuple<String, String> ParseReferenceExpression (
			String raw
		) {
			var rawMatch = Regex.Match(raw, @"^RTID\(([0-9a-zA-Z.$]+)[@]([0-9a-zA-Z.$]+)\)$");
			AssertTest(rawMatch.Success);
			var ripe = new Tuple<String, String>(rawMatch.Groups[2].Value, rawMatch.Groups[1].Value);
			return ripe;
		}

		// ----------------

		public static JsonNode MakeDataValue (
			PopcapReflectionModel.DescriptorMap descriptor,
			PopcapReflectionModel.CompositeType type,
			Object                              ripe
		) {
			var raw = default(JsonNode);
			switch (type.Primitive) {
				case PopcapReflectionModel.PrimitiveType.Boolean: {
					AssertTest(type.Element == null && type.Class == null);
					raw = JsonHelper.SerializeNode(ripe.As<Boolean>());
					break;
				}
				case PopcapReflectionModel.PrimitiveType.Integer: {
					AssertTest(type.Element == null && type.Class == null);
					raw = JsonHelper.SerializeNode(ripe.As<Integer>());
					break;
				}
				case PopcapReflectionModel.PrimitiveType.Floater: {
					AssertTest(type.Element == null && type.Class == null);
					raw = JsonHelper.SerializeNode(ripe.As<Floater>());
					break;
				}
				case PopcapReflectionModel.PrimitiveType.String: {
					AssertTest(type.Element == null && type.Class == null);
					raw = JsonHelper.SerializeNode(ripe.As<String>());
					break;
				}
				case PopcapReflectionModel.PrimitiveType.Enumeration: {
					AssertTest(type.Element == null && type.Class != null);
					raw = JsonHelper.SerializeNode(PopcapReflectionHelper.MakeEnumerationExpression(ripe.As<Tuple<String>>()));
					break;
				}
				case PopcapReflectionModel.PrimitiveType.Reference: {
					AssertTest(type.Element == null && type.Class != null);
					raw = JsonHelper.SerializeNode(PopcapReflectionHelper.MakeReferenceExpression(ripe.As<Tuple<String, String>>()));
					break;
				}
				case PopcapReflectionModel.PrimitiveType.List: {
					AssertTest(type.Element != null && type.Class == null);
					var ripeValue = ripe.As<List<PopcapReflectionModel.FixedValue>>();
					var rawValue = new JsonArray();
					foreach (var ripeValueItem in ripeValue) {
						rawValue.Add(PopcapReflectionHelper.MakeDataValue(descriptor, type.Element, ripeValueItem.Value));
					}
					raw = rawValue;
					break;
				}
				case PopcapReflectionModel.PrimitiveType.Map: {
					AssertTest(type.Element != null && type.Class == null);
					var ripeValue = ripe.As<List<PopcapReflectionModel.NamedValue>>();
					var rawValue = new JsonObject();
					foreach (var ripeValueItem in ripeValue) {
						rawValue.Add(ripeValueItem.Name, PopcapReflectionHelper.MakeDataValue(descriptor, type.Element, ripeValueItem.Value));
					}
					raw = rawValue;
					break;
				}
				case PopcapReflectionModel.PrimitiveType.Object: {
					AssertTest(type.Element == null && type.Class != null);
					var objectDescriptor = descriptor.Object[type.Class];
					var ripeValue = ripe.As<PopcapReflectionModel.FixedObject>();
					var rawValue = new JsonObject();
					AssertTest(ripeValue.Property.Count == objectDescriptor.Count);
					for (var propertyGroupIndex = 0; propertyGroupIndex < objectDescriptor.Count; propertyGroupIndex++) {
						var propertyGroupDescriptor = objectDescriptor[propertyGroupIndex];
						var propertyGroupValue = ripeValue.Property[propertyGroupIndex];
						for (var propertyItemIndex = 0; propertyItemIndex < propertyGroupDescriptor.Property.Count; propertyItemIndex++) {
							var propertyItemDescriptor = propertyGroupDescriptor.Property[propertyItemIndex];
							var propertyItemValue = propertyGroupValue[propertyItemIndex];
							if (propertyItemValue != null) {
								rawValue.Add(propertyItemDescriptor.Name, PopcapReflectionHelper.MakeDataValue(descriptor, propertyItemDescriptor.Type, propertyItemValue.Value));
							}
						}
					}
					foreach (var ripeValueExtraProperty in ripeValue.ExtraProperty) {
						rawValue.Add(ripeValueExtraProperty.Name, PopcapReflectionHelper.MakeDataValue(descriptor, ripeValueExtraProperty.Type, ripeValueExtraProperty.Value));
					}
					raw = rawValue;
					break;
				}
				default: throw new UnreachableException();
			}
			return raw;
		}

		public static Object ParseDataValue (
			PopcapReflectionModel.DescriptorMap descriptor,
			PopcapReflectionModel.CompositeType type,
			JsonNode                            raw
		) {
			var ripe = default(Object);
			switch (type.Primitive) {
				case PopcapReflectionModel.PrimitiveType.Boolean: {
					AssertTest(type.Element == null && type.Class == null);
					ripe = JsonHelper.DeserializeNode<Boolean>(raw);
					break;
				}
				case PopcapReflectionModel.PrimitiveType.Integer: {
					AssertTest(type.Element == null && type.Class == null);
					ripe = JsonHelper.DeserializeNode<Integer>(raw);
					break;
				}
				case PopcapReflectionModel.PrimitiveType.Floater: {
					AssertTest(type.Element == null && type.Class == null);
					ripe = JsonHelper.DeserializeNode<Floater>(raw);
					break;
				}
				case PopcapReflectionModel.PrimitiveType.String: {
					AssertTest(type.Element == null && type.Class == null);
					ripe = JsonHelper.DeserializeNode<String>(raw);
					break;
				}
				case PopcapReflectionModel.PrimitiveType.Enumeration: {
					AssertTest(type.Element == null && type.Class != null);
					ripe = PopcapReflectionHelper.ParseEnumerationExpression(JsonHelper.DeserializeNode<String>(raw));
					break;
				}
				case PopcapReflectionModel.PrimitiveType.Reference: {
					AssertTest(type.Element == null && type.Class != null);
					ripe = PopcapReflectionHelper.ParseReferenceExpression(JsonHelper.DeserializeNode<String>(raw));
					break;
				}
				case PopcapReflectionModel.PrimitiveType.List: {
					AssertTest(type.Element != null && type.Class == null);
					var rawValue = raw.AsArray();
					var ripeValue = new List<PopcapReflectionModel.FixedValue>(rawValue.Count);
					foreach (var rawValueItem in rawValue) {
						AssertTest(rawValueItem != null);
						ripeValue.Add(new () { Value = PopcapReflectionHelper.ParseDataValue(descriptor, type.Element, rawValueItem) });
					}
					ripe = ripeValue;
					break;
				}
				case PopcapReflectionModel.PrimitiveType.Map: {
					AssertTest(type.Element != null && type.Class == null);
					var rawValue = raw.AsObject();
					var ripeValue = new List<PopcapReflectionModel.NamedValue>(rawValue.Count);
					foreach (var rawValueItem in rawValue) {
						AssertTest(rawValueItem.Value != null);
						ripeValue.Add(new () { Name = rawValueItem.Key, Value = PopcapReflectionHelper.ParseDataValue(descriptor, type.Element, rawValueItem.Value) });
					}
					ripe = ripeValue;
					break;
				}
				case PopcapReflectionModel.PrimitiveType.Object: {
					AssertTest(type.Element == null && type.Class != null);
					var objectDescriptor = descriptor.Object[type.Class];
					var rawValue = raw.AsObject();
					var ripeValue = new PopcapReflectionModel.FixedObject() {
						Property = [],
						ExtraProperty = [],
					};
					foreach (var propertyGroupDescriptor in objectDescriptor) {
						var ripePropertyGroup = new List<PopcapReflectionModel.FixedValue?>(propertyGroupDescriptor.Property.Count);
						foreach (var propertyItemDescriptor in propertyGroupDescriptor.Property) {
							if (rawValue.ContainsKey(propertyItemDescriptor.Name)) {
								ripePropertyGroup.Add(new () { Value = PopcapReflectionHelper.ParseDataValue(descriptor, propertyItemDescriptor.Type, rawValue[propertyItemDescriptor.Name].AsNotNull()) });
								rawValue.Remove(propertyItemDescriptor.Name);
							}
							else {
								ripePropertyGroup.Add(null);
							}
						}
						ripeValue.Property.Add(ripePropertyGroup);
					}
					// TODO: extra property
					//foreach (var extraItem in sourceValue) {
					//	var extraName = extraItem.Key;
					//	var extraValue = extraItem.Value;
					//	var extraType = (ObjectModel.CompositeType)null;
					//	destinationValue.ExtraProperty.Add(new ObjectModel.ExtraValue() { Value = extraItem.Value, Name = extraItem.Key, Type =  });
					//}
					ripe = ripeValue;
					break;
				}
				default: throw new UnreachableException();
			}
			return ripe;
		}

		// ----------------

		public static Object GenerateDataValueDefault (
			PopcapReflectionModel.DescriptorMap descriptor,
			PopcapReflectionModel.CompositeType type,
			Boolean                             withElement
		) {
			var value = default(Object);
			switch (type.Primitive) {
				case PopcapReflectionModel.PrimitiveType.Boolean: {
					AssertTest(type.Element == null && type.Class == null);
					value = (Boolean)false;
					break;
				}
				case PopcapReflectionModel.PrimitiveType.Integer: {
					AssertTest(type.Element == null && type.Class == null);
					value = (Integer)0;
					break;
				}
				case PopcapReflectionModel.PrimitiveType.Floater: {
					AssertTest(type.Element == null && type.Class == null);
					value = (Floater)0.0;
					break;
				}
				case PopcapReflectionModel.PrimitiveType.String: {
					AssertTest(type.Element == null && type.Class == null);
					value = (String)"";
					break;
				}
				case PopcapReflectionModel.PrimitiveType.Enumeration: {
					AssertTest(type.Element == null && type.Class != null);
					value = new Tuple<String>("");
					break;
				}
				case PopcapReflectionModel.PrimitiveType.Reference: {
					AssertTest(type.Element == null && type.Class != null);
					value = new Tuple<String, String>("", "");
					break;
				}
				case PopcapReflectionModel.PrimitiveType.List: {
					AssertTest(type.Element != null && type.Class == null);
					value = new List<PopcapReflectionModel.FixedValue>();
					if (withElement) {
						value.As<List<PopcapReflectionModel.FixedValue>>().Add(new () { Value = PopcapReflectionHelper.GenerateDataValueDefault(descriptor, type.Element, withElement) });
					}
					break;
				}
				case PopcapReflectionModel.PrimitiveType.Map: {
					AssertTest(type.Element != null && type.Class == null);
					value = new List<PopcapReflectionModel.NamedValue>();
					if (withElement) {
						value.As<List<PopcapReflectionModel.NamedValue>>().Add(new () { Name = "", Value = PopcapReflectionHelper.GenerateDataValueDefault(descriptor, type.Element, withElement) });
					}
					break;
				}
				case PopcapReflectionModel.PrimitiveType.Object: {
					AssertTest(type.Element == null && type.Class != null);
					var objectDescriptor = descriptor.Object[type.Class];
					value = new PopcapReflectionModel.FixedObject() {
						Property = objectDescriptor.Select((propertyGroupDescriptor) => (propertyGroupDescriptor.Property.Select((propertyItemDescriptor) => (!withElement ? null : new PopcapReflectionModel.FixedValue() { Value = PopcapReflectionHelper.GenerateDataValueDefault(descriptor, propertyItemDescriptor.Type, withElement) })).ToList())).ToList(),
						ExtraProperty = [],
					};
					break;
				}
				default: throw new UnreachableException();
			}
			return value;
		}

		#endregion

		#region compile

		public static void CompileEnumerationDescriptor (
			Dictionary<String, PopcapReflectionModel.EnumerationDescriptor> ripe,
			PopcapReflectionModel.EnumerationDescriptor                     raw
		) {
			ripe.Add(raw.Name, raw);
			return;
		}

		public static void CompileObjectDescriptor (
			Dictionary<String, List<PopcapReflectionModel.ObjectDescriptor>> ripe,
			PopcapReflectionModel.ObjectDescriptor                           raw,
			List<PopcapReflectionModel.ObjectDescriptor>                     rawParent
		) {
			var ripeItem = new List<PopcapReflectionModel.ObjectDescriptor>([..rawParent, raw]);
			ripe.Add(raw.Name, ripeItem);
			foreach (var rawDerived in raw.Derived) {
				PopcapReflectionHelper.CompileObjectDescriptor(ripe, rawDerived, ripeItem);
			}
			return;
		}

		public static PopcapReflectionModel.DescriptorMap CompileDescriptorArchive (
			PopcapReflectionModel.DescriptorArchive raw
		) {
			var ripe = new PopcapReflectionModel.DescriptorMap() {
				Enumeration = [],
				Object = [],
			};
			foreach (var rawEnumeration in raw.Enumeration) {
				PopcapReflectionHelper.CompileEnumerationDescriptor(ripe.Enumeration, rawEnumeration);
			}
			foreach (var rawObject in raw.Object) {
				PopcapReflectionHelper.CompileObjectDescriptor(ripe.Object, rawObject, []);
			}
			return ripe;
		}

		// ----------------

		public static List<PopcapReflectionModel.AddressedFixedObject> CompileDataArchive (
			PopcapReflectionModel.DescriptorMap descriptor,
			PopcapReflectionModel.DataArchive   raw
		) {
			var ripe = new List<PopcapReflectionModel.AddressedFixedObject>(raw.objects.Count);
			foreach (var rawObject in raw.objects) {
				var objectType = new PopcapReflectionModel.CompositeType() {
					Primitive = PopcapReflectionModel.PrimitiveType.Object,
					Element = null,
					Class = rawObject.objclass,
				};
				var ripeObject = PopcapReflectionHelper.ParseDataValue(descriptor, objectType, rawObject.objdata).As<PopcapReflectionModel.FixedObject>();
				ripe.Add(new () {
					Alias = rawObject.aliases != null && rawObject.aliases.Count == 1 ? rawObject.aliases[0] : null,
					Class = rawObject.objclass,
					Property = ripeObject.Property,
					ExtraProperty = ripeObject.ExtraProperty,
				});
			}
			return ripe;
		}

		#endregion

		#region load

		public static async Task<PopcapReflectionModel.DescriptorArchive> LoadDescriptorArchive (
			String file
		) {
			return await JsonHelper.DeserializeFile<PopcapReflectionModel.DescriptorArchive>(file);
		}

		public static async Task<PopcapReflectionModel.DataArchive> LoadDataArchive (
			String file
		) {
			return await JsonHelper.DeserializeFile<PopcapReflectionModel.DataArchive>(file);
		}

		#endregion

	}

}
