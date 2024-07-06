#pragma warning disable 0,
// ReSharper disable

using AssistantPlus;
using AssistantPlus.Utility;
using Microsoft.UI.Xaml.Media;
using Microsoft.UI.Xaml.Media.Animation;
using Microsoft.UI.Xaml.Media.Imaging;

namespace AssistantPlus.View.AnimationViewer {

	public sealed partial class SpriteControl : UserControl {

		#region life

		public SpriteControl (
		) {
			this.InitializeComponent();
		}

		#endregion

		#region time offset

		public TimeSpan BasicTimeOffsetValue {
			get {
				return TimeSpan.FromSeconds(GameAnimationHelper.BasicOffset);
			}
		}

		public TimeSpan BasicTimeOffset {
			get {
				return TimeSpan.FromSeconds(GameAnimationHelper.BasicOffset / this.Speed);
			}
		}

		#endregion

		#region element

		private Canvas? mCanvas = null;

		public Canvas? Canvas {
			get {
				return this.mCanvas;
			}
			private set {
				this.mCanvas = value;
				return;
			}
		}

		// ----------------

		private Storyboard? mStoryboard = null;

		public Storyboard? Storyboard {
			get {
				return this.mStoryboard;
			}
			private set {
				this.mStoryboard = value;
				return;
			}
		}

		private void Storyboard_Completed (
			Object? sender,
			Object  e
		) {
			GF.AssertTest(this.Loaded);
			this.State = StateType.Idle;
			if (this.Repeat) {
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
			GameAnimationModel.Animation animation,
			List<BitmapSource?>          imageSource,
			List<Boolean>                imageFilter,
			List<Boolean>                spriteFilter,
			Size                         workingSpriteIndex
		) {
			GF.AssertTest(!this.Loaded);
			var workingSprite = GameAnimationHelper.SelectSprite(animation, workingSpriteIndex);
			var visual = GameAnimationHelper.CreateVisual(animation, imageSource, imageFilter, spriteFilter, workingSpriteIndex);
			this.Width = animation.Size.Item1;
			this.Height = animation.Size.Item2;
			this.Padding = new (animation.Position.Item1, animation.Position.Item2, 0.0, 0.0);
			this.Canvas = visual.Canvas;
			this.Storyboard = visual.Storyboard;
			this.Storyboard.RepeatBehavior = new (1.0);
			this.Storyboard.Completed += this.Storyboard_Completed;
			this.FrameRange = new () {
				Start = 0,
				Duration = workingSprite.Frame.Count,
			};
			this.HoldEnd = this.HoldEnd;
			this.Repeat = this.Repeat;
			this.Speed = this.Speed;
			this.ShowBoundary = this.ShowBoundary;
			this.Content = this.Canvas;
			this.State = StateType.Idle;
			return;
		}

		public void Unload (
		) {
			GF.AssertTest(this.Loaded);
			this.State = StateType.Idle;
			this.Content = null;
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
				GF.AssertTest(this.Loaded);
				return this.mState;
			}
			set {
				GF.AssertTest(this.Loaded);
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
						default: throw new ();
					}
				}
				this.mState = value;
				return;
			}
		}

		#endregion

		#region attribute

		private GameAnimationHelper.FrameRange? mFrameRange = null;

		public GameAnimationHelper.FrameRange FrameRange {
			get {
				GF.AssertTest(this.Loaded);
				return this.mFrameRange.AsNotNull();
			}
			set {
				GF.AssertTest(this.Loaded);
				this.Storyboard.BeginTime = -TimeSpan.FromSeconds(value.Start) / this.Speed;
				this.Storyboard.Duration = new (TimeSpan.FromSeconds(value.Start + value.Duration) - this.BasicTimeOffsetValue);
				this.mFrameRange = value;
				return;
			}
		}

		// ----------------

		public TimeSpan CurrentTime {
			get {
				GF.AssertTest(this.Loaded);
				GF.AssertTest(this.State != StateType.Idle);
				return this.Storyboard.GetCurrentTime();
			}
			set {
				GF.AssertTest(this.Loaded);
				GF.AssertTest(this.State != StateType.Idle);
				this.Storyboard.Seek((-TimeSpan.FromSeconds(this.FrameRange.Start) + value) / this.Speed + this.BasicTimeOffset);
				return;
			}
		}

		// ----------------

		private Boolean mRepeat = false;

		public Boolean Repeat {
			get {
				return this.mRepeat;
			}
			set {
				this.mRepeat = value;
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

		private Floater mSpeed = 1.0;

		public Floater Speed {
			get {
				return this.mSpeed;
			}
			set {
				if (this.Loaded) {
					var timeBeforeChange = default(TimeSpan?);
					if (this.State != StateType.Idle) {
						timeBeforeChange = this.CurrentTime;
					}
					this.Storyboard.SpeedRatio = value;
					this.Storyboard.BeginTime = -TimeSpan.FromSeconds(this.FrameRange.Start) / value;
					if (this.State != StateType.Idle) {
						this.Storyboard.Seek((-TimeSpan.FromSeconds(this.FrameRange.Start) + timeBeforeChange.AsNotNull()) / value + this.BasicTimeOffsetValue / value);
					}
				}
				this.mSpeed = value;
				return;
			}
		}

		// ----------------

		private Boolean mShowBoundary;

		public Boolean ShowBoundary {
			get {
				return this.mShowBoundary;
			}
			set {
				if (this.Loaded) {
					if (value) {
						// TODO : incorrect if theme != system
						this.Canvas.Background = this.FindResource("CardBackgroundFillColorDefaultBrush").As<Brush>();
					}
					else {
						this.Canvas.Background = null;
					}
				}
				this.mShowBoundary = value;
				return;
			}
		}

		#endregion

	}

}
