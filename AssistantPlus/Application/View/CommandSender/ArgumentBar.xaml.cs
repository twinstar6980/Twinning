#pragma warning disable 0,
// ReSharper disable

using Twinning.AssistantPlus;
using Twinning.AssistantPlus.Utility;
using Microsoft.UI.Xaml.Media;
using Windows.ApplicationModel.DataTransfer;
using Windows.Globalization.NumberFormatting;

namespace Twinning.AssistantPlus.View.CommandSender {

	public sealed partial class ArgumentBar : CustomControl {

		#region life

		private ArgumentBarController Controller { get; }

		// ----------------

		public ArgumentBar (
		) {
			this.InitializeComponent();
			this.Controller = new () { View = this };
			return;
		}

		// ----------------

		protected override async Task StampUpdate (
		) {
			VisualStateManager.GoToState(this, $"{(this.Type == null ? "Null" : this.Option == null ? this.Type : "Enumeration")}State", false);
			await this.Controller.UpdateView();
			return;
		}

		#endregion

		#region property

		public static readonly DependencyProperty LabelProperty = DependencyProperty.Register(
			nameof(ArgumentBar.Label),
			typeof(String),
			typeof(ArgumentBar),
			new ("")
		);

		public String Label {
			get => this.GetValue(ArgumentBar.LabelProperty).As<String>();
			set => this.SetValue(ArgumentBar.LabelProperty, value);
		}

		// ----------------

		public static readonly DependencyProperty TypeProperty = DependencyProperty.Register(
			nameof(ArgumentBar.Type),
			typeof(ArgumentType),
			typeof(ArgumentBar),
			new (null)
		);

		public ArgumentType? Type {
			get => this.GetValue(ArgumentBar.TypeProperty)?.As<ArgumentType>();
			set => this.SetValue(ArgumentBar.TypeProperty, value);
		}

		// ----------------

		public static readonly DependencyProperty OptionProperty = DependencyProperty.Register(
			nameof(ArgumentBar.Option),
			typeof(List<ValueExpression>),
			typeof(ArgumentBar),
			new (null)
		);

		public List<ValueExpression>? Option {
			get => this.GetValue(ArgumentBar.OptionProperty)?.As<List<ValueExpression>>();
			set => this.SetValue(ArgumentBar.OptionProperty, value);
		}

		// ----------------

		public static readonly DependencyProperty ValueProperty = DependencyProperty.Register(
			nameof(ArgumentBar.Value),
			typeof(Wrapper<ValueExpression>),
			typeof(ArgumentBar),
			new (new Wrapper<ValueExpression>())
		);

		public Wrapper<ValueExpression> Value {
			get => this.GetValue(ArgumentBar.ValueProperty).As<Wrapper<ValueExpression>>();
			set => this.SetValue(ArgumentBar.ValueProperty, value);
		}

		// ----------------

		public static readonly DependencyProperty BatchProperty = DependencyProperty.Register(
			nameof(ArgumentBar.Batch),
			typeof(Boolean),
			typeof(ArgumentBar),
			new (false)
		);

		public Boolean Batch {
			get => this.GetValue(ArgumentBar.BatchProperty).As<Boolean>();
			set => this.SetValue(ArgumentBar.BatchProperty, value);
		}

		#endregion

	}

	public partial class ArgumentBarController : CustomController {

		#region data

		public ArgumentBar View { get; init; } = default!;

		// ----------------

		public String Label => this.View.Label;

		public ArgumentType? Type => this.View.Type;

		public List<ValueExpression>? Option => this.View.Option;

		public Wrapper<ValueExpression> Value => this.View.Value;

		public Boolean Batch => this.View.Batch;

		#endregion

		#region life

