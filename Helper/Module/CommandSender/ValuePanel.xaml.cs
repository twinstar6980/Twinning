#pragma warning disable 0,
// ReSharper disable

using Helper;
using Helper.Utility;
using Windows.ApplicationModel.DataTransfer;
using Windows.Globalization.NumberFormatting;

namespace Helper.Module.CommandSender {

	public sealed partial class ValuePanel : CustomControl {

		#region life

		public ValuePanel (
		) {
			this.InitializeComponent();
			this.Controller = new ValuePanelController() { View = this };
		}

		// ----------------

		private ValuePanelController Controller { get; }

		// ----------------

		protected override void StampUpdate (
		) {
			VisualStateManager.GoToState(this, $"{(this.Type is null ? "Null" : this.Option is null ? this.Type : "Enumeration")}State", false);
			this.Controller.Update();
			return;
		}

		#endregion

		#region property

		public static readonly DependencyProperty TypeProperty = DependencyProperty.Register(
			nameof(ValuePanel.Type),
			typeof(ArgumentType),
			typeof(ValuePanel),
			new PropertyMetadata(null)
		);

		public ArgumentType? Type {
			get => this.GetValue(ValuePanel.TypeProperty).AsStructOrNull<ArgumentType>();
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
			get => this.GetValue(ValuePanel.OptionProperty).AsClassOrNull<List<Object>>();
			set => this.SetValue(ValuePanel.OptionProperty, value);
		}

		// ----------------

		public static readonly DependencyProperty ValueProperty = DependencyProperty.Register(
			nameof(ValuePanel.Value),
			typeof(ArgumentValue),
			typeof(ValuePanel),
			new PropertyMetadata(new ArgumentValue() { Data = null })
		);

		public ArgumentValue Value {
			get => this.GetValue(ValuePanel.ValueProperty).AsClass<ArgumentValue>();
			set => this.SetValue(ValuePanel.ValueProperty, value);
		}

		#endregion

	}

	public class ValuePanelController : CustomController {

		#region data

		public ValuePanel View { get; init; } = default!;

		// ----------------

		public ArgumentType? Type => this.View.Type;

		public List<Object>? Option => this.View.Option;

		public ArgumentValue Value => this.View.Value;

		#endregion

		#region update

		public async void Update (
		) {
			if (this.Type is not null) {
				this.NotifyPropertyChanged(
					nameof(this.uAction_IsEnabled)
				);
				if (this.Option is null) {
					switch (this.Type) {
						case ArgumentType.Boolean: {
							this.NotifyPropertyChanged(
								nameof(this.uBooleanValue_Text)
							);
							break;
						}
						case ArgumentType.Integer: {
							this.NotifyPropertyChanged(
								nameof(this.uIntegerValue_Value)
							);
							break;
						}
						case ArgumentType.Floater: {
							this.NotifyPropertyChanged(
								nameof(this.uFloaterValue_Value)
							);
							break;
						}
						case ArgumentType.Size: {
							this.NotifyPropertyChanged(
								nameof(this.uSizeValue_Value),
								nameof(this.uSizeUnit_SelectedItem)
							);
							break;
						}
						case ArgumentType.String: {
							this.NotifyPropertyChanged(
								nameof(this.uStringValue_Text)
							);
							break;
						}
						case ArgumentType.Path: {
							this.NotifyPropertyChanged(
								nameof(this.uPathValue_Text)
							);
							break;
						}
						default: throw new ArgumentOutOfRangeException();
					}
				}
				else {
					this.NotifyPropertyChanged(
						nameof(this.uEnumerationValue_ItemsSource),
						nameof(this.uEnumerationValue_SelectedValue)
					);
				}
			}
			return;
		}

		#endregion

		#region common

		public Boolean uAction_IsEnabled {
			get {
				if (this.Value.Data is null) {
					return false;
				}
				return true;
			}
		}

		#endregion

		#region boolean

		public String uBooleanValue_Text {
			get {
				if (this.Type is not ArgumentType.Boolean || this.Option is not null || this.Value.Data is null) {
					return " ";
				}
				return ConvertHelper.BooleanToConfirmationStringLower(this.Value.OfBoolean);
			}
		}

		public async void uBooleanValue_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<Button>();
			if (this.Type is not ArgumentType.Boolean || this.Option is not null || this.Value.Data is null) {
				return;
			}
			this.Value.OfBoolean = !this.Value.OfBoolean;
			this.NotifyPropertyChanged(
				nameof(this.uBooleanValue_Text)
			);
			return;
		}

		#endregion

		#region integer

		public DecimalFormatter uIntegerValue_NumberFormatter {
			get {
				return new DecimalFormatter() { IntegerDigits = 1, FractionDigits = 0 };
			}
		}

		public Floater uIntegerValue_Value {
			get {
				if (this.Type is not ArgumentType.Integer || this.Option is not null || this.Value.Data is null) {
					return Floater.NaN;
				}
				return (Floater)this.Value.OfInteger;
			}
		}

		public async void uIntegerValue_ValueChanged (
			NumberBox                      sender,
			NumberBoxValueChangedEventArgs args
		) {
			var senders = sender.AsClass<NumberBox>();
			if (this.Type is not ArgumentType.Integer || this.Option is not null || this.Value.Data is null) {
				return;
			}
			if (!Floater.IsNaN(args.NewValue) && (Integer.MinValue < args.NewValue && args.NewValue < Integer.MaxValue)) {
				this.Value.OfInteger = (Integer)args.NewValue;
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
				return new DecimalFormatter() { IntegerDigits = 1, FractionDigits = 1 };
			}
		}

		public Floater uFloaterValue_Value {
			get {
				if (this.Type is not ArgumentType.Floater || this.Option is not null || this.Value.Data is null) {
					return Floater.NaN;
				}
				return this.Value.OfFloater;
			}
		}

		public async void uFloaterValue_ValueChanged (
			NumberBox                      sender,
			NumberBoxValueChangedEventArgs args
		) {
			var senders = sender.AsClass<NumberBox>();
			if (this.Type is not ArgumentType.Floater || this.Option is not null || this.Value.Data is null) {
				return;
			}
			if (!Floater.IsNaN(args.NewValue)) {
				this.Value.OfFloater = args.NewValue;
			}
			this.NotifyPropertyChanged(
				nameof(this.uFloaterValue_Value)
			);
			return;
		}

		#endregion

		#region size

		public DecimalFormatter uSizeValue_NumberFormatter {
			get {
				return new DecimalFormatter() { IntegerDigits = 1, FractionDigits = 1 };
			}
		}

		public Floater uSizeValue_Value {
			get {
				if (this.Type is not ArgumentType.Size || this.Option is not null || this.Value.Data is null) {
					return Floater.NaN;
				}
				return this.Value.OfSize.Value;
			}
		}

		public async void uSizeValue_ValueChanged (
			NumberBox                      sender,
			NumberBoxValueChangedEventArgs args
		) {
			var senders = sender.AsClass<NumberBox>();
			if (this.Type is not ArgumentType.Size || this.Option is not null || this.Value.Data is null) {
				return;
			}
			if (!Floater.IsNaN(args.NewValue)) {
				this.Value.OfSize.Value = args.NewValue;
			}
			this.NotifyPropertyChanged(
				nameof(this.uSizeValue_Value)
			);
			return;
		}

		// ----------------

		public List<SizeUnit> uSizeUnit_ItemsSource {
			get {
				return new List<SizeUnit>() { SizeUnit.B, SizeUnit.K, SizeUnit.M, SizeUnit.G };
			}
		}

		public SizeUnit uSizeUnit_SelectedItem {
			get {
				if (this.Type is not ArgumentType.Size || this.Option is not null || this.Value.Data is null) {
					return SizeUnit.M;
				}
				return this.Value.OfSize.Unit;
			}
		}

		public async void uSizeUnit_SelectionChanged (
			Object                    sender,
			SelectionChangedEventArgs args
		) {
			var senders = sender.AsClass<ComboBox>();
			if (this.Type is not ArgumentType.Size || this.Option is not null || this.Value.Data is null) {
				return;
			}
			this.Value.OfSize.Unit = senders.SelectedItem.AsStruct<SizeUnit>();
			return;
		}

