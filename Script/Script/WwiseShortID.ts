/** 计算 Wwise Short ID */
namespace TwinStar.Script.WwiseShortID {

	export function execute(
	): void {
		Console.notify('i', '请输入字符串', []);
		while (true) {
			let s = Console.string(null).toLowerCase();
			let v = CoreX.Tool.Data.Hash.FNV.hash_s(s, 'm_1', 'b_32');
			Console.notify('s', `${v}`, []);
		}
		return;
	}

}

TwinStar.Script.WwiseShortID.execute();