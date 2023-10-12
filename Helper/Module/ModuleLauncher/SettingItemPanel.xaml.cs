#pragma warning disable 0,
// ReSharper disable

using Helper;
using Helper.Utility;
using Helper.CustomControl;

namespace Helper.Module.ModuleLauncher {

	public sealed partial class SettingItemPanel : UserControl {

		#region life

		public SettingItemPanel (
		) {
			this.InitializeComponent();
			this.Controller = new SettingItemPanelController() { View = this };
		}

		// ----------------

		private SettingItemPanelController Controller { get; }

		#endregion

		#region property

		public static readonly DependencyProperty IconProperty = DependencyProperty.Register(
			nameof(SettingItemPanel.Icon),
			typeof(String),
			typeof(SettingItemPanel),
			new PropertyMetadata("")
		);

		public String Icon {
			get => this.GetValue(SettingItemPanel.IconProperty) as String ?? throw new NullReferenceException();
			set => this.SetValue(SettingItemPanel.IconProperty, value);
		}

		// ----------------

		public static readonly DependencyProperty TitleProperty = DependencyProperty.Register(
			nameof(SettingItemPanel.Title),
			typeof(String),
			typeof(SettingItemPanel),
			new PropertyMetadata("")
		);

		public String Title {
			get => this.GetValue(SettingItemPanel.TitleProperty) as String ?? throw new NullReferenceException();
			set => this.SetValue(SettingItemPanel.TitleProperty, value);
		}

		// ----------------

		public static readonly DependencyProperty ActionProperty = DependencyProperty.Register(
			nameof(SettingItemPanel.Action),
			typeof(UIElement),
			typeof(SettingItemPanel),
			new PropertyMetadata(new MBox())
		);

		public UIElement Action {
			get => this.GetValue(SettingItemPanel.ActionProperty) as UIElement ?? throw new NullReferenceException();
			set => this.SetValue(SettingItemPanel.ActionProperty, value);
		}

		// ----------------

		public static readonly DependencyProperty StampProperty = DependencyProperty.Register(
			nameof(SettingItemPanel.Stamp),
			typeof(UniqueStamp),
			typeof(SettingItemPanel),
			new PropertyMetadata(UniqueStamp.Default, (o, e) => {
				(o as SettingItemPanel)!.Controller.Update();
			})
		);

		public UniqueStamp Stamp {
			get => this.GetValue(SettingItemPanel.StampProperty) as UniqueStamp ?? throw new Exception();
			set => this.SetValue(SettingItemPanel.StampProperty, value);
		}

		#endregion

	}

	public class SettingItemPanelController : CustomController {

		#region data

		public SettingItemPanel View { get; init; } = default!;

		// ----------------

		public String Icon => this.View.Icon;

		public String Title => this.View.Title;

		public UIElement Action => this.View.Action;

		#endregion

		#region update

		public async void Update (
		) {
			this.NotifyPropertyChanged(
				nameof(this.uIcon_Glyph),
				nameof(this.uTitle_Text),
				nameof(this.uAction_Content)
			);
			return;
		}

		#endregion

		#region view

		public String uIcon_Glyph {
			get {
				return this.Icon;
			}
		}

		public String uTitle_Text {
			get {
				return this.Title;
			}
		}

		public UIElement uAction_Content {
			get {
				return this.Action;
			}
		}

		#endregion

	}

}
