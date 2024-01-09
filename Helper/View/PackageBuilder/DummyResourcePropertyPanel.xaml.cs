#pragma warning disable 0,
// ReSharper disable

using Helper;
using Helper.Utility;

namespace Helper.View.PackageBuilder {

	public sealed partial class DummyResourcePropertyPanel : CustomControl {

		#region life

		public DummyResourcePropertyPanel (
		) {
			this.InitializeComponent();
			this.Controller = new () { View = this };
		}

		// ----------------

		private DummyResourcePropertyPanelController Controller { get; }

		// ----------------

		protected override void StampUpdate (
		) {
			this.Controller.Update();
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
			get => this.GetValue(DummyResourcePropertyPanel.ValueProperty).AsClass<DummyResourceProperty>();
			set => this.SetValue(DummyResourcePropertyPanel.ValueProperty, value);
		}

		#endregion

	}

	public class DummyResourcePropertyPanelController : CustomController {

		#region data

		public DummyResourcePropertyPanel View { get; init; } = default!;

		// ----------------

		public DummyResourceProperty Value => this.View.Value;

		#endregion

		#region update

		public async void Update (
		) {
			return;
		}

		#endregion

	}

}
