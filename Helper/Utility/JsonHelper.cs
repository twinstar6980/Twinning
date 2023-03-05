#pragma warning disable 0,
// ReSharper disable

using Helper;
using Newtonsoft.Json.Serialization;
using Newtonsoft.Json;
using Newtonsoft.Json.Converters;

namespace Helper.Utility {

	public static class JsonHelper {

		#region serialize

		private static readonly JsonSerializerSettings JsonSerializerSettings = new () {
			Converters = new List<JsonConverter>() {
				new StringEnumConverter() {
					NamingStrategy = new SnakeCaseNamingStrategy(),
				},
			},
			ContractResolver = new DefaultContractResolver() {
				NamingStrategy = new SnakeCaseNamingStrategy(),
			},
		};

		// ----------------

		public static String Serialize<TValue> (
			TValue value
		) {
			return JsonConvert.SerializeObject(value, JsonHelper.JsonSerializerSettings);
		}

		public static TValue Deserialize<TValue> (
			String text
		) {
			return JsonConvert.DeserializeObject<TValue>(text, JsonHelper.JsonSerializerSettings)!;
		}

		#endregion

	}

}
