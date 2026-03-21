namespace Twinning.Script.Executable.ComputeWwiseShortIdentifier {

	// #region interface

	export function run(
	): void {
		Console.information(los('executable.compute_wwise_short_identifier:please_input_string'), []);
		while (true) {
			let name = Console.string(true, null);
			if (name === null) {
				break;
			}
			let value = KernelX.Tool.Data.Hash.Fnv.hash_s(name.toLowerCase(), 'm_1', 'b_32');
			Console.success(`${value}`, []);
		}
		return;
	}

	// #endregion

}
