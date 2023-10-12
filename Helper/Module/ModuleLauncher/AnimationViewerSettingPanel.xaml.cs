#pragma warning disable 0,
// ReSharper disable

using Helper;
using Helper.Utility;

namespace Helper.Module.ModuleLauncher {

	public sealed partial class AnimationViewerSettingPanel : UserControl {

		#region life

		public AnimationViewerSettingPanel (
		) {
			this.InitializeComponent();
			this.Controller = new AnimationViewerSettingPanelController() { View = this };
		}

		// ----------------

		public AnimationViewerSettingPanelController Controller { get; }

		#endregion

	}

	public class AnimationViewerSettingPanelController : CustomController {

		#region data

		public AnimationViewerSettingPanel View { get; init; } = default!;

		#endregion

		#region immediate select

		public Boolean uImmediateSelectToggle_IsChecked {
			get {
				return Setting.Data.AnimationViewer.ImmediateSelect;
			}
		}

		public async void uImmediateSelectToggle_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not ToggleButton senders) { return; }
			var newValue = senders.IsChecked!.Value;
			Setting.Data.AnimationViewer.ImmediateSelect = newValue;
			return;
		}

		#endregion

		#region automatic play

		public Boolean uAutomaticPlayToggle_IsChecked {
			get {
				return Setting.Data.AnimationViewer.AutomaticPlay;
			}
		}

		public async void uAutomaticPlayToggle_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not ToggleButton senders) { return; }
			var newValue = senders.IsChecked!.Value;
			Setting.Data.AnimationViewer.AutomaticPlay = newValue;
			return;
		}

		#endregion

		#region repeat play

		public Boolean uRepeatPlayToggle_IsChecked {
			get {
				return Setting.Data.AnimationViewer.RepeatPlay;
			}
		}

		public async void uRepeatPlayToggle_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not ToggleButton senders) { return; }
			var newValue = senders.IsChecked!.Value;
			Setting.Data.AnimationViewer.RepeatPlay = newValue;
			return;
		}

		#endregion

		#region remain frame rate

		public Boolean uRemainFrameRateToggle_IsChecked {
			get {
				return Setting.Data.AnimationViewer.RemainFrameRate;
			}
		}

		public async void uRemainFrameRateToggle_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not ToggleButton senders) { return; }
			var newValue = senders.IsChecked!.Value;
			Setting.Data.AnimationViewer.RemainFrameRate = newValue;
			return;
		}

		#endregion

		#region show sprite boundary

		public Boolean uShowSpriteBoundaryToggle_IsChecked {
			get {
				return Setting.Data.AnimationViewer.ShowSpriteBoundary;
			}
		}

		public async void uShowSpriteBoundaryToggle_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not ToggleButton senders) { return; }
			var newValue = senders.IsChecked!.Value;
			Setting.Data.AnimationViewer.ShowSpriteBoundary = newValue;
			return;
		}

		#endregion

		#region sprite filter rule

		public String uSpriteFilterRuleText_Text {
			get {
				return Setting.Data.AnimationViewer.SpriteFilterRule;
			}
		}

		public async void uSpriteFilterRuleText_OnTextChanged (
			Object               sender,
			TextChangedEventArgs args
		) {
			if (sender is not TextBox senders) { return; }
			Setting.Data.AnimationViewer.SpriteFilterRule = senders.Text;
			return;
		}

		#endregion

	}

}
