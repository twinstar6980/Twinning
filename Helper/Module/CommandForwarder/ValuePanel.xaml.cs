#pragma warning disable 0,
// ReSharper disable

using Helper;
using Helper.Utility;
using Windows.ApplicationModel.DataTransfer;
using Windows.Globalization.NumberFormatting;

namespace Helper.Module.CommandForwarder {

	public sealed partial class ValuePanel : UserControl {

		#region life

		public ValuePanel (
		) {
			this.InitializeComponent();
			this.Controller = new ValuePanelController() { View = this };
		}

		// ----------------

		private ValuePanelController Controller { get; }

		// ----------------

		private void UpdateVisualState (
		) {
			VisualStateManager.GoToState(this, $"{(this.Option is not null ? "Enumeration" : this.Type is null ? "Null" : this.Type)}State", false);
			return;
		}

		#endregion

		#region property

		public static readonly DependencyProperty TypeProperty = DependencyProperty.Register(
			nameof(ValuePanel.Type),
			typeof(CommandConfigurationModel.ArgumentType),
			typeof(ValuePanel),
			new PropertyMetadata(null)
		);

		public CommandConfigurationModel.ArgumentType? Type {
			get => this.GetValue(ValuePanel.TypeProperty) as CommandConfigurationModel.ArgumentType?;
			set => this.SetValue(ValuePanel.TypeProperty, value);
		}

		// ----------------

		public static readonly DependencyProperty OptionProperty = DependencyProperty.Register(
			nameof(ValuePanel.Option),
			typeof(List<Object>),
			typeof(ValuePanel),
			new PropertyMetadata(null)
		);

		public List<Object>? Option {
			get => this.GetValue(ValuePanel.OptionProperty) as List<Object>;
			set => this.SetValue(ValuePanel.OptionProperty, value);
		}

		// ----------------

		public static readonly DependencyProperty ValueProperty = DependencyProperty.Register(
			nameof(ValuePanel.Value),
			typeof(CommandConfigurationModel.ArgumentValue),
			typeof(ValuePanel),
			new PropertyMetadata(null)
		);

		public CommandConfigurationModel.ArgumentValue? Value {
			get => this.GetValue(ValuePanel.ValueProperty) as CommandConfigurationModel.ArgumentValue;
			set => this.SetValue(ValuePanel.ValueProperty, value);
		}

		// ----------------

		public static readonly DependencyProperty StampProperty = DependencyProperty.Register(
			nameof(ValuePanel.Stamp),
			typeof(UniqueStamp),
			typeof(ValuePanel),
			new PropertyMetadata(UniqueStamp.Default, (o, e) => {
				(o as ValuePanel)!.UpdateVisualState();
				(o as ValuePanel)!.Controller.Update();
			})
		);

		public UniqueStamp Stamp {
			get => this.GetValue(ValuePanel.StampProperty) as UniqueStamp ?? throw new Exception();
			set => this.SetValue(ValuePanel.StampProperty, value);
		}

		#endregion

	}

	public class ValuePanelController : CustomController {

		#region data

		public ValuePanel View { get; init; } = default!;

		// ----------------

		public CommandConfigurationModel.ArgumentType? Type => this.View.Type;

		public List<Object>? Option => this.View.Option;

		public CommandConfigurationModel.ArgumentValue? Value => this.View.Value;

		#endregion

		#region update

		public async void Update (
		) {
			if (this.Type is not null && this.Value is not null) {
				this.NotifyPropertyChanged(
					nameof(this.uAction_IsEnabled)
				);
				if (this.Option is not null) {
					this.NotifyPropertyChanged(
						nameof(this.uEnumerationValue_ItemsSource),
						nameof(this.uEnumerationValue_SelectedValue)
					);
				} else {
					switch (this.Type) {
						case CommandConfigurationModel.ArgumentType.Confirmation: {
							this.NotifyPropertyChanged(
								nameof(this.uConfirmationValue_Text)
							);
							break;
						}
						case CommandConfigurationModel.ArgumentType.Number: {
							this.NotifyPropertyChanged(
								nameof(this.uNumberValue_Value)
							);
							break;
						}
						case CommandConfigurationModel.ArgumentType.Integer: {
							this.NotifyPropertyChanged(
								nameof(this.uIntegerValue_Value)
							);
							break;
						}
						case CommandConfigurationModel.ArgumentType.Size: {
							this.NotifyPropertyChanged(
								nameof(this.uSizeValue_Value),
								nameof(this.uSizeUnit_SelectedItem)
							);
							break;
						}
						case CommandConfigurationModel.ArgumentType.String: {
							this.NotifyPropertyChanged(
								nameof(this.uStringValue_Text)
							);
							break;
						}
						case CommandConfigurationModel.ArgumentType.Path: {
							this.NotifyPropertyChanged(
								nameof(this.uPathValue_Text)
							);
							break;
						}
						default: throw new ArgumentOutOfRangeException();
					}
				}
			}
			return;
		}

