namespace Twinning.Script.Executable.ComputeWwiseShortID {

	// #region run

	export function run(
	): void {
		Console.information(los('executable.compute_wwise_short_id:please_input_string'), []);
		while (true) {
			let name = Console.string(true, null);
			if (name === null) {
				break;
			}
			let value = KernelX.Tool.Data.Hash.FNV.hash_s(name.toLowerCase(), 'm_1', 'b_32');
			Console.success(`${value}`, []);
		}
		return;
	}

	// #endregion

}

Twinning.Script.Executable.ComputeWwiseShortID.run();