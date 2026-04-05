#pragma warning disable 0,
// ReSharper disable

using Twinning.AssistantPlus;
using Twinning.AssistantPlus.Utility;

namespace Twinning.AssistantPlus.View.CoreCommandSender {

	public record ValueExpression {
	}

	// ----------------

	public record BooleanExpression : ValueExpression {
		public required Boolean Value { get; set; }
	}

	public record IntegerExpression : ValueExpression {
		public required Integer Value { get; set; }
	}

	public record FloaterExpression : ValueExpression {
		public required Floater Value { get; set; }
	}

	public record StringExpression : ValueExpression {
		public required String Value { get; set; }
	}

	public record SizeExpression : ValueExpression {
		public required Floater Count    { get; set; }
		public required Integer Exponent { get; set; }
	}

	public record PathExpression : ValueExpression {
		public required StoragePath Content { get; set; }
	}

	// ----------------

	public static class ValueExpressionHelper {

		#region utility

		public static String MakeString(
			ValueExpression value
		) {
			return value switch {
				BooleanExpression values => $"{ConvertHelper.MakeBooleanToStringOfConfirmationCharacter(values.Value)}",
				IntegerExpression values => $"{ConvertHelper.MakeIntegerToString(values.Value)}",
				FloaterExpression values => $"{ConvertHelper.MakeFloaterToString(values.Value)}",
				StringExpression values  => $"{values.Value}",
				SizeExpression values    => $"{ConvertHelper.MakeFloaterToString(values.Count)}{new[] { "b", "k", "m", "g" }[values.Exponent]}",
				PathExpression values    => $"{values.Content.Emit()}",
				_                        => throw new UnreachableException(),
			};
		}

		#endregion

	}

}
