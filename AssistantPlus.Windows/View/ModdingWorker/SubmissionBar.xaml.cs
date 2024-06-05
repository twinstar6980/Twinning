#pragma warning disable 0,
// ReSharper disable

using AssistantPlus;
using AssistantPlus.Utility;
using Windows.ApplicationModel.DataTransfer;
using Windows.Globalization.NumberFormatting;
using Microsoft.UI.Xaml.Media;

namespace AssistantPlus.View.ModdingWorker {

	public sealed partial class SubmissionBar : CustomControl {

		#region life

		public SubmissionBar (
		) {
			this.InitializeComponent();
			this.Controller = new () { View = this };
		}

		// ----------------

		private SubmissionBarController Controller { get; }

		// ----------------

		protected override void StampUpdate (
		) {
			VisualStateManager.GoToState(this, $"{(this.Type is null ? "Idle" : this.Type)}State", false);
			this.Controller.Update();
			return;
		}

		#endregion

		#region property

		public static readonly DependencyProperty TypeProperty = DependencyProperty.Register(
			nameof(SubmissionBar.Type),
			typeof(SubmissionType),
			typeof(SubmissionBar),
			new (null)
		);

		public SubmissionType? Type {
			get => this.GetValue(SubmissionBar.TypeProperty).AsStructOrNull<SubmissionType>();
			set => this.SetValue(SubmissionBar.TypeProperty, value);
		}

		// ----------------

		public static readonly DependencyProperty OptionProperty = DependencyProperty.Register(
			nameof(SubmissionBar.Option),
			typeof(List<String>),
			typeof(SubmissionBar),
			new (null)
		);

		public List<String> Option {
			get => this.GetValue(SubmissionBar.OptionProperty).AsClass<List<String>>();
			set => this.SetValue(SubmissionBar.OptionProperty, value);
		}

		// ----------------

		public static readonly DependencyProperty HistoryProperty = DependencyProperty.Register(
			nameof(SubmissionBar.History),
			typeof(List<ValueExpression>),
			typeof(SubmissionBar),
			new (new List<ValueExpression>())
		);

		public List<ValueExpression> History {
			get => this.GetValue(SubmissionBar.HistoryProperty).AsClass<List<ValueExpression>>();
			set => this.SetValue(SubmissionBar.HistoryProperty, value);
		}

		// ----------------

		public static readonly DependencyProperty ValueProperty = DependencyProperty.Register(
			nameof(SubmissionBar.Value),
			typeof(SubmissionValue),
			typeof(SubmissionBar),
			new (new SubmissionValue() { Data = null })
		);

		public SubmissionValue Value {
			get => this.GetValue(SubmissionBar.ValueProperty).AsClass<SubmissionValue>();
			set => this.SetValue(SubmissionBar.ValueProperty, value);
		}

		// ----------------

		public event Action? ValueSubmitted;

		public void OnValueSubmitted (
		) => this.ValueSubmitted?.Invoke();

		#endregion

	}

	public class SubmissionBarController : CustomController {

		#region data

		public SubmissionBar View { get; init; } = default!;

		// ----------------

		public SubmissionType? Type => this.View.Type;

		public List<String> Option => this.View.Option;

		public List<ValueExpression> History => this.View.History;

		public SubmissionValue Value => this.View.Value;

		#endregion

		#region update

		public async void Update (
		) {
			this.NotifyPropertyChanged(
				nameof(this.uHistory_IsEnabled),
				nameof(this.uHistoryBadge_Visibility),
				nameof(this.uHistoryBadge_Value),
				nameof(this.uSubmit_IsEnabled)
			);
			switch (this.Type) {
				case null: {
					this.NotifyPropertyChanged(
					);
					break;
				}
				case SubmissionType.Pause: {
					this.NotifyPropertyChanged(
					);
					break;
				}
				case SubmissionType.Boolean: {
					this.NotifyPropertyChanged(
						nameof(this.uBooleanValue_Text),
						nameof(this.uBooleanValueNo_IsChecked),
						nameof(this.uBooleanValueYes_IsChecked)
					);
					break;
				}
				case SubmissionType.Integer: {
					this.NotifyPropertyChanged(
						nameof(this.uIntegerValue_Value)
					);
					break;
				}
				case SubmissionType.Floater: {
					this.NotifyPropertyChanged(
						nameof(this.uFloaterValue_Value)
					);
					break;
				}
				case SubmissionType.Size: {
					if (this.Value.OfSize is not null) {
						this.uSizeExponent__Value = this.Value.OfSize.Exponent;
					}
					this.NotifyPropertyChanged(
						nameof(this.uSizeCount_Value),
						nameof(this.uSizeExponent_Content)
					);
					break;
				}
				case SubmissionType.String: {
					this.NotifyPropertyChanged(
						nameof(this.uStringValue_Text)
					);
					break;
				}
				case SubmissionType.Path: {
					this.NotifyPropertyChanged(
						nameof(this.uPathContent_Text)
					);
					break;
				}
				case SubmissionType.Enumeration: {
					this.NotifyPropertyChanged(
						nameof(this.uEnumerationItem_ItemsSource),
						nameof(this.uEnumerationItem_SelectedItem)
					);
					break;
				}
				default: throw new ();
			}
			return;
		}