		public async Task UpdateView (
		) {
			this.NotifyPropertyChanged([
				nameof(this.uLabel_ToolTip),
				nameof(this.uLabelText_Foreground),
				nameof(this.uLabelText_Text),
			]);
			if (this.Type != null) {
				if (this.Option == null) {
					switch (this.Type) {
						case ArgumentType.Boolean: {
							this.NotifyPropertyChanged([
								nameof(this.uBooleanValue_Text),
								nameof(this.uBooleanValueNo_IsChecked),
								nameof(this.uBooleanValueYes_IsChecked),
							]);
							break;
						}
						case ArgumentType.Integer: {
							this.NotifyPropertyChanged([
								nameof(this.uIntegerValue_Value),
							]);
							break;
						}
						case ArgumentType.Floater: {
							this.NotifyPropertyChanged([
								nameof(this.uFloaterValue_Value),
							]);
							break;
						}
						case ArgumentType.Size: {
							if (this.ValueOfSize != null) {
								this.uSizeExponent__Value = this.ValueOfSize.Exponent;
							}
							this.NotifyPropertyChanged([
								nameof(this.uSizeCount_Value),
								nameof(this.uSizeExponent_Content),
							]);
							break;
						}
						case ArgumentType.String: {
							this.NotifyPropertyChanged([
								nameof(this.uStringValue_Text),
							]);
							break;
						}
						case ArgumentType.Path: {
							this.NotifyPropertyChanged([
								nameof(this.uPathContent_Text),
							]);
							break;
						}
						default: throw new UnreachableException();
					}
				}
				else {
					this.NotifyPropertyChanged([
						nameof(this.uEnumerationItem_ItemsSource),
						nameof(this.uEnumerationItem_SelectedIndex),
					]);
				}
			}
			return;
		}

		#endregion

		#region common

		public String uLabel_ToolTip {
			get {
				return this.Label;
			}
		}

		public Brush uLabelText_Foreground {
			get {
				return this.View.FindResource(!this.Batch ? "TextFillColorPrimaryBrush" : "AccentTextFillColorPrimaryBrush").As<Brush>();
			}
		}

		public String uLabelText_Text {
			get {
				return this.Label;
			}
		}

		#endregion

		#region boolean

		public BooleanExpression? ValueOfBoolean {
			get => this.Value.Value?.As<BooleanExpression>();
			set => this.Value.Value = value;
		}

		// ----------------

		public async void uBooleanValue_LostFocus (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<TextBox>();
			if (this.Type != ArgumentType.Boolean || this.Option != null) {
				return;
			}
			if (senders.Text.Length == 0) {
				this.ValueOfBoolean = null;
			}
			else if (senders.Text == "n" || senders.Text == "y") {
				this.ValueOfBoolean = new () { Value = senders.Text == "y" };
			}
			this.NotifyPropertyChanged([
				nameof(this.uBooleanValue_Text),
				nameof(this.uBooleanValueNo_IsChecked),
				nameof(this.uBooleanValueYes_IsChecked),
			]);
			return;
		}

		public String uBooleanValue_Text {
			get {
				if (this.Type != ArgumentType.Boolean || this.Option != null) {
					return "";
				}
				return this.ValueOfBoolean == null ? "" : this.ValueOfBoolean.Value == false ? "n" : "y";
			}
		}

		// ----------------

		public Boolean uBooleanValueNo_IsChecked {
			get {
				if (this.Type != ArgumentType.Boolean || this.Option != null) {
					return false;
				}
				return this.ValueOfBoolean == null ? false : this.ValueOfBoolean.Value == false;
			}
		}

		public async void uBooleanValueNo_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<ToggleButton>();
			if (this.Type != ArgumentType.Boolean || this.Option != null) {
				return;
			}
			if (!senders.IsChecked.AsNotNull()) {
				this.ValueOfBoolean = null;
			}
			else {
				this.ValueOfBoolean = new () { Value = false };
			}
			this.NotifyPropertyChanged([
				nameof(this.uBooleanValue_Text),
				nameof(this.uBooleanValueNo_IsChecked),
				nameof(this.uBooleanValueYes_IsChecked),
			]);
			return;
		}

		// ----------------

		public Boolean uBooleanValueYes_IsChecked {
			get {
				if (this.Type != ArgumentType.Boolean || this.Option != null) {
					return false;
				}
				return this.ValueOfBoolean == null ? false : this.ValueOfBoolean.Value == true;
			}
		}

