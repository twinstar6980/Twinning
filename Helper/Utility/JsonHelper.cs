#pragma warning disable 0,
// ReSharper disable

using Helper;
using System.Globalization;
using Newtonsoft.Json;
using Newtonsoft.Json.Converters;
using Newtonsoft.Json.Serialization;

namespace Helper.Utility {

	public static class JsonHelper {

		#region serialize

		private static readonly JsonSerializer Serializer = JsonSerializer.CreateDefault(new JsonSerializerSettings() {
			NullValueHandling = NullValueHandling.Include,
			Converters = new List<JsonConverter>() {
				new StringEnumConverter() {
					NamingStrategy = new SnakeCaseNamingStrategy(),
				},
			},
			ContractResolver = new DefaultContractResolver() {
				NamingStrategy = new SnakeCaseNamingStrategy(),
			},
		});

		// ----------------

		public static String Serialize<TValue> (
			TValue  value,
			Boolean indented
		) {
			var text = new StringWriter(new StringBuilder(256), CultureInfo.InvariantCulture);
			var writer = new JsonTextWriter(text) {
				Indentation = 1,
				IndentChar = '\t',
			};
			JsonHelper.Serializer.Formatting = indented ? Formatting.Indented : Formatting.None;
			JsonHelper.Serializer.Serialize(writer, value);
			return text.ToString() ?? throw new Exception();
		}

		public static TValue Deserialize<TValue> (
			String text
		) {
			var reader = new JsonTextReader(new StringReader(text));
			return JsonHelper.Serializer.Deserialize<TValue>(reader) ?? throw new Exception();
		}

		#endregion

	}

}
