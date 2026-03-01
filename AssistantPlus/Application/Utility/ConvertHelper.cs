#pragma warning disable 0,
// ReSharper disable

using Twinning.AssistantPlus;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Graphics.Imaging;
using Windows.UI;
using Microsoft.UI.Xaml.Media.Imaging;

namespace Twinning.AssistantPlus.Utility {

	public static class ConvertHelper {

		#region type

		public static Boolean IsTypeOfTuple (
			Type type
		) {
			if (!type.IsGenericType) {
				return false;
			}
			var genericType = type.GetGenericTypeDefinition();
			return genericType == typeof(Tuple<>)
				|| genericType == typeof(Tuple<,>)
				|| genericType == typeof(Tuple<,,>)
				|| genericType == typeof(Tuple<,,,>)
				|| genericType == typeof(Tuple<,,,,>)
				|| genericType == typeof(Tuple<,,,,,>)
				|| genericType == typeof(Tuple<,,,,,,>)
				|| genericType == typeof(Tuple<,,,,,,,>);
		}

		public static Boolean IsTypeOfValueTuple (
			Type type
		) {
			if (!type.IsGenericType) {
				return false;
			}
			var genericType = type.GetGenericTypeDefinition();
			return genericType == typeof(ValueTuple<>)
				|| genericType == typeof(ValueTuple<,>)
				|| genericType == typeof(ValueTuple<,,>)
				|| genericType == typeof(ValueTuple<,,,>)
				|| genericType == typeof(ValueTuple<,,,,>)
				|| genericType == typeof(ValueTuple<,,,,,>)
				|| genericType == typeof(ValueTuple<,,,,,,>)
				|| genericType == typeof(ValueTuple<,,,,,,,>);
		}

		#endregion

		#region enumeration

		public static String MakeEnumerationToStringOfSnakeCase<TType> (
			TType value
		)
			where TType : struct, Enum {
			return ConvertHelper.ChangeStringFromCamelCaseToSnakeCase(Enum.GetName(value).AsNotNull());
		}

		public static TType ParseEnumerationFromStringOfSnakeCase<TType> (
			String text
		)
			where TType : struct, Enum {
			return Enum.GetValues<TType>().First((it) => ConvertHelper.MakeEnumerationToStringOfSnakeCase(it) == text);
		}

		#endregion

		#region boolean

		public static String MakeBooleanToString (
			Boolean value
		) {
			return value switch {
				false => "false",
				true  => "true",
			};
		}

		public static String MakeBooleanToStringOfConfirmationCharacter (
			Boolean value
		) {
			return value switch {
				false => "n",
				true  => "y",
			};
		}

		public static Floater MakeBooleanToFloaterOfOpacityVisibility (
			Boolean value
		) {
			return value switch {
				false => 0.0,
				true  => 1.0,
			};
		}

		public static Floater MakeBooleanToFloaterOfOpacityEnabled (
			Boolean value
		) {
			return value switch {
				false => 0.5,
				true  => 1.0,
			};
		}

		#endregion

		#region integer

		public static String MakeIntegerToString (
			Integer value,
			Boolean showPositiveSign
		) {
			return value switch {
				0   => "0",
				< 0 => $"{value}",
				> 0 => $"{(!showPositiveSign ? "" : "+")}{value}",
			};
		}

		#endregion

		#region floater

		public static String MakeFloaterToString (
			Floater value,
			Boolean showPositiveSign
		) {
			var valueDecimal = new Decimal(value);
			return value switch {
				0.0   => "0",
				< 0.0 => $"{valueDecimal}",
				> 0.0 => $"{(!showPositiveSign ? "" : "+")}{valueDecimal}",
				_     => throw new (),
			} + (!Decimal.IsInteger(valueDecimal) ? "" : ".0");
		}

		#endregion

		#region string

		public static List<String> SplitStringOfCamelCase (
			String source
		) {
			return new Regex(@"([A-Z][a-z]*)|([a-z]+)|([0-9]+)", RegexOptions.Compiled)
				.Matches(source)
				.Select((it) => it.Value)
				.ToList();
		}

		// ----------------

		public static String ChangeStringFromCamelCaseToSnakeCase (
			String source
		) {
			return String.Join('_', ConvertHelper.SplitStringOfCamelCase(source)).ToLower();
		}

		public static String ChangeStringFromCamelCaseThenInsertSpace (
			String source
		) {
			return String.Join(' ', ConvertHelper.SplitStringOfCamelCase(source));
		}

		#endregion

		#region string list

		public static String MakeStringListToStringWithLine (
			List<String> value
		) {
			return String.Join('\r', value) + (value.Count != 0 && value[^1].Length == 0 ? "\r" : "");
		}

		public static List<String> ParseStringListFromStringWithLine (
			String text
		) {
			var value = text.Split('\r').ToList();
			if (value.Count != 0 && value[^1].Length == 0) {
				value.RemoveAt(value.Count - 1);
			}
			return value;
		}

		#endregion

		#region color

		public static Integer MakeColorToInteger (
			Color value
		) {
			return (value.A.CastPrimitive<Integer>() << 24) | (value.R.CastPrimitive<Integer>() << 16) | (value.G.CastPrimitive<Integer>() << 8) | (value.B.CastPrimitive<Integer>() << 0);
		}

		public static Color ParseColorFromInteger (
			Integer integer
		) {
			return Color.FromArgb(
				((integer & 0xFF000000L) >> 24).CastPrimitive<Byte>(),
				((integer & 0x00FF0000L) >> 16).CastPrimitive<Byte>(),
				((integer & 0x0000FF00L) >> 8).CastPrimitive<Byte>(),
				((integer & 0x000000FFL) >> 0).CastPrimitive<Byte>()
			);
		}

		#endregion

		#region bitmap

		public static async Task<WriteableBitmap> ParseBitmapFromBinary (
			Byte[] data
		) {
			using var stream = data.AsBuffer().AsStream().AsRandomAccessStream();
			var decoder = await BitmapDecoder.CreateAsync(stream);
			var image = new WriteableBitmap(decoder.PixelWidth.CastPrimitive<Size>(), decoder.PixelHeight.CastPrimitive<Size>());
			stream.Seek(0);
			await image.SetSourceAsync(stream);
			return image;
		}

		#endregion

	}

}
