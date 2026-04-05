#pragma warning disable 0,
// ReSharper disable

using Twinning.AssistantPlus;
using Twinning.AssistantPlus.Utility;
using Windows.ApplicationModel.DataTransfer;
using Windows.Globalization.NumberFormatting;
using Microsoft.UI.Xaml.Media;
using FluentIconGlyph = Twinning.AssistantPlus.Control.FluentIconGlyph;

namespace Twinning.AssistantPlus.View.CoreTaskWorker {

	public sealed partial class SubmissionBar : CustomControl {

		#region life

		private SubmissionBarController Controller { get; }

		// ----------------

		public SubmissionBar(
		) {
			this.InitializeComponent();
			this.Controller = new () { View = this };
			return;
		}

		// ----------------

		protected override async Task StampUpdate(
		) {
			await this.Controller.UpdateView();
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
			get => this.GetValue(SubmissionBar.TypeProperty)?.As<SubmissionType>();
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
			get => this.GetValue(SubmissionBar.OptionProperty).As<List<String>>();
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
			get => this.GetValue(SubmissionBar.HistoryProperty).As<List<ValueExpression>>();
			set => this.SetValue(SubmissionBar.HistoryProperty, value);
		}

		// ----------------

		public static readonly DependencyProperty ValueProperty = DependencyProperty.Register(
			nameof(SubmissionBar.Value),
			typeof(Wrapper<ValueExpression>),
			typeof(SubmissionBar),
			new (new Wrapper<ValueExpression>())
		);

		public Wrapper<ValueExpression> Value {
			get => this.GetValue(SubmissionBar.ValueProperty).As<Wrapper<ValueExpression>>();
			set => this.SetValue(SubmissionBar.ValueProperty, value);
		}

		// ----------------

		public event Action? ValueSubmitted;

		public void OnValueSubmitted(
		) => this.ValueSubmitted?.Invoke();

		#endregion

	}

	public partial class SubmissionBarController : CustomController {

		#region data

		public SubmissionBar View { get; init; } = default!;

		// ----------------

		public SubmissionType? Type => this.View.Type;

		public List<String> Option => this.View.Option;

		public List<ValueExpression> History => this.View.History;

		public Wrapper<ValueExpression> Value => this.View.Value;

		public Action OnValueSubmitted => this.View.OnValueSubmitted;

		#endregion

		#region life

		public async Task UpdateView(
		) {
			VisualStateManager.GoToState(this.View, $"{(this.Type == null ? "Idle" : this.ValueOfMacro?.Macro == null ? this.Type : "Macro")}State", false);
			this.NotifyPropertyChanged([
				nameof(this.uIcon_Glyph),
				nameof(this.uHistory_IsEnabled),
				nameof(this.uHistoryBadge_Visibility),
				nameof(this.uHistoryBadge_Value),
				nameof(this.uMacro_ToolTip),
				nameof(this.uMacro_IsEnabled),
				nameof(this.uMacro_IsChecked),
				nameof(this.uSubmit_IsEnabled),
			]);
			if (this.Type != null) {
				if (this.ValueOfMacro?.Macro == null) {
					switch (this.Type) {
						case SubmissionType.Pause: {
							this.NotifyPropertyChanged([
							]);
							break;
						}
						case SubmissionType.Boolean: {
							this.NotifyPropertyChanged([
								nameof(this.uBooleanValue_Text),
								nameof(this.uBooleanValueNo_IsChecked),
								nameof(this.uBooleanValueYes_IsChecked),
							]);
							break;
						}
						case SubmissionType.Integer: {
							this.NotifyPropertyChanged([
								nameof(this.uIntegerValue_Value),
							]);
							break;
						}
						case SubmissionType.Floater: {
							this.NotifyPropertyChanged([
								nameof(this.uFloaterValue_Value),
							]);
							break;
						}
						case SubmissionType.String: {
							this.NotifyPropertyChanged([
								nameof(this.uStringValue_Text),
							]);
							break;
						}
						case SubmissionType.Size: {
							if (this.ValueOfSize != null) {
								this.uSizeExponent_mValue = this.ValueOfSize.Exponent;
							}
							this.NotifyPropertyChanged([
								nameof(this.uSizeCount_Value),
								nameof(this.uSizeExponent_Content),
							]);
							break;
						}
						case SubmissionType.Path: {
							this.NotifyPropertyChanged([
								nameof(this.uPathContent_Text),
							]);
							break;
						}
						case SubmissionType.Enumeration: {
							this.NotifyPropertyChanged([
								nameof(this.uEnumerationItem_ItemsSource),
								nameof(this.uEnumerationItem_SelectedItem),
							]);
							break;
						}
						default: throw new UnreachableException();
					}
				}
				else {
					this.NotifyPropertyChanged([
						nameof(this.uMacroValue_Text),
						nameof(this.uMacroPreset_Flyout),
					]);
				}
			}
			return;
		}

