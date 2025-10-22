#pragma warning disable 0,
// ReSharper disable

using Twinning.AssistantPlus;
using Microsoft.Windows.AppNotifications;
using Microsoft.Windows.AppNotifications.Builder;

namespace Twinning.AssistantPlus.Utility {

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
			String title,
			String description
		) {
			if (!AppNotificationManager.IsSupported()) {
				return;
			}
			var notification = new AppNotificationBuilder()
				.AddText(title)
				.AddText(description)
				.BuildNotification();
			AppNotificationManager.Default.Show(notification);
			return;
		}

		#endregion

	}

}
