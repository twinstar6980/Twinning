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

		public static Floater BooleanToFloaterOfOpacityVisibility (
			Boolean value
		) {
			return value switch {
				false => 0.0,
				true  => 1.0,
			};
		}

		public static Floater BooleanToFloaterOfOpacityEnabled (
			Boolean value
		) {
			return value switch {
				false => 0.5,
				true  => 1.0,
			};
		}

		public static String BooleanToConfirmationStringLower (
			Boolean value
		) {
			return value switch {
				false => "no",
				true  => "yes",
			};
		}

		#endregion

		#region string

		public static String StringInsertSpaceInWord (
			String value
		) {
			return Regex.Replace(value, @"([A-Z])", " $1", RegexOptions.Compiled).TrimStart();
		}

		#endregion

		#region string list

		public static String StringListToTextWithCr (
			List<String> value
		) {
			return String.Join('\r', value) + (value.Count != 0 && value[^1].Length == 0 ? "\r" : "");
		}

		public static List<String> StringListFromTextWithCr (
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

		public static async Task<WriteableBitmap> BitmapFromBinary (
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
