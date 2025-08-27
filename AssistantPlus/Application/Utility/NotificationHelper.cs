#pragma warning disable 0,
// ReSharper disable

using AssistantPlus;
using Microsoft.Windows.AppNotifications;

namespace AssistantPlus.Utility {

	public static class NotificationHelper {

		#region utility

		public static void Initialize (
		) {
			if (!AppNotificationManager.IsSupported()) {
				return;
			}
			AppNotificationManager.Default.NotificationInvoked += (_, _) => {
				if (App.MainWindowIsInitialized) {
					WindowHelper.ShowAsForeground(App.MainWindow);
				}
				return;
			};
			{
				AppNotificationManager.Default.Register();
			}
			AppDomain.CurrentDomain.ProcessExit += (_, _) => {
				AppNotificationManager.Default.Unregister();
				return;
			};
			return;
		}

		public static void Push (
			AppNotification notification
		) {
			if (!AppNotificationManager.IsSupported()) {
				return;
			}
			AppNotificationManager.Default.Show(notification);
			return;
		}

		#endregion

	}

}
