namespace Twinning.Script {

	export enum StoragePathType {
		nothing,
		relative,
		absolute,
	}

	export enum StoragePathStyle {
		generic,
		native,
		posix,
		windows,
	}

	// ----------------

	export class StoragePath {

		// #region constructor

		private m_type: StoragePathType;

		private m_root: string | null;

		private m_part: Array<string>;

		// ----------------

		public constructor(
		);

		public constructor(
			that: StoragePath,
		);

		public constructor(
			type: StoragePathType,
		);

		public constructor(
			text: string,
		);

		public constructor(
			argument?: StoragePath | StoragePathType | string,
		) {
			this.m_type = StoragePathType.nothing;
			this.m_root = null;
			this.m_part = [];
			if (argument instanceof StoragePath) {
				this.m_type = argument.m_type;
				this.m_root = argument.m_root;
				this.m_part = argument.m_part.slice();
			}
			if (typeof argument === 'number') {
				this.m_type = argument as StoragePathType;
			}
			if (typeof argument === 'string') {
				this.parse(argument);
			}
			return;
		}

		// #endregion

		// #region query

		public type(
		): StoragePathType {
			return this.m_type;
		}

		public root(
		): string | null {
			return this.m_root;
		}

		public part(
		): Array<string> {
			return this.m_part;
		}

		// #endregion

		// #region segment

		public parent(
		): StoragePath | null {
			let result = null as StoragePath | null;
			if (this.m_type !== StoragePathType.nothing && this.m_part.length !== 0) {
				result = new StoragePath();
				result.m_type = this.m_type;
				result.m_root = this.m_root;
				result.m_part.push(...this.m_part.slice(0, this.m_part.length - 1));
			}
			return result;
		}

		public name(
		): string | null {
			let result = null as string | null;
			if (this.m_type !== StoragePathType.nothing && this.m_part.length !== 0) {
				result = this.m_part.at(-1)!;
			}
			return result;
		}

		// ----------------

		public stem(
		): string | null {
			let result = null as string | null;
			if (this.m_type !== StoragePathType.nothing && this.m_part.length !== 0) {
				let position = this.m_part.at(-1)!.lastIndexOf('.');
				if (position !== -1) {
					result = this.m_part.at(-1)!.substring(0, position);
				}
				else {
					result = this.m_part.at(-1)!;
				}
			}
			return result;
		}

		public extension(
		): string | null {
			let result = null as string | null;
			if (this.m_type !== StoragePathType.nothing && this.m_part.length !== 0) {
				let position = this.m_part.at(-1)!.lastIndexOf('.');
				if (position !== -1) {
					result = this.m_part.at(-1)!.substring(position + 1);
				}
			}
			return result;
		}

		// #endregion

		// #region compose

		public join(
			other: string,
		): StoragePath {
			assert_test(this.m_type !== StoragePathType.nothing);
			let result = new StoragePath();
			result.m_type = this.m_type;
			result.m_root = this.m_root;
			result.m_part.push(...this.m_part);
			result.m_part.push(other);
			return result;
		}

		public push(
			other: StoragePath,
		): StoragePath {
			assert_test(this.m_type !== StoragePathType.nothing);
			assert_test(other.m_type === StoragePathType.relative);
			let result = new StoragePath();
			result.m_type = this.m_type;
			result.m_root = this.m_root;
			result.m_part.push(...this.m_part);
			result.m_part.push(...other.m_part);
			return result;
		}

		// #endregion

		// #region convert

		public parse(
			text: string,
		): void {
			this.m_type = StoragePathType.nothing;
			this.m_root = null;
			this.m_part = [];
			if (text.length !== 0) {
				let position = 0;
				if (text.length >= 2 && text[1] === ':' && ConvertHelper.is_letter(text[0])) {
					this.m_root = text.substring(0, 2);
					position += 2;
				}
				if (text.length > position && ConvertHelper.is_path_separator(text[position])) {
					this.m_type = StoragePathType.absolute;
					position += 1;
				}
				else {
					this.m_type = StoragePathType.relative;
				}
				let location = position;
				while (true) {
					if (position === text.length || ConvertHelper.is_path_separator(text[position])) {
						let segment = text.substring(location, position);
						if (segment === '' || segment === '.') {
						}
						else if (segment === '..' && this.m_part.length !== 0 && this.m_part.at(-1)! !== '..') {
							this.m_part.pop();
						}
						else {
							this.m_part.push(segment);
						}
						if (position === text.length) {
							break;
						}
						position += 1;
						location = position;
						continue;
					}
					position += 1;
				}
			}
			return;
		}

		public emit(
			style: StoragePathStyle = StoragePathStyle.generic,
		): string {
			let text = '';
			if (this.m_type != StoragePathType.nothing) {
				let mapped_style = style;
				if (style === StoragePathStyle.generic) {
					mapped_style = StoragePathStyle.posix;
				}
				if (style === StoragePathStyle.native) {
					if (KernelX.is_windows) {
						mapped_style = StoragePathStyle.windows;
					}
					if (KernelX.is_linux || KernelX.is_macintosh || KernelX.is_android || KernelX.is_iphone) {
						mapped_style = StoragePathStyle.posix;
					}
				}
				let separator = mapped_style === StoragePathStyle.posix ? '/' : '\\';
				if (this.m_root !== null) {
					text += this.m_root!;
				}
				if (this.m_type === StoragePathType.relative) {
					text += '.';
				}
				if (this.m_part.length === 0) {
					text += separator;
				}
				for (let segment of this.m_part) {
					text += separator;
					if (mapped_style === StoragePathStyle.posix) {
						text += segment;
					}
					if (mapped_style === StoragePathStyle.windows) {
						let segment_size = segment.length;
						while (segment_size !== 0) {
							if (segment[segment_size - 1] === ' ') {
								segment_size -= 1;
								continue;
							}
							if (segment[segment_size - 1] === '.' && !((segment_size === 1) || (segment_size === 2 && segment[0] === '.'))) {
								segment_size -= 1;
								continue;
							}
							break;
						}
						text += segment.substring(0, segment_size);
					}
				}
			}
			return text;
		}

		// ----------------

		public emit_generic(
		): string {
			return this.emit(StoragePathStyle.generic);
		}

		public emit_native(
		): string {
			return this.emit(StoragePathStyle.native);
		}

		public emit_posix(
		): string {
			return this.emit(StoragePathStyle.posix);
		}

		public emit_windows(
		): string {
			return this.emit(StoragePathStyle.windows);
		}

		// #endregion

	}

}
