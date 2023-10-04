#pragma warning disable 0,
// ReSharper disable

using Helper;

namespace Helper.Utility {

	public static unsafe class MemoryHelper {

		#region utility

		private static readonly IntPtr Heap = ExternalLibrary.Kernel32.GetProcessHeap();

		// ----------------

		public static void* Alloc (
			Size size
		) {
			var data = ExternalLibrary.Kernel32.HeapAlloc(MemoryHelper.Heap, 0x00000008, (UIntPtr)size);
			if (data == null) {
				throw new OutOfMemoryException();
			}
			return data;
		}

		public static void Free (
			void* data
		) {
			var state = ExternalLibrary.Kernel32.HeapFree(MemoryHelper.Heap, 0x00000000, data);
			if (state == false) {
				throw new InvalidOperationException();
			}
			return;
		}

		// ----------------

		public static void Copy (
			void* source,
			void* destination,
			Size  size
		) {
			var sourcePointer = (Byte*)source;
			var destinationPointer = (Byte*)destination;
			if (sourcePointer > destinationPointer) {
				for (; size != 0; size--) {
					*destinationPointer++ = *sourcePointer++;
				}
			} else if (sourcePointer < destinationPointer) {
				for (sourcePointer += size, destinationPointer += size; size != 0; size--) {
					*--destinationPointer = *--sourcePointer;
				}
			}
			return;
		}

		#endregion

	}

}
