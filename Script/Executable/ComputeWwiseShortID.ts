/** 计算 Wwise Short ID */
namespace TwinStar.Script.Executable.ComputeWwiseShortID {

	export function execute(
	): void {
		Console.information(los('executable.compute_wwise_short_id:please_input_string'), []);
		while (true) {
			let name = Console.string(null, null).toLowerCase();
			let value = KernelX.Tool.Data.Hash.FNV.hash_s(name, 'm_1', 'b_32');
			Console.success(`${value}`, []);
		}
		return;
	}

}

TwinStar.Script.Executable.ComputeWwiseShortID.execute();