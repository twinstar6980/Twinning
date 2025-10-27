#pragma warning disable 0,
// ReSharper disable

using Twinning.AssistantPlus;

namespace Twinning.AssistantPlus.Utility {

	public static unsafe class MemoryHelper {

		#region utility

		private static readonly Win32.Foundation.HANDLE Heap = Win32.PInvoke.GetProcessHeap();

		// ----------------

		public static void* Allocate (
			Size size
		) {
			var data = Win32.PInvoke.HeapAlloc(MemoryHelper.Heap, Win32.System.Memory.HEAP_FLAGS.HEAP_ZERO_MEMORY, size.CastPrimitive<IntegerUN>());
			AssertTest(data != null);
			return data;
		}

		public static void Free (
			void* data
		) {
			var state = Win32.PInvoke.HeapFree(MemoryHelper.Heap, Win32.System.Memory.HEAP_FLAGS.HEAP_NONE, data);
			AssertTest(state);
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
