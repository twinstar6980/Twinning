#pragma warning disable 0,
// ReSharper disable

using AssistantPlus;
using AssistantPlus.Utility;

namespace AssistantPlus.View.ModdingWorker {

	public enum SubmissionType {
		Pause,
		Boolean,
		Integer,
		Floater,
		Size,
		String,
		Path,
		Enumeration,
	}

	// ----------------

	public class SubmissionValue {

		public ValueExpression? Data = null;

		// ----------------

		public BooleanExpression? OfBoolean {
			get => this.Data.AsClassOrNull<BooleanExpression>();
			set => this.Data = value;
		}

		public IntegerExpression? OfInteger {
			get => this.Data.AsClassOrNull<IntegerExpression>();
			set => this.Data = value;
		}

		public FloaterExpression? OfFloater {
			get => this.Data.AsClassOrNull<FloaterExpression>();
			set => this.Data = value;
		}

		public SizeExpression? OfSize {
			get => this.Data.AsClassOrNull<SizeExpression>();
			set => this.Data = value;
		}

		public StringExpression? OfString {
			get => this.Data.AsClassOrNull<StringExpression>();
			set => this.Data = value;
		}

		public PathExpression? OfPath {
			get => this.Data.AsClassOrNull<PathExpression>();
			set => this.Data = value;
		}

		public EnumerationExpression? OfEnumeration {
			get => this.Data.AsClassOrNull<EnumerationExpression>();
			set => this.Data = value;
		}

	}

}