		#endregion

		#region common

		public Boolean uHistory_IsEnabled {
			get {
				if (this.Type is null) {
					return false;
				}
				return true;
			}
		}

		public async void uHistory_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<Button>();
			if (this.Type is null) {
				return;
			}
			var font = this.View.FindResource("ModdingWorker.MessageFont").AsClass<FontFamily>();
			var flyout = new MenuFlyout() {
				Placement = FlyoutPlacementMode.TopEdgeAlignedLeft,
			};
			foreach (var item in this.History) {
				flyout.Items.Add(new MenuFlyoutItem() {
					Text = ValueExpressionHelper.MakeString(item),
					FontFamily = font,
				}.ApplySelf((it) => {
					it.Click += (_, _) => {
						this.Value.Data = item;
						this.Update();
					};
					if (this.Type is SubmissionType.Enumeration) {
						it.IsEnabled = this.Option.Contains(item.AsClass<EnumerationExpression>().Item);
					}
				}));
			}
			flyout.ShowAt(senders);
			return;
		}

		public Boolean uHistoryBadge_Visibility {
			get {
				if (this.Type is null) {
					return false;
				}
				return true;
			}
		}

		public Size uHistoryBadge_Value {
			get {
				if (this.Type is null) {
					return 0;
				}
				return this.History.Count;
			}
		}

		// ----------------

		public Boolean uSubmit_IsEnabled {
			get {
				if (this.Type is null) {
					return false;
				}
				return true;
			}
		}

