#pragma warning disable 0,
// ReSharper disable

using Helper;
using Windows.Foundation;

namespace Helper.CustomControl {

	public class MStack : MPanel {

		#region measure & arrange

		protected override Windows.Foundation.Size MeasureWithoutPadding (
			Windows.Foundation.Size availableSize
		) {
			var desiredSize = new Windows.Foundation.Size(0.0, 0.0);
			var isHorizontal = this.Orientation == Orientation.Horizontal;
			var childSpacing = this.Spacing;
			var totalSpacing = childSpacing * Math.Max(0, this.Children.Count - 1);
			var totalWeight = (Floater)0.0;
			var perSpace = (Floater)0.0;
			if (isHorizontal) {
				desiredSize.Width += totalSpacing;
			} else {
				desiredSize.Height += totalSpacing;
			}
			foreach (var child in this.Children) {
				var childWeight = MStack.GetWeight(child);
				if (!Floater.IsNaN(childWeight)) {
					totalWeight += childWeight;
					continue;
				}
				if (isHorizontal) {
					child.Measure(new Windows.Foundation.Size(Math.Max(0.0, availableSize.Width - desiredSize.Width), availableSize.Height));
					desiredSize.Height = Math.Max(desiredSize.Height, child.DesiredSize.Height);
					desiredSize.Width += child.DesiredSize.Width;
				} else {
					child.Measure(new Windows.Foundation.Size(availableSize.Width, Math.Max(0.0, availableSize.Height - desiredSize.Height)));
					desiredSize.Width = Math.Max(desiredSize.Width, child.DesiredSize.Width);
					desiredSize.Height += child.DesiredSize.Height;
				}
				//Debug.WriteLine($"child is {childWeight} {child.DesiredSize} then {desiredSize}");
			}
			if (totalWeight != 0.0) {
				if (isHorizontal) {
					perSpace = Math.Max(0.0, availableSize.Width - desiredSize.Width) / totalWeight;
				} else {
					perSpace = Math.Max(0.0, availableSize.Height - desiredSize.Height) / totalWeight;
				}
			}
			//Debug.WriteLine($"per {perSpace}");
			foreach (var child in this.Children) {
				var childWeight = MStack.GetWeight(child);
				if (Floater.IsNaN(childWeight)) {
					continue;
				}
				var childSpace = perSpace * childWeight;
				if (isHorizontal) {
					child.Measure(new Windows.Foundation.Size(childSpace, availableSize.Height));
					desiredSize.Height = Math.Max(desiredSize.Height, child.DesiredSize.Height);
					desiredSize.Width += child.DesiredSize.Width;
				} else {
					child.Measure(new Windows.Foundation.Size(availableSize.Width, childSpace));
					desiredSize.Width = Math.Max(desiredSize.Width, child.DesiredSize.Width);
					desiredSize.Height += child.DesiredSize.Height;
				}
				//Debug.WriteLine($"child is {childWeight} {childSpace} {child.DesiredSize} then {reservedSize}");
			}
			//Debug.WriteLine($"available {availableSize} reserved {reservedSize} desired {desiredSize}");
			return new Windows.Foundation.Size(Math.Min(availableSize.Width, desiredSize.Width), Math.Min(availableSize.Height, desiredSize.Height));
		}

		protected override Windows.Foundation.Size ArrangeWithoutPadding (
			Windows.Foundation.Point finalOffset,
			Windows.Foundation.Size  finalSize
		) {
			//Debug.WriteLine($"final {finalSize}");
			var desiredSize = new Windows.Foundation.Size(0.0, 0.0);
			var isHorizontal = this.Orientation == Orientation.Horizontal;
			var childSpacing = this.Spacing;
			var totalSpacing = childSpacing * Math.Max(0, this.Children.Count - 1);
			var totalWeight = (Floater)0.0;
			var perSpace = (Floater)0.0;
			if (isHorizontal) {
				desiredSize.Width += totalSpacing;
			} else {
				desiredSize.Height += totalSpacing;
			}
			foreach (var child in this.Children) {
				var childWeight = MStack.GetWeight(child);
				if (!Floater.IsNaN(childWeight)) {
					totalWeight += childWeight;
					continue;
				}
				if (isHorizontal) {
					desiredSize.Width += child.DesiredSize.Width;
				} else {
					desiredSize.Height += child.DesiredSize.Height;
				}
			}
			if (totalWeight != 0.0) {
				if (isHorizontal) {
					perSpace = Math.Max(0.0, finalSize.Width - desiredSize.Width) / totalWeight;
				} else {
					perSpace = Math.Max(0.0, finalSize.Height - desiredSize.Height) / totalWeight;
				}
			}
			//Debug.WriteLine($"per {perSpace}");
			var currentOffset = new Point(finalOffset.X, finalOffset.Y);
			foreach (var child in this.Children) {
				var childSize = new Windows.Foundation.Size();
				var childWeight = MStack.GetWeight(child);
				if (Floater.IsNaN(childWeight)) {
					if (isHorizontal) {
						childSize.Width = child.DesiredSize.Width;
						childSize.Height = finalSize.Height;
					} else {
						childSize.Height = child.DesiredSize.Height;
						childSize.Width = finalSize.Width;
					}
				} else {
					var childSpace = perSpace * childWeight;
					if (isHorizontal) {
						childSize.Width = childSpace;
						childSize.Height = finalSize.Height;
					} else {
						childSize.Height = childSpace;
						childSize.Width = finalSize.Width;
					}
				}
				child.Arrange(new Rect(currentOffset, childSize));
				//Debug.WriteLine($"before {currentOffset} {childSize} {child.DesiredSize} {child.RenderSize} {child.ActualSize}");
				if (isHorizontal) {
					currentOffset.X += childSize.Width + childSpacing;
				} else {
					currentOffset.Y += childSize.Height + childSpacing;
				}
				//Debug.WriteLine($"after {child.DesiredSize} {child.RenderSize} {child.ActualSize}");
			}
			return finalSize;
		}

		#endregion

		#region property

		public static readonly DependencyProperty OrientationProperty = DependencyProperty.Register(
			nameof(MStack.Orientation), typeof(Orientation), typeof(MStack),
			new PropertyMetadata(Orientation.Horizontal, (o, e) => {
				((MStack)o).InvalidateMeasure();
			})
		);

		public Orientation Orientation {
			get => (Orientation)this.GetValue(MStack.OrientationProperty);
			set => this.SetValue(MStack.OrientationProperty, value);
		}

		// ----------------

		public static readonly DependencyProperty SpacingProperty = DependencyProperty.Register(
			nameof(MStack.Spacing), typeof(Floater), typeof(MStack),
			new PropertyMetadata(0.0, (o, e) => {
				((MStack)o).InvalidateMeasure();
			})
		);

		public Floater Spacing {
			get => (Floater)this.GetValue(MStack.SpacingProperty);
			set => this.SetValue(MStack.SpacingProperty, value);
		}

		// ----------------

		public static readonly DependencyProperty WeightProperty = DependencyProperty.RegisterAttached(
			"Weight", typeof(Floater), typeof(MStack),
			new PropertyMetadata(Floater.NaN, (element, args) => {
				var parent = ((FrameworkElement)element).Parent;
				if (parent != null) {
					((MStack)parent).InvalidateMeasure();
				}
			})
		);

		public static void SetWeight (
			DependencyObject element,
			Floater          value
		) => element.SetValue(MStack.WeightProperty, value);

		public static Floater GetWeight (
			DependencyObject element
		) => (Floater)element.GetValue(MStack.WeightProperty);

		#endregion

	}

}
