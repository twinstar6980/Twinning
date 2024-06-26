#pragma warning disable 0,
// ReSharper disable

using AssistantPlus;
using AssistantPlus.Utility;

namespace AssistantPlus.View.PackageBuilder {

	public sealed partial class VariableListPanel : CustomControl {

		#region life

		public VariableListPanel (
		) {
			this.InitializeComponent();
			this.Controller = new () { View = this };
		}

		// ----------------

		private VariableListPanelController Controller { get; }

		// ----------------

		protected override void StampUpdate (
		) {
			this.Controller.Update();
			return;
		}

		#endregion

		#region property

		public static readonly DependencyProperty ValueProperty = DependencyProperty.Register(
			nameof(VariableListPanel.Value),
			typeof(List<Variable>),
			typeof(VariableListPanel),
			new (new List<Variable>())
		);

		public List<Variable> Value {
			get => this.GetValue(VariableListPanel.ValueProperty).As<List<Variable>>();
			set => this.SetValue(VariableListPanel.ValueProperty, value);
		}

		#endregion

	}

	public class VariableListPanelController : CustomController {

		#region data

		public VariableListPanel View { get; init; } = default!;

		// ----------------

		public List<Variable> Value => this.View.Value;

		#endregion

		#region update

		public async void Update (
		) {
			this.uList_ItemsSource = this.Value.Select((value) => (new VariableListPanelItemController() { Host = this, Item = value })).ToObservableCollection();
			this.NotifyPropertyChanged(
				nameof(this.uList_ItemsSource)
			);
			return;
		}

		#endregion

		#region item

		public async void uList_DragItemsCompleted (
			Object                      sender,
			DragItemsCompletedEventArgs args
		) {
			var senders = sender.As<ListView>();
			this.Value.Clear();
			this.Value.AddRange(this.uList_ItemsSource.Select((value) => (value.Item)));
			return;
		}

		public ObservableCollection<VariableListPanelItemController> uList_ItemsSource { get; set; } = [];

		// ----------------

		public async void uAppend_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<Button>();
			var newItem = new Variable() { Name = "", Value = "" };
			this.Value.Add(newItem);
			this.uList_ItemsSource.Add(new () { Host = this, Item = newItem });
			return;
		}

		#endregion

	}

	public class VariableListPanelItemController : CustomController {

		#region data

		public VariableListPanelController Host { get; init; } = default!;

		// ----------------

		public Variable Item { get; set; } = default!;

		#endregion

		#region view

		public async void uName_LostFocus (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<TextBox>();
			this.Item.Name = senders.Text;
			this.NotifyPropertyChanged(
				nameof(this.uName_Text)
			);
			return;
		}

		public String uName_Text {
			get {
				return this.Item.Name;
			}
		}

		// ----------------

		public async void uValue_LostFocus (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<TextBox>();
			this.Item.Value = senders.Text;
			this.NotifyPropertyChanged(
				nameof(this.uValue_Text)
			);
			return;
		}

		public String uValue_Text {
			get {
				return this.Item.Value;
			}
		}

		// ----------------

		public async void uRemove_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<Button>();
			this.Host.Value.Remove(this.Item);
			this.Host.uList_ItemsSource.Remove(this);
			return;
		}

		#endregion

	}

}
