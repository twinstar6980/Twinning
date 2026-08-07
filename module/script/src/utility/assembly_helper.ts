namespace Twinning.Script.AssemblyHelper {

	// #region platform

	export type PlatformType = 'intel32' | 'arm32' | 'arm64';

	// #endregion

	// #region instruction

	export const k_instruction_code_nop_intel32 = 0x90n;

	export const k_instruction_code_nop_arm32 = 0xE320F000n;

	export const k_instruction_code_nop_arm64 = 0xD503201Fn;

	// ----------------

	export function find_call_instruction(
		platform_type: PlatformType,
		data: ByteStreamView,
		limit: number,
		address: Array<number>,
		overwrite: boolean,
	): boolean {
		let state = false;
		let dataEnd = Math.min(data.size(), data.p() + limit);
		if (platform_type === 'intel32') {
			while (data.p() < dataEnd) {
				let instruction_code = data.u8();
				// call #X = E8 XX XX XX XX
				if (instruction_code !== 0xE8n) {
					continue;
				}
				let jump_offset = data.i32();
				let jump_address = data.p() + Number(jump_offset);
				if (!address.includes(jump_address)) {
					data.pr(-4);
					continue;
				}
				if (overwrite) {
					data.pr(-5);
					data.u8(k_instruction_code_nop_intel32);
					data.u8(k_instruction_code_nop_intel32);
					data.u8(k_instruction_code_nop_intel32);
					data.u8(k_instruction_code_nop_intel32);
					data.u8(k_instruction_code_nop_intel32);
				}
				state = true;
				break;
			}
		}
		if (platform_type === 'arm32') {
			while (data.p() < dataEnd) {
				let instruction_code = data.u32();
				// bl #X = EB XX XX XX
				if ((instruction_code & 0xFF000000n) !== 0xEB000000n) {
					continue;
				}
				let jump_offset = instruction_code & 0x00FFFFFFn;
				if ((jump_offset & 0x800000n) === 0x800000n) {
					jump_offset = -(0x1000000n - jump_offset);
				}
				let jump_address = data.p() - 4 + 8 + Number(jump_offset) * 4;
				if (!address.includes(jump_address)) {
					continue;
				}
				if (overwrite) {
					data.pr(-4);
					data.u32(k_instruction_code_nop_arm32);
				}
				state = true;
				break;
			}
		}
		if (platform_type === 'arm64') {
			while (data.p() < dataEnd) {
				let instruction_code = data.u32();
				// bl #X = 97 XX XX XX
				if ((instruction_code & 0xFF000000n) !== 0x97000000n) {
					continue;
				}
				let jump_offset = instruction_code & 0x00FFFFFFn;
				if ((jump_offset & 0x800000n) === 0x800000n) {
					jump_offset = -(0x1000000n - jump_offset);
				}
				let jump_address = data.p() - 4 + Number(jump_offset) * 4;
				if (!address.includes(jump_address)) {
					continue;
				}
				if (overwrite) {
					data.pr(-4);
					data.u32(k_instruction_code_nop_arm64);
				}
				state = true;
				break;
			}
		}
		return state;
	}

	// #endregion

}
