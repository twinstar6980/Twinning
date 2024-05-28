// 计算 Wwise Short ID
namespace Twinning.Script.Executable.ComputeWwiseShortID {

	export function execute(
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

}

Twinning.Script.Executable.ComputeWwiseShortID.execute();