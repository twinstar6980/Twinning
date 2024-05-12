#pragma warning disable 0,
// ReSharper disable

using AssistantPlus;
using AssistantPlus.Utility;

namespace AssistantPlus.View.AnimationViewer {

	public sealed partial class SettingPanel : CustomControl {

		#region life

		public SettingPanel (
		) {
			this.InitializeComponent();
			this.Controller = new () { View = this };
		}

		// ----------------

		private SettingPanelController Controller { get; }

		// ----------------

		protected override void StampUpdate (
		) {
			this.Controller.Update();
			return;
		}

		#endregion

		#region property

		public static readonly DependencyProperty DataProperty = DependencyProperty.Register(
			nameof(SettingPanel.Data),
			typeof(Setting),
			typeof(SettingPanel),
			new (new Setting() {
				ImmediateSelect = false,
				AutomaticPlay = false,
				RepeatPlay = false,
				RemainFrameRate = false,
				ShowSpriteBoundary = false,
				ImageFilterRule = "",
				SpriteFilterRule = "",
			})
		);

		public Setting Data {
			get => this.GetValue(SettingPanel.DataProperty).AsClass<Setting>();
			set => this.SetValue(SettingPanel.DataProperty, value);
		}

		#endregion

	}

	public class SettingPanelController : CustomController {

		#region data

		public SettingPanel View { get; init; } = default!;

		// ----------------

		public Setting Data => this.View.Data;

		#endregion

		#region update

		public async void Update (
		) {
			this.NotifyPropertyChanged(
				nameof(this.uImmediateSelectToggle_IsChecked),
				nameof(this.uAutomaticPlayToggle_IsChecked),
				nameof(this.uRepeatPlayToggle_IsChecked),
				nameof(this.uRemainFrameRateToggle_IsChecked),
				nameof(this.uShowSpriteBoundaryToggle_IsChecked),
				nameof(this.uImageFilterRuleText_Text),
				nameof(this.uSpriteFilterRuleText_Text)
			);
			return;
		}

		#endregion

		#region immediate select

		public Boolean uImmediateSelectToggle_IsChecked {
			get {
				return this.Data.ImmediateSelect;
			}
		}

		public async void uImmediateSelectToggle_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<ToggleButton>();
			this.Data.ImmediateSelect = senders.IsChecked.AsNotNull();
			return;
		}

		#endregion

		#region automatic play

		public Boolean uAutomaticPlayToggle_IsChecked {
			get {
				return this.Data.AutomaticPlay;
			}
		}

		public async void uAutomaticPlayToggle_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<ToggleButton>();
			this.Data.AutomaticPlay = senders.IsChecked.AsNotNull();
			return;
		}

		#endregion

		#region repeat play

		public Boolean uRepeatPlayToggle_IsChecked {
			get {
				return this.Data.RepeatPlay;
			}
		}

		public async void uRepeatPlayToggle_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<ToggleButton>();
			this.Data.RepeatPlay = senders.IsChecked.AsNotNull();
			return;
		}

		#endregion

		#region remain frame rate

		public Boolean uRemainFrameRateToggle_IsChecked {
			get {
				return this.Data.RemainFrameRate;
			}
		}

		public async void uRemainFrameRateToggle_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<ToggleButton>();
			this.Data.RemainFrameRate = senders.IsChecked.AsNotNull();
			return;
		}

		#endregion

		#region show sprite boundary

		public Boolean uShowSpriteBoundaryToggle_IsChecked {
			get {
				return this.Data.ShowSpriteBoundary;
			}
		}

		public async void uShowSpriteBoundaryToggle_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<ToggleButton>();
			this.Data.ShowSpriteBoundary = senders.IsChecked.AsNotNull();
			return;
		}

		#endregion

		#region image filter rule

		public async void uImageFilterRuleText_LostFocus (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<TextBox>();
			this.Data.ImageFilterRule = senders.Text;
			this.NotifyPropertyChanged(
				nameof(this.uImageFilterRuleText_Text)
			);
			return;
		}

		public String uImageFilterRuleText_Text {
			get {
				return this.Data.ImageFilterRule;
			}
		}

		#endregion

		#region sprite filter rule

		public async void uSpriteFilterRuleText_LostFocus (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<TextBox>();
			this.Data.SpriteFilterRule = senders.Text;
			this.NotifyPropertyChanged(
				nameof(this.uSpriteFilterRuleText_Text)
			);
			return;
		}

		public String uSpriteFilterRuleText_Text {
			get {
				return this.Data.SpriteFilterRule;
			}
		}

		#endregion

	}

}
