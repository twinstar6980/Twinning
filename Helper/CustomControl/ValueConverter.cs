#pragma warning disable 0,
// ReSharper disable

using Helper;
using Microsoft.UI.Xaml.Data;

namespace Helper.CustomControl {

	#region icon

	public class FontIconConverter : IValueConverter {

		public Object Convert (
			Object value,
			Type   targetType,
			Object parameter,
			String language
		) {
			if (value is not String values) {
				return new FontIcon();
			}
			return new FontIcon() {
				Glyph = values,
			};
		}

		public Object ConvertBack (
			Object value,
			Type   targetType,
			Object parameter,
			String language
		) {
			throw new NotImplementedException();
		}

	}

	#endregion

}
