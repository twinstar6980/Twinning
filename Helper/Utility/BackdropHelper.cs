#pragma warning disable 0,
// ReSharper disable

using Helper;
using Microsoft.UI.Composition;
using Microsoft.UI.Composition.SystemBackdrops;
using Microsoft.UI.Xaml.Media;
using WinRT;

namespace Helper.Utility {

	public class BackdropHelper {

		#region structor

		private Window                              mWindow;
		private WindowsSystemDispatcherQueueHelper? mWindowsSystemDispatcherQueueHelper;
		private SystemBackdropConfiguration?        mSystemBackdropConfiguration;
		private MicaController?                     mMicaController;

		// ----------------

		public BackdropHelper (
			Window window
		) {
			this.mWindow = window;
		}

		#endregion

		#region apply

		public Boolean TryApplyMica (
		) {
			if (MicaController.IsSupported()) {
				this.mWindowsSystemDispatcherQueueHelper = new WindowsSystemDispatcherQueueHelper();
				this.mWindowsSystemDispatcherQueueHelper.EnsureWindowsSystemDispatcherQueueController();
				this.mSystemBackdropConfiguration = new SystemBackdropConfiguration();
				this.mSystemBackdropConfiguration.IsInputActive = true;
				this.mMicaController = new MicaController();
				this.mMicaController.AddSystemBackdropTarget(this.mWindow.As<ICompositionSupportsSystemBackdrop>());
				this.mMicaController.SetSystemBackdropConfiguration(this.mSystemBackdropConfiguration);
				this.mWindow.Activated += this.Window_Activated;
				this.mWindow.Closed += this.Window_Closed;
				((FrameworkElement)this.mWindow.Content).ActualThemeChanged += this.Window_ThemeChanged;
				this.SetConfigurationSourceTheme();
				return true;
			} else {
				return false;
			}
		}

		private void SetConfigurationSourceTheme (
		) {
			if (this.mSystemBackdropConfiguration != null) {
				this.mSystemBackdropConfiguration.Theme = ((FrameworkElement)this.mWindow.Content).ActualTheme switch {
					ElementTheme.Dark => SystemBackdropTheme.Dark,
					ElementTheme.Light => SystemBackdropTheme.Light,
					ElementTheme.Default => SystemBackdropTheme.Default,
					_ => throw new ArgumentOutOfRangeException(),
				};
			}
			return;
		}

		// ----------------

		private void Window_Activated (
			Object                   sender,
			WindowActivatedEventArgs args
		) {
			if (this.mSystemBackdropConfiguration != null) {
				this.mSystemBackdropConfiguration.IsInputActive = args.WindowActivationState != WindowActivationState.Deactivated;
			}
			return;
		}

		private void Window_Closed (
			Object          sender,
			WindowEventArgs args
		) {
			if (this.mMicaController != null) {
				this.mMicaController.Dispose();
				this.mMicaController = null;
			}
			this.mWindow.Activated -= this.Window_Activated;
			this.mSystemBackdropConfiguration = null;
			return;
		}

		private void Window_ThemeChanged (
			FrameworkElement sender,
			Object           args
		) {
			if (this.mSystemBackdropConfiguration != null) {
				this.SetConfigurationSourceTheme();
			}
			return;
		}

		#endregion

	}

}
