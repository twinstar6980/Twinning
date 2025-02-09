#pragma warning disable 0,
// ReSharper disable

using AssistantPlus;
using AssistantPlus.Utility;

namespace AssistantPlus.Bridge {

	public class MessageProxy {

		#region structor

		public List<String> Value;

		// ----------------

		public MessageProxy (
			List<String>? value = null
		) {
			this.Value = value ?? [];
		}

		#endregion

		#region convert

		public static unsafe MessageProxy Parse (
			Message* instance
		) {
			var proxy = new MessageProxy();
			var value = proxy.Value;
			var dataPosition = 0;
			var valueSize = (Size)(*(IntegerU32*)(instance->data + dataPosition));
			dataPosition += sizeof(IntegerU32);
			value.Capacity = valueSize;
			for (var valueIndex = 0; valueIndex < valueSize; valueIndex++) {
				var valueItemSize = (Size)(*(IntegerU32*)(instance->data + dataPosition));
				dataPosition += sizeof(IntegerU32);
				var valueItem = Encoding.UTF8.GetString(instance->data + dataPosition, valueItemSize);
				dataPosition += sizeof(IntegerU8) * valueItemSize;
				value.Add(valueItem);
				var dataPadding = dataPosition % sizeof(IntegerU32);
				if (dataPadding != 0) {
					dataPadding = sizeof(IntegerU32) - dataPadding;
				}
				dataPosition += dataPadding;
			}
			GF.AssertTest(dataPosition == (Size)instance->size);
			return proxy;
		}

		public static unsafe void Construct (
			Message*     instance,
			MessageProxy proxy
		) {
			var value = proxy.Value.Select((value) => (Encoding.UTF8.GetBytes(value.ToCharArray()))).ToList();
			var dataSize = 0;
			dataSize += sizeof(IntegerU32);
			foreach (var valueItem in value) {
				dataSize += sizeof(IntegerU32);
				dataSize += sizeof(IntegerU8) * valueItem.Length;
				var dataPadding = dataSize % sizeof(IntegerU32);
				if (dataPadding != 0) {
					dataPadding = sizeof(IntegerU32) - dataPadding;
				}
				dataSize += dataPadding;
			}
			instance->data = MemoryHelper.Allocate<IntegerU8>(dataSize);
			instance->size = (IntegerU32)dataSize;
			var dataPosition = 0;
			var valueSize = value.Count;
			*(IntegerU32*)(instance->data + dataPosition) = (IntegerU32)valueSize;
			dataPosition += sizeof(IntegerU32);
			for (var valueIndex = 0; valueIndex < valueSize; valueIndex++) {
				var valueItem = value[valueIndex];
				var valueItemSize = valueItem.Length;
				*(IntegerU32*)(instance->data + dataPosition) = (IntegerU32)valueItemSize;
				dataPosition += sizeof(IntegerU32);
				fixed (Byte* valueItemPointer = valueItem) {
					MemoryHelper.Copy(valueItemPointer, instance->data + dataPosition, valueItemSize);
				}
				dataPosition += sizeof(IntegerU8) * valueItemSize;
				var dataPadding = dataPosition % sizeof(IntegerU32);
				if (dataPadding != 0) {
					dataPadding = sizeof(IntegerU32) - dataPadding;
				}
				dataPosition += dataPadding;
			}
			GF.AssertTest(dataPosition == (Size)instance->size);
			return;
		}

		public static unsafe void Destruct (
			Message* instance
		) {
			MemoryHelper.Free(instance->data);
			instance->data = null;
			instance->size = 0;
			return;
		}

		#endregion

	}

	public class ExecutorProxy {

		#region structor

		public Action<ExecutorProxy, MessageProxy, MessageProxy> Value;

		// ----------------

		public ExecutorProxy (
			Action<ExecutorProxy, MessageProxy, MessageProxy>? value = null
		) {
			this.Value = value ?? ((_, _, _) => throw new NotImplementedException());
		}

		#endregion

		#region convert

