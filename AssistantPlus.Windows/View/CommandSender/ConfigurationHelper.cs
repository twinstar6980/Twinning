#pragma warning disable 0,
// ReSharper disable CheckNamespace

using AssistantPlus;
using AssistantPlus.Utility;
using Newtonsoft.Json.Linq;

namespace AssistantPlus.View.CommandSender {

	public static class ConfigurationHelper {

		#region make

		public static String MakeArgumentValueString (
			Object value
		) {
			return value switch {
				Boolean values        => ConvertHelper.MakeBooleanToStringOfConfirmationCharacter(values),
				Integer values        => ConvertHelper.MakeIntegerToString(values, false),
				Floater values        => ConvertHelper.MakeFloaterToString(values, false),
				SizeExpression values => SizeExpression.MakeString(values),
				String values         => values,
				PathExpression values => PathExpression.MakeString(values),
				_                     => throw new (),
			};
		}

		// ----------------

		public static Object MakeArgumentValueExpression (
			Object value
		) {
			return value switch {
				Boolean values        => values,
				Integer values        => values,
				Floater values        => values,
				SizeExpression values => SizeExpression.MakeString(values),
				String values         => values,
				PathExpression values => PathExpression.MakeString(values),
				_                     => throw new (),
			};
		}

		// ----------------

		public static Object MakeArgumentValueDefault (
			ArgumentType type
		) {
			return type switch {
				ArgumentType.Boolean => (Boolean)false,
				ArgumentType.Integer => (Integer)0,
				ArgumentType.Floater => (Floater)0.0,
				ArgumentType.Size    => new SizeExpression(),
				ArgumentType.String  => (String)"",
				ArgumentType.Path    => new PathExpression(),
				_                    => throw new (),
			};
		}

		public static Object? MakeArgumentValueDefault (
			ArgumentType type,
			Object?      initial
		) {
			return type switch {
				ArgumentType.Boolean => initial as Boolean?,
				ArgumentType.Integer => initial as Integer?,
				ArgumentType.Floater => initial as Floater?,
				ArgumentType.Size    => initial is String initialValue ? SizeExpression.ParseString(initialValue) : null,
				ArgumentType.String  => initial as String,
				ArgumentType.Path    => initial is String initialValue ? PathExpression.ParseString(initialValue) : null,
				_                    => throw new (),
			};
		}

		public static List<ArgumentValue> MakeArgumentValueDefault (
			List<ArgumentConfiguration> configuration
		) {
			return configuration.Select((value) => (new ArgumentValue() { Data = ConfigurationHelper.MakeArgumentValueDefault(value.Type, value.Initial) })).ToList();
		}

		// ----------------

		public static String MakeArgumentObjectString (
			List<ArgumentConfiguration> configuration,
			List<ArgumentValue>         value
		) {
			GF.AssertTest(configuration.Count == value.Count);
			var json = new JObject();
			for (var index = 0; index < configuration.Count; index++) {
				var itemConfiguration = configuration[index];
				var itemValue = value[index];
				if (itemValue.Data is not null) {
					json.Add(itemConfiguration.Id, new JValue(ConfigurationHelper.MakeArgumentValueExpression(itemValue.Data)));
				}
			}
			return JsonHelper.SerializeText(json, false);
		}

		#endregion

	}

}
