import * as ts from 'typescript';
import * as tcp from 'ts-patch';

export default function (program: ts.Program, pluginConfig: tcp.PluginConfig, { ts: tsInstance }: tcp.TransformerExtras) {
	return (context: ts.TransformationContext) => {
		const factory = context.factory;
		return (sourceFile: ts.SourceFile) => {
			const visit = (node: ts.Node): ts.Node => {
				if (ts.isCallExpression(node) && ts.isIdentifier(node.expression)) {
					if (node.expression.text === 'assert_test' && node.arguments.length === 1) {
						const assertionCondition = node.arguments[0];
						const assertionExpression = factory.createStringLiteral(ts.createPrinter().printNode(ts.EmitHint.Unspecified, assertionCondition, sourceFile));
						return factory.updateCallExpression(
							node,
							node.expression,
							node.typeArguments,
							[assertionCondition, assertionExpression],
						);
					}
				}
				return ts.visitEachChild(node, visit, context);
			}
			return ts.visitNode(sourceFile, visit) as ts.SourceFile;
		};
	};
}