		public async void uSubmit_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<Button>();
			if (this.Type is null) {
				return;
			}
			this.View.OnValueSubmitted();
			return;
		}

		#endregion

		#region boolean

		public String uBooleanValue_Text {
			get {
				if (this.Type is not SubmissionType.Boolean) {
					return "";
				}
				return this.Value.OfBoolean is null ? "" : this.Value.OfBoolean.Value == false ? "n" : "y";
			}
		}

		// ----------------

		public Boolean uBooleanValueNo_IsChecked {
			get {
				if (this.Type is not SubmissionType.Boolean) {
					return false;
				}
				return this.Value.OfBoolean is null ? false : this.Value.OfBoolean.Value == false;
			}
		}

		public async void uBooleanValueNo_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<ToggleButton>();
			if (this.Type is not SubmissionType.Boolean) {
				return;
			}
			if (!senders.IsChecked.AsNotNull()) {
				this.Value.OfBoolean = null;
			}
			else {
				this.Value.OfBoolean = new () { Value = false };
			}
			this.NotifyPropertyChanged(
				nameof(this.uBooleanValue_Text),
				nameof(this.uBooleanValueNo_IsChecked),
				nameof(this.uBooleanValueYes_IsChecked)
			);
			return;
		}

		// ----------------

		public Boolean uBooleanValueYes_IsChecked {
			get {
				if (this.Type is not SubmissionType.Boolean) {
					return false;
				}
				return this.Value.OfBoolean is null ? false : this.Value.OfBoolean.Value == true;
			}
		}

		public async void uBooleanValueYes_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<ToggleButton>();
			if (this.Type is not SubmissionType.Boolean) {
				return;
			}
			if (!senders.IsChecked.AsNotNull()) {
				this.Value.OfBoolean = null;
			}
			else {
				this.Value.OfBoolean = new () { Value = true };
			}
			this.NotifyPropertyChanged(
				nameof(this.uBooleanValue_Text),
				nameof(this.uBooleanValueNo_IsChecked),
				nameof(this.uBooleanValueYes_IsChecked)
			);
			return;
		}

		#endregion

		#region integer

		public DecimalFormatter uIntegerValue_NumberFormatter {
			get {
				return new () { IntegerDigits = 1, FractionDigits = 0 };
			}
		}

		public Floater uIntegerValue_Value {
			get {
				if (this.Type is not SubmissionType.Integer) {
					return Floater.NaN;
				}
				return this.Value.OfInteger is null ? Floater.NaN : this.Value.OfInteger.Value;
			}
		}

		public async void uIntegerValue_ValueChanged (
			NumberBox                      sender,
			NumberBoxValueChangedEventArgs args
		) {
			var senders = sender.AsClass<NumberBox>();
			if (this.Type is not SubmissionType.Integer) {
				return;
			}
			if (Floater.IsNaN(args.NewValue) || !(Integer.MinValue < args.NewValue && args.NewValue < Integer.MaxValue)) {
				this.Value.OfInteger = null;
			}
			else {
				this.Value.OfInteger = new () { Value = (Integer)args.NewValue };
			}
			this.NotifyPropertyChanged(
				nameof(this.uIntegerValue_Value)
			);
			return;
		}

		#endregion

		#region floater

		public DecimalFormatter uFloaterValue_NumberFormatter {
			get {
				return new () { IntegerDigits = 1, FractionDigits = 1 };
			}
		}

		public Floater uFloaterValue_Value {
			get {
				if (this.Type is not SubmissionType.Floater) {
					return Floater.NaN;
				}
				return this.Value.OfFloater is null ? Floater.NaN : this.Value.OfFloater.Value;
			}
		}

		public async void uFloaterValue_ValueChanged (
			NumberBox                      sender,
			NumberBoxValueChangedEventArgs args
		) {
			var senders = sender.AsClass<NumberBox>();
			if (this.Type is not SubmissionType.Floater) {
				return;
			}
			if (Floater.IsNaN(args.NewValue)) {
				this.Value.OfFloater = null;
			}
			else {
				this.Value.OfFloater = new () { Value = args.NewValue };
			}
			this.NotifyPropertyChanged(
				nameof(this.uFloaterValue_Value)
			);
			return;
		}

		#endregion

		#region size

		public DecimalFormatter uSizeCount_NumberFormatter {
			get {
				return new () { IntegerDigits = 1, FractionDigits = 1 };
			}
		}

		public Floater uSizeCount_Value {
			get {
				if (this.Type is not SubmissionType.Size) {
					return Floater.NaN;
				}
				return this.Value.OfSize is null ? Floater.NaN : this.Value.OfSize.Count;
			}
		}

		public async void uSizeCount_ValueChanged (
			NumberBox                      sender,
			NumberBoxValueChangedEventArgs args
		) {
			var senders = sender.AsClass<NumberBox>();
			if (this.Type is not SubmissionType.Size) {
				return;
			}
			if (Floater.IsNaN(args.NewValue)) {
				this.Value.OfSize = null;
			}
			else {
				this.Value.OfSize = new () { Count = args.NewValue, Exponent = this.uSizeExponent__Value };
			}
			this.NotifyPropertyChanged(
				nameof(this.uSizeCount_Value)
			);
			return;
		}

		// ----------------

		public Integer uSizeExponent__Value { get; set; } = 2;

		public String uSizeExponent_Content {
			get {
				return new[] { "B", "K", "M", "G" }[this.uSizeExponent__Value];
			}
		}

		public async void uSizeExponentMenu_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			if (this.Type is not SubmissionType.Size) {
				return;
			}
			this.uSizeExponent__Value = Integer.Parse(senders.Tag.AsClass<String>());
			if (this.Value.OfSize is not null) {
				this.Value.OfSize.Exponent = this.uSizeExponent__Value;
			}
			this.NotifyPropertyChanged(
				nameof(this.uSizeExponent_Content)
			);
			return;
		}

		#endregion

		#region string

		public async void uStringValue_LostFocus (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<TextBox>();
			if (this.Type is not SubmissionType.String) {
				return;
			}
			if (senders.Text.Length == 0) {
				this.Value.OfString = null;
			}
			else {
				this.Value.OfString = new () { Value = senders.Text };
			}
			this.NotifyPropertyChanged(
				nameof(this.uStringValue_Text)
			);
			return;
		}

		public String uStringValue_Text {
			get {
				if (this.Type is not SubmissionType.String) {
					return "";
				}
				return this.Value.OfString is null ? "" : this.Value.OfString.Value;
			}
		}

		#endregion

		#region path

		public async void uPathContent_DragOver (
			Object        sender,
			DragEventArgs args
		) {
			var senders = sender.AsClass<TextBox>();
			if (this.Type is not SubmissionType.Path) {
				return;
			}
			if (args.DataView.Contains(StandardDataFormats.StorageItems)) {
				args.AcceptedOperation = Windows.ApplicationModel.DataTransfer.DataPackageOperation.Link;
			}
			return;
		}

		public async void uPathContent_Drop (
			Object        sender,
			DragEventArgs args
		) {
			var senders = sender.AsClass<TextBox>();
			if (this.Type is not SubmissionType.Path) {
				return;
			}
			if (args.DataView.Contains(StandardDataFormats.StorageItems)) {
				args.Handled = true;
				var item = await args.DataView.GetStorageItemsAsync();
				this.Value.OfPath = new () { Content = StorageHelper.GetLongPath(item[0].Path) };
				this.NotifyPropertyChanged(
					nameof(this.uPathContent_Text)
				);
			}
			return;
		}

		public async void uPathContent_LostFocus (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<TextBox>();
			if (this.Type is not SubmissionType.Path) {
				return;
			}
			if (senders.Text.Length == 0) {
				this.Value.OfPath = null;
			}
			else {
				this.Value.OfPath = new () { Content = StorageHelper.Regularize(senders.Text) };
			}
			this.NotifyPropertyChanged(
				nameof(this.uPathContent_Text)
			);
			return;
		}

		public String uPathContent_Text {
			get {
				if (this.Type is not SubmissionType.Path) {
					return "";
				}
				return this.Value.OfPath is null ? "" : this.Value.OfPath.Content;
			}
		}

		// ----------------

		public async void uPathCommandMenu_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			if (this.Type is not SubmissionType.Path) {
				return;
			}
			this.Value.OfPath = new () { Content = $":{senders.Tag.AsClass<String>()[0].ToString().ToLower()}" };
			this.NotifyPropertyChanged(
				nameof(this.uPathContent_Text)
			);
			return;
		}

		public async void uPathPickMenu_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			if (this.Type is not SubmissionType.Path) {
				return;
			}
			var value = senders.Tag.AsClass<String>() switch {
				"OpenFile"      => await StorageHelper.PickOpenFile(WindowHelper.Find(this.View), $"{nameof(ModdingWorker)}.Generic"),
				"OpenDirectory" => await StorageHelper.PickOpenDirectory(WindowHelper.Find(this.View), $"{nameof(ModdingWorker)}.Generic"),
				"SaveFile"      => await StorageHelper.PickSaveFile(WindowHelper.Find(this.View), $"{nameof(ModdingWorker)}.Generic", null, null),
				_               => throw new (),
			};
			if (value is not null) {
				this.Value.OfPath = new () { Content = value };
				this.NotifyPropertyChanged(
					nameof(this.uPathContent_Text)
				);
			}
			return;
		}

		#endregion

		#region enumeration

		public List<String> uEnumerationItem_ItemsSource {
			get {
				if (this.Type is not SubmissionType.Enumeration) {
					return [];
				}
				var optionItem = this.Option[0..];
				return optionItem;
			}
		}

		public String? uEnumerationItem_SelectedItem {
			get {
				if (this.Type is not SubmissionType.Enumeration) {
					return null;
				}
				return this.Value.OfEnumeration is null ? null : this.Value.OfEnumeration.Item;
			}
		}

		public async void uEnumerationItem_SelectionChanged (
			Object                    sender,
			SelectionChangedEventArgs args
		) {
			var senders = sender.AsClass<ComboBox>();
			if (this.Type is not SubmissionType.Enumeration) {
				return;
			}
			var item = senders.SelectedItem.AsClassOrNull<String>();
			if (item != null) {
				this.Value.OfEnumeration = new () { Item = item };
			}
			return;
		}

		// ----------------

		public async void uEnumerationItemNull_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<Button>();
			if (this.Type is not SubmissionType.Enumeration) {
				return;
			}
			this.Value.OfEnumeration = null;
			this.NotifyPropertyChanged(
				nameof(this.uEnumerationItem_SelectedItem)
			);
			return;
		}

		#endregion

	}

}
