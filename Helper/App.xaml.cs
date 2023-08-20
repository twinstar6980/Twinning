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
			Setting.Initialize();
			var window = default(Window);
			var argument = Environment.GetCommandLineArgs()[1..];
			if (argument.Length == 0) {
				window = new Module.MainWindow();
			} else {
				switch (argument[0]) {
					case "forward": {
						window = new Module.MainWindow(Module.ModuleType.CommandForwarderQuick, argument[1..].ToList());
						WindowHelper.Resize(window, Setting.CommandForwarderQuickWindowSizeWidth, Setting.CommandForwarderQuickWindowSizeHeight);
						WindowHelper.AlwaysOnTop(window, true);
						break;
					}
					default: throw new Exception("invalid command");
				}
			}
			if (window is not null) {
				WindowHelper.Track(window);
				Setting.AppearanceThemeMode = Setting.AppearanceThemeMode;
				WindowHelper.Center(window);
				window.Activate();
			}
			return;
		}

		#endregion

	}

}
