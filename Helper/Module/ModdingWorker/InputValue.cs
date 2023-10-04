#pragma warning disable 0,
// ReSharper disable

using Helper;

namespace Helper.Module.ModdingWorker {

	#region type

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

		public static SizeExpression Parse (
			String expression
		) {
			if (expression.Length < 2) {
				throw new Exception();
			}
			return new SizeExpression() {
				Unit = expression[^1] switch {
					'b' => SizeUnit.B,
					'k' => SizeUnit.K,
					'm' => SizeUnit.M,
					'g' => SizeUnit.G,
					_   => throw new ArgumentOutOfRangeException(),
				},
				Value = Floater.Parse(expression[..^1]),
			};
		}

	}

	public class PathExpression {

		public String Value = "";

		// ----------------

		public override String ToString (
		) {
			return $"{this.Value}";
		}

		public static PathExpression Parse (
			String expression
		) {
			return new PathExpression() {
				Value = expression,
			};
		}

	}

	public class EnumerationExpression {

		public Size Value = 0;

		// ----------------

		public override String ToString (
		) {
			return $"{this.Value}";
		}

		public static PathExpression Parse (
			String expression
		) {
			return new PathExpression() {
				Value = expression,
			};
		}

	}

	// ----------------

	public enum InputType {
		Pause,
		Boolean,
		Integer,
		Floater,
		Size,
		String,
		Path,
		Enumeration,
	}

	public class InputValue {

		public Object? Data = default!;

		// ----------------

		public Boolean? OfBoolean {
			get => this.Data as Boolean?;
			set => this.Data = value;
		}

		public Integer? OfInteger {
			get => this.Data as Integer?;
			set => this.Data = value;
		}

		public Floater? OfFloater {
			get => this.Data as Floater?;
			set => this.Data = value;
		}

		public SizeExpression? OfSize {
			get => this.Data as SizeExpression;
			set => this.Data = value;
		}

		public String? OfString {
			get => this.Data as String;
			set => this.Data = value;
		}

		public PathExpression? OfPath {
			get => this.Data as PathExpression;
			set => this.Data = value;
		}

		public EnumerationExpression? OfEnumeration {
			get => this.Data as EnumerationExpression;
			set => this.Data = value;
		}

	}

	#endregion

}
