#pragma warning disable 0,
// ReSharper disable CheckNamespace MemberHidesStaticFromOuterClass

using Helper;
using Newtonsoft.Json;

namespace Helper {

	public static class CommandConfigurationModel {

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

			public PathExpression OfPath {
				get => this.Value as PathExpression ?? throw new Exception();
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

		[JsonObject(ItemRequired = Required.AllowNull)]
		public class ArgumentConfiguration {
			public String        Id      = default!;
			public String        Name    = default!;
			public ArgumentType  Type    = default!;
			public List<Object>? Option  = default!;
			public Object?       Initial = default!;
		}

		[JsonObject(ItemRequired = Required.AllowNull)]
		public class MethodConfiguration {
			public String                      Id        = default!;
			public String                      Name      = default!;
			public String                      Icon      = default!;
			public List<ArgumentConfiguration> Argument  = default!;
			public List<String>?               Batchable = default!;
		}

		[JsonObject(ItemRequired = Required.AllowNull)]
		public class MethodGroupConfiguration {
			public String                    Id   = default!;
			public String                    Name = default!;
			public String                    Icon = default!;
			public List<MethodConfiguration> Item = default!;
		}

		// ----------------

		public enum QuickOptionFilterFileObjectType {
			Any,
			File,
			Directory,
		}

		[JsonObject(ItemRequired = Required.AllowNull)]
		public class QuickOptionFilterConfiguration {
			public QuickOptionFilterFileObjectType Type = default!;
			public String                          Name = default!;
		}

		[JsonObject(ItemRequired = Required.AllowNull)]
		public class QuickOptionPresetConfiguration {
			public String                     Name     = default!;
			public Dictionary<String, Object> Argument = default!;
		}

		[JsonObject(ItemRequired = Required.AllowNull)]
		public class QuickOptionConfiguration {
			public String                                Name      = default!;
			public String                                Icon      = default!;
			public QuickOptionFilterConfiguration        Filter    = default!;
			public Boolean                               Batchable = default!;
			public String?                               Method    = default!;
			public List<QuickOptionPresetConfiguration?> Preset    = default!;
		}

		[JsonObject(ItemRequired = Required.AllowNull)]
		public class QuickOptionGroupConfiguration {
			public String                         Name = default!;
			public String                         Icon = default!;
			public List<QuickOptionConfiguration> Item = default!;
		}

		// ----------------

		[JsonObject(ItemRequired = Required.AllowNull)]
		public class Configuration {
			public List<MethodGroupConfiguration>      Method      = default!;
			public List<QuickOptionGroupConfiguration> QuickOption = default!;
		}

		#endregion

	}

}
