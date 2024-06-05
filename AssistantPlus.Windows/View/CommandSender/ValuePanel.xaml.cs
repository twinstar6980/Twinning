#pragma warning disable 0,
// ReSharper disable

using AssistantPlus;
using AssistantPlus.Utility;
using Windows.ApplicationModel.DataTransfer;
using Windows.Globalization.NumberFormatting;

namespace AssistantPlus.View.CommandSender {

	public sealed partial class ValuePanel : CustomControl {

		#region life

		public ValuePanel (
		) {
			this.InitializeComponent();
			this.Controller = new () { View = this };
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
			new (null)
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
			new (null)
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
			new (new ArgumentValue() { Data = null })
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
								nameof(this.uBooleanValue_Content)
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
								nameof(this.uSizeCount_Value),
								nameof(this.uSizeExponent_SelectedIndex)
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
								nameof(this.uPathContent_Text)
							);
							break;
						}
						default: throw new ();
					}
				}
				else {
					this.NotifyPropertyChanged(
						nameof(this.uEnumerationItem_ItemsSource),
						nameof(this.uEnumerationItem_SelectedValue)
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
				nameof(this.uBooleanValue_Content)
			);
			return;
		}

		public String uBooleanValue_Content {
			get {
				if (this.Type is not ArgumentType.Boolean || this.Option is not null || this.Value.Data is null) {
					return " ";
				}
				return ConvertHelper.MakeBooleanToStringOfConfirmationCharacter(this.Value.OfBoolean);
			}
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
				return new () { IntegerDigits = 1, FractionDigits = 1 };
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

		public DecimalFormatter uSizeCount_NumberFormatter {
			get {
				return new () { IntegerDigits = 1, FractionDigits = 1 };
			}
		}

		public Floater uSizeCount_Value {
			get {
				if (this.Type is not ArgumentType.Size || this.Option is not null || this.Value.Data is null) {
					return Floater.NaN;
				}
				return this.Value.OfSize.Count;
			}
		}

		public async void uSizeCount_ValueChanged (
			NumberBox                      sender,
			NumberBoxValueChangedEventArgs args
		) {
			var senders = sender.AsClass<NumberBox>();
			if (this.Type is not ArgumentType.Size || this.Option is not null || this.Value.Data is null) {
				return;
			}
			if (!Floater.IsNaN(args.NewValue)) {
				this.Value.OfSize.Count = args.NewValue;
			}
			this.NotifyPropertyChanged(
				nameof(this.uSizeCount_Value)
			);
			return;
		}

		// ----------------

		public List<String> uSizeExponent_ItemsSource { get; } = ["B", "K", "M", "G"];

		public Size uSizeExponent_SelectedIndex {
			get {
				if (this.Type is not ArgumentType.Size || this.Option is not null || this.Value.Data is null) {
					return 2;
				}
				return (Size)this.Value.OfSize.Exponent;
			}
		}

		public async void uSizeExponent_SelectionChanged (
			Object                    sender,
			SelectionChangedEventArgs args
		) {
			var senders = sender.AsClass<ComboBox>();
			if (this.Type is not ArgumentType.Size || this.Option is not null || this.Value.Data is null) {
				return;
			}
			this.Value.OfSize.Exponent = senders.SelectedIndex;
			return;
		}

		#endregion

		#region string

		public async void uStringValue_LostFocus (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<TextBox>();
			if (this.Type is not ArgumentType.String || this.Option is not null || this.Value.Data is null) {
				return;
			}
			this.Value.OfString = senders.Text;
			this.NotifyPropertyChanged(
				nameof(this.uStringValue_Text)
			);
			return;
		}

		public String uStringValue_Text {
			get {
				if (this.Type is not ArgumentType.String || this.Option is not null || this.Value.Data is null) {
					return "";
				}
				return this.Value.OfString;
			}
		}

		#endregion

		#region path

		public async void uPathContent_DragOver (
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

		public async void uPathContent_Drop (
			Object        sender,
			DragEventArgs args
		) {
			var senders = sender.AsClass<TextBox>();
			if (this.Type is not ArgumentType.Path || this.Option is not null || this.Value.Data is null) {
				return;
			}
			if (args.DataView.Contains(StandardDataFormats.StorageItems)) {
				args.Handled = true;
				var item = await args.DataView.GetStorageItemsAsync();
				this.Value.OfPath.Content = StorageHelper.GetLongPath(item[0].Path);
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
			if (this.Type is not ArgumentType.Path || this.Option is not null || this.Value.Data is null) {
				return;
			}
			this.Value.OfPath.Content = StorageHelper.Regularize(senders.Text);
			this.NotifyPropertyChanged(
				nameof(this.uPathContent_Text)
			);
			return;
		}

		public String uPathContent_Text {
			get {
				if (this.Type is not ArgumentType.Path || this.Option is not null || this.Value.Data is null) {
					return "";
				}
				return this.Value.OfPath.Content;
			}
		}

		// ----------------

		public async void uPathPickMenu_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			if (this.Type is not ArgumentType.Path || this.Option is not null || this.Value.Data is null) {
				return;
			}
			var value = senders.Tag.AsClass<String>() switch {
				"OpenFile"      => await StorageHelper.PickOpenFile(WindowHelper.Find(this.View), $"{nameof(CommandSender)}.Generic"),
				"OpenDirectory" => await StorageHelper.PickOpenDirectory(WindowHelper.Find(this.View), $"{nameof(CommandSender)}.Generic"),
				"SaveFile"      => await StorageHelper.PickSaveFile(WindowHelper.Find(this.View), $"{nameof(CommandSender)}.Generic", null, null),
				_               => throw new (),
			};
			if (value is not null) {
				this.Value.OfPath.Content = value;
				this.NotifyPropertyChanged(
					nameof(this.uPathContent_Text)
				);
			}
			return;
		}

		#endregion

		#region enumeration

		public List<Tuple<Object, String>> uEnumerationItem_ItemsSource {
			get {
				if (this.Type is null || this.Option is null || this.Value.Data is null) {
					return [];
				}
				return this.Option.Select((value) => (new Tuple<Object, String>(value, ConfigurationHelper.MakeArgumentValueString(value)))).ToList();
			}
		}

		public Object? uEnumerationItem_SelectedValue {
			get {
				if (this.Type is null || this.Option is null || this.Value.Data is null) {
					return null;
				}
				return this.Value.Data;
			}
		}

		public async void uEnumerationItem_SelectionChanged (
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
