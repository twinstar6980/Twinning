#pragma warning disable 0,
// ReSharper disable

using Helper;
using Newtonsoft.Json.Linq;

namespace Helper.Utility {

	public static class CommandConfigurationHelper {

		#region make

		public static String MakeArgumentValueString (
			Object value
		) {
			return value switch {
				Boolean valueValue                                  => ConvertHelper.BooleanToSwitchString(valueValue),
				Floater valueValue                                  => valueValue.ToString() ?? throw new Exception(),
				Integer valueValue                                  => valueValue.ToString(),
				CommandConfigurationModel.SizeExpression valueValue => valueValue.ToString(),
				String valueValue                                   => valueValue.ToString(),
				CommandConfigurationModel.PathExpression valueValue => valueValue.ToString(),
				_                                                   => throw new Exception(),
			};
		}

		// ----------------

		public static Object MakeArgumentValueExpression (
			Object value
		) {
			return value switch {
				Boolean valueValue                                  => valueValue,
				Floater valueValue                                  => valueValue,
				Integer valueValue                                  => valueValue,
				CommandConfigurationModel.SizeExpression valueValue => valueValue.ToString(),
				String valueValue                                   => valueValue,
				CommandConfigurationModel.PathExpression valueValue => valueValue.ToString(),
				_                                                   => throw new Exception(),
			};
		}

		// ----------------

		public static Object MakeArgumentValueDefault (
			CommandConfigurationModel.ArgumentType type
		) {
			return type switch {
				CommandConfigurationModel.ArgumentType.Confirmation => false,
				CommandConfigurationModel.ArgumentType.Number       => (Floater)0.0,
				CommandConfigurationModel.ArgumentType.Integer      => (Integer)0,
				CommandConfigurationModel.ArgumentType.Size         => new CommandConfigurationModel.SizeExpression(),
				CommandConfigurationModel.ArgumentType.String       => "",
				CommandConfigurationModel.ArgumentType.Path         => new CommandConfigurationModel.PathExpression(),
				_                                                   => throw new Exception(),
			};
		}

		public static Object? MakeArgumentValueDefault (
			CommandConfigurationModel.ArgumentType type,
			Object?                                initial
		) {
			return type switch {
				CommandConfigurationModel.ArgumentType.Confirmation => initial as Boolean?,
				CommandConfigurationModel.ArgumentType.Number       => initial as Floater?,
				CommandConfigurationModel.ArgumentType.Integer      => initial as Integer?,
				CommandConfigurationModel.ArgumentType.Size         => initial is String initialValue ? CommandConfigurationModel.SizeExpression.Parse(initialValue) : null,
				CommandConfigurationModel.ArgumentType.String       => initial as String,
				CommandConfigurationModel.ArgumentType.Path         => initial is String initialValue ? CommandConfigurationModel.PathExpression.Parse(initialValue) : null,
				_                                                   => throw new Exception(),
			};
		}

		public static List<CommandConfigurationModel.ArgumentValue> MakeArgumentValueDefault (
			List<CommandConfigurationModel.ArgumentConfiguration> configuration
		) {
			return configuration.Select((value) => new CommandConfigurationModel.ArgumentValue() {
				Value = CommandConfigurationHelper.MakeArgumentValueDefault(value.Type, value.Initial),
			}).ToList();
		}

		// ----------------

		public static String MakeArgumentObjectString (
			List<CommandConfigurationModel.ArgumentConfiguration> configuration,
			List<CommandConfigurationModel.ArgumentValue>         value
		) {
			Debug.Assert(configuration.Count == value.Count);
			var json = new JObject();
			for (var index = 0; index < configuration.Count; index++) {
				var itemConfiguration = configuration[index];
				var itemValue = value[index];
				if (itemValue.Value is not null) {
					json.Add(
						itemConfiguration.Id,
						new JValue(CommandConfigurationHelper.MakeArgumentValueExpression(itemValue.Value))
					);
				}
			}
			return JsonHelper.Serialize(json, false);
		}

		#endregion

	}

}
