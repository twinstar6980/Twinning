#pragma warning disable 0,
// ReSharper disable

using Helper;
using Helper.Utility;

namespace Helper.CommonControl {

	public sealed class HeaderBar : Control {

		#region life

		public HeaderBar (
		) {
			this.DefaultStyleKey = typeof(HeaderBar);
		}

		#endregion

		#region property

		public static readonly DependencyProperty IconProperty = DependencyProperty.Register(
			nameof(HeaderBar.Icon),
			typeof(String),
			typeof(HeaderBar),
			new PropertyMetadata("")
		);

		public String Icon {
			get => this.GetValue(HeaderBar.IconProperty).AsClass<String>();
			set => this.SetValue(HeaderBar.IconProperty, value);
		}

		// ----------------

		public static readonly DependencyProperty TitleProperty = DependencyProperty.Register(
			nameof(HeaderBar.Title),
			typeof(String),
			typeof(HeaderBar),
			new PropertyMetadata("")
		);

		public String Title {
			get => this.GetValue(HeaderBar.TitleProperty).AsClass<String>();
			set => this.SetValue(HeaderBar.TitleProperty, value);
		}

		// ----------------

		public static readonly DependencyProperty ActionProperty = DependencyProperty.Register(
			nameof(HeaderBar.Action),
			typeof(UIElement),
			typeof(HeaderBar),
			new PropertyMetadata(new Box())
		);

		public UIElement Action {
			get => this.GetValue(HeaderBar.ActionProperty).AsClass<UIElement>();
			set => this.SetValue(HeaderBar.ActionProperty, value);
		}

		#endregion

	}

}
