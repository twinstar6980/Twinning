#pragma warning disable 0,
// ReSharper disable

using AssistantPlus;
using AssistantPlus.Utility;
using Windows.ApplicationModel.DataTransfer;
using Windows.Globalization.NumberFormatting;

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

		public SubmissionValue Value => this.View.Value;

		#endregion

		#region update

		public async void Update (
		) {
			this.NotifyPropertyChanged(
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
						nameof(this.uBooleanNoValue_IsChecked),
						nameof(this.uBooleanYesValue_IsChecked)
					);
					break;
				}
				case SubmissionType.Boolean: {
					this.NotifyPropertyChanged(
						nameof(this.uBooleanNoValue_IsChecked),
						nameof(this.uBooleanYesValue_IsChecked)
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
					this.NotifyPropertyChanged(
						nameof(this.uSizeValue_Value),
						nameof(this.uSizeUnit_SelectedItem)
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
						nameof(this.uPathValue_Text)
					);
					break;
				}
				case SubmissionType.Enumeration: {
					this.NotifyPropertyChanged(
						nameof(this.uEnumerationValue_ItemsSource),
						nameof(this.uEnumerationValue_SelectedIndex)
					);
					break;
				}
				default: throw new ();
			}
			return;
		}

		#endregion

		#region common

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

		public Boolean uBooleanNoValue_IsChecked {
			get {
				if (this.Type is not SubmissionType.Boolean) {
					return false;
				}
				return this.Value.OfBoolean is null ? false : this.Value.OfBoolean == false;
			}
		}

		public async void uBooleanNoValue_Click (
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
				this.Value.OfBoolean = false;
			}
			this.NotifyPropertyChanged(
				nameof(this.uBooleanYesValue_IsChecked)
			);
			return;
		}

		// ----------------

		public Boolean uBooleanYesValue_IsChecked {
			get {
				if (this.Type is not SubmissionType.Boolean) {
					return false;
				}
				return this.Value.OfBoolean is null ? false : this.Value.OfBoolean == true;
			}
		}

		public async void uBooleanYesValue_Click (
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
				this.Value.OfBoolean = true;
			}
			this.NotifyPropertyChanged(
				nameof(this.uBooleanNoValue_IsChecked)
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
				return this.Value.OfInteger is null ? Floater.NaN : (Floater)this.Value.OfInteger;
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
				if (this.Type is not SubmissionType.Floater) {
					return Floater.NaN;
				}
				return this.Value.OfFloater is null ? Floater.NaN : (Floater)this.Value.OfFloater;
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
				return new () { IntegerDigits = 1, FractionDigits = 1 };
			}
		}

		public Floater uSizeValue_Value {
			get {
				if (this.Type is not SubmissionType.Size) {
					return Floater.NaN;
				}
				return this.Value.OfSize is null ? Floater.NaN : this.Value.OfSize.Value;
			}
		}

		public async void uSizeValue_ValueChanged (
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
				this.Value.OfSize = new () { Value = args.NewValue, Unit = this.uSizeUnit_SelectedItem };
			}
			this.NotifyPropertyChanged(
				nameof(this.uSizeValue_Value)
			);
			return;
		}

		// ----------------

		public SizeUnit uSizeUnit_SelectedItem { get; set; } = SizeUnit.M;

		public async void uSizeUnit_SelectionChanged (
			Object                    sender,
			SelectionChangedEventArgs args
		) {
			var senders = sender.AsClass<ComboBox>();
			if (this.Type is not SubmissionType.Size) {
				return;
			}
			this.uSizeUnit_SelectedItem = senders.SelectedItem.AsStruct<SizeUnit>();
			if (this.Value.OfSize is not null) {
				this.Value.OfSize.Unit = this.uSizeUnit_SelectedItem;
			}
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
				this.Value.OfString = senders.Text;
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
				return this.Value.OfString is null ? "" : this.Value.OfString;
			}
		}

		#endregion

		#region path

		public async void uPathValue_DragOver (
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

		public async void uPathValue_Drop (
			Object        sender,
			DragEventArgs args
		) {
			var senders = sender.AsClass<TextBox>();
			if (this.Type is not SubmissionType.Path) {
				return;
			}
			if (args.DataView.Contains(StandardDataFormats.StorageItems)) {
				args.Handled = true;
				var data = await args.DataView.GetStorageItemsAsync();
				this.Value.OfPath = new () { Value = StorageHelper.Regularize(data[0].Path) };
				this.NotifyPropertyChanged(
					nameof(this.uPathValue_Text)
				);
			}
			return;
		}

		public async void uPathValue_LostFocus (
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
				this.Value.OfPath = new () { Value = StorageHelper.Regularize(senders.Text) };
			}
			this.NotifyPropertyChanged(
				nameof(this.uPathValue_Text)
			);
			return;
		}

		public String uPathValue_Text {
			get {
				if (this.Type is not SubmissionType.Path) {
					return "";
				}
				return this.Value.OfPath is null ? "" : this.Value.OfPath.Value;
			}
		}

		// ----------------

		public async void uPathCommand_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			if (this.Type is not SubmissionType.Path) {
				return;
			}
			this.Value.OfPath = new () { Value = $":{senders.Tag.AsClass<String>()[0].ToString().ToLower()}" };
			this.NotifyPropertyChanged(
				nameof(this.uPathValue_Text)
			);
			return;
		}

		public async void uPathPick_Click (
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
				this.Value.OfPath = new () { Value = value };
				this.NotifyPropertyChanged(
					nameof(this.uPathValue_Text)
				);
			}
			return;
		}

		#endregion

		#region enumeration

		public List<String> uEnumerationValue_ItemsSource {
			get {
				if (this.Type is not SubmissionType.Enumeration) {
					return [];
				}
				var item = this.Option[0..];
				return ["\0", ..item];
			}
		}

		public Size uEnumerationValue_SelectedIndex {
			get {
				if (this.Type is not SubmissionType.Enumeration) {
					return -1;
				}
				return this.Value.OfEnumeration is null ? -1 : this.Value.OfEnumeration.Value;
			}
		}

		public async void uEnumerationValue_SelectionChanged (
			Object                    sender,
			SelectionChangedEventArgs args
		) {
			var senders = sender.AsClass<ComboBox>();
			if (this.Type is not SubmissionType.Enumeration) {
				return;
			}
			if (senders.SelectedIndex == -1 || senders.SelectedIndex == 0) {
				this.Value.OfEnumeration = null;
				this.NotifyPropertyChanged(
					nameof(this.uEnumerationValue_SelectedIndex)
				);
			}
			else {
				this.Value.OfEnumeration = new () { Value = senders.SelectedIndex };
			}
			return;
		}

		#endregion

	}

}
