#pragma warning disable 0,
// ReSharper disable

using AssistantPlus;
using AssistantPlus.Utility;

namespace AssistantPlus.View.PackageBuilder {

	public sealed partial class PackageSettingPanel : CustomControl {

		#region life

		public PackageSettingPanel (
		) {
			this.InitializeComponent();
			this.Controller = new () { View = this };
		}

		// ----------------

		private PackageSettingPanelController Controller { get; }

		// ----------------

		protected override void StampUpdate (
		) {
			this.Controller.Update();
			return;
		}

		#endregion

		#region property

		public static readonly DependencyProperty ValueProperty = DependencyProperty.Register(
			nameof(PackageSettingPanel.Value),
			typeof(PackageSetting),
			typeof(PackageSettingPanel),
			new (new PackageSetting() {
				Name = "",
				Part = [],
				Version = new () {
					Number = 0,
					ExtendedTextureInformationForPvz2Cn = 0,
				},
				Compression = new () {
					General = false,
					Texture = false,
					Filter = [],
				},
				Manifest = new () {
					Type = PackageManifestType.Internal,
					Suffix = "",
				},
				Category = new () {
					Resolution = [],
					Locale = [],
				},
				Variable = [],
			})
		);

		public PackageSetting Value {
			get => this.GetValue(PackageSettingPanel.ValueProperty).As<PackageSetting>();
			set => this.SetValue(PackageSettingPanel.ValueProperty, value);
		}

		#endregion

	}

	public class PackageSettingPanelController : CustomController {

		#region data

		public PackageSettingPanel View { get; init; } = default!;

		// ----------------

		public PackageSetting Value => this.View.Value;

		#endregion

		#region update

		public async void Update (
		) {
			this.NotifyPropertyChanged(
				nameof(this.uVersionNumber_SelectedItem),
				nameof(this.uVersionExtendedTextureInformationForPvz2Cn_SelectedItem),
				nameof(this.uCompressionGeneral_IsChecked),
				nameof(this.uCompressionTexture_IsChecked),
				nameof(this.uCompressionFilter_Text),
				nameof(this.uManifestType_SelectedIndex),
				nameof(this.uManifestSuffix_Text),
				nameof(this.uCategoryResolution_Text),
				nameof(this.uCategoryLocale_Text)
			);
			return;
		}

		#endregion

		#region version

		public List<String> uVersionNumber_ItemsSource {
			get {
				return ["1", "3", "4"];
			}
		}

		public String uVersionNumber_SelectedItem {
			get {
				return this.Value.Version.Number.ToString();
			}
		}

		public async void uVersionNumber_SelectionChanged (
			Object                    sender,
			SelectionChangedEventArgs args
		) {
			var senders = sender.As<ComboBox>();
			this.Value.Version.Number = Integer.Parse(senders.SelectedItem.As<String>());
			return;
		}

		// ----------------

		public List<String> uVersionExtendedTextureInformationForPvz2Cn_ItemsSource {
			get {
				return ["0", "1", "2", "3"];
			}
		}

		public String uVersionExtendedTextureInformationForPvz2Cn_SelectedItem {
			get {
				return this.Value.Version.ExtendedTextureInformationForPvz2Cn.ToString();
			}
		}

		public async void uVersionExtendedTextureInformationForPvz2Cn_SelectionChanged (
			Object                    sender,
			SelectionChangedEventArgs args
		) {
			var senders = sender.As<ComboBox>();
			this.Value.Version.ExtendedTextureInformationForPvz2Cn = Integer.Parse(senders.SelectedItem.As<String>());
			return;
		}

		#endregion

		#region compression

		public Boolean uCompressionGeneral_IsChecked {
			get {
				return this.Value.Compression.General;
			}
		}

		public async void uCompressionGeneral_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<ToggleButton>();
			this.Value.Compression.General = senders.IsChecked.AsNotNull();
			return;
		}

		// ----------------

		public Boolean uCompressionTexture_IsChecked {
			get {
				return this.Value.Compression.Texture;
			}
		}

		public async void uCompressionTexture_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<ToggleButton>();
			this.Value.Compression.Texture = senders.IsChecked.AsNotNull();
			return;
		}

		// ----------------

		public async void uCompressionFilter_LostFocus (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<TextBox>();
			this.Value.Compression.Filter = ConvertHelper.ParseStringListFromStringWithLine(senders.Text);
			this.NotifyPropertyChanged(
				nameof(this.uCompressionFilter_Text)
			);
			return;
		}

		public String uCompressionFilter_Text {
			get {
				return ConvertHelper.MakeStringListToStringWithLine(this.Value.Compression.Filter);
			}
		}

		#endregion

		#region manifest

		public List<String> uManifestType_ItemsSource {
			get {
				return [
					"Internal",
					"External - RTON - Array Path",
					"External - RTON - String Path",
					"External - NEWTON",
				];
			}
		}

		public Size uManifestType_SelectedIndex {
			get {
				return (Size)this.Value.Manifest.Type;
			}
		}

		public async void uManifestType_SelectionChanged (
			Object                    sender,
			SelectionChangedEventArgs args
		) {
			var senders = sender.As<ComboBox>();
			this.Value.Manifest.Type = (PackageManifestType)senders.SelectedIndex;
			return;
		}

		// ----------------

		public async void uManifestSuffix_LostFocus (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<TextBox>();
			if (senders.Text.Length == 0 || StorageHelper.CheckName(senders.Text)) {
				this.Value.Manifest.Suffix = senders.Text;
			}
			this.NotifyPropertyChanged(
				nameof(this.uManifestSuffix_Text)
			);
			return;
		}

		public String uManifestSuffix_Text {
			get {
				return this.Value.Manifest.Suffix;
			}
		}

		#endregion

		#region category

		public async void uCategoryResolution_LostFocus (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<TextBox>();
			this.Value.Category.Resolution = ConvertHelper.ParseStringListFromStringWithLine(senders.Text).Select((value) => (ProjectSettingHelper.ParseResolutionString(value, null))).Where((value) => (value != null)).Select(GF.AsNotNull).ToList();
			this.NotifyPropertyChanged(
				nameof(this.uCategoryResolution_Text)
			);
			return;
		}

		public String uCategoryResolution_Text {
			get {
				return ConvertHelper.MakeStringListToStringWithLine(this.Value.Category.Resolution.Select((value) => (value.ToString())).ToList());
			}
		}

		// ----------------

		public async void uCategoryLocale_LostFocus (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<TextBox>();
			this.Value.Category.Locale = ConvertHelper.ParseStringListFromStringWithLine(senders.Text).Select((value) => (ProjectSettingHelper.ParseLocaleString(value, null))).Where((value) => (value != null)).Select(GF.AsNotNull).ToList();
			this.NotifyPropertyChanged(
				nameof(this.uCategoryLocale_Text)
			);
			return;
		}

		public String uCategoryLocale_Text {
			get {
				return ConvertHelper.MakeStringListToStringWithLine(this.Value.Category.Locale);
			}
		}

		#endregion

	}

}
