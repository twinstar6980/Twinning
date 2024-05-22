#pragma warning disable 0,
// ReSharper disable CheckNamespace

using AssistantPlus;
using AssistantPlus.Utility;
using Microsoft.UI.Xaml.Media;
using Microsoft.UI.Xaml.Media.Animation;
using Microsoft.UI.Xaml.Media.Imaging;

namespace AssistantPlus {

	public static class GameAnimationHelper {

		#region utility

		public static String ParseImageFileName (
			String value
		) {
			var result = value;
			var indexA1 = result.IndexOf('(');
			var indexA2 = result.IndexOf(')');
			if (indexA1 != -1 || indexA2 != -1) {
				GF.AssertTest(indexA1 != -1 && indexA2 != -1 && indexA1 < indexA2);
				result = result[..indexA1] + result[(indexA2 + 1)..];
			}
			var indexB1 = result.IndexOf('$');
			if (indexB1 != -1) {
				GF.AssertTest(indexA1 == -1 && indexA2 == -1);
				result = result[(indexB1 + 1)..];
			}
			var indexC1 = result.IndexOf('[');
			var indexC2 = result.IndexOf(']');
			if (indexC1 != -1 || indexC2 != -1) {
				GF.AssertTest(indexC1 != -1 && indexC2 != -1 && indexC1 < indexC2);
				result = result[..indexC1] + result[(indexC2 + 1)..];
			}
			var indexD1 = result.IndexOf('|');
			if (indexD1 != -1) {
				result = result[..indexD1];
			}
			return result;
		}

		// ----------------

		public static GameAnimationModel.Sprite SelectSprite (
			GameAnimationModel.Animation animation,
			Size                         index
		) {
			var result = default(GameAnimationModel.Sprite);
			if (0 <= index && index < animation.Sprite.Count) {
				result = animation.Sprite[index];
			}
			else if (index == animation.Sprite.Count) {
				result = animation.MainSprite.AsNotNull();
			}
			else {
				throw new ();
			}
			return result;
		}

		#endregion

		#region create

		public const Floater BasicOffset = 0.001;

		// ----------------

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

		private static Matrix MakeTransformMatrixFromVariant (
			List<Floater> source
		) {
			var result = new Matrix();
			switch (source.Count) {
				case 2: {
					result.M11 = 1.0;
					result.M12 = 0.0;
					result.M21 = 0.0;
					result.M22 = 1.0;
					result.OffsetX = source[0];
					result.OffsetY = source[1];
					break;
				}
				case 3: {
					var cosValue = Math.Cos(source[0]);
					var sinValue = Math.Sin(source[0]);
					result.M11 = cosValue;
					result.M12 = sinValue;
					result.M21 = -sinValue;
					result.M22 = cosValue;
					result.OffsetX = source[1];
					result.OffsetY = source[2];
					break;
				}
				case 6: {
					result.M11 = source[0];
					result.M12 = source[1];
					result.M21 = source[2];
					result.M22 = source[3];
					result.OffsetX = source[4];
					result.OffsetY = source[5];
					break;
				}
				default: throw new ();
			}
			return result;
		}

		public static SpriteUI CreateUI (
			GameAnimationModel.Animation animation,
			List<BitmapSource?>          imageSource,
			List<Boolean>                imageFilter,
			List<Boolean>                spriteFilter,
			Size                         workingSpriteIndex
		) {
			var sprite = GameAnimationHelper.SelectSprite(animation, workingSpriteIndex);
			var ui = new SpriteUI() {
				Canvas = new () { },
				Storyboard = new () { },
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
						layer.Canvas = new () {
							Children = {
								new Image() {
									Source = resourceSource,
									Width = resource.Size?[0] ?? resourceSource?.PixelWidth ?? 0,
									Height = resource.Size?[1] ?? resourceSource?.PixelHeight ?? 0,
									RenderTransform = new MatrixTransform() {
										Matrix = GameAnimationHelper.MakeTransformMatrixFromVariant(resource.Transform),
									},
								},
							},
						};
					}
					else {
						var resource = animation.Sprite[(Size)append.Resource];
						var resourceUI = GameAnimationHelper.CreateUI(animation, imageSource, imageFilter, spriteFilter, (Size)append.Resource);
						layer.Canvas = resourceUI.Canvas;
						ui.Storyboard.Children.Add(resourceUI.Storyboard);
					}
					Canvas.SetZIndex(layer.Canvas, (Size)append.Index);
					layer.Canvas.Visibility = Visibility.Collapsed;
					layer.Canvas.RenderTransform = new MatrixTransform() {
						Matrix = new (0.0, 0.0, 0.0, 0.0, 0.0, 0.0),
					};
					layer.Canvas.Opacity = 0.0;
					layer.Visibility = new () { };
					layer.Transform = new () { };
					layer.ColorAlpha = new () { };
					Storyboard.SetTargetProperty(layer.Visibility, "Visibility");
					Storyboard.SetTargetProperty(layer.Transform, "(Canvas.RenderTransform).(MatrixTransform.Matrix)");
					Storyboard.SetTargetProperty(layer.ColorAlpha, "Opacity");
					layer.Visibility.KeyFrames.Add(
						new DiscreteObjectKeyFrame() {
							KeyTime = keyTime,
							Value = Visibility.Visible,
						}
					);
					layer.Storyboard = new () {
						Duration = new (TimeSpan.FromSeconds(duration)),
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
					var transform = GameAnimationHelper.MakeTransformMatrixFromVariant(change.Transform);
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
					}
					else if (layer.IsFirst) {
						layer.ColorAlpha.KeyFrames.Add(
							new DiscreteObjectKeyFrame() {
								KeyTime = keyTime,
								Value = 1.0,
							}
						);
					}
					layer.IsFirst = false;
				}
				frameIndex++;
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

		#region load

		public static async Task<GameAnimationModel.Animation> LoadAnimation (
			String file
		) {
			return await JsonHelper.DeserializeFile<GameAnimationModel.Animation>(file);
		}

		public static async Task<List<BitmapSource?>> LoadImageSource (
			String                       directory,
			GameAnimationModel.Animation animation
		) {
			var imageSourceList = new List<BitmapSource?>(animation.Image.Count);
			foreach (var image in animation.Image) {
				var file = $"{directory}/{GameAnimationHelper.ParseImageFileName(image.Name)}.png";
				var source = default(BitmapSource);
				if (StorageHelper.ExistFile(file)) {
					source = await ConvertHelper.ParseBitmapFromBinary(await StorageHelper.ReadFile(file));
				}
				imageSourceList.Add(source);
			}
			return imageSourceList;
		}

		#endregion

	}

}
