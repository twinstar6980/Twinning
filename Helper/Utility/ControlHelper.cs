#pragma warning disable 0,
// ReSharper disable

using Helper;

namespace Helper.Utility {

	public static class ControlHelper {

		#region utility

		public static async Task WaitUntilLoaded (
			FrameworkElement target,
			Size             delay = 40
		) {
			while (!target.IsLoaded) {
				await Task.Delay(delay);
			}
			return;
		}

		#endregion

		#region dialog

		private static List<ContentDialog> CurrentDialog { get; } = new List<ContentDialog>();

		private static async Task PushDialog (
			ContentDialog item
		) {
			ControlHelper.CurrentDialog.Add(item);
			if (ControlHelper.CurrentDialog.Count > 1) {
				ControlHelper.CurrentDialog[^2].Hide();
			}
			var semaphore = new SemaphoreSlim(0, 1);
			var index = ControlHelper.CurrentDialog.Count;
			item.Closed += (_, _) => {
				if (ControlHelper.CurrentDialog.Count == index) {
					semaphore.Release();
					ControlHelper.CurrentDialog.RemoveAt(index - 1);
					if (index > 1) {
						_ = ControlHelper.CurrentDialog[^1].ShowAsync();
					}
				}
			};
			_ = item.ShowAsync();
			await semaphore.WaitAsync();
			return;
		}

		// ----------------

		public static async Task ShowDialogSimple (
			UIElement root,
			String    title,
			Object?   content
		) {
			var dialog = new ContentDialog() {
				XamlRoot = root.XamlRoot,
				Title = title,
				Content = content,
				CloseButtonText = "Close",
				DefaultButton = ContentDialogButton.Close,
			};
			await ControlHelper.PushDialog(dialog);
			return;
		}

		public static async Task ShowDialogFixedSize (
			UIElement root,
			String    title,
			Object?   content
		) {
			var dialog = new ContentDialog() {
				XamlRoot = root.XamlRoot,
				Resources = new ResourceDictionary() {
					new KeyValuePair<Object, Object>("ContentDialogMinWidth", 720.0),
					new KeyValuePair<Object, Object>("ContentDialogMaxWidth", 720.0),
					new KeyValuePair<Object, Object>("ContentDialogMinHeight", 640.0),
					new KeyValuePair<Object, Object>("ContentDialogMaxHeight", 640.0),
				},
				Title = title,
				Content = new ScrollViewer() {
					IsTabStop = true,
					Padding = new Thickness(12, 0, 12, 0),
					VerticalScrollMode = ScrollMode.Enabled,
					VerticalScrollBarVisibility = ScrollBarVisibility.Visible,
					Content = content,
				},
				CloseButtonText = "Close",
				DefaultButton = ContentDialogButton.Close,
			};
			await ControlHelper.PushDialog(dialog);
			return;
		}

		// ----------------

		public static Action ShowDialogForWaiting (
			UIElement root
		) {
			var dialog = new ContentDialog() {
				XamlRoot = root.XamlRoot,
				Title = "Waiting ...",
				Content = new ProgressBar() {
					HorizontalAlignment = HorizontalAlignment.Stretch,
					IsIndeterminate = true,
				},
				CloseButtonText = "Force Close",
				DefaultButton = ContentDialogButton.None,
			};
			_ = ControlHelper.PushDialog(dialog);
			return () => { dialog.Hide(); };
		}

		#endregion

	}

}
