#pragma warning disable 0,
// ReSharper disable

using AssistantPlus;
using AssistantPlus.Utility;

namespace AssistantPlus.View.ModdingWorker {

	public enum SizeUnit {
		B,
		K,
		M,
		G,
	}

	public class SizeExpression {

		public Floater Value = 0.0;

		public SizeUnit Unit = SizeUnit.M;

		// ----------------

		public override String ToString (
		) {
			return $"{this.Value}{(!Floater.IsInteger(this.Value) ? "" : ".0")}{this.Unit.ToString().ToLower()}";
		}

	}

	public class PathExpression {

		public String Value = "";

		// ----------------

		public override String ToString (
		) {
			return $"{this.Value}";
		}

	}

	public class EnumerationExpression {

		public Size Value = 0;

		// ----------------

		public override String ToString (
		) {
			return $"{this.Value}";
		}

	}

	// ----------------

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

	public class SubmissionValue {

		public Object? Data = null;

		// ----------------

		public Boolean? OfBoolean {
			get => this.Data.AsStructOrNull<Boolean>();
			set => this.Data = value;
		}

		public Integer? OfInteger {
			get => this.Data.AsStructOrNull<Integer>();
			set => this.Data = value;
		}

		public Floater? OfFloater {
			get => this.Data.AsStructOrNull<Floater>();
			set => this.Data = value;
		}

		public SizeExpression? OfSize {
			get => this.Data.AsClassOrNull<SizeExpression>();
			set => this.Data = value;
		}

		public String? OfString {
			get => this.Data.AsClassOrNull<String>();
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
