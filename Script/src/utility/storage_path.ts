namespace Twinning.Script {

	export enum StoragePathType {
		detached,
		relative,
		absolute,
	}

	export enum StoragePathStyle {
		posix,
		windows,
	}

	// ----------------

	export class StoragePath {

		// #region constructor

		private m_type: StoragePathType;

		private m_root: string | null;

		private m_segment: Array<string>;

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
			this.m_type = StoragePathType.detached;
			this.m_root = null;
			this.m_segment = [];
			if (argument instanceof StoragePath) {
				this.m_type = argument.m_type;
				this.m_root = argument.m_root;
				this.m_segment = argument.m_segment.slice();
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

		public segment(
		): Array<string> {
			return this.m_segment;
		}

		// #endregion

		// #region part

		public parent(
		): StoragePath | null {
			let result = null as StoragePath | null;
			if (this.m_segment.length !== 0) {
				result = new StoragePath();
				result.m_type = this.m_type;
				result.m_root = this.m_root;
				result.m_segment.push(...this.m_segment.slice(0, this.m_segment.length - 1));
			}
			return result;
		}

		public name(
		): string | null {
			let result = null as string | null;
			if (this.m_segment.length !== 0) {
				result = this.m_segment.at(-1)!;
			}
			return result;
		}

		// ----------------

		public stem(
		): string | null {
			let result = null as string | null;
			if (this.m_segment.length !== 0) {
				let segment = this.m_segment.at(-1)!;
				let position = segment.lastIndexOf('.');
				if (position !== -1) {
					result = segment.substring(0, position);
				}
				else {
					result = segment;
				}
			}
			return result;
		}

		public extension(
		): string | null {
			let result = null as string | null;
			if (this.m_segment.length !== 0) {
				let segment = this.m_segment.at(-1)!;
				let position = segment.lastIndexOf('.');
				if (position !== -1) {
					result = segment.substring(position + 1);
				}
			}
			return result;
		}

		// #endregion

		// #region compose

		public join(
			other: string,
		): StoragePath {
			let result = new StoragePath();
			result.m_type = this.m_type;
			result.m_root = this.m_root;
			result.m_segment.push(...this.m_segment);
			result.m_segment.push(other);
			return result;
		}

		public push(
			other: StoragePath,
		): StoragePath {
			assert_test(other.m_type === StoragePathType.detached);
			let result = new StoragePath();
			result.m_type = this.m_type;
			result.m_root = this.m_root;
			result.m_segment.push(...this.m_segment);
			result.m_segment.push(...other.m_segment);
			return result;
		}

		// #endregion

		// #region convert

		public parse(
			text: string,
		): void {
			this.m_type = StoragePathType.detached;
			this.m_root = null;
			this.m_segment = [];
			let position = 0;
			if (text.length >= 2 && ConvertHelper.is_letter(text[0]) && ConvertHelper.is_path_volume_separator(text[1])) {
				position += 2;
				if (position < text.length && ConvertHelper.is_path_directory_separator(text[position])) {
					this.m_type = StoragePathType.absolute;
					this.m_root = text.substring(0, position);
					position += 1;
				}
				else {
					this.m_type = StoragePathType.relative;
					this.m_root = text.substring(0, position);
				}
			}
			else if (text.length >= 2 && ConvertHelper.is_path_directory_separator(text[0]) && ConvertHelper.is_path_directory_separator(text[1])) {
				position += 2;
				for (; position < text.length; position++) {
					if (ConvertHelper.is_path_directory_separator(text[position])) {
						break;
					}
				}
				this.m_type = StoragePathType.absolute;
				this.m_root = '//' + text.substring(2, position);
				if (position < text.length && ConvertHelper.is_path_directory_separator(text[position])) {
					position += 1;
				}
			}
			else if (text.length >= 1 && ConvertHelper.is_path_directory_separator(text[position])) {
				position += 1;
				this.m_type = StoragePathType.absolute;
				this.m_root = '';
			}
			else if (text.length >= 2 && ConvertHelper.is_path_dot(text[0]) && ConvertHelper.is_path_dot(text[1])) {
				position += 2;
				if (position == text.length || ConvertHelper.is_path_directory_separator(text[position])) {
					this.m_type = StoragePathType.relative;
					this.m_root = '';
				}
				position = 0;
			}
			else if (text.length >= 1 && ConvertHelper.is_path_dot(text[0])) {
				position += 1;
				if (position == text.length || ConvertHelper.is_path_directory_separator(text[position])) {
					this.m_type = StoragePathType.relative;
					this.m_root = '';
					if (position < text.length) {
						position += 1;
					}
				}
				else {
					position = 0;
				}
			}
			let location = position;
			for (; position <= text.length; position++) {
				if (position === text.length || ConvertHelper.is_path_directory_separator(text[position])) {
					let segment = text.substring(location, position);
					if (segment.length !== 0) {
						this.m_segment.push(segment);
					}
					location = position + 1;
				}
			}
			return;
		}

		public emit(
			style: StoragePathStyle = StoragePathStyle.posix,
			rectify: boolean = false,
		): string {
			let text = '';
			let pathDot = '.';
			let pathDirectorySeparator = style === StoragePathStyle.posix ? '/' : '\\';
			if (this.m_type !== StoragePathType.detached) {
				for (let index = 0; index < this.m_root!.length; index++) {
					text += !ConvertHelper.is_path_directory_separator(this.m_root![index]) ? this.m_root![index] : pathDirectorySeparator;
				}
				if (this.m_type === StoragePathType.relative) {
					text += pathDot;
				}
				text += pathDirectorySeparator;
			}
			for (let segment_index = 0; segment_index < this.m_segment.length; segment_index++) {
				let segment = this.m_segment[segment_index];
				if (segment_index !== 0) {
					text += pathDirectorySeparator;
				}
				if (!rectify) {
					text += segment;
				}
				else {
					if (style === StoragePathStyle.posix) {
						text += segment;
					}
					if (style === StoragePathStyle.windows) {
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

		public emit_posix(
			rectify: boolean = false,
		): string {
			return this.emit(StoragePathStyle.posix, rectify);
		}

		public emit_windows(
			rectify: boolean = false,
		): string {
			return this.emit(StoragePathStyle.windows, rectify);
		}

		public emit_native(
		): string {
			let mapped_style = null as StoragePathStyle | null;
			if (KernelX.is_windows) {
				mapped_style = StoragePathStyle.windows;
			}
			if (KernelX.is_linux || KernelX.is_macintosh || KernelX.is_android || KernelX.is_iphone) {
				mapped_style = StoragePathStyle.posix;
			}
			return this.emit(mapped_style!, true);
		}

		// #endregion

	}

}
