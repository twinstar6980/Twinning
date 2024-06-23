#pragma warning disable 0,
// ReSharper disable

using AssistantPlus;

namespace AssistantPlus.Utility {

	public static unsafe class MemoryHelper {

		#region utility

		private static readonly IntPtr Heap = PlatformInvoke.Kernel32.GetProcessHeap();

		// ----------------

		public static void* Allocate (
			Size size
		) {
			var data = PlatformInvoke.Kernel32.HeapAlloc(MemoryHelper.Heap, 0x00000008, (UIntPtr)size);
			GF.AssertTest(data != IntPtr.Zero);
			return (void*)data;
		}

		public static void Free (
			void* data
		) {
			var state = PlatformInvoke.Kernel32.HeapFree(MemoryHelper.Heap, 0x00000000, (IntPtr)data);
			GF.AssertTest(state);
			return;
		}

		// ----------------

		public static TValue* Allocate<TValue> (
			Size size = 1
		)
			where TValue : unmanaged {
			return (TValue*)MemoryHelper.Allocate(size * sizeof(TValue));
		}

		public static void Free<TValue> (
			TValue* data
		)
			where TValue : unmanaged {
			MemoryHelper.Free((void*)data);
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
			}
			else if (sourcePointer < destinationPointer) {
				for (sourcePointer += size, destinationPointer += size; size != 0; size--) {
					*--destinationPointer = *--sourcePointer;
				}
			}
			return;
		}

		#endregion

	}

}