		#endregion

		#region string

		public String uStringValue_Text {
			get {
				if (this.Type is not ArgumentType.String || this.Option is not null || this.Value.Data is null) {
					return "";
				}
				return this.Value.OfString;
			}
		}

		public async void uStringValue_TextChanged (
			Object               sender,
			TextChangedEventArgs args
		) {
			var senders = sender.AsClass<TextBox>();
			if (this.Type is not ArgumentType.String || this.Option is not null || this.Value.Data is null) {
				return;
			}
			this.Value.OfString = senders.Text;
			return;
		}

		#endregion

		#region path

		public String uPathValue_Text {
			get {
				if (this.Type is not ArgumentType.Path || this.Option is not null || this.Value.Data is null) {
					return "";
				}
				return this.Value.OfPath.Value;
			}
		}

		public async void uPathValue_TextChanged (
			Object               sender,
			TextChangedEventArgs args
		) {
			var senders = sender.AsClass<TextBox>();
			if (this.Type is not ArgumentType.Path || this.Option is not null || this.Value.Data is null) {
				return;
			}
			this.Value.OfPath.Value = senders.Text;
			return;
		}

		public async void uPathValue_DragOver (
			Object        sender,
			DragEventArgs args
		) {
			var senders = sender.AsClass<TextBox>();
			if (this.Type is not ArgumentType.Path || this.Option is not null || this.Value.Data is null) {
				return;
			}
			if (args.DataView.Contains(StandardDataFormats.StorageItems)) {
				args.AcceptedOperation = Windows.ApplicationModel.DataTransfer.DataPackageOperation.Link;
			}
			return;
		}

		public async void uPathValue_Drop (
			Object        sender,
			DragEventArgs args
		) {
			var senders = sender.AsClass<TextBox>();
			if (this.Type is not ArgumentType.Path || this.Option is not null || this.Value.Data is null) {
				return;
			}
			if (args.DataView.Contains(StandardDataFormats.StorageItems)) {
				args.Handled = true;
				var data = await args.DataView.GetStorageItemsAsync();
				this.Value.OfPath.Value = StorageHelper.Regularize(data[0].Path);
				this.NotifyPropertyChanged(
					nameof(this.uPathValue_Text)
				);
			}
			return;
		}

		// ----------------

		public async void uPathPickFile_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			if (this.Type is not ArgumentType.Path || this.Option is not null || this.Value.Data is null) {
				return;
			}
			var value = await StorageHelper.PickFile(WindowHelper.GetForElement(this.View));
			if (value is not null) {
				this.Value.OfPath.Value = value;
				this.NotifyPropertyChanged(
					nameof(this.uPathValue_Text)
				);
			}
			return;
		}

		public async void uPathPickDirectory_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			if (this.Type is not ArgumentType.Path || this.Option is not null || this.Value.Data is null) {
				return;
			}
			var value = await StorageHelper.PickDirectory(WindowHelper.GetForElement(this.View));
			if (value is not null) {
				this.Value.OfPath.Value = value;
				this.NotifyPropertyChanged(
					nameof(this.uPathValue_Text)
				);
			}
			return;
		}

		#endregion

		#region enumeration

		public List<Tuple<Object, String>> uEnumerationValue_ItemsSource {
			get {
				if (this.Type is null || this.Option is null || this.Value.Data is null) {
					return new List<Tuple<Object, String>>();
				}
				return this.Option.Select((value) => (new Tuple<Object, String>(value, ConfigurationHelper.MakeArgumentValueString(value)))).ToList();
			}
		}

		public Object? uEnumerationValue_SelectedValue {
			get {
				if (this.Type is null || this.Option is null || this.Value.Data is null) {
					return null;
				}
				return this.Value.Data;
			}
		}

		public async void uEnumerationValue_SelectionChanged (
			Object                    sender,
			SelectionChangedEventArgs args
		) {
			var senders = sender.AsClass<ComboBox>();
			if (this.Type is null || this.Option is null || this.Value.Data is null) {
				return;
			}
			this.Value.Data = senders.SelectedValue;
			return;
		}

		#endregion

	}

}
