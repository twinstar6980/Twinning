#pragma warning disable 0,
// ReSharper disable

using Helper;
using Helper.Utility;
using Windows.ApplicationModel.DataTransfer;
using Windows.Globalization.NumberFormatting;

namespace Helper.Module.ModdingWorker {

	public sealed partial class InputBar : UserControl {

		#region life

		public InputBar (
		) {
			this.InitializeComponent();
			this.Controller = new InputBarController() { View = this };
		}

		// ----------------

		private InputBarController Controller { get; }

		// ----------------

		private void UpdateVisualState (
		) {
			VisualStateManager.GoToState(this, $"{(this.Type is null ? "Idle" : this.Type)}State", false);
			return;
		}

		#endregion

		#region property

		public static readonly DependencyProperty TypeProperty = DependencyProperty.Register(
			nameof(InputBar.Type),
			typeof(InputType),
			typeof(InputBar),
			new PropertyMetadata(null)
		);

		public InputType? Type {
			get => this.GetValue(InputBar.TypeProperty) as InputType?;
			set => this.SetValue(InputBar.TypeProperty, value);
		}

		// ----------------

		public static readonly DependencyProperty OptionProperty = DependencyProperty.Register(
			nameof(InputBar.Option),
			typeof(Object),
			typeof(InputBar),
			new PropertyMetadata(null)
		);

		public Object? Option {
			get => this.GetValue(InputBar.OptionProperty) as Object;
			set => this.SetValue(InputBar.OptionProperty, value);
		}

		// ----------------

		public static readonly DependencyProperty ValueProperty = DependencyProperty.Register(
			nameof(InputBar.Value),
			typeof(InputValue),
			typeof(InputBar),
			new PropertyMetadata(null)
		);

		public InputValue? Value {
			get => this.GetValue(InputBar.ValueProperty) as InputValue;
			set => this.SetValue(InputBar.ValueProperty, value);
		}

		// ----------------

		public delegate void ValueSubmittedEventHandler (Object? value);

		public event ValueSubmittedEventHandler? ValueSubmitted;

		public void DoValueSubmitted (
			Object? value
		) {
			this.ValueSubmitted?.Invoke(value);
			return;
		}

		// ----------------

		public static readonly DependencyProperty StampProperty = DependencyProperty.Register(
			nameof(InputBar.Stamp),
			typeof(UniqueStamp),
			typeof(InputBar),
			new PropertyMetadata(UniqueStamp.Default, (o, e) => {
				(o as InputBar)!.UpdateVisualState();
				(o as InputBar)!.Controller.Update();
			})
		);

		public UniqueStamp Stamp {
			get => this.GetValue(InputBar.StampProperty) as UniqueStamp ?? throw new Exception();
			set => this.SetValue(InputBar.StampProperty, value);
		}

		#endregion

	}

	public class InputBarController : CustomController {

		#region data

		public InputBar View { get; init; } = default!;

		// ----------------

		public InputType? Type => this.View.Type;

		public Object? Option => this.View.Option;

		public InputValue? Value => this.View.Value;

		#endregion

		#region update

		public async void Update (
		) {
			this.NotifyPropertyChanged(
				nameof(this.uAction_IsEnabled),
				nameof(this.uSubmit_IsEnabled)
			);
			switch (this.Type) {
				case null: {
					this.NotifyPropertyChanged(
					);
					break;
				}
				case InputType.Pause: {
					this.NotifyPropertyChanged(
						nameof(this.uBooleanNoValue_IsChecked),
						nameof(this.uBooleanYesValue_IsChecked)
					);
					break;
				}
				case InputType.Boolean: {
					this.NotifyPropertyChanged(
						nameof(this.uBooleanNoValue_IsChecked),
						nameof(this.uBooleanYesValue_IsChecked)
					);
					break;
				}
				case InputType.Integer: {
					this.NotifyPropertyChanged(
						nameof(this.uIntegerValue_Value)
					);
					break;
				}
				case InputType.Floater: {
					this.NotifyPropertyChanged(
						nameof(this.uFloaterValue_Value)
					);
					break;
				}
				case InputType.Size: {
					this.NotifyPropertyChanged(
						nameof(this.uSizeValue_Value),
						nameof(this.uSizeUnit_SelectedItem)
					);
					break;
				}
				case InputType.String: {
					this.NotifyPropertyChanged(
						nameof(this.uStringValue_Text)
					);
					break;
				}
				case InputType.Path: {
					this.NotifyPropertyChanged(
						nameof(this.uPathValue_Text)
					);
					break;
				}
				case InputType.Enumeration: {
					this.NotifyPropertyChanged(
						nameof(this.uEnumerationValue_ItemsSource),
						nameof(this.uEnumerationValue_SelectedIndex)
					);
					break;
				}
				default: throw new ArgumentOutOfRangeException();
			}
			return;
		}

