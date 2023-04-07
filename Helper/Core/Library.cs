#pragma warning disable 0,
// ReSharper disable

using Helper;

namespace Helper.Core {

	public abstract unsafe class Library {

		#region interface

		public abstract Interface.String* Version (
			Interface.Size** number
		);

		public abstract Interface.String* Execute (
			Interface.Callback**   callback,
			Interface.String**     script,
			Interface.StringList** argument,
			Interface.String**     result
		);

		public abstract Interface.String* Prepare (
		);

		#endregion

	}

}