		#endregion

		#region action

		public Boolean uAction_IsEnabled {
			get {
				if (this.Value is not { Value: not null }) { return false; }
				return true;
			}
		}

		// ----------------

		public String uConfirmationValue_Text {
			get {
				if (this.Type is not CommandConfigurationModel.ArgumentType.Confirmation || this.Option is not null || this.Value is not { Value: not null }) { return ""; }
				return ConvertHelper.BooleanToConfirmationStringLower(this.Value.OfConfirmation);
			}
		}

		public async void uConfirmationValue_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not Button senders) { return; }
			if (this.Type is not CommandConfigurationModel.ArgumentType.Confirmation || this.Option is not null || this.Value is not { Value: not null }) { return; }
			this.Value.OfConfirmation = !this.Value.OfConfirmation;
			this.NotifyPropertyChanged(
				nameof(this.uConfirmationValue_Text)
			);
			return;
		}

		// ----------------

		public DecimalFormatter uNumberValue_NumberFormatter {
			get {
				return new DecimalFormatter() { IntegerDigits = 1, FractionDigits = 1 };
			}
		}

		public Floater uNumberValue_Value {
			get {
				if (this.Type is not CommandConfigurationModel.ArgumentType.Number || this.Option is not null || this.Value is not { Value: not null }) { return Floater.NaN; }
				return this.Value.OfNumber;
			}
		}

		public async void uNumberValue_OnValueChanged (
			NumberBox                      sender,
			NumberBoxValueChangedEventArgs args
		) {
			if (sender is not NumberBox senders) { return; }
			if (this.Type is not CommandConfigurationModel.ArgumentType.Number || this.Option is not null || this.Value is not { Value: not null }) { return; }
			if (!Floater.IsNaN(args.NewValue)) {
				this.Value.OfNumber = args.NewValue;
			}
			this.NotifyPropertyChanged(
				nameof(this.uNumberValue_Value)
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
				if (this.Type is not CommandConfigurationModel.ArgumentType.Integer || this.Option is not null || this.Value is not { Value: not null }) { return Floater.NaN; }
				return (Floater)this.Value.OfInteger;
			}
		}

		public async void uIntegerValue_OnValueChanged (
			NumberBox                      sender,
			NumberBoxValueChangedEventArgs args
		) {
			if (sender is not NumberBox senders) { return; }
			if (this.Type is not CommandConfigurationModel.ArgumentType.Integer || this.Option is not null || this.Value is not { Value: not null }) { return; }
			if (!Floater.IsNaN(args.NewValue)) {
				this.Value.OfInteger = (Integer)args.NewValue;
			}
			this.NotifyPropertyChanged(
				nameof(this.uIntegerValue_Value)
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
				if (this.Type is not CommandConfigurationModel.ArgumentType.Size || this.Option is not null || this.Value is not { Value: not null }) { return Floater.NaN; }
				return this.Value.OfSize.Value;
			}
		}

		public async void uSizeValue_OnValueChanged (
			NumberBox                      sender,
			NumberBoxValueChangedEventArgs args
		) {
			if (sender is not NumberBox senders) { return; }
			if (this.Type is not CommandConfigurationModel.ArgumentType.Size || this.Option is not null || this.Value is not { Value: not null }) { return; }
			if (!Floater.IsNaN(args.NewValue)) {
				this.Value.OfSize.Value = args.NewValue;
			}
			this.NotifyPropertyChanged(
				nameof(this.uSizeValue_Value)
			);
			return;
		}

		public List<CommandConfigurationModel.SizeUnit> uSizeUnit_ItemsSource {
			get {
				return new List<CommandConfigurationModel.SizeUnit>() { CommandConfigurationModel.SizeUnit.B, CommandConfigurationModel.SizeUnit.K, CommandConfigurationModel.SizeUnit.M, CommandConfigurationModel.SizeUnit.G };
			}
		}

		public CommandConfigurationModel.SizeUnit uSizeUnit_SelectedItem {
			get {
				if (this.Type is not CommandConfigurationModel.ArgumentType.Size || this.Option is not null || this.Value is not { Value: not null }) { return CommandConfigurationModel.SizeUnit.B; }
				return this.Value.OfSize.Unit;
			}
		}

		public async void uSizeUnit_OnSelectionChanged (
			Object                    sender,
			SelectionChangedEventArgs args
		) {
			if (sender is not ComboBox senders) { return; }
			if (this.Type is not CommandConfigurationModel.ArgumentType.Size || this.Option is not null || this.Value is not { Value: not null }) { return; }
			this.Value.OfSize.Unit = senders.SelectedItem as CommandConfigurationModel.SizeUnit? ?? throw new Exception();
			return;
		}

		// ----------------

		public String uStringValue_Text {
			get {
				if (this.Type is not CommandConfigurationModel.ArgumentType.String || this.Option is not null || this.Value is not { Value: not null }) { return ""; }
				return this.Value.OfString;
			}
		}

		public async void uStringValue_OnTextChanged (
			Object               sender,
			TextChangedEventArgs args
		) {
			if (sender is not TextBox senders) { return; }
			if (this.Type is not CommandConfigurationModel.ArgumentType.String || this.Option is not null || this.Value is not { Value: not null }) { return; }
			this.Value.OfString = senders.Text;
			this.NotifyPropertyChanged(
				nameof(this.uStringValue_Text)
			);
			return;
		}

		// ----------------

		public String uPathValue_Text {
			get {
				if (this.Type is not CommandConfigurationModel.ArgumentType.Path || this.Option is not null || this.Value is not { Value: not null }) { return ""; }
				return this.Value.OfPath.Value;
			}
		}

		public async void uPathValue_OnTextChanged (
			Object               sender,
			TextChangedEventArgs args
		) {
			if (sender is not TextBox senders) { return; }
			if (this.Type is not CommandConfigurationModel.ArgumentType.Path || this.Option is not null || this.Value is not { Value: not null }) { return; }
			this.Value.OfPath.Value = senders.Text;
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
			if (this.Type is not CommandConfigurationModel.ArgumentType.Path || this.Option is not null || this.Value is not { Value: not null }) { return; }
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
			if (this.Type is not CommandConfigurationModel.ArgumentType.Path || this.Option is not null || this.Value is not { Value: not null }) { return; }
			if (args.DataView.Contains(StandardDataFormats.StorageItems)) {
				var newValue = StorageHelper.Normalize((await args.DataView.GetStorageItemsAsync())[0].Path);
				this.Value.OfPath.Value = newValue;
				this.NotifyPropertyChanged(
					nameof(this.uPathValue_Text)
				);
			}
			return;
		}

		public async void uPathPickFile_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not MenuFlyoutItem senders) { return; }
			if (this.Type is not CommandConfigurationModel.ArgumentType.Path || this.Option is not null || this.Value is not { Value: not null }) { return; }
			var newValue = await StorageHelper.PickFile(WindowHelper.GetForElement(this.View));
			if (newValue is not null) {
				this.Value.OfPath.Value = newValue;
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
			if (this.Type is not CommandConfigurationModel.ArgumentType.Path || this.Option is not null || this.Value is not { Value: not null }) { return; }
			var newValue = await StorageHelper.PickDirectory(WindowHelper.GetForElement(this.View));
			if (newValue is not null) {
				this.Value.OfPath.Value = newValue;
				this.NotifyPropertyChanged(
					nameof(this.uPathValue_Text)
				);
			}
			return;
		}

		// ----------------

		public List<Tuple<Object, String>> uEnumerationValue_ItemsSource {
			get {
				if (this.Type is null || this.Option is null || this.Value is null) { return new List<Tuple<Object, String>>(); }
				return this.Option.Select((value) => (new Tuple<Object, String>(value, CommandConfigurationHelper.MakeArgumentValueString(value)))).ToList();
			}
		}

		public Object? uEnumerationValue_SelectedValue {
			get {
				if (this.Type is null || this.Option is null || this.Value is null) { return null; }
				return this.Value.Value;
			}
		}

		public async void uEnumerationValue_OnSelectionChanged (
			Object                    sender,
			SelectionChangedEventArgs args
		) {
			if (sender is not ComboBox senders) { return; }
			if (this.Type is null || this.Option is null || this.Value is null) { return; }
			this.Value.Value = senders.SelectedValue;
			this.NotifyPropertyChanged(
				nameof(this.uEnumerationValue_SelectedValue)
			);
			return;
		}

		#endregion

	}

}
