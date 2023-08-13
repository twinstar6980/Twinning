#pragma warning disable 0,
// ReSharper disable

using Helper;

namespace Helper.CustomControl {

	public sealed class MExpandBar : Control {

		#region life

		public MExpandBar (
		) {
			this.DefaultStyleKey = typeof(MExpandBar);
		}

		#endregion

		#region property

		public static readonly DependencyProperty IconProperty = DependencyProperty.Register(
			nameof(MExpandBar.Icon),
			typeof(String),
			typeof(MExpandBar),
			new PropertyMetadata("")
		);

		public String Icon {
			get => this.GetValue(MExpandBar.IconProperty) as String ?? throw new NullReferenceException();
			set => this.SetValue(MExpandBar.IconProperty, value);
		}

		// ----------------

		public static readonly DependencyProperty TitleProperty = DependencyProperty.Register(
			nameof(MExpandBar.Title),
			typeof(String),
			typeof(MExpandBar),
			new PropertyMetadata("")
		);

		public String Title {
			get => this.GetValue(MExpandBar.TitleProperty) as String ?? throw new NullReferenceException();
			set => this.SetValue(MExpandBar.TitleProperty, value);
		}

		// ----------------

		public static readonly DependencyProperty DescriptionProperty = DependencyProperty.Register(
			nameof(MExpandBar.Description),
			typeof(String),
			typeof(MExpandBar),
			new PropertyMetadata(new MBox())
		);

		public String Description {
			get => this.GetValue(MExpandBar.DescriptionProperty) as String ?? throw new NullReferenceException();
			set => this.SetValue(MExpandBar.DescriptionProperty, value);
		}

		// ----------------

		public static readonly DependencyProperty ActionProperty = DependencyProperty.Register(
			nameof(MExpandBar.Action),
			typeof(UIElement),
			typeof(MExpandBar),
			new PropertyMetadata(new MBox())
		);

		public UIElement Action {
			get => this.GetValue(MExpandBar.ActionProperty) as UIElement ?? throw new NullReferenceException();
			set => this.SetValue(MExpandBar.ActionProperty, value);
		}

		#endregion

	}

}
