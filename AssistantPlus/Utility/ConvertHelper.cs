#pragma warning disable 0,
// ReSharper disable

using AssistantPlus;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Graphics.Imaging;
using Microsoft.UI.Xaml.Media.Imaging;

namespace AssistantPlus.Utility {

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

		public static String InsertSpaceBetweenStringWord (
			String value
		) {
			return Regex.Replace(value, @"([A-Z])", " $1", RegexOptions.Compiled).TrimStart();
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

		#region bitmap

		public static async Task<WriteableBitmap> ParseBitmapFromBinary (
			Byte[] data
		) {
			var stream = data.AsBuffer().AsStream().AsRandomAccessStream();
			var decoder = await BitmapDecoder.CreateAsync(stream);
			var image = new WriteableBitmap((Size)decoder.PixelWidth, (Size)decoder.PixelHeight);
			stream.Seek(0);
			await image.SetSourceAsync(stream);
			return image;
		}

		#endregion

	}

}
