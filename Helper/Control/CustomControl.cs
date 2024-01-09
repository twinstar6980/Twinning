#pragma warning disable 0,
// ReSharper disable

using Helper;
using Helper.Utility;

namespace Helper {

	public abstract class CustomControl : UserControl {

		#region interface

		protected abstract void StampUpdate (
		);

		#endregion

		#region property

		public static readonly DependencyProperty StampProperty = DependencyProperty.Register(
			nameof(CustomControl.Stamp),
			typeof(UniqueStamp),
			typeof(CustomControl),
			new (UniqueStamp.Default, (o, e) => { o.AsClass<CustomControl>().StampUpdate(); })
		);

		public UniqueStamp Stamp {
			get => this.GetValue(CustomControl.StampProperty).AsClass<UniqueStamp>();
			set => this.SetValue(CustomControl.StampProperty, value);
		}

		#endregion

	}

}
