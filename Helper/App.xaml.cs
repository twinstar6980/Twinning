#pragma warning disable 0,
// ReSharper disable

using Helper;
using Helper.Utility;

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
			var window = WindowHelper.TrackWindow(new Module.MainWindow());
			window.Activate();
			ThemeHelper.Initialize();
		}

		#endregion

	}

}
