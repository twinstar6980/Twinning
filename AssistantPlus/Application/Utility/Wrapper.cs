#pragma warning disable 0,
// ReSharper disable

using AssistantPlus;

namespace AssistantPlus.Utility {

	public class Wrapper<TValue>
		where TValue : notnull {

		#region structor

		private WrapperData mData;

		// ----------------

		private Wrapper (
			WrapperData data
		) {
			this.mData = data;
		}

		public Wrapper (
		) : this(new WrapperData()) {
		}

		public Wrapper (
			TValue? value
		) : this() {
			this.Value = value;
		}

		#endregion

		#region query

		public TValue? Value {
			get => (TValue?)this.mData.Value;
			set => this.mData.Value = value;
		}

		// ----------------

		public Wrapper<TTarget> Cast<TTarget> (
		)
			where TTarget : class {
			return new (this.mData);
		}

		#endregion

	}

	public record WrapperData {
		public Object? Value = default!;
	}

}
