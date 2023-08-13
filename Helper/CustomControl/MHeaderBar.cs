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
			nameof(MHeaderBar.Icon),
			typeof(String),
			typeof(MHeaderBar),
			new PropertyMetadata("")
		);

		public String Icon {
			get => this.GetValue(MHeaderBar.IconProperty) as String ?? throw new NullReferenceException();
			set => this.SetValue(MHeaderBar.IconProperty, value);
		}

		// ----------------

		public static readonly DependencyProperty TitleProperty = DependencyProperty.Register(
			nameof(MHeaderBar.Title),
			typeof(String),
			typeof(MHeaderBar),
			new PropertyMetadata("")
		);

		public String Title {
			get => this.GetValue(MHeaderBar.TitleProperty) as String ?? throw new NullReferenceException();
			set => this.SetValue(MHeaderBar.TitleProperty, value);
		}

		// ----------------

		public static readonly DependencyProperty ActionProperty = DependencyProperty.Register(
			nameof(MHeaderBar.Action),
			typeof(UIElement),
			typeof(MHeaderBar),
			new PropertyMetadata(new MBox())
		);

		public UIElement Action {
			get => this.GetValue(MHeaderBar.ActionProperty) as UIElement ?? throw new NullReferenceException();
			set => this.SetValue(MHeaderBar.ActionProperty, value);
		}

		#endregion

	}

}