		public async void uBooleanValueYes_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<ToggleButton>();
			if (this.Type != ArgumentType.Boolean || this.Option != null) {
				return;
			}
			if (!senders.IsChecked.AsNotNull()) {
				this.ValueOfBoolean = null;
			}
			else {
				this.ValueOfBoolean = new () { Value = true };
			}
			this.NotifyPropertyChanged([
				nameof(this.uBooleanValue_Text),
				nameof(this.uBooleanValueNo_IsChecked),
				nameof(this.uBooleanValueYes_IsChecked),
			]);
			return;
		}

		#endregion

		#region integer

		public IntegerExpression? ValueOfInteger {
			get => this.Value.Value?.As<IntegerExpression>();
			set => this.Value.Value = value;
		}

		// ----------------

		public async void uIntegerValue_LostFocus (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<NumberBox>();
			if (this.Type != ArgumentType.Integer || this.Option != null) {
				return;
			}
			if (senders.Text.Length == 0) {
				this.ValueOfInteger = null;
			}
			else if (Floater.IsFinite(senders.Value) && Integer.MinValue <= senders.Value && senders.Value <= Integer.MaxValue) {
				this.ValueOfInteger = new () { Value = senders.Value.CastPrimitive<Integer>() };
			}
			this.NotifyPropertyChanged([
				nameof(this.uIntegerValue_Value),
			]);
			return;
		}

		public DecimalFormatter uIntegerValue_NumberFormatter {
			get {
				return new () { IntegerDigits = 1, FractionDigits = 0 };
			}
		}

		public Floater uIntegerValue_Value {
			get {
				if (this.Type != ArgumentType.Integer || this.Option != null) {
					return Floater.NaN;
				}
				return this.ValueOfInteger == null ? Floater.NaN : this.ValueOfInteger.Value;
			}
		}

		#endregion

		#region floater

		public FloaterExpression? ValueOfFloater {
			get => this.Value.Value?.As<FloaterExpression>();
			set => this.Value.Value = value;
		}

		// ----------------

		public async void uFloaterValue_LostFocus (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<NumberBox>();
			if (this.Type != ArgumentType.Floater || this.Option != null) {
				return;
			}
			if (senders.Text.Length == 0) {
				this.ValueOfFloater = null;
			}
			else if (Floater.IsFinite(senders.Value)) {
				this.ValueOfFloater = new () { Value = senders.Value };
			}
			this.NotifyPropertyChanged([
				nameof(this.uFloaterValue_Value),
			]);
			return;
		}

		public DecimalFormatter uFloaterValue_NumberFormatter {
			get {
				return new () { IntegerDigits = 1, FractionDigits = 1 };
			}
		}

		public Floater uFloaterValue_Value {
			get {
				if (this.Type != ArgumentType.Floater || this.Option != null) {
					return Floater.NaN;
				}
				return this.ValueOfFloater == null ? Floater.NaN : this.ValueOfFloater.Value;
			}
		}

		#endregion

		#region size

		public SizeExpression? ValueOfSize {
			get => this.Value.Value?.As<SizeExpression>();
			set => this.Value.Value = value;
		}

		// ----------------

		public async void uSizeCount_LostFocus (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<NumberBox>();
			if (this.Type != ArgumentType.Size || this.Option != null) {
				return;
			}
			if (senders.Text.Length == 0) {
				this.ValueOfSize = null;
			}
			else if (Floater.IsFinite(senders.Value) && senders.Value >= 0.0) {
				this.ValueOfSize = new () { Count = senders.Value, Exponent = this.uSizeExponent__Value };
			}
			this.NotifyPropertyChanged([
				nameof(this.uSizeCount_Value),
			]);
			return;
		}

		public DecimalFormatter uSizeCount_NumberFormatter {
			get {
				return new () { IntegerDigits = 1, FractionDigits = 1 };
			}
		}

		public Floater uSizeCount_Value {
			get {
				if (this.Type != ArgumentType.Size || this.Option != null) {
					return Floater.NaN;
				}
				return this.ValueOfSize == null ? Floater.NaN : this.ValueOfSize.Count;
			}
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
			var senders = sender.As<MenuFlyoutItem>();
			if (this.Type != ArgumentType.Size || this.Option != null) {
				return;
			}
			this.uSizeExponent__Value = Integer.Parse(senders.Tag.As<String>());
			if (this.ValueOfSize != null) {
				this.ValueOfSize = new () { Count = this.ValueOfSize.Count, Exponent = this.uSizeExponent__Value };
			}
			this.NotifyPropertyChanged([
				nameof(this.uSizeExponent_Content),
			]);
			return;
		}

		#endregion

		#region string

		public StringExpression? ValueOfString {
			get => this.Value.Value?.As<StringExpression>();
			set => this.Value.Value = value;
		}

		// ----------------

		public async void uStringValue_LostFocus (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<TextBox>();
			if (this.Type != ArgumentType.String || this.Option != null) {
				return;
			}
			if (senders.Text.Length == 0) {
				this.ValueOfString = null;
			}
			else {
				this.ValueOfString = new () { Value = senders.Text };
			}
			this.NotifyPropertyChanged([
				nameof(this.uStringValue_Text),
			]);
			return;
		}

		public String uStringValue_Text {
			get {
				if (this.Type != ArgumentType.String || this.Option != null) {
					return "";
				}
				return this.ValueOfString == null ? "" : this.ValueOfString.Value;
			}
		}

		#endregion

		#region path

		public PathExpression? ValueOfPath {
			get => this.Value.Value?.As<PathExpression>();
			set => this.Value.Value = value;
		}

		// ----------------

		public async void uPathContent_DragOver (
			Object        sender,
			DragEventArgs args
		) {
			var senders = sender.As<TextBox>();
			if (this.Type != ArgumentType.Path || this.Option != null) {
				return;
			}
			if (args.DataView.Contains(StandardDataFormats.StorageItems)) {
				args.AcceptedOperation = DataPackageOperation.Link;
			}
			return;
		}

		public async void uPathContent_Drop (
			Object        sender,
			DragEventArgs args
		) {
			var senders = sender.As<TextBox>();
			if (this.Type != ArgumentType.Path || this.Option != null) {
				return;
			}
			if (args.DataView.Contains(StandardDataFormats.StorageItems)) {
				args.Handled = true;
				var item = await args.DataView.GetStorageItemsAsync();
				this.ValueOfPath = new () { Content = StorageHelper.GetLongPath(item[0].Path) };
				this.NotifyPropertyChanged([
					nameof(this.uPathContent_Text),
				]);
			}
			return;
		}

		public async void uPathContent_LostFocus (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<TextBox>();
			if (this.Type != ArgumentType.Path || this.Option != null) {
				return;
			}
			if (senders.Text.Length == 0) {
				this.ValueOfPath = null;
			}
			else {
				this.ValueOfPath = new () { Content = StorageHelper.Regularize(senders.Text) };
			}
			this.NotifyPropertyChanged([
				nameof(this.uPathContent_Text),
			]);
			return;
		}

		public String uPathContent_Text {
			get {
				if (this.Type != ArgumentType.Path || this.Option != null) {
					return "";
				}
				return this.ValueOfPath == null ? "" : this.ValueOfPath.Content;
			}
		}

		// ----------------

		public async void uPathPickMenu_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<MenuFlyoutItem>();
			if (this.Type != ArgumentType.Path || this.Option != null) {
				return;
			}
			var value = await StorageHelper.Pick(senders.Tag.As<String>(), App.MainWindow, $"@{nameof(CommandSender)}.Generic", null);
			if (value != null) {
				this.ValueOfPath = new () { Content = value };
				this.NotifyPropertyChanged([
					nameof(this.uPathContent_Text),
				]);
			}
			return;
		}

		#endregion

		#region enumeration

		public List<String> uEnumerationItem_ItemsSource {
			get {
				if (this.Type == null || this.Option == null) {
					return [];
				}
				return this.Option.Select((value) => (ValueExpressionHelper.MakeString(value))).ToList();
			}
		}

		public Size uEnumerationItem_SelectedIndex {
			get {
				if (this.Type == null || this.Option == null) {
					return -1;
				}
				return this.Value.Value == null ? -1 : this.Option.IndexOf(this.Value.Value);
			}
		}

		public async void uEnumerationItem_SelectionChanged (
			Object                    sender,
			SelectionChangedEventArgs args
		) {
			var senders = sender.As<ComboBox>();
			if (this.Type == null || this.Option == null) {
				return;
			}
			if (senders.SelectedIndex == -1) {
				return;
			}
			this.Value.Value = this.Option[senders.SelectedIndex];
			return;
		}

		// ----------------

		public async void uEnumerationItemNull_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<Button>();
			if (this.Type == null || this.Option == null) {
				return;
			}
			this.Value.Value = null;
			this.NotifyPropertyChanged([
				nameof(this.uEnumerationItem_SelectedIndex),
			]);
			return;
		}

		#endregion

	}

}
