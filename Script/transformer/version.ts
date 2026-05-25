import * as ts from 'typescript';
import * as tcp from 'ts-patch';
import * as packageJson from '../package.json'

export default function (program: ts.Program, pluginConfig: tcp.PluginConfig, {ts: tsInstance}: tcp.TransformerExtras) {
	return (context: ts.TransformationContext) => {
		return (sourceFile: ts.SourceFile) => {
			const visit = (node: ts.Node): ts.Node => {
				do {
					if (!ts.isModuleDeclaration(node) || !ts.isIdentifier(node.name) || node.name.text !== 'Twinning') {
						break;
					}
					if (node.body === undefined) {
						break;
					}
					if (!ts.isModuleDeclaration(node.body) || !ts.isIdentifier(node.body.name) || node.body.name.text !== 'Script') {
						break;
					}
					if (node.body.body === undefined) {
						break;
					}
					if (!ts.isModuleBlock(node.body.body)) {
						break;
					}
					const oldVersionDefinition = node.body.body.statements.find((it) =>
						ts.isVariableStatement(it) &&
						it.declarationList.declarations.length === 1 &&
						ts.isIdentifier(it.declarationList.declarations[0].name) &&
						it.declarationList.declarations[0].name.text === 'k_version'
					) as ts.VariableStatement | undefined;
					if (oldVersionDefinition === undefined) {
						break;
					}
					const oldVersionDeclaration = oldVersionDefinition.declarationList.declarations[0];
					const newVersionDefinition = ts.factory.updateVariableStatement(
						oldVersionDefinition,
						oldVersionDefinition.modifiers!.filter((it) => it.kind !== ts.SyntaxKind.DeclareKeyword),
						ts.factory.updateVariableDeclarationList(
							oldVersionDefinition.declarationList,
							[
								ts.factory.createVariableDeclaration(
									oldVersionDeclaration.name,
									oldVersionDeclaration.exclamationToken,
									oldVersionDeclaration.type,
									ts.factory.createStringLiteral(packageJson.version.replaceAll('.0.0', ''), true),
								),
							],
						),
					);
					const newRoot = ts.factory.updateModuleDeclaration(
						node,
						node.modifiers,
						node.name,
						ts.factory.updateModuleDeclaration(
							node.body,
							node.body.modifiers,
							node.body.name,
							ts.factory.updateModuleBlock(
								node.body.body,
								node.body.body.statements.map((it) => it !== oldVersionDefinition ? it : newVersionDefinition),
							),
						) as ts.ModuleBody,
					);
					return newRoot;
				}
				while (false);
				return ts.visitEachChild(node, visit, context);
			};
			return ts.visitNode(sourceFile, visit) as ts.SourceFile;
		};
	};
}
