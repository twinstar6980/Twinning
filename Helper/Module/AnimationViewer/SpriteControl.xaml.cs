#pragma warning disable 0,
// ReSharper disable

using Helper;
using Helper.Utility;
using Microsoft.UI.Xaml.Media;
using Microsoft.UI.Xaml.Media.Animation;
using Microsoft.UI.Xaml.Media.Imaging;
using RepeatBehavior = Microsoft.UI.Xaml.Media.Animation.RepeatBehavior;

namespace Helper.Module.AnimationViewer {

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
				return TimeSpan.FromSeconds(AnimationHelper.BasicOffset);
			}
		}

		public TimeSpan BasicTimeOffset {
			get {
				return TimeSpan.FromSeconds(AnimationHelper.BasicOffset / this.Speed);
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

		private void Storyboard_OnCompleted (
			Object? sender,
			Object  e
		) {
			Debug.Assert(this.Loaded);
			this.State = StateType.Idle;
			if (this.Repeat) {
				this.State = StateType.Playing;
			}
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
			AnimationModel.Animation animation,
			List<BitmapSource?>      imageSource,
			List<Boolean>            imageFilter,
			List<Boolean>            spriteFilter,
			Size                     workingSpriteIndex
		) {
			Debug.Assert(!this.Loaded);
			var workingSprite = AnimationHelper.SelectSprite(animation, workingSpriteIndex);
			var ui = AnimationHelper.CreateUI(animation, imageSource, imageFilter, spriteFilter, workingSpriteIndex);
			this.Width = animation.Size[0];
			this.Height = animation.Size[1];
			this.Padding = new Thickness(animation.Position[0], animation.Position[1], 0.0, 0.0);
			this.Canvas = ui.Canvas;
			this.Storyboard = ui.Storyboard;
			this.Storyboard.RepeatBehavior = new RepeatBehavior(1.0);
			this.Storyboard.Completed += this.Storyboard_OnCompleted;
			this.FrameRange = new AnimationHelper.FrameRange() {
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
			Debug.Assert(this.Loaded);
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
				Debug.Assert(this.Loaded);
				return this.mState;
			}
			set {
				Debug.Assert(this.Loaded);
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
							} else {
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
						default: throw new ArgumentOutOfRangeException(nameof(value), value, null);
					}
				}
				this.mState = value;
				return;
			}
		}

		#endregion

		#region attribute

		private AnimationHelper.FrameRange? mFrameRange = null;

		public AnimationHelper.FrameRange FrameRange {
			get {
				Debug.Assert(this.Loaded);
				return this.mFrameRange!;
			}
			set {
				Debug.Assert(this.Loaded);
				this.Storyboard.BeginTime = -TimeSpan.FromSeconds(value.Start) / this.Speed;
				this.Storyboard.Duration = new Duration(TimeSpan.FromSeconds(value.Start + value.Duration) - this.BasicTimeOffsetValue);
				this.mFrameRange = value;
				return;
			}
		}

		// ----------------

		public TimeSpan CurrentTime {
			get {
				Debug.Assert(this.Loaded);
				Debug.Assert(this.State != StateType.Idle);
				return this.Storyboard.GetCurrentTime();
			}
			set {
				Debug.Assert(this.Loaded);
				Debug.Assert(this.State != StateType.Idle);
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
					var timeBeforeChange = (TimeSpan?)null;
					if (this.State != StateType.Idle) {
						timeBeforeChange = this.CurrentTime;
					}
					this.Storyboard.SpeedRatio = value;
					this.Storyboard.BeginTime = -TimeSpan.FromSeconds(this.FrameRange.Start) / value;
					if (this.State != StateType.Idle) {
						this.Storyboard.Seek((-TimeSpan.FromSeconds(this.FrameRange.Start) + timeBeforeChange!.Value) / value + this.BasicTimeOffsetValue / value);
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
						this.Canvas.Background = (AcrylicBrush)Application.Current.Resources["AcrylicInAppFillColorDefaultBrush"];
					} else {
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