		#endregion

		#region common

		public String uIcon_Glyph {
			get {
				if (this.Type == null) {
					return FluentIconGlyph.More;
				}
				return this.Type switch {
					SubmissionType.Pause       => FluentIconGlyph.Pause,
					SubmissionType.Boolean     => FluentIconGlyph.CheckboxComposite,
					SubmissionType.Integer     => FluentIconGlyph.Dial12,
					SubmissionType.Floater     => FluentIconGlyph.Dial12,
					SubmissionType.String      => FluentIconGlyph.FontSize,
					SubmissionType.Size        => FluentIconGlyph.Component,
					SubmissionType.Path        => FluentIconGlyph.Link,
					SubmissionType.Enumeration => FluentIconGlyph.BulletedList,
					_                          => throw new UnreachableException(),
				};
			}
		}

		// ----------------

		public Boolean uHistory_IsEnabled {
			get {
				if (this.Type == null) {
					return false;
				}
				return true;
			}
		}

		public async void uHistory_Click(
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<Button>();
			if (this.Type == null) {
				return;
			}
			var font = this.View.FindResource("CoreTaskWorker.MessageFont").As<FontFamily>();
			var flyout = new MenuFlyout() {
				Placement = FlyoutPlacementMode.TopEdgeAlignedLeft,
			};
			foreach (var item in this.History) {
				flyout.Items.Add(new MenuFlyoutItem() {
					Text = ValueExpressionHelper.MakeString(item),
					FontFamily = font,
				}.SelfAlso((it) => {
					ToolTipService.SetToolTip(it, it.Text);
					it.Click += async (_, _) => {
						this.Value.Value = item;
						await this.UpdateView();
						return;
					};
					if (this.Type == SubmissionType.Enumeration) {
						it.IsEnabled = this.Option.Contains(item.As<EnumerationExpression>().Item);
					}
				}));
			}
			flyout.ShowAt(senders);
			return;
		}

		public Boolean uHistoryBadge_Visibility {
			get {
				if (this.Type == null) {
					return false;
				}
				return true;
			}
		}

		public Size uHistoryBadge_Value {
			get {
				if (this.Type == null) {
					return 0;
				}
				return this.History.Count;
			}
		}

		// ----------------

		public String uMacro_ToolTip {
			get {
				if (this.Type == null || this.Type == SubmissionType.Pause) {
					return "Macro";
				}
				return this.ValueOfMacro?.Macro != null ? "Reset" : "Macro";
			}
		}

		public Boolean uMacro_IsEnabled {
			get {
				if (this.Type == null || this.Type == SubmissionType.Pause) {
					return false;
				}
				return true;
			}
		}

		public Boolean uMacro_IsChecked {
			get {
				if (this.Type == null || this.Type == SubmissionType.Pause) {
					return false;
				}
				return this.ValueOfMacro?.Macro != null;
			}
		}

		public async void uMacro_Click(
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<ToggleButton>();
			if (this.Type == null || this.Type == SubmissionType.Pause) {
				return;
			}
			if (this.ValueOfMacro?.Macro != null) {
				this.ValueOfMacro = null;
			}
			else {
				this.ValueOfMacro = new () { Macro = "" };
			}
			await this.UpdateView();
			return;
		}

