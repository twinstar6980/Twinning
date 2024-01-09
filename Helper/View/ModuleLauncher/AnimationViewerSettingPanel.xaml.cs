#pragma warning disable 0,
// ReSharper disable

using Helper;
using Helper.Utility;

namespace Helper.View.ModuleLauncher {

	public sealed partial class AnimationViewerSettingPanel : CustomControl {

		#region life

		public AnimationViewerSettingPanel (
		) {
			this.InitializeComponent();
			this.Controller = new () { View = this };
		}

		// ----------------

		private AnimationViewerSettingPanelController Controller { get; }

		// ----------------

		protected override void StampUpdate (
		) {
			this.Controller.Update();
			return;
		}

		#endregion

		#region property

		#endregion

	}

	public class AnimationViewerSettingPanelController : CustomController {

		#region data

		public AnimationViewerSettingPanel View { get; init; } = default!;

		#endregion

		#region update

		public async void Update (
		) {
			this.NotifyPropertyChanged(
			);
			return;
		}

		#endregion

		#region immediate select

		public Boolean uImmediateSelectToggle_IsChecked {
			get {
				return Setting.Data.AnimationViewer.ImmediateSelect;
			}
		}

		public async void uImmediateSelectToggle_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<ToggleButton>();
			Setting.Data.AnimationViewer.ImmediateSelect = senders.IsChecked.AsNotNull();
			return;
		}

		#endregion

		#region automatic play

		public Boolean uAutomaticPlayToggle_IsChecked {
			get {
				return Setting.Data.AnimationViewer.AutomaticPlay;
			}
		}

		public async void uAutomaticPlayToggle_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<ToggleButton>();
			Setting.Data.AnimationViewer.AutomaticPlay = senders.IsChecked.AsNotNull();
			return;
		}

		#endregion

		#region repeat play

		public Boolean uRepeatPlayToggle_IsChecked {
			get {
				return Setting.Data.AnimationViewer.RepeatPlay;
			}
		}

		public async void uRepeatPlayToggle_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<ToggleButton>();
			Setting.Data.AnimationViewer.RepeatPlay = senders.IsChecked.AsNotNull();
			return;
		}

		#endregion

		#region remain frame rate

		public Boolean uRemainFrameRateToggle_IsChecked {
			get {
				return Setting.Data.AnimationViewer.RemainFrameRate;
			}
		}

		public async void uRemainFrameRateToggle_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<ToggleButton>();
			Setting.Data.AnimationViewer.RemainFrameRate = senders.IsChecked.AsNotNull();
			return;
		}

		#endregion

		#region show sprite boundary

		public Boolean uShowSpriteBoundaryToggle_IsChecked {
			get {
				return Setting.Data.AnimationViewer.ShowSpriteBoundary;
			}
		}

		public async void uShowSpriteBoundaryToggle_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<ToggleButton>();
			Setting.Data.AnimationViewer.ShowSpriteBoundary = senders.IsChecked.AsNotNull();
			return;
		}

		#endregion

		#region image filter rule

		public String uImageFilterRuleText_Text {
			get {
				return Setting.Data.AnimationViewer.ImageFilterRule;
			}
		}

		public async void uImageFilterRuleText_TextChanged (
			Object               sender,
			TextChangedEventArgs args
		) {
			var senders = sender.AsClass<TextBox>();
			Setting.Data.AnimationViewer.ImageFilterRule = senders.Text;
			return;
		}

		#endregion

		#region sprite filter rule

		public String uSpriteFilterRuleText_Text {
			get {
				return Setting.Data.AnimationViewer.SpriteFilterRule;
			}
		}

		public async void uSpriteFilterRuleText_TextChanged (
			Object               sender,
			TextChangedEventArgs args
		) {
			var senders = sender.AsClass<TextBox>();
			Setting.Data.AnimationViewer.SpriteFilterRule = senders.Text;
			return;
		}

		#endregion

	}

}
