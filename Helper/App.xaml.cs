#pragma warning disable 0,
// ReSharper disable

using Helper;

namespace Helper {

	public partial class App : Application {

		#region life

		public App (
		) {
			this.InitializeComponent();
		}

		// ----------------

		protected override void OnLaunched (
			LaunchActivatedEventArgs args
		) {
			var window = Utility.WindowHelper.TrackWindow(new Module.MainWindow());
			window.Activate();
			Utility.ThemeHelper.Initialize();
		}

		#endregion

	}

}
