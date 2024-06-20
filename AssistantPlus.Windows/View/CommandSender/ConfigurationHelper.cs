#pragma warning disable 0,
// ReSharper disable CheckNamespace

using AssistantPlus;
using AssistantPlus.Utility;
using Newtonsoft.Json.Linq;

namespace AssistantPlus.View.CommandSender {

	public static class ConfigurationHelper {

		#region utility

		public static Object MakeArgumentValueJson (
			ValueExpression value
		) {
			return value switch {
				BooleanExpression values => values.Value,
				IntegerExpression values => values.Value,
				FloaterExpression values => values.Value,
				SizeExpression values    => $"{ConvertHelper.MakeFloaterToString(values.Count, false)}{new[] { "b", "k", "m", "g" }[values.Exponent]}",
				StringExpression values  => values.Value,
				PathExpression values    => $"{values.Content}",
				_                        => throw new (),
			};
		}

		public static ValueExpression ParseArgumentValueJson (
			ArgumentType type,
			Object       json
		) {
			return type switch {
				ArgumentType.Boolean => new BooleanExpression() {
					Value = json.AsStruct<Boolean>(),
				},
				ArgumentType.Integer => new IntegerExpression() {
					Value = json.AsStruct<Integer>(),
				},
				ArgumentType.Floater => new FloaterExpression() {
					Value = json.AsStruct<Floater>(),
				},
				ArgumentType.Size => new SizeExpression() {
					Count = Floater.Parse(json.AsClass<String>()[..^1]),
					Exponent = new[] { 'b', 'k', 'm', 'g' }.ToList().IndexOf(json.AsClass<String>()[^1]).ApplySelf((it) => { GF.AssertTest(it != -1); }),
				},
				ArgumentType.String => new StringExpression() {
					Value = json.AsClass<String>(),
				},
				ArgumentType.Path => new PathExpression() {
					Content = json.AsClass<String>(),
				},
				_ => throw new (),
			};
		}

		// ----------------

		public static Dictionary<String, Object> MakeArgumentValueListJson (
			List<ArgumentConfiguration>    configuration,
			List<Wrapper<ValueExpression>> value
		) {
			GF.AssertTest(configuration.Count == value.Count);
			var json = new Dictionary<String, Object>();
			for (var index = 0; index < configuration.Count; index++) {
				var itemConfiguration = configuration[index];
				var itemValue = value[index];
				if (itemValue.Value != null) {
					json.Add(itemConfiguration.Id, new JValue(ConfigurationHelper.MakeArgumentValueJson(itemValue.Value)));
				}
			}
			return json;
		}

		public static List<Wrapper<ValueExpression>> ParseArgumentValueListJson (
			List<ArgumentConfiguration> configuration,
			Dictionary<String, Object>  json
		) {
			var value = new List<Wrapper<ValueExpression>>();
			for (var index = 0; index < configuration.Count; index++) {
				var itemConfiguration = configuration[index];
				var itemJson = json.GetValueOrDefault(itemConfiguration.Id);
				value.Add(new (itemJson == null ? null : ConfigurationHelper.ParseArgumentValueJson(itemConfiguration.Type, itemJson)));
			}
			return value;
		}

		#endregion

	}

}
