#pragma warning disable 0,
// ReSharper disable

using Twinning.AssistantPlus;
using Microsoft.Windows.AppNotifications;
using Microsoft.Windows.AppNotifications.Builder;

namespace Twinning.AssistantPlus.Utility {

	public static class NotificationHelper {

		#region utility

		private static Boolean Initialized { get; set; } = false;

		private static Action? Handler { get; set; } = null;

		// ----------------

		public static void Initialize (
		) {
			AssertTest(!NotificationHelper.Initialized);
			if (!AppNotificationManager.IsSupported()) {
				NotificationHelper.Initialized = true;
				return;
			}
			AppNotificationManager.Default.NotificationInvoked += async (_, _) => {
				NotificationHelper.Handler?.Invoke();
				return;
			};
			{
				AppNotificationManager.Default.Register();
			}
			AppDomain.CurrentDomain.ProcessExit += async (_, _) => {
				AppNotificationManager.Default.Unregister();
				return;
			};
			NotificationHelper.Initialized = true;
			return;
		}

		public static void Listen (
			Action? handler
		) {
			AssertTest(NotificationHelper.Initialized);
			NotificationHelper.Handler = handler;
			return;
		}

		// ----------------

		public static void Push (
			String title,
			String description
		) {
			AssertTest(NotificationHelper.Initialized);
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
