#pragma warning disable 0,
// ReSharper disable

using Helper;
using Microsoft.UI.Xaml.Media;
using Microsoft.UI.Xaml.Media.Animation;
using Microsoft.UI.Xaml.Media.Imaging;
using Newtonsoft.Json;

namespace Helper.Utility {

	public static class Animation {

		#region load from storage

		public static async Task<Model.Animation> LoadAnimation (
			String file
		) {
			var text = await Storage.LoadFileText(file);
			return JsonConvert.DeserializeObject<Model.Animation>(text)!;
		}

		public static async Task<List<ImageSource?>> LoadImage (
			String          directory,
			Model.Animation animation
		) {
			var imageSourceList = new List<ImageSource?>(animation.image.Count);
			foreach (var image in animation.image) {
				var file = $"{directory}/{image.name.Split('|')[0]}.png";
				var imageSource = (ImageSource?)null;
				if (File.Exists(file)) {
					imageSource = new BitmapImage(new Uri(file));
				}
				imageSourceList.Add(imageSource);
			}
			return imageSourceList;
		}

		#endregion

		#region utility

		public const Floating BasicOffset = 0.001;

		// ----------------

		public static Model.Sprite SelectSprite (
			Model.Animation animation,
			Size            index
		) {
			Debug.Assert(index >= 0);
			var result = (Model.Sprite?)null;
			if (index < animation.sprite.Count) {
				result = animation.sprite[index];
			} else if (index == animation.sprite.Count) {
				Debug.Assert(animation.main_sprite != null);
				result = animation.main_sprite;
			} else {
				throw new ArgumentException();
			}
			return result;
		}

		#endregion

		#region create ui

		public record SpriteUI {
			public required Canvas     Canvas;
			public required Storyboard Storyboard;
		}

		public record FrameRange {
			public required Size Start;
			public required Size Duration;
		}

		public record FrameRangeInformation : FrameRange {
			public required Size Count;
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
			Model.Animation    animation,
			List<ImageSource?> imageSource,
			List<Boolean>      imageFilter,
			List<Boolean>      spriteFilter,
			Size               workingSpriteIndex
		) {
			var sprite = Animation.SelectSprite(animation, workingSpriteIndex);
			var ui = new SpriteUI() {
				Canvas = new Canvas(),
				Storyboard = new Storyboard(),
			};
			var layerList = new Dictionary<Integer, WorkLayer?>();
			var duration = sprite.frame.Count;
			var frameIndex = 0;
			foreach (var frame in sprite.frame) {
				var keyTime = KeyTime.FromTimeSpan(TimeSpan.FromSeconds(frameIndex));
				foreach (var remove in frame.remove) {
					var layer = layerList[remove.index];
					if (layer == null) {
						continue;
					}
					layer.Visibility.KeyFrames.Add(
						new DiscreteObjectKeyFrame() {
							KeyTime = keyTime,
							Value = Visibility.Collapsed,
						}
					);
				}
				foreach (var append in frame.append) {
					if ((!append.sprite && !imageFilter[(Size)append.resource]) ||
						(append.sprite && !spriteFilter[(Size)append.resource])) {
						layerList.Add(append.index, null);
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
					if (!append.sprite) {
						var resource = animation.image[(Size)append.resource];
						var resourceSource = imageSource[(Size)append.resource];
						layer.Canvas = new Canvas() {
							Children = {
								new Image() {
									Source = resourceSource,
									Width = resource.size[0],
									Height = resource.size[1],
									RenderTransform = new MatrixTransform() {
										Matrix = Transform.ConvertFromStandard(resource.transform),
									},
								},
							},
						};
					} else {
						var resource = animation.sprite[(Size)append.resource];
						var resourceUI = Animation.CreateUI(
							animation,
							imageSource,
							imageFilter,
							spriteFilter,
							(Size)append.resource
						);
						layer.Canvas = resourceUI.Canvas;
						ui.Storyboard.Children.Add(resourceUI.Storyboard);
					}
					Canvas.SetZIndex(layer.Canvas, (Size)append.index);
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
					layerList.Add(append.index, layer);
					layer.IsFirst = true;
				}
				foreach (var change in frame.change) {
					var layer = layerList[(UInt32)change.index];
					if (layer == null) {
						continue;
					}
					var transform = Transform.ConvertFromVariant(change.transform);
					layer.Transform.KeyFrames.Add(
						new DiscreteObjectKeyFrame() {
							KeyTime = keyTime,
							Value = transform,
						}
					);
					if (change.color != null) {
						layer.ColorAlpha.KeyFrames.Add(
							new DiscreteObjectKeyFrame() {
								KeyTime = keyTime,
								Value = change.color[3],
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
				if (layer.Value == null) {
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
