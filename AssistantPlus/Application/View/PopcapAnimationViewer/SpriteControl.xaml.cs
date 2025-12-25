#pragma warning disable 0,
// ReSharper disable

using Twinning.AssistantPlus;
using Twinning.AssistantPlus.Utility;
using Microsoft.UI.Xaml.Media.Animation;
using Microsoft.UI.Xaml.Media.Imaging;

namespace Twinning.AssistantPlus.View.PopcapAnimationViewer {

	public sealed partial class SpriteControl : UserControl {

		#region life

		public SpriteControl (
		) {
			this.InitializeComponent();
			return;
		}

		#endregion

		#region time offset

		public TimeSpan BasicTimeOffsetValue {
			get {
				return TimeSpan.FromSeconds(PopcapAnimationHelper.BasicOffset);
			}
		}

		public TimeSpan BasicTimeOffset {
			get {
				return TimeSpan.FromSeconds(PopcapAnimationHelper.BasicOffset / this.FrameSpeed);
			}
		}

		#endregion

		#region element

		public Canvas? Canvas { get; private set; } = null;

		public Storyboard? Storyboard { get; private set; } = null;

		private void Storyboard_Completed (
			Object? sender,
			Object  args
		) {
			AssertTest(this.Loaded);
			this.State = StateType.Idle;
			if (this.RepeatPlay) {
				this.State = StateType.Playing;
			}
			return;
		}

		#endregion

		#region load

		[MemberNotNullWhen(true, nameof(SpriteControl.Canvas))]
		[MemberNotNullWhen(true, nameof(SpriteControl.Storyboard))]
		public new Boolean Loaded {
			get {
				return this.Canvas != null;
			}
		}

		// ----------------

		public void Load (
			PopcapAnimationModel.Animation     animation,
			Dictionary<String, BitmapSource> texture,
			List<Boolean>                    imageFilter,
			List<Boolean>                    spriteFilter,
			PopcapAnimationModel.Sprite        activeSprite
		) {
			AssertTest(!this.Loaded);
			var visual = PopcapAnimationHelper.VisualizeSprite(animation, texture, activeSprite, imageFilter, spriteFilter);
			this.Padding = new (animation.Position.Item1, animation.Position.Item2, 0.0, 0.0);
			this.Canvas = visual.Canvas;
			this.Storyboard = visual.Storyboard;
			this.Storyboard.RepeatBehavior = new (1.0);
			this.Storyboard.Completed += this.Storyboard_Completed;
			this.FrameRange = new () {
				Begin = 0,
				End = activeSprite.Frame.Count - 1,
			};
			this.HoldEnd = this.HoldEnd;
			this.RepeatPlay = this.RepeatPlay;
			this.FrameSpeed = this.FrameSpeed;
			this.ShowBoundary = this.ShowBoundary;
			this.uCanvas.Content = this.Canvas;
			this.State = StateType.Idle;
			return;
		}

		public void Unload (
		) {
			AssertTest(this.Loaded);
			this.State = StateType.Idle;
			this.uCanvas.Content = null;
			this.Canvas = null;
			this.Storyboard = null;
			this.mFrameRange = null;
			return;
		}

		#endregion

		#region state

		public enum StateType {
			Idle,
			Playing,
			Paused,
		}

		private StateType mState = StateType.Idle;

		public StateType State {
			get {
				AssertTest(this.Loaded);
				return this.mState;
			}
			set {
				AssertTest(this.Loaded);
				if (this.mState != value) {
					switch (value) {
						case StateType.Idle: {
							this.Storyboard.Stop();
							break;
						}
						case StateType.Playing: {
							if (this.mState == StateType.Idle) {
								this.Storyboard.Stop();
								this.Storyboard.Begin();
								this.Storyboard.Seek(this.BasicTimeOffset);
							}
							else {
								this.Storyboard.Resume();
							}
							break;
						}
						case StateType.Paused: {
							if (this.mState == StateType.Idle) {
								this.Storyboard.Stop();
								this.Storyboard.Begin();
								this.Storyboard.Seek(this.BasicTimeOffset);
							}
							this.Storyboard.Pause();
							break;
						}
						default: throw new UnreachableException();
					}
				}
				this.mState = value;
				return;
			}
		}

		#endregion

		#region attribute

		private PopcapAnimationHelper.FrameRange? mFrameRange = null;

		public PopcapAnimationHelper.FrameRange FrameRange {
			get {
				AssertTest(this.Loaded);
				return this.mFrameRange.AsNotNull();
			}
			set {
				AssertTest(this.Loaded);
				this.Storyboard.BeginTime = -TimeSpan.FromSeconds(value.Begin) / this.FrameSpeed;
				this.Storyboard.Duration = new (TimeSpan.FromSeconds(value.End + 1) - this.BasicTimeOffsetValue);
				this.mFrameRange = value;
				return;
			}
		}

		// ----------------

		public TimeSpan CurrentTime {
			get {
				AssertTest(this.Loaded);
				AssertTest(this.State != StateType.Idle);
				return this.Storyboard.GetCurrentTime();
			}
			set {
				AssertTest(this.Loaded);
				AssertTest(this.State != StateType.Idle);
				this.Storyboard.Seek((-TimeSpan.FromSeconds(this.FrameRange.Begin) + value) / this.FrameSpeed + this.BasicTimeOffset);
				return;
			}
		}

		// ----------------

		private Boolean mRepeatPlay = false;

		public Boolean RepeatPlay {
			get {
				return this.mRepeatPlay;
			}
			set {
				this.mRepeatPlay = value;
				return;
			}
		}

		// ----------------

		private Boolean mHoldEnd = false;

		public Boolean HoldEnd {
			get {
				return this.mHoldEnd;
			}
			set {
				if (this.Loaded) {
					this.Storyboard.FillBehavior = !value ? FillBehavior.Stop : FillBehavior.HoldEnd;
				}
				this.mHoldEnd = value;
				return;
			}
		}

		// ----------------

		private Floater mFrameSpeed = 1.0;

		public Floater FrameSpeed {
			get {
				return this.mFrameSpeed;
			}
			set {
				if (this.Loaded) {
					var timeBeforeChange = default(TimeSpan?);
					if (this.State != StateType.Idle) {
						timeBeforeChange = this.CurrentTime;
					}
					this.Storyboard.SpeedRatio = value;
					this.Storyboard.BeginTime = -TimeSpan.FromSeconds(this.FrameRange.Begin) / value;
					if (this.State != StateType.Idle) {
						this.Storyboard.Seek((-TimeSpan.FromSeconds(this.FrameRange.Begin) + timeBeforeChange.AsNotNull()) / value + this.BasicTimeOffsetValue / value);
					}
				}
				this.mFrameSpeed = value;
				return;
			}
		}

		// ----------------

		private Boolean mShowBoundary = false;

		public Boolean ShowBoundary {
			get {
				return this.mShowBoundary;
			}
			set {
				this.uBackground.Visibility = value ? Visibility.Visible : Visibility.Collapsed;
				this.mShowBoundary = value;
				return;
			}
		}

		#endregion

	}

}
