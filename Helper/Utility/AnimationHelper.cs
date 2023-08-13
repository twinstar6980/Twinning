#pragma warning disable 0,
// ReSharper disable

using Helper;
using Microsoft.UI.Xaml.Media;
using Microsoft.UI.Xaml.Media.Animation;
using Microsoft.UI.Xaml.Media.Imaging;

namespace Helper.Utility {

	public static class AnimationHelper {

		#region load

		public static async Task<AnimationModel.Animation> LoadAnimation (
			String file
		) {
			var text = await StorageHelper.ReadFileText(file);
			return JsonHelper.Deserialize<AnimationModel.Animation>(text);
		}

		public static async Task<List<BitmapSource?>> LoadImageSource (
			String                   directory,
			AnimationModel.Animation animation
		) {
			var imageSourceList = new List<BitmapSource?>(animation.Image.Count);
			foreach (var image in animation.Image) {
				var file = $"{directory}/{image.Name.Split('|')[0]}.png";
				var source = default(BitmapSource);
				if (File.Exists(file)) {
					source = await StorageHelper.ReadFileImage(file);
				}
				imageSourceList.Add(source);
			}
			return imageSourceList;
		}

		#endregion

		#region utility

		public const Floater BasicOffset = 0.001;

		// ----------------

		public static AnimationModel.Sprite SelectSprite (
			AnimationModel.Animation animation,
			Size                     index
		) {
			var result = default(AnimationModel.Sprite);
			if (0 <= index && index < animation.Sprite.Count) {
				result = animation.Sprite[index];
			} else if (index == animation.Sprite.Count) {
				result = animation.MainSprite ?? throw new Exception();
			} else {
				throw new ArgumentException();
			}
			return result;
		}

		#endregion

		#region create

		public record SpriteUI {
			public required Canvas     Canvas;
			public required Storyboard Storyboard;
		}

		public record FrameRange {
			public required Size Start;
			public required Size Duration;
		}

		private record WorkLayer {
			public required Canvas                        Canvas;
			public required Storyboard                    Storyboard;
			public required ObjectAnimationUsingKeyFrames Visibility;
			public required ObjectAnimationUsingKeyFrames Transform;
			public required ObjectAnimationUsingKeyFrames ColorAlpha;
			public required Boolean                       IsFirst;
		}

		// ----------------

		public static SpriteUI CreateUI (
			AnimationModel.Animation animation,
			List<BitmapSource?>      imageSource,
			List<Boolean>            imageFilter,
			List<Boolean>            spriteFilter,
			Size                     workingSpriteIndex
		) {
			var sprite = AnimationHelper.SelectSprite(animation, workingSpriteIndex);
			var ui = new SpriteUI() {
				Canvas = new Canvas(),
				Storyboard = new Storyboard(),
			};
			var layerList = new Dictionary<Integer, WorkLayer?>();
			var duration = sprite.Frame.Count;
			var frameIndex = 0;
			foreach (var frame in sprite.Frame) {
				var keyTime = KeyTime.FromTimeSpan(TimeSpan.FromSeconds(frameIndex));
				foreach (var remove in frame.Remove) {
					var layer = layerList[remove.Index];
					if (layer is null) {
						continue;
					}
					layer.Visibility.KeyFrames.Add(
						new DiscreteObjectKeyFrame() {
							KeyTime = keyTime,
							Value = Visibility.Collapsed,
						}
					);
				}
				foreach (var append in frame.Append) {
					if ((!append.Sprite && !imageFilter[(Size)append.Resource]) || (append.Sprite && !spriteFilter[(Size)append.Resource])) {
						layerList.Add(append.Index, null);
						continue;
					}
					var layer = new WorkLayer() {
						Canvas = null!,
						Storyboard = null!,
						Visibility = null!,
						Transform = null!,
						ColorAlpha = null!,
						IsFirst = false,
					};
					if (!append.Sprite) {
						var resource = animation.Image[(Size)append.Resource];
						var resourceSource = imageSource[(Size)append.Resource];
						layer.Canvas = new Canvas() {
							Children = {
								new Image() {
									Source = resourceSource,
									Width = resource.Size[0],
									Height = resource.Size[1],
									RenderTransform = new MatrixTransform() {
										Matrix = TransformHelper.ConvertFromStandard(resource.Transform),
									},
								},
							},
						};
					} else {
						var resource = animation.Sprite[(Size)append.Resource];
						var resourceUI = AnimationHelper.CreateUI(
							animation,
							imageSource,
							imageFilter,
							spriteFilter,
							(Size)append.Resource
						);
						layer.Canvas = resourceUI.Canvas;
						ui.Storyboard.Children.Add(resourceUI.Storyboard);
					}
					Canvas.SetZIndex(layer.Canvas, (Size)append.Index);
					layer.Canvas.Visibility = Visibility.Collapsed;
					layer.Canvas.RenderTransform = new MatrixTransform() {
						Matrix = new Matrix(0.0, 0.0, 0.0, 0.0, 0.0, 0.0),
					};
					layer.Canvas.Opacity = 0.0;
					layer.Visibility = new ObjectAnimationUsingKeyFrames() { };
					layer.Transform = new ObjectAnimationUsingKeyFrames() { };
					layer.ColorAlpha = new ObjectAnimationUsingKeyFrames() { };
					Storyboard.SetTargetProperty(layer.Visibility, "Visibility");
					Storyboard.SetTargetProperty(layer.Transform, "(Canvas.RenderTransform).(MatrixTransform.Matrix)");
					Storyboard.SetTargetProperty(layer.ColorAlpha, "Opacity");
					layer.Visibility.KeyFrames.Add(
						new DiscreteObjectKeyFrame() {
							KeyTime = keyTime,
							Value = Visibility.Visible,
						}
					);
					layer.Storyboard = new Storyboard() {
						Duration = new Duration(TimeSpan.FromSeconds(duration)),
						RepeatBehavior = RepeatBehavior.Forever,
						Children = {
							layer.Visibility,
							layer.Transform,
							layer.ColorAlpha,
						},
					};
					Storyboard.SetTarget(layer.Storyboard, layer.Canvas);
					layerList.Add(append.Index, layer);
					layer.IsFirst = true;
				}
				foreach (var change in frame.Change) {
					var layer = layerList[change.Index];
					if (layer is null) {
						continue;
					}
					var transform = TransformHelper.ConvertFromVariant(change.Transform);
					layer.Transform.KeyFrames.Add(
						new DiscreteObjectKeyFrame() {
							KeyTime = keyTime,
							Value = transform,
						}
					);
					if (change.Color is not null) {
						layer.ColorAlpha.KeyFrames.Add(
							new DiscreteObjectKeyFrame() {
								KeyTime = keyTime,
								Value = change.Color[3],
							}
						);
					} else if (layer.IsFirst) {
						layer.ColorAlpha.KeyFrames.Add(
							new DiscreteObjectKeyFrame() {
								KeyTime = keyTime,
								Value = 1.0,
							}
						);
					}
					layer.IsFirst = false;
				}
				++frameIndex;
			}
			foreach (var layer in layerList) {
				if (layer.Value is null) {
					continue;
				}
				ui.Canvas.Children.Add(layer.Value.Canvas);
				ui.Storyboard.Children.Add(layer.Value.Storyboard);
			}
			return ui;
		}

		#endregion

	}

}
