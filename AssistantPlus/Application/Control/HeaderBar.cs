#pragma warning disable 0,
// ReSharper disable

using AssistantPlus;
using AssistantPlus.Utility;

namespace AssistantPlus.Control {

	public sealed partial class HeaderBar : Microsoft.UI.Xaml.Controls.Control {

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
			new ("")
		);

		public String Icon {
			get => this.GetValue(HeaderBar.IconProperty).As<String>();
			set => this.SetValue(HeaderBar.IconProperty, value);
		}

		// ----------------

		public static readonly DependencyProperty TitleProperty = DependencyProperty.Register(
			nameof(HeaderBar.Title),
			typeof(String),
			typeof(HeaderBar),
			new ("")
		);

		public String Title {
			get => this.GetValue(HeaderBar.TitleProperty).As<String>();
			set => this.SetValue(HeaderBar.TitleProperty, value);
		}

		// ----------------

		public static readonly DependencyProperty ActionProperty = DependencyProperty.Register(
			nameof(HeaderBar.Action),
			typeof(UIElement),
			typeof(HeaderBar),
			new (new Box())
		);

		public UIElement Action {
			get => this.GetValue(HeaderBar.ActionProperty).As<UIElement>();
			set => this.SetValue(HeaderBar.ActionProperty, value);
		}

		#endregion

	}

}
