#pragma warning disable 0,
// ReSharper disable

using AssistantPlus;
using AssistantPlus.Utility;

namespace AssistantPlus.View.CommandSender {

	public class SizeExpression {

		public Floater Count = 0.0;

		public Integer Exponent = 2;

		// ----------------

		public static String MakeString (
			SizeExpression value
		) {
			return $"{ConvertHelper.MakeFloaterToString(value.Count, false)}{new[] { "b", "k", "m", "g" }[value.Exponent]}";
		}

		public static SizeExpression ParseString (
			String text
		) {
			GF.AssertTest(text.Length >= 2);
			return new () {
				Exponent = text[^1] switch {
					'b' => 0,
					'k' => 1,
					'm' => 2,
					'g' => 3,
					_   => throw new (),
				},
				Count = Floater.Parse(text[..^1]),
			};
		}

	}

	public class PathExpression {

		public String Content = "";

		// ----------------

		public static String MakeString (
			PathExpression value
		) {
			return $"{value.Content}";
		}

		public static PathExpression ParseString (
			String text
		) {
			return new () {
				Content = text,
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

}