		// ----------------

		public Boolean uSubmit_IsEnabled {
			get {
				if (this.Type == null) {
					return false;
				}
				return true;
			}
		}

		public async void uSubmit_Click(
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<Button>();
			if (this.Type == null) {
				return;
			}
			this.OnValueSubmitted();
			return;
		}

		#endregion

		#region pause

		public PauseExpression? ValueOfPause {
			get => this.Value.Value?.As<PauseExpression>();
			set => this.Value.Value = value;
		}

		#endregion

		#region boolean

		public BooleanExpression? ValueOfBoolean {
			get => this.Value.Value?.As<BooleanExpression>();
			set => this.Value.Value = value;
		}

		// ----------------

		public async void uBooleanValue_LostFocus(
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<TextBox>();
			if (this.Type != SubmissionType.Boolean) {
				return;
			}
			if (senders.Text.IsEmpty()) {
				this.ValueOfBoolean = null;
			}
			else if (senders.Text == "n" || senders.Text == "y") {
				this.ValueOfBoolean = new () { Macro = null, Value = senders.Text == "y" };
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
				if (this.Type != SubmissionType.Boolean) {
					return "";
				}
				return this.ValueOfBoolean == null ? "" : this.ValueOfBoolean.Value.SelfLet((it) => ConvertHelper.MakeBooleanToStringOfConfirmationCharacter(it));
			}
		}

		// ----------------

		public Boolean uBooleanValueNo_IsChecked {
			get {
				if (this.Type != SubmissionType.Boolean) {
					return false;
				}
				return this.ValueOfBoolean == null ? false : this.ValueOfBoolean.Value == false;
			}
		}

		public async void uBooleanValueNo_Click(
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<ToggleButton>();
			if (this.Type != SubmissionType.Boolean) {
				return;
			}
			if (!senders.IsChecked.AsNotNull()) {
				this.ValueOfBoolean = null;
			}
			else {
				this.ValueOfBoolean = new () { Macro = null, Value = false };
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
				if (this.Type != SubmissionType.Boolean) {
					return false;
				}
				return this.ValueOfBoolean == null ? false : this.ValueOfBoolean.Value == true;
			}
		}

