#pragma warning disable 0,
// ReSharper disable

using AssistantPlus;
using AssistantPlus.Utility;

namespace AssistantPlus.View.PackageBuilder {

	public sealed partial class PackagePartPanel : CustomControl {

		#region life

		public PackagePartPanel (
		) {
			this.InitializeComponent();
			this.Controller = new () { View = this };
		}

		// ----------------

		private PackagePartPanelController Controller { get; }

		// ----------------

		protected override void StampUpdate (
		) {
			this.Controller.Update();
			return;
		}

		#endregion

		#region property

		public static readonly DependencyProperty SourceProperty = DependencyProperty.Register(
			nameof(PackagePartPanel.Source),
			typeof(List<String>),
			typeof(PackagePartPanel),
			new (new List<String>())
		);

		public List<String> Source {
			get => this.GetValue(PackagePartPanel.SourceProperty).As<List<String>>();
			set => this.SetValue(PackagePartPanel.SourceProperty, value);
		}

		// ----------------

		public static readonly DependencyProperty ValueProperty = DependencyProperty.Register(
			nameof(PackagePartPanel.Value),
			typeof(List<String>),
			typeof(PackagePartPanel),
			new (new List<String>())
		);

		public List<String> Value {
			get => this.GetValue(PackagePartPanel.ValueProperty).As<List<String>>();
			set => this.SetValue(PackagePartPanel.ValueProperty, value);
		}

		#endregion

	}

	public class PackagePartPanelController : CustomController {

		#region data

		public PackagePartPanel View { get; init; } = default!;

		// ----------------

		public List<String> Source => this.View.Source;

		public List<String> Value => this.View.Value;

		#endregion

		#region update

		public async void Update (
		) {
			this.uList_ItemsSource = this.Source.Select((value) => (new PackagePartPanelItemController() { Host = this, Name = value })).ToList();
			this.NotifyPropertyChanged(
				nameof(this.uList_ItemsSource)
			);
			await Task.Delay(40);
			foreach (var item in this.uList_ItemsSource) {
				if (this.Value.Contains(item.Name)) {
					this.View.uList.SelectedItems.Add(item);
				}
			}
			return;
		}

		#endregion

		#region item

		public List<PackagePartPanelItemController> uList_ItemsSource { get; set; } = [];

		public async void uList_SelectionChanged (
			Object                    sender,
			SelectionChangedEventArgs args
		) {
			var senders = sender.As<ListView>();
			foreach (var item in args.AddedItems.Select(GF.As<PackagePartPanelItemController>)) {
				if (!this.Value.Contains(item.Name)) {
					this.Value.Add(item.Name);
				}
			}
			foreach (var item in args.RemovedItems.Select(GF.As<PackagePartPanelItemController>)) {
				if (this.Value.Contains(item.Name)) {
					this.Value.Remove(item.Name);
				}
			}
			return;
		}

		#endregion

	}

	public class PackagePartPanelItemController : CustomController {

		#region data

		public PackagePartPanelController Host { get; init; } = default!;

		// ----------------

		public String Name { get; set; } = default!;

		#endregion

		#region view

		public String uName_Text {
			get {
				return this.Name;
			}
		}

		#endregion

	}

}