		#endregion

		#region action

		public Boolean uSubmit_IsEnabled {
			get {
				if (this.Type is null || this.Value is null) { return false; }
				return true;
			}
		}

		public async void uSubmit_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not Button senders) { return; }
			if (this.Type is null || this.Value is null) { return; }
			this.View.DoValueSubmitted(this.Value);
			return;
		}

		public Boolean uAction_IsEnabled {
			get {
				if (this.Type is null) { return false; }
				return true;
			}
		}

		// ----------------

		public Boolean uBooleanNoValue_IsChecked {
			get {
				if (this.Type is not InputType.Boolean || this.Option is not null || this.Value is not { Data: null or Boolean }) { return false; }
				return this.Value.OfBoolean is null ? false : this.Value.OfBoolean == false;
			}
		}

		public async void uBooleanNoValue_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not ToggleButton senders) { return; }
			if (this.Type is not InputType.Boolean || this.Option is not null || this.Value is not { Data: null or Boolean }) { return; }
			this.Value.OfBoolean = !senders.IsChecked!.Value ? null : false;
			this.NotifyPropertyChanged(
				nameof(this.uBooleanNoValue_IsChecked),
				nameof(this.uBooleanYesValue_IsChecked)
			);
			return;
		}

		public Boolean uBooleanYesValue_IsChecked {
			get {
				if (this.Type is not InputType.Boolean || this.Option is not null || this.Value is not { Data: null or Boolean }) { return false; }
				return this.Value.OfBoolean is null ? false : this.Value.OfBoolean == true;
			}
		}

		public async void uBooleanYesValue_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not ToggleButton senders) { return; }
			if (this.Type is not InputType.Boolean || this.Option is not null || this.Value is not { Data: null or Boolean }) { return; }
			this.Value.OfBoolean = !senders.IsChecked!.Value ? null : true;
			this.NotifyPropertyChanged(
				nameof(this.uBooleanNoValue_IsChecked),
				nameof(this.uBooleanYesValue_IsChecked)
			);
			return;
		}

		// ----------------

		public DecimalFormatter uIntegerValue_NumberFormatter {
			get {
				return new DecimalFormatter() { IntegerDigits = 1, FractionDigits = 0 };
			}
		}

		public Floater uIntegerValue_Value {
			get {
				if (this.Type is not InputType.Integer || this.Option is not null || this.Value is not { Data: null or Integer }) { return Floater.NaN; }
				return this.Value.OfInteger is null ? Floater.NaN : (Floater)this.Value.OfInteger;
			}
		}

		public async void uIntegerValue_OnValueChanged (
			NumberBox                      sender,
			NumberBoxValueChangedEventArgs args
		) {
			if (sender is not NumberBox senders) { return; }
			if (this.Type is not InputType.Integer || this.Option is not null || this.Value is not { Data: null or Integer }) { return; }
			this.Value.OfInteger = Floater.IsNaN(args.NewValue) ? null : (Integer)args.NewValue;
			this.NotifyPropertyChanged(
				nameof(this.uIntegerValue_Value)
			);
			return;
		}

		// ----------------

		public DecimalFormatter uFloaterValue_NumberFormatter {
			get {
				return new DecimalFormatter() { IntegerDigits = 1, FractionDigits = 1 };
			}
		}

		public Floater uFloaterValue_Value {
			get {
				if (this.Type is not InputType.Floater || this.Option is not null || this.Value is not { Data: null or Floater }) { return Floater.NaN; }
				return this.Value.OfFloater is null ? Floater.NaN : (Floater)this.Value.OfFloater;
			}
		}

		public async void uFloaterValue_OnValueChanged (
			NumberBox                      sender,
			NumberBoxValueChangedEventArgs args
		) {
			if (sender is not NumberBox senders) { return; }
			if (this.Type is not InputType.Floater || this.Option is not null || this.Value is not { Data: null or Floater }) { return; }
			this.Value.OfFloater = Floater.IsNaN(args.NewValue) ? null : args.NewValue;
			this.NotifyPropertyChanged(
				nameof(this.uFloaterValue_Value)
			);
			return;
		}

		// ----------------

		public DecimalFormatter uSizeValue_NumberFormatter {
			get {
				return new DecimalFormatter() { IntegerDigits = 1, FractionDigits = 1 };
			}
		}

		public Floater uSizeValue_Value {
			get {
				if (this.Type is not InputType.Size || this.Option is not null || this.Value is not { Data: null or SizeExpression }) { return Floater.NaN; }
				return this.Value.OfSize is null ? Floater.NaN : this.Value.OfSize.Value;
			}
		}

		public async void uSizeValue_OnValueChanged (
			NumberBox                      sender,
			NumberBoxValueChangedEventArgs args
		) {
			if (sender is not NumberBox senders) { return; }
			if (this.Type is not InputType.Size || this.Option is not null || this.Value is not { Data: null or SizeExpression }) { return; }
			this.Value.OfSize = Floater.IsNaN(args.NewValue) ? null : new SizeExpression() { Value = args.NewValue, Unit = this.uSizeUnit__Value };
			this.NotifyPropertyChanged(
				nameof(this.uSizeValue_Value)
			);
			return;
		}

		public SizeUnit uSizeUnit__Value = SizeUnit.M;

		public List<SizeUnit> uSizeUnit_ItemsSource {
			get {
				return Enum.GetValues<SizeUnit>().ToList();
			}
		}

		public SizeUnit uSizeUnit_SelectedItem {
			get {
				if (this.Type is not InputType.Size || this.Option is not null || this.Value is not { Data: null or SizeExpression }) { return SizeUnit.B; }
				return this.uSizeUnit__Value;
			}
		}

		public async void uSizeUnit_OnSelectionChanged (
			Object                    sender,
			SelectionChangedEventArgs args
		) {
			if (sender is not ComboBox senders) { return; }
			if (this.Type is not InputType.Size || this.Option is not null || this.Value is not { Data: null or SizeExpression }) { return; }
			this.uSizeUnit__Value = senders.SelectedItem as SizeUnit? ?? throw new Exception();
			if (this.Value.OfSize is not null) {
				this.Value.OfSize.Unit = this.uSizeUnit__Value;
			}
			return;
		}

		// ----------------

		public String uStringValue_Text {
			get {
				if (this.Type is not InputType.String || this.Option is not null || this.Value is not { Data: null or String }) { return ""; }
				return this.Value.OfString is null ? "" : this.Value.OfString;
			}
		}

		public async void uStringValue_OnTextChanged (
			Object               sender,
			TextChangedEventArgs args
		) {
			if (sender is not TextBox senders) { return; }
			if (this.Type is not InputType.String || this.Option is not null || this.Value is not { Data: null or String }) { return; }
			this.Value.OfString = senders.Text.Length == 0 ? null : senders.Text;
			this.NotifyPropertyChanged(
				nameof(this.uStringValue_Text)
			);
			return;
		}

		// ----------------

		public String uPathValue_Text {
			get {
				if (this.Type is not InputType.Path || this.Option is not null || this.Value is not { Data: null or PathExpression }) { return ""; }
				return this.Value.OfPath is null ? "" : this.Value.OfPath.Value;
			}
		}

		public async void uPathValue_OnTextChanged (
			Object               sender,
			TextChangedEventArgs args
		) {
			if (sender is not TextBox senders) { return; }
			if (this.Type is not InputType.Path || this.Option is not null || this.Value is not { Data: null or PathExpression }) { return; }
			this.Value.OfPath = senders.Text.Length == 0 ? null : new PathExpression() { Value = senders.Text };
			this.NotifyPropertyChanged(
				nameof(this.uPathValue_Text)
			);
			return;
		}

		public async void uPathValue_OnDragOver (
			Object        sender,
			DragEventArgs args
		) {
			if (sender is not TextBox senders) { return; }
			if (this.Type is not InputType.Path || this.Option is not null || this.Value is not { Data: null or PathExpression }) { return; }
			if (args.DataView.Contains(StandardDataFormats.StorageItems)) {
				args.AcceptedOperation = Windows.ApplicationModel.DataTransfer.DataPackageOperation.Link;
			}
			return;
		}

		public async void uPathValue_OnDrop (
			Object        sender,
			DragEventArgs args
		) {
			if (sender is not TextBox senders) { return; }
			if (this.Type is not InputType.Path || this.Option is not null || this.Value is not { Data: null or PathExpression }) { return; }
			if (args.DataView.Contains(StandardDataFormats.StorageItems)) {
				var newValue = StorageHelper.Normalize((await args.DataView.GetStorageItemsAsync())[0].Path);
				this.Value.OfPath = new PathExpression() { Value = newValue };
				this.NotifyPropertyChanged(
					nameof(this.uPathValue_Text)
				);
			}
			return;
		}

		public async void uPathOptionOverride_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not MenuFlyoutItem senders) { return; }
			if (this.Type is not InputType.Path || this.Option is not null || this.Value is not { Data: null or PathExpression }) { return; }
			this.Value.OfPath = new PathExpression() { Value = ":o" };
			this.NotifyPropertyChanged(
				nameof(this.uPathValue_Text)
			);
			return;
		}

		public async void uPathOptionDelete_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not MenuFlyoutItem senders) { return; }
			if (this.Type is not InputType.Path || this.Option is not null || this.Value is not { Data: null or PathExpression }) { return; }
			this.Value.OfPath = new PathExpression() { Value = ":d" };
			this.NotifyPropertyChanged(
				nameof(this.uPathValue_Text)
			);
			return;
		}

		public async void uPathOptionTrash_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not MenuFlyoutItem senders) { return; }
			if (this.Type is not InputType.Path || this.Option is not null || this.Value is not { Data: null or PathExpression }) { return; }
			this.Value.OfPath = new PathExpression() { Value = ":t" };
			this.NotifyPropertyChanged(
				nameof(this.uPathValue_Text)
			);
			return;
		}

		public async void uPathPickFile_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not MenuFlyoutItem senders) { return; }
			if (this.Type is not InputType.Path || this.Option is not null || this.Value is not { Data: null or PathExpression }) { return; }
			var newValue = await StorageHelper.PickFile(WindowHelper.GetForElement(this.View));
			if (newValue is not null) {
				this.Value.OfPath = new PathExpression() { Value = newValue };
				this.NotifyPropertyChanged(
					nameof(this.uPathValue_Text)
				);
			}
			return;
		}

		public async void uPathPickDirectory_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not MenuFlyoutItem senders) { return; }
			if (this.Type is not InputType.Path || this.Option is not null || this.Value is not { Data: null or PathExpression }) { return; }
			var newValue = await StorageHelper.PickDirectory(WindowHelper.GetForElement(this.View));
			if (newValue is not null) {
				this.Value.OfPath = new PathExpression() { Value = newValue };
				this.NotifyPropertyChanged(
					nameof(this.uPathValue_Text)
				);
			}
			return;
		}

		// ----------------

		public List<String> uEnumerationValue_ItemsSource {
			get {
				if (this.Type is not InputType.Enumeration || this.Option is not List<String> || this.Value is not { Data: null or EnumerationExpression }) { return new List<String>(); }
				var result = new List<String>();
				result.Add("\0");
				result.AddRange((this.Option as List<String>)!);
				return result;
			}
		}

		public Size uEnumerationValue_SelectedIndex {
			get {
				if (this.Type is not InputType.Enumeration || this.Option is null || this.Value is not { Data: null or EnumerationExpression }) { return -1; }
				return this.Value.OfEnumeration is null ? -1 : this.Value.OfEnumeration.Value;
			}
		}

		public async void uEnumerationValue_OnSelectionChanged (
			Object                    sender,
			SelectionChangedEventArgs args
		) {
			if (sender is not ComboBox senders) { return; }
			if (this.Type is not InputType.Enumeration || this.Option is null || this.Value is not { Data: null or EnumerationExpression }) { return; }
			this.Value.OfEnumeration = senders.SelectedIndex == -1 || senders.SelectedIndex == 0 ? null : new EnumerationExpression() { Value = senders.SelectedIndex };
			this.NotifyPropertyChanged(
				nameof(this.uEnumerationValue_SelectedIndex)
			);
			return;
		}

		#endregion

	}

}
