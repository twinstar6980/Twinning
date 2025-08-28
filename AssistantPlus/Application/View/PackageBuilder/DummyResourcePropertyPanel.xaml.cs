#pragma warning disable 0,
// ReSharper disable

using AssistantPlus;
using AssistantPlus.Utility;

namespace AssistantPlus.View.PackageBuilder {

	public sealed partial class DummyResourcePropertyPanel : CustomControl {

		#region life

		private DummyResourcePropertyPanelController Controller { get; }

		// ----------------

		public DummyResourcePropertyPanel (
		) {
			this.InitializeComponent();
			this.Controller = new () { View = this };
			return;
		}

		// ----------------

		protected override async Task StampUpdate (
		) {
			await this.Controller.UpdateView();
			return;
		}

		#endregion

		#region property

		public static readonly DependencyProperty ValueProperty = DependencyProperty.Register(
			nameof(DummyResourcePropertyPanel.Value),
			typeof(DummyResourceProperty),
			typeof(DummyResourcePropertyPanel),
			new (new DummyResourceProperty() {
			})
		);

		public DummyResourceProperty Value {
			get => this.GetValue(DummyResourcePropertyPanel.ValueProperty).As<DummyResourceProperty>();
			set => this.SetValue(DummyResourcePropertyPanel.ValueProperty, value);
		}

		#endregion

	}

	public partial class DummyResourcePropertyPanelController : CustomController {

		#region data

		public DummyResourcePropertyPanel View { get; init; } = default!;

		// ----------------

		public DummyResourceProperty Value => this.View.Value;

		#endregion

		#region life

		public async Task UpdateView (
		) {
			return;
		}

		#endregion

	}

}
