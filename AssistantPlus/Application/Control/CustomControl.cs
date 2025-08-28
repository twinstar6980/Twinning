#pragma warning disable 0,
// ReSharper disable

using AssistantPlus;
using AssistantPlus.Utility;

namespace AssistantPlus {

	public abstract class CustomControl : UserControl {

		#region life

		public CustomControl (
		) {
			return;
		}

		#endregion

		#region property

		public static readonly DependencyProperty StampProperty = DependencyProperty.Register(
			nameof(CustomControl.Stamp),
			typeof(UniqueStamp),
			typeof(CustomControl),
			new (UniqueStamp.Default, (o, e) => { o.As<CustomControl>().StampUpdate().SelfLet(ExceptionHelper.WrapTask); })
		);

		public UniqueStamp Stamp {
			get => this.GetValue(CustomControl.StampProperty).As<UniqueStamp>();
			set => this.SetValue(CustomControl.StampProperty, value);
		}

		#endregion

		#region interface

		protected abstract Task StampUpdate (
		);

		#endregion

	}

}
