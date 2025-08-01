#pragma warning disable 0,
// ReSharper disable

using AssistantPlus;

namespace AssistantPlus.Utility {

	public static class ControlHelper {

		#region common

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

		private static List<ContentDialog> CurrentDialog { get; } = [];

		private static async Task<ContentDialogResult> PushDialog (
			ContentDialog item
		) {
			ControlHelper.CurrentDialog.Add(item);
			if (ControlHelper.CurrentDialog.Count > 1) {
				ControlHelper.CurrentDialog[^2].Hide();
			}
			var semaphore = new SemaphoreSlim(0, 1);
			var result = ContentDialogResult.None;
			var index = ControlHelper.CurrentDialog.Count;
			item.Closed += (sender, args) => {
				if (ControlHelper.CurrentDialog.Count == index) {
					result = args.Result;
					semaphore.Release();
					ControlHelper.CurrentDialog.RemoveAt(index - 1);
					if (index > 1) {
						_ = ControlHelper.CurrentDialog[^1].ShowAsync().AsTask().SelfLet(App.Instance.WithTaskExceptionHandler);
					}
				}
				return;
			};
			_ = item.ShowAsync().AsTask().SelfLet(App.Instance.WithTaskExceptionHandler);
			await semaphore.WaitAsync();
			return result;
		}

		public static async Task IterateDialog (
			Func<ContentDialog, Task> action
		) {
			foreach (var item in ControlHelper.CurrentDialog) {
				await action(item);
			}
			return;
		}

		// ----------------

		public static async Task<ContentDialogResult> ShowDialogAsAutomatic (
			UIElement                         root,
			String                            title,
			Object?                           content,
			Tuple<String?, String?, String?>? action
		) {
			var dialog = new ContentDialog() {
				XamlRoot = root.XamlRoot,
				RequestedTheme = root.XamlRoot.Content.As<FrameworkElement>().RequestedTheme,
				Title = title,
				Content = content == null
					? null
					: new ScrollViewer() {
						HorizontalAlignment = HorizontalAlignment.Stretch,
						VerticalAlignment = VerticalAlignment.Stretch,
						Margin = new (-16, 0, -16, 0),
						Padding = new (16, 0, 16, 0),
						IsTabStop = true,
						HorizontalScrollMode = ScrollMode.Disabled,
						HorizontalScrollBarVisibility = ScrollBarVisibility.Disabled,
						VerticalScrollMode = ScrollMode.Enabled,
						VerticalScrollBarVisibility = ScrollBarVisibility.Visible,
						Content = content,
					},
				CloseButtonText = action == null ? "Close" : action.Item1,
				PrimaryButtonText = action?.Item2,
				SecondaryButtonText = action?.Item3,
				DefaultButton = action == null
					? ContentDialogButton.Close
					: action.Item2 != null
						? ContentDialogButton.Primary
						: action.Item3 != null
							? ContentDialogButton.Secondary
							: ContentDialogButton.Close,
			};
			return await ControlHelper.PushDialog(dialog);
		}

		public static async Task<ContentDialogResult> ShowDialogAsFixed (
			UIElement                         root,
			String                            title,
			Object?                           content,
			Tuple<String?, String?, String?>? action,
			Tuple<Size, Size>?                size = null
		) {
			var dialog = new ContentDialog() {
				XamlRoot = root.XamlRoot,
				RequestedTheme = root.XamlRoot.Content.As<FrameworkElement>().RequestedTheme,
				Title = title,
				Content = content == null
					? null
					: new ScrollViewer() {
						HorizontalAlignment = HorizontalAlignment.Stretch,
						VerticalAlignment = VerticalAlignment.Stretch,
						Margin = new (-16, 0, -16, 0),
						Padding = new (16, 0, 16, 0),
						IsTabStop = true,
						HorizontalScrollMode = ScrollMode.Disabled,
						HorizontalScrollBarVisibility = ScrollBarVisibility.Disabled,
						VerticalScrollMode = ScrollMode.Enabled,
						VerticalScrollBarVisibility = ScrollBarVisibility.Visible,
						Content = content,
					},
				CloseButtonText = action == null ? "Close" : action.Item1,
				PrimaryButtonText = action?.Item2,
				SecondaryButtonText = action?.Item3,
				DefaultButton = action == null
					? ContentDialogButton.Close
					: action.Item2 != null
						? ContentDialogButton.Primary
						: action.Item3 != null
							? ContentDialogButton.Secondary
							: ContentDialogButton.Close,
				Resources = [
					new ("ContentDialogMinWidth", size?.Item1 ?? 720.0),
					new ("ContentDialogMaxWidth", size?.Item1 ?? 720.0),
					new ("ContentDialogMinHeight", size?.Item2 ?? 640.0),
					new ("ContentDialogMaxHeight", size?.Item2 ?? 640.0),
				],
			};
			return await ControlHelper.PushDialog(dialog);
		}

		// ----------------

		public static async Task<Func<Task>> ShowDialogForWait (
			UIElement root
		) {
			var dialog = new ContentDialog() {
				XamlRoot = root.XamlRoot,
				RequestedTheme = root.XamlRoot.Content.As<FrameworkElement>().RequestedTheme,
				Title = "Waiting ...",
				Content = new ProgressBar() {
					HorizontalAlignment = HorizontalAlignment.Stretch,
					VerticalAlignment = VerticalAlignment.Center,
					IsIndeterminate = true,
				},
				CloseButtonText = "Hide",
				DefaultButton = ContentDialogButton.None,
			};
			var task = ControlHelper.PushDialog(dialog);
			return async () => {
				dialog.Hide();
				await task;
				return;
			};
		}

		public static async Task<Boolean> ShowDialogForConfirm (
			UIElement root,
			String?   title,
			Object?   content
		) {
			return await ControlHelper.ShowDialogAsAutomatic(root, title ?? "Confirm ?", content, new ("Cancel", "Continue", null)) == ContentDialogResult.Primary;
		}

		#endregion

	}

}
