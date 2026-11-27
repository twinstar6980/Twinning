namespace Twinning.Script.Executable.ComputeWwiseShortIdentifier {

	// #region interface

	export function run(
	): void {
		Console.information(los('executable.compute_wwise_short_identifier:please_input_string'), [
		]);
		while (true) {
			let name = Console.string(true, null);
			if (name === null) {
				break;
			}
			let value = KernelX.Tool.Data.Hashing.Fnv.hash_s(name.toLowerCase(), 'v1', 32n);
			Console.success(`${value}`, []);
		}
		return;
	}

	// #endregion

}
