#pragma warning disable 0, 1998
// ReSharper disable

using Helper;

namespace Helper.Module.HomeSetting {

	public sealed partial class MainPage : Page {

		#region life

		public MainPage (
		) {
			this.InitializeComponent();
			this.Controller = new MainPageController() { View = this };
		}

		// ----------------

		private MainPageController Controller { get; }

		#endregion

	}

	public class MainPageController : CustomController {

		#region data

		public required MainPage View;

		#endregion

		#region module

		public List<ModuleItemController> uModuleList_ItemSource {
			get {
				return new List<ModuleItemController>(Enumerable.Range(1, ModuleInformationConstant.Value.Count - 1).Select(i => new ModuleItemController() { Host = this, Index = i }));
			}
		}

		#endregion

		#region customize setting

		public List<Tuple<ElementTheme, String>> uThemeComboBox_ItemsSource {
			get {
				return new List<Tuple<ElementTheme, String>> {
					new (ElementTheme.Default, "System"),
					new (ElementTheme.Light, "Light"),
					new (ElementTheme.Dark, "Dark"),
				};
			}
		}

		public Size uThemeComboBox_SelectedIndex {
			get {
				return (Size)Utility.ThemeHelper.RootTheme;
			}
		}

		public void uThemeComboBox_OnSelectionChanged (
			Object                    sender,
			SelectionChangedEventArgs e
		) {
			var selectedValue = (ElementTheme)((ComboBox)sender).SelectedValue;
			Utility.ThemeHelper.RootTheme = selectedValue;
		}

		#endregion

	}

	public class ModuleItemController : CustomController {

		#region data

		public required MainPageController Host;

		public required Size Index;

		#endregion

		#region view

		public String uIcon_Glyph {
			get {
				var model = ModuleInformationConstant.Value[this.Index];
				return $"{model.Icon}";
			}
		}

		// ----------------

		public String uTitle_Text {
			get {
				var model = ModuleInformationConstant.Value[this.Index];
				return $"{model.Title}";
			}
		}

		// ----------------

		public String uDescription_Text {
			get {
				var model = ModuleInformationConstant.Value[this.Index];
				return $"{model.Description}";
			}
		}

		// ----------------

		public async void uCreate_OnClick (
			Object          sender,
			RoutedEventArgs e
		) {
			var model = ModuleInformationConstant.Value[this.Index];
			MainWindow.Instance.PushTabItem(model.Type, true);
		}

		#endregion

	}

}
