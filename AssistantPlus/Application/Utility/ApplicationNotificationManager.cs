#pragma warning disable 0,
// ReSharper disable

using Twinning.AssistantPlus;
using Microsoft.Windows.AppNotifications;
using Microsoft.Windows.AppNotifications.Builder;

namespace Twinning.AssistantPlus.Utility {

	public class ApplicationNotificationManager {

		#region singleton

		public static readonly ApplicationNotificationManager Instance = new ();

		#endregion

		#region constructor

		private Boolean mInitialized;

		private Func<Task>? mHandler;

		// ----------------

		private ApplicationNotificationManager(
		) {
			this.mInitialized = false;
			this.mHandler = null;
			return;
		}

		#endregion

		#region utility

		public async Task Initialize(
		) {
			AssertTest(!this.mInitialized);
			if (!AppNotificationManager.IsSupported()) {
				this.mInitialized = true;
				return;
			}
			AppNotificationManager.Default.NotificationInvoked += async (_, _) => {
				this.mHandler?.Invoke();
				return;
			};
			{
				AppNotificationManager.Default.Register();
			}
			AppDomain.CurrentDomain.ProcessExit += async (_, _) => {
				AppNotificationManager.Default.Unregister();
				return;
			};
			this.mInitialized = true;
			return;
		}

		public async Task Listen(
			Func<Task>? handler
		) {
			AssertTest(this.mInitialized);
			this.mHandler = handler;
			return;
		}

		// ----------------

		public async Task Push(
			String title,
			String description
		) {
			AssertTest(this.mInitialized);
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