		public async void uBooleanValueYes_Click(
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<ToggleButton>();
			if (this.Type != SubmissionType.Boolean) {
				return;
			}
			if (!senders.IsChecked.AsNotNull()) {
				this.ValueOfBoolean = null;
			}
			else {
				this.ValueOfBoolean = new () { Macro = null, Value = true };
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

		public async void uIntegerValue_LostFocus(
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<NumberBox>();
			if (this.Type != SubmissionType.Integer) {
				return;
			}
			if (senders.Text.IsEmpty()) {
				this.ValueOfInteger = null;
			}
			else if (Floater.IsFinite(senders.Value) && Integer.MinValue <= senders.Value && senders.Value <= Integer.MaxValue) {
				this.ValueOfInteger = new () { Macro = null, Value = senders.Value.CastPrimitive<Integer>() };
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
				if (this.Type != SubmissionType.Integer) {
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

		public async void uFloaterValue_LostFocus(
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<NumberBox>();
			if (this.Type != SubmissionType.Floater) {
				return;
			}
			if (senders.Text.IsEmpty()) {
				this.ValueOfFloater = null;
			}
			else if (Floater.IsFinite(senders.Value)) {
				this.ValueOfFloater = new () { Macro = null, Value = senders.Value };
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
				if (this.Type != SubmissionType.Floater) {
					return Floater.NaN;
				}
				return this.ValueOfFloater == null ? Floater.NaN : this.ValueOfFloater.Value;
			}
		}

		#endregion

		#region string

		public StringExpression? ValueOfString {
			get => this.Value.Value?.As<StringExpression>();
			set => this.Value.Value = value;
		}

		// ----------------

		public async void uStringValue_LostFocus(
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<TextBox>();
			if (this.Type != SubmissionType.String) {
				return;
			}
			if (senders.Text.IsEmpty()) {
				this.ValueOfString = null;
			}
			else {
				this.ValueOfString = new () { Macro = null, Value = senders.Text };
			}
			this.NotifyPropertyChanged([
				nameof(this.uStringValue_Text),
			]);
			return;
		}

		public String uStringValue_Text {
			get {
				if (this.Type != SubmissionType.String) {
					return "";
				}
				return this.ValueOfString == null ? "" : this.ValueOfString.Value;
			}
		}

		#endregion

		#region size

		public SizeExpression? ValueOfSize {
			get => this.Value.Value?.As<SizeExpression>();
			set => this.Value.Value = value;
		}

		// ----------------

		public async void uSizeCount_LostFocus(
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<NumberBox>();
			if (this.Type != SubmissionType.Size) {
				return;
			}
			if (senders.Text.IsEmpty()) {
				this.ValueOfSize = null;
			}
			else if (Floater.IsFinite(senders.Value) && senders.Value >= 0.0) {
				this.ValueOfSize = new () { Macro = null, Count = senders.Value, Exponent = this.uSizeExponent_mValue };
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
				if (this.Type != SubmissionType.Size) {
					return Floater.NaN;
				}
				return this.ValueOfSize == null ? Floater.NaN : this.ValueOfSize.Count;
			}
		}

		// ----------------

		private Integer uSizeExponent_mValue = 2;

		public String uSizeExponent_Content {
			get {
				return new[] { "B", "K", "M", "G" }[this.uSizeExponent_mValue];
			}
		}

		public async void uSizeExponentMenu_Click(
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<MenuFlyoutItem>();
			if (this.Type != SubmissionType.Size) {
				return;
			}
			this.uSizeExponent_mValue = Integer.Parse(senders.Tag.As<String>());
			if (this.ValueOfSize != null) {
				this.ValueOfSize = new () { Macro = null, Count = this.ValueOfSize.Count, Exponent = this.uSizeExponent_mValue };
			}
			this.NotifyPropertyChanged([
				nameof(this.uSizeExponent_Content),
			]);
			return;
		}

		#endregion

		#region path

		public PathExpression? ValueOfPath {
			get => this.Value.Value?.As<PathExpression>();
			set => this.Value.Value = value;
		}

		// ----------------

		public async void uPathContent_DragOver(
			Object        sender,
			DragEventArgs args
		) {
			var senders = sender.As<TextBox>();
			if (this.Type != SubmissionType.Path) {
				return;
			}
			if (args.DataView.Contains(StandardDataFormats.StorageItems)) {
				args.AcceptedOperation = DataPackageOperation.Link;
			}
			return;
		}

		public async void uPathContent_Drop(
			Object        sender,
			DragEventArgs args
		) {
			var senders = sender.As<TextBox>();
			if (this.Type != SubmissionType.Path) {
				return;
			}
			if (args.DataView.Contains(StandardDataFormats.StorageItems)) {
				args.Handled = true;
				var item = await args.DataView.SelfLet(ConvertHelper.DataViewGetStoragePath);
				this.ValueOfPath = new () { Macro = null, Content = item.First() };
				this.NotifyPropertyChanged([
					nameof(this.uPathContent_Text),
				]);
			}
			return;
		}

		public async void uPathContent_LostFocus(
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<TextBox>();
			if (this.Type != SubmissionType.Path) {
				return;
			}
			if (senders.Text.IsEmpty()) {
				this.ValueOfPath = null;
			}
			else {
				this.ValueOfPath = new () { Macro = null, Content = senders.Text.SelfLet((it) => new StoragePath(it)) };
			}
			this.NotifyPropertyChanged([
				nameof(this.uPathContent_Text),
			]);
			return;
		}

		public String uPathContent_Text {
			get {
				if (this.Type != SubmissionType.Path) {
					return "";
				}
				return this.ValueOfPath == null ? "" : this.ValueOfPath.Content.SelfLet((it) => it.Emit());
			}
		}

		// ----------------

		public async void uPathPickMenu_Click(
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<MenuFlyoutItem>();
			if (this.Type != SubmissionType.Path) {
				return;
			}
			var value = await StorageHelper.Pick(senders.Tag.As<String>(), App.Instance.MainWindow, $"@{ModuleHelper.Query(ModuleType.CoreTaskWorker).Identifier}.generic", null);
			if (value != null) {
				this.ValueOfPath = new () { Macro = null, Content = value };
				this.NotifyPropertyChanged([
					nameof(this.uPathContent_Text),
				]);
			}
			return;
		}

		#endregion

		#region enumeration

		public EnumerationExpression? ValueOfEnumeration {
			get => this.Value.Value?.As<EnumerationExpression>();
			set => this.Value.Value = value;
		}

		// ----------------

		public List<String> uEnumerationItem_ItemsSource {
			get {
				if (this.Type != SubmissionType.Enumeration) {
					return [];
				}
				return this.Option;
			}
		}

		public String? uEnumerationItem_SelectedItem {
			get {
				if (this.Type != SubmissionType.Enumeration) {
					return null;
				}
				return this.ValueOfEnumeration == null ? null : this.ValueOfEnumeration.Item;
			}
		}

		public async void uEnumerationItem_SelectionChanged(
			Object                    sender,
			SelectionChangedEventArgs args
		) {
			var senders = sender.As<ComboBox>();
			if (this.Type != SubmissionType.Enumeration) {
				return;
			}
			if (senders.SelectedIndex == -1) {
				return;
			}
			this.ValueOfEnumeration = new () { Macro = null, Item = senders.SelectedItem.As<String>() };
			return;
		}

		// ----------------

		public async void uEnumerationItemNull_Click(
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<Button>();
			if (this.Type != SubmissionType.Enumeration) {
				return;
			}
			this.ValueOfEnumeration = null;
			this.NotifyPropertyChanged([
				nameof(this.uEnumerationItem_SelectedItem),
			]);
			return;
		}

		#endregion

		#region command

		public ValueExpression? ValueOfMacro {
			get => this.Value.Value?.As<ValueExpression>();
			set => this.Value.Value = value;
		}

		// ----------------

		public async void uMacroValue_LostFocus(
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<TextBox>();
			if (this.Type == null || this.ValueOfMacro == null || this.ValueOfMacro.Macro == null) {
				return;
			}
			this.ValueOfMacro = new () { Macro = senders.Text };
			this.NotifyPropertyChanged([
				nameof(this.uMacroValue_Text),
			]);
			return;
		}

		public String uMacroValue_Text {
			get {
				if (this.Type == null || this.ValueOfMacro == null || this.ValueOfMacro.Macro == null) {
					return "";
				}
				return this.ValueOfMacro.Macro.AsNotNull();
			}
		}

		// ----------------

		public MenuFlyout uMacroPreset_Flyout {
			get {
				var menu = new MenuFlyout() {
					Placement = FlyoutPlacementMode.TopEdgeAlignedRight,
				};
				if (this.Type == null || this.ValueOfMacro == null || this.ValueOfMacro.Macro == null) {
					return menu;
				}
				var presetList = null as List<Tuple<String, String>>;
				presetList = this.Type switch {
					SubmissionType.Pause => [
					],
					SubmissionType.Boolean => [
					],
					SubmissionType.Integer => [
					],
					SubmissionType.Floater => [
					],
					SubmissionType.String => [
						new ("empty", "Empty"),
					],
					SubmissionType.Size => [
					],
					SubmissionType.Path => [
						new ("generate", "Generate"),
						new ("move", "Move"),
						new ("delete", "Delete"),
						new ("overwrite", "Overwrite"),
					],
					SubmissionType.Enumeration => [
					],
					_ => throw new UnreachableException(),
				};
				foreach (var preset in presetList) {
					menu.Items.Add(new MenuFlyoutItem() {
						Text = preset.Item2,
					}.SelfAlso((it) => {
						it.Click += async (_, _) => {
							this.ValueOfMacro = new () { Macro = preset.Item1 };
							this.NotifyPropertyChanged([
								nameof(this.uMacroValue_Text),
							]);
							return;
						};
					}));
				}
				return menu;
			}
		}

		#endregion

	}

}
