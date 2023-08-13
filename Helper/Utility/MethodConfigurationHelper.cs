#pragma warning disable 0,
// ReSharper disable

using Helper;
using Newtonsoft.Json.Linq;

namespace Helper.Utility {

	public static class MethodConfigurationHelper {

		#region make

		public static Object MakeArgumentValueDefault (
			MethodConfigurationModel.ArgumentType type
		) {
			return type switch {
				MethodConfigurationModel.ArgumentType.Confirmation => false,
				MethodConfigurationModel.ArgumentType.Number       => (Floater)0.0,
				MethodConfigurationModel.ArgumentType.Integer      => (Integer)0,
				MethodConfigurationModel.ArgumentType.Size         => new MethodConfigurationModel.SizeExpression(),
				MethodConfigurationModel.ArgumentType.String       => "",
				MethodConfigurationModel.ArgumentType.Path         => "",
				_                                                  => throw new Exception(),
			};
		}

		public static Object? MakeArgumentValueDefault (
			MethodConfigurationModel.ArgumentType type,
			Object?                               initial
		) {
			return type switch {
				MethodConfigurationModel.ArgumentType.Confirmation => initial as Boolean?,
				MethodConfigurationModel.ArgumentType.Number       => initial as Floater?,
				MethodConfigurationModel.ArgumentType.Integer      => initial as Integer?,
				MethodConfigurationModel.ArgumentType.Size         => initial is String initialValue ? MethodConfigurationModel.SizeExpression.Parse(initialValue) : null,
				MethodConfigurationModel.ArgumentType.String       => initial as String,
				MethodConfigurationModel.ArgumentType.Path         => initial as String,
				_                                                  => throw new Exception(),
			};
		}

		public static List<MethodConfigurationModel.ArgumentValue> MakeArgumentValueDefault (
			List<MethodConfigurationModel.ArgumentConfiguration> configuration
		) {
			return configuration.Select((value) => new MethodConfigurationModel.ArgumentValue() {
				Value = MethodConfigurationHelper.MakeArgumentValueDefault(value.Type, value.Initial),
			}).ToList();
		}

		// ----------------

		public static String MakeArgumentValueString (
			List<MethodConfigurationModel.ArgumentConfiguration> configuration,
			List<MethodConfigurationModel.ArgumentValue>         value
		) {
			Debug.Assert(configuration.Count == value.Count);
			var json = new JObject();
			for (var i = 0; i < configuration.Count; i++) {
				var itemConfiguration = configuration[i];
				var itemValue = value[i];
				if (itemValue.Value is not null) {
					json.Add(itemConfiguration.Id, new JValue(itemValue.Value is MethodConfigurationModel.SizeExpression itemValueSize ? itemValueSize.ToString() : itemValue.Value));
				}
			}
			return JsonHelper.Serialize(json, false);
		}

		#endregion

	}

}