		[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
		private unsafe delegate void ExecutorDelegate (
			Executor* self,
			Executor* callback,
			Message*  argument,
			Message*  result,
			Message*  exception
		);

		private static Dictionary<IntPtr, Tuple<GCHandle, GCHandle>> sGuard = new ();

		// ----------------

		public static unsafe ExecutorProxy Parse (
			Executor* instance
		) {
			var proxy = new ExecutorProxy();
			proxy.Value = (callbackProxy, argumentProxy, resultProxy) => {
				var exceptionProxy = new MessageProxy();
				var callback = default(Executor*);
				var argument = default(Message*);
				var result = default(Message*);
				var exception = default(Message*);
				{
					callback = MemoryHelper.Allocate<Executor>();
					argument = MemoryHelper.Allocate<Message>();
					result = MemoryHelper.Allocate<Message>();
					exception = MemoryHelper.Allocate<Message>();
				}
				{
					ExecutorProxy.Construct(callback, callbackProxy);
					MessageProxy.Construct(argument, argumentProxy);
				}
				{
					instance->invoke(instance, callback, argument, result, exception);
					resultProxy.Value = MessageProxy.Parse(result).Value;
					exceptionProxy.Value = MessageProxy.Parse(exception).Value;
					instance->clear(instance, callback, argument, result, exception);
				}
				{
					ExecutorProxy.Destruct(callback);
					MessageProxy.Destruct(argument);
				}
				{
					MemoryHelper.Free(callback);
					MemoryHelper.Free(argument);
					MemoryHelper.Free(result);
					MemoryHelper.Free(exception);
				}
				if (exceptionProxy.Value.Count != 0) {
					throw new (exceptionProxy.Value.First());
				}
				return;
			};
			return proxy;
		}

		public static unsafe void Construct (
			Executor*     instance,
			ExecutorProxy proxy
		) {
			var guardForInvoke = GCHandle.Alloc((ExecutorDelegate)(void (
				Executor* self,
				Executor* callback,
				Message*  argument,
				Message*  result,
				Message*  exception
			) => {
				try {
					var callbackProxy = ExecutorProxy.Parse(callback);
					var argumentProxy = MessageProxy.Parse(argument);
					var resultProxy = new MessageProxy();
					proxy.Value(callbackProxy, argumentProxy, resultProxy);
					MessageProxy.Construct(result, resultProxy);
					MessageProxy.Construct(exception, new ([]));
				}
				catch (Exception e) {
					MessageProxy.Construct(exception, new ([e.ToString()]));
					MessageProxy.Construct(result, new ([]));
				}
				return;
			}));
			var guardForClear = GCHandle.Alloc((ExecutorDelegate)(void (
				Executor* self,
				Executor* callback,
				Message*  argument,
				Message*  result,
				Message*  exception
			) => {
				if (result != null) {
					MessageProxy.Destruct(result);
				}
				if (exception != null) {
					MessageProxy.Destruct(exception);
				}
				return;
			}));
			GF.AssertTest(!ExecutorProxy.sGuard.ContainsKey((IntPtr)instance));
			ExecutorProxy.sGuard.Add((IntPtr)instance, new (guardForInvoke, guardForClear));
			instance->invoke = (delegate* unmanaged<Executor*, Executor*, Message*, Message*, Message*, void>)Marshal.GetFunctionPointerForDelegate(guardForInvoke.Target.AsNotNull());
			instance->clear = (delegate* unmanaged<Executor*, Executor*, Message*, Message*, Message*, void>)Marshal.GetFunctionPointerForDelegate(guardForClear.Target.AsNotNull());
			return;
		}

		public static unsafe void Destruct (
			Executor* instance
		) {
			var guard = ExecutorProxy.sGuard[(IntPtr)instance];
			guard.Item1.Free();
			guard.Item2.Free();
			GF.AssertTest(ExecutorProxy.sGuard.Remove((IntPtr)instance));
			instance->invoke = null;
			instance->clear = null;
			return;
		}

		#endregion

	}

}
