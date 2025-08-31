#pragma warning disable 0,
// ReSharper disable

using AssistantPlus;
using Microsoft.Windows.AppNotifications;

namespace AssistantPlus.Utility {

	public static class NotificationHelper {

		#region utility

		public static void Initialize (
			Action handler
		) {
			if (!AppNotificationManager.IsSupported()) {
				return;
			}
			AppNotificationManager.Default.NotificationInvoked += async (_, _) => {
				handler();
				return;
			};
			{
				AppNotificationManager.Default.Register();
			}
			AppDomain.CurrentDomain.ProcessExit += async (_, _) => {
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
