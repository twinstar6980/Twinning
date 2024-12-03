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
			VisualStateManager.GoToState(this, $"{(this.Type == null ? "Idle" : this.Type)}State", false);
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

		public Wrapper<ValueExpression> Value => this.View.Value;

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
					if (this.ValueOfSize != null) {
						this.uSizeExponent__Value = this.ValueOfSize.Exponent;
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
				if (this.Type == null) {
					return false;
				}
				return true;
			}
		}

		public async void uHistory_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<Button>();
			if (this.Type == null) {
				return;
			}
			var font = this.View.FindResource("ModdingWorker.MessageFont").As<FontFamily>();
			var flyout = new MenuFlyout() {
				Placement = FlyoutPlacementMode.TopEdgeAlignedLeft,
			};
			foreach (var item in this.History) {
				flyout.Items.Add(new MenuFlyoutItem() {
					Text = ValueExpressionHelper.MakeString(item),
					FontFamily = font,
				}.SelfAlso((it) => {
					it.Click += (_, _) => {
						this.Value.Value = item;
						this.Update();
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

		public Boolean uSubmit_IsEnabled {
			get {
				if (this.Type == null) {
					return false;
				}
				return true;
			}
		}

		public async void uSubmit_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<Button>();
			if (this.Type == null) {
				return;
			}
			this.View.OnValueSubmitted();
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

		public async void uBooleanValue_LostFocus (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<TextBox>();
			if (this.Type != SubmissionType.Boolean) {
				return;
			}
			if (senders.Text.Length == 0) {
				this.ValueOfBoolean = null;
			}
			else if (senders.Text == "n" || senders.Text == "y") {
				this.ValueOfBoolean = new () { Value = senders.Text == "y" };
			}
			this.NotifyPropertyChanged(
				nameof(this.uBooleanValue_Text),
				nameof(this.uBooleanValueNo_IsChecked),
				nameof(this.uBooleanValueYes_IsChecked)
			);
			return;
		}

		public String uBooleanValue_Text {
			get {
				if (this.Type != SubmissionType.Boolean) {
					return "";
				}
				return this.ValueOfBoolean == null ? "" : this.ValueOfBoolean.Value == false ? "n" : "y";
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

		public async void uBooleanValueNo_Click (
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
				this.ValueOfBoolean = new () { Value = false };
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
				if (this.Type != SubmissionType.Boolean) {
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
			if (this.Type != SubmissionType.Boolean) {
				return;
			}
			if (!senders.IsChecked.AsNotNull()) {
				this.ValueOfBoolean = null;
			}
			else {
				this.ValueOfBoolean = new () { Value = true };
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
			if (this.Type != SubmissionType.Integer) {
				return;
			}
			if (senders.Text.Length == 0) {
				this.ValueOfInteger = null;
			}
			else if (Floater.IsFinite(senders.Value) && Integer.MinValue <= senders.Value && senders.Value <= Integer.MaxValue) {
				this.ValueOfInteger = new () { Value = (Integer)senders.Value };
			}
			this.NotifyPropertyChanged(
				nameof(this.uIntegerValue_Value)
			);
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

		public async void uFloaterValue_LostFocus (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<NumberBox>();
			if (this.Type != SubmissionType.Floater) {
				return;
			}
			if (senders.Text.Length == 0) {
				this.ValueOfFloater = null;
			}
			else if (Floater.IsFinite(senders.Value)) {
				this.ValueOfFloater = new () { Value = senders.Value };
			}
			this.NotifyPropertyChanged(
				nameof(this.uFloaterValue_Value)
			);
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
			if (this.Type != SubmissionType.Size) {
				return;
			}
			if (senders.Text.Length == 0) {
				this.ValueOfSize = null;
			}
			else if (Floater.IsFinite(senders.Value) && senders.Value >= 0.0) {
				this.ValueOfSize = new () { Count = senders.Value, Exponent = this.uSizeExponent__Value };
			}
			this.NotifyPropertyChanged(
				nameof(this.uSizeCount_Value)
			);
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
			if (this.Type != SubmissionType.Size) {
				return;
			}
			this.uSizeExponent__Value = Integer.Parse(senders.Tag.As<String>());
			if (this.ValueOfSize != null) {
				this.ValueOfSize = new () { Count = this.ValueOfSize.Count, Exponent = this.uSizeExponent__Value };
			}
			this.NotifyPropertyChanged(
				nameof(this.uSizeExponent_Content)
			);
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
			if (this.Type != SubmissionType.String) {
				return;
			}
			if (senders.Text.Length == 0) {
				this.ValueOfString = null;
			}
			else {
				this.ValueOfString = new () { Value = senders.Text };
			}
			this.NotifyPropertyChanged(
				nameof(this.uStringValue_Text)
			);
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
			if (this.Type != SubmissionType.Path) {
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
			if (this.Type != SubmissionType.Path) {
				return;
			}
			if (args.DataView.Contains(StandardDataFormats.StorageItems)) {
				args.Handled = true;
				var item = await args.DataView.GetStorageItemsAsync();
				this.ValueOfPath = new () { Content = StorageHelper.GetLongPath(item[0].Path) };
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
			var senders = sender.As<TextBox>();
			if (this.Type != SubmissionType.Path) {
				return;
			}
			if (senders.Text.Length == 0) {
				this.ValueOfPath = null;
			}
			else {
				this.ValueOfPath = new () { Content = StorageHelper.Regularize(senders.Text) };
			}
			this.NotifyPropertyChanged(
				nameof(this.uPathContent_Text)
			);
			return;
		}

		public String uPathContent_Text {
			get {
				if (this.Type != SubmissionType.Path) {
					return "";
				}
				return this.ValueOfPath == null ? "" : this.ValueOfPath.Content;
			}
		}

		// ----------------

		public async void uPathCommandMenu_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<MenuFlyoutItem>();
			if (this.Type != SubmissionType.Path) {
				return;
			}
			this.ValueOfPath = new () { Content = $":{senders.Tag.As<String>()[0].ToString().ToLower()}" };
			this.NotifyPropertyChanged(
				nameof(this.uPathContent_Text)
			);
			return;
		}

		public async void uPathPickMenu_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<MenuFlyoutItem>();
			if (this.Type != SubmissionType.Path) {
				return;
			}
			var value = senders.Tag.As<String>() switch {
				"LoadFile"      => await StorageHelper.PickLoadFile(WindowHelper.Find(this.View), $"{nameof(ModdingWorker)}.Generic"),
				"LoadDirectory" => await StorageHelper.PickLoadDirectory(WindowHelper.Find(this.View), $"{nameof(ModdingWorker)}.Generic"),
				"SaveFile"      => await StorageHelper.PickSaveFile(WindowHelper.Find(this.View), $"{nameof(ModdingWorker)}.Generic", null, null),
				_               => throw new (),
			};
			if (value != null) {
				this.ValueOfPath = new () { Content = value };
				this.NotifyPropertyChanged(
					nameof(this.uPathContent_Text)
				);
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

		public async void uEnumerationItem_SelectionChanged (
			Object                    sender,
			SelectionChangedEventArgs args
		) {
			var senders = sender.As<ComboBox>();
			if (this.Type != SubmissionType.Enumeration) {
				return;
			}
			var item = senders.SelectedItem?.As<String>();
			if (item != null) {
				this.ValueOfEnumeration = new () { Item = item };
			}
			return;
		}

		// ----------------

		public async void uEnumerationItemNull_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<Button>();
			if (this.Type != SubmissionType.Enumeration) {
				return;
			}
			this.ValueOfEnumeration = null;
			this.NotifyPropertyChanged(
				nameof(this.uEnumerationItem_SelectedItem)
			);
			return;
		}

		#endregion

	}

}
