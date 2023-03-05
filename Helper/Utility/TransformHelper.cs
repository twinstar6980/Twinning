#pragma warning disable 0,
// ReSharper disable

using Helper;
using Microsoft.UI.Xaml.Media;

namespace Helper.Utility {

	public static class TransformHelper {

		#region convert

		public static Matrix ConvertFromStandard (
			List<Floating> source
		) {
			var result = new Matrix();
			switch (source.Count) {
				case 6: {
					result.M11 = source[0];
					result.M12 = source[1];
					result.M21 = source[2];
					result.M22 = source[3];
					result.OffsetX = source[4];
					result.OffsetY = source[5];
					break;
				}
				default: throw new ArgumentException($"invalid standard transform");
			}
			return result;
		}

		public static Matrix ConvertFromVariant (
			List<Floating> source
		) {
			var result = new Matrix();
			switch (source.Count) {
				case 2: {
					result.M11 = 1.0;
					result.M12 = 0.0;
					result.M21 = 0.0;
					result.M22 = 1.0;
					result.OffsetX = source[0];
					result.OffsetY = source[1];
					break;
				}
				case 3: {
					var cosValue = Math.Cos(source[0]);
					var sinValue = Math.Sin(source[0]);
					result.M11 = cosValue;
					result.M12 = sinValue;
					result.M21 = -sinValue;
					result.M22 = cosValue;
					result.OffsetX = source[1];
					result.OffsetY = source[2];
					break;
				}
				case 6: {
					result.M11 = source[0];
					result.M12 = source[1];
					result.M21 = source[2];
					result.M22 = source[3];
					result.OffsetX = source[4];
					result.OffsetY = source[5];
					break;
				}
				default: throw new ArgumentException($"invalid variant transform");
			}
			return result;
		}

		#endregion

	}

}
