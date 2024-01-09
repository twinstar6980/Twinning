#pragma warning disable 0,
// ReSharper disable

using Helper;
using Helper.Utility;

namespace Helper.View.CommandSender {

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
			GF.AssertTest(expression.Length >= 2);
			return new () {
				Unit = expression[^1] switch {
					'b' => SizeUnit.B,
					'k' => SizeUnit.K,
					'm' => SizeUnit.M,
					'g' => SizeUnit.G,
					_   => throw new (),
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
			return new () {
				Value = expression,
			};
		}

	}

	// ----------------

	public enum ArgumentType {
		Boolean,
		Integer,
		Floater,
		Size,
		String,
		Path,
	}

	public class ArgumentValue {

		public Object? Data = null;

		// ----------------

		public Boolean OfBoolean {
			get => this.Data.AsStruct<Boolean>();
			set => this.Data = value;
		}

		public Integer OfInteger {
			get => this.Data.AsStruct<Integer>();
			set => this.Data = value;
		}

		public Floater OfFloater {
			get => this.Data.AsStruct<Floater>();
			set => this.Data = value;
		}

		public SizeExpression OfSize {
			get => this.Data.AsClass<SizeExpression>();
			set => this.Data = value;
		}

		public String OfString {
			get => this.Data.AsClass<String>();
			set => this.Data = value;
		}

		public PathExpression OfPath {
			get => this.Data.AsClass<PathExpression>();
			set => this.Data = value;
		}

	}

	#endregion

}
