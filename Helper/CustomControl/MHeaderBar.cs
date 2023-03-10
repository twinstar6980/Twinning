#pragma warning disable 0,
// ReSharper disable

using Helper;

namespace Helper.CustomControl {

	public sealed class MHeaderBar : Control {

		#region life

		public MHeaderBar (
		) {
			this.DefaultStyleKey = typeof(MHeaderBar);
		}

		#endregion

		#region property

		public static readonly DependencyProperty IconProperty = DependencyProperty.Register(
			nameof(MHeaderBar.Icon), typeof(String), typeof(MHeaderBar),
			new PropertyMetadata("", (o, e) => {
			})
		);

		public String Icon {
			get => (String)this.GetValue(MHeaderBar.IconProperty);
			set => this.SetValue(MHeaderBar.IconProperty, value);
		}

		// ----------------

		public static readonly DependencyProperty TitleProperty = DependencyProperty.Register(
			nameof(MHeaderBar.Title), typeof(String), typeof(MHeaderBar),
			new PropertyMetadata("", (o, e) => {
			})
		);

		public String Title {
			get => (String)this.GetValue(MHeaderBar.TitleProperty);
			set => this.SetValue(MHeaderBar.TitleProperty, value);
		}

		// ----------------

		public static readonly DependencyProperty ActionProperty = DependencyProperty.Register(
			nameof(MHeaderBar.Action), typeof(UIElement), typeof(MHeaderBar),
			new PropertyMetadata(new MBox(), (o, e) => {
			})
		);

		public UIElement? Action {
			get => (UIElement)this.GetValue(MHeaderBar.ActionProperty);
			set => this.SetValue(MHeaderBar.ActionProperty, value);
		}

		#endregion

	}

}
