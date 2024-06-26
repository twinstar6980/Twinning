#pragma warning disable 0,
// ReSharper disable

using AssistantPlus;
using AssistantPlus.Utility;

namespace AssistantPlus.View.ModdingWorker {

	public record ValueExpression {
	}

	// ----------------

	public record PauseExpression : ValueExpression {
	}

	public record BooleanExpression : ValueExpression {
		public Boolean Value = false;
	}

	public record IntegerExpression : ValueExpression {
		public Integer Value = 0;
	}

	public record FloaterExpression : ValueExpression {
		public Floater Value = 0.0;
	}

	public record SizeExpression : ValueExpression {
		public Floater Count    = 0.0;
		public Integer Exponent = 0;
	}

	public record StringExpression : ValueExpression {
		public String Value = "";
	}

	public record PathExpression : ValueExpression {
		public String Content = "";
	}

	public record EnumerationExpression : ValueExpression {
		public String Item = "";
	}

	// ----------------

	public static class ValueExpressionHelper {

		#region utility

		public static String MakeString (
			ValueExpression value
		) {
			return value switch {
				PauseExpression values       => "",
				BooleanExpression values     => ConvertHelper.MakeBooleanToStringOfConfirmationCharacter(values.Value),
				IntegerExpression values     => ConvertHelper.MakeIntegerToString(values.Value, true),
				FloaterExpression values     => ConvertHelper.MakeFloaterToString(values.Value, true),
				SizeExpression values        => $"{ConvertHelper.MakeFloaterToString(values.Count, false)}{new[] { "b", "k", "m", "g" }[values.Exponent]}",
				StringExpression values      => values.Value,
				PathExpression values        => $"{values.Content}",
				EnumerationExpression values => $"{values.Item}",
				_                            => throw new (),
			};
		}

		#endregion

	}

}
