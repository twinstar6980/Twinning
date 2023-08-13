#pragma warning disable 0,
// ReSharper disable CheckNamespace MemberHidesStaticFromOuterClass

using Helper;

namespace Helper {

	public static class MethodConfigurationModel {

		#region value

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
				return $"{this.Value}{this.Unit.ToString().ToLower()}";
			}

			public static SizeExpression Parse (
				String expression
			) {
				if (expression.Length < 2) {
					throw new Exception();
				}
				var value = new SizeExpression();
				value.Unit = expression[^1] switch {
					'b' => SizeUnit.B,
					'k' => SizeUnit.K,
					'm' => SizeUnit.M,
					'g' => SizeUnit.G,
					_   => throw new ArgumentOutOfRangeException(),
				};
				value.Value = Floater.Parse(expression[..^1]);
				return value;
			}

		}

		// ----------------

		public class ArgumentValue {

			public Object? Value = default!;

			// ----------------

			public Boolean OfConfirmation {
				get => this.Value as Boolean? ?? throw new Exception();
				set => this.Value = value;
			}

			public Floater OfNumber {
				get => this.Value as Floater? ?? throw new Exception();
				set => this.Value = value;
			}

			public Integer OfInteger {
				get => this.Value as Integer? ?? throw new Exception();
				set => this.Value = value;
			}

			public SizeExpression OfSize {
				get => this.Value as SizeExpression ?? throw new Exception();
				set => this.Value = value;
			}

			public String OfString {
				get => this.Value as String ?? throw new Exception();
				set => this.Value = value;
			}

			public String OfPath {
				get => this.Value as String ?? throw new Exception();
				set => this.Value = value;
			}

		}

		#endregion

		#region configuration

		public enum ArgumentType {
			Confirmation,
			Number,
			Integer,
			Size,
			String,
			Path,
		}

		public class ArgumentConfiguration {
			public String        Id      = default!;
			public String        Name    = default!;
			public ArgumentType  Type    = default!;
			public List<Object>? Option  = default!;
			public Object?       Initial = default!;
		}

		public class MethodConfiguration {
			public String                      Id       = default!;
			public String                      Name     = default!;
			public List<ArgumentConfiguration> Argument = default!;
		}

		public class MethodGroupConfiguration {
			public String                    Id   = default!;
			public String                    Name = default!;
			public List<MethodConfiguration> Item = default!;
		}

		// ----------------

		public class Configuration {
			public List<MethodGroupConfiguration> Group = default!;
		}

		#endregion

	}

}