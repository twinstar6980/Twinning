import '/common.dart';
import '/utility/convert_helper.dart';
import '/bridge/data.dart';
import 'dart:convert' as lib;
import 'dart:ffi' as lib;
import 'package:ffi/ffi.dart' as lib;

// ----------------

class MessageProxy {

  // #region constructor

  List<String> value;

  // ----------------

  MessageProxy(
    List<String>? value,
  ) :
    this.value = value ?? [];

  // #endregion

  // #region convert

  static MessageProxy parse(
    lib.Pointer<Message> instance,
  ) {
    var proxy = MessageProxy(null);
    var value = proxy.value;
    var dataPosition = 0;
    var valueSize = (instance.ref.data + dataPosition).cast<lib.Uint32>().value;
    dataPosition += lib.sizeOf<lib.Uint32>();
    for (var valueIndex = 0; valueIndex < valueSize; valueIndex++) {
      var valueItemSize = (instance.ref.data + dataPosition).cast<lib.Uint32>().value;
      dataPosition += lib.sizeOf<lib.Uint32>();
      var valueItem = lib.utf8.decode((instance.ref.data + dataPosition).cast<lib.Uint8>().asTypedList(valueItemSize));
      dataPosition += lib.sizeOf<lib.Uint8>() * valueItemSize;
      value.add(valueItem);
      var dataPadding = dataPosition % lib.sizeOf<lib.Uint32>();
      if (dataPadding != 0) {
        dataPadding = lib.sizeOf<lib.Uint32>() - dataPadding;
      }
      dataPosition += dataPadding;
    }
    assertTest(dataPosition == instance.ref.size);
    return proxy;
  }

  static Void construct(
    lib.Pointer<Message> instance,
    MessageProxy         proxy,
  ) {
    var value = proxy.value.map((value) => lib.utf8.encode(value)).toList();
    var dataSize = 0;
    dataSize += lib.sizeOf<lib.Uint32>();
    for (var valueItem in value) {
      dataSize += lib.sizeOf<lib.Uint32>();
      dataSize += lib.sizeOf<lib.Uint8>() * valueItem.length;
      var dataPadding = dataSize % lib.sizeOf<lib.Uint32>();
      if (dataPadding != 0) {
        dataPadding = lib.sizeOf<lib.Uint32>() - dataPadding;
      }
      dataSize += dataPadding;
    }
    instance.ref.data = lib.calloc.call<lib.Uint8>(dataSize);
    instance.ref.size = dataSize;
    var dataPosition = 0;
    var valueSize = value.length;
    (instance.ref.data + dataPosition).cast<lib.Uint32>().value = valueSize;
    dataPosition += lib.sizeOf<lib.Uint32>();
    for (var valueIndex = 0; valueIndex < valueSize; valueIndex++) {
      var valueItem = value[valueIndex];
      var valueItemSize = valueItem.length;
      (instance.ref.data + dataPosition).cast<lib.Uint32>().value = valueItemSize;
      dataPosition += lib.sizeOf<lib.Uint32>();
      (instance.ref.data + dataPosition).cast<lib.Uint8>().asTypedList(valueItemSize).setAll(0, valueItem);
      dataPosition += lib.sizeOf<lib.Uint8>() * valueItemSize;
      var dataPadding = dataPosition % lib.sizeOf<lib.Uint32>();
      if (dataPadding != 0) {
        dataPadding = lib.sizeOf<lib.Uint32>() - dataPadding;
      }
      dataPosition += dataPadding;
    }
    assertTest(dataPosition == instance.ref.size);
    return;
  }

  static Void destruct(
    lib.Pointer<Message> instance,
  ) {
    lib.calloc.free(instance.ref.data);
    instance.ref.data = lib.nullptr;
    instance.ref.size = 0;
    return;
  }

  // #endregion

}

class ExecutorProxy {

  // #region constructor

  Void Function(ExecutorProxy callback, MessageProxy argument, MessageProxy result) value;

  // ----------------

  ExecutorProxy(
    Void Function(ExecutorProxy callback, MessageProxy argument, MessageProxy result)? value,
  ) :
    this.value = value ?? ((_, _, _) => throw UnimplementedException());

  // #endregion

  // #region convert

  static final Map<lib.Pointer<Executor>, ({lib.NativeCallable invoke, lib.NativeCallable clear})> _guard = {};

  // ----------------

  static ExecutorProxy parse(
    lib.Pointer<Executor> instance,
  ) {
    var proxy = ExecutorProxy(null);
    proxy.value = (callbackProxy, argumentProxy, resultProxy) {
      var exceptionProxy = MessageProxy(null);
      var callback = lib.Pointer<Executor>.fromAddress(0);
      var argument = lib.Pointer<Message>.fromAddress(0);
      var result = lib.Pointer<Message>.fromAddress(0);
      var exception = lib.Pointer<Message>.fromAddress(0);
      {
        callback = lib.calloc.call<Executor>();
        argument = lib.calloc.call<Message>();
        result = lib.calloc.call<Message>();
        exception = lib.calloc.call<Message>();
      }
      {
        ExecutorProxy.construct(callback, callbackProxy);
        MessageProxy.construct(argument, argumentProxy);
      }
      {
        instance.ref.invoke.asFunction<Void Function(lib.Pointer<Executor> self, lib.Pointer<Executor> callback, lib.Pointer<Message> argument, lib.Pointer<Message> result, lib.Pointer<Message> exception)>()(instance, callback, argument, result, exception);
        resultProxy.value = MessageProxy.parse(result).value;
        exceptionProxy.value = MessageProxy.parse(exception).value;
        instance.ref.clear.asFunction<Void Function(lib.Pointer<Executor> self, lib.Pointer<Executor> callback, lib.Pointer<Message> argument, lib.Pointer<Message> result, lib.Pointer<Message> exception)>()(instance, callback, argument, result, exception);
      }
      {
        ExecutorProxy.destruct(callback);
        MessageProxy.destruct(argument);
      }
      {
        lib.calloc.free(callback);
        lib.calloc.free(argument);
        lib.calloc.free(result);
        lib.calloc.free(exception);
      }
      if (!exceptionProxy.value.isEmpty) {
        throw exceptionProxy.value.first;
      }
      return;
    };
    return proxy;
  }

  static Void construct(
    lib.Pointer<Executor> instance,
    ExecutorProxy         proxy,
  ) {
    assertTest(!ExecutorProxy._guard.containsKey(instance));
    var guardForInvoke = lib.NativeCallable<lib.Void Function(lib.Pointer<Executor> self, lib.Pointer<Executor> callback, lib.Pointer<Message> argument, lib.Pointer<Message> result, lib.Pointer<Message> exception)>.isolateLocal((
      lib.Pointer<Executor> self,
      lib.Pointer<Executor> callback,
      lib.Pointer<Message>  argument,
      lib.Pointer<Message>  result,
      lib.Pointer<Message>  exception,
    ) {
      try {
        var callbackProxy = ExecutorProxy.parse(callback);
        var argumentProxy = MessageProxy.parse(argument);
        var resultProxy = MessageProxy(null);
        proxy.value(callbackProxy, argumentProxy, resultProxy);
        MessageProxy.construct(result, resultProxy);
        MessageProxy.construct(exception, .new([]));
      }
      catch (e, s) {
        MessageProxy.construct(exception, .new([ConvertHelper.generateExceptionMessage(e, s).join('\n')]));
        MessageProxy.construct(result, .new([]));
      }
      return null as Void;
    });
    var guardForClear = lib.NativeCallable<lib.Void Function(lib.Pointer<Executor> self, lib.Pointer<Executor> callback, lib.Pointer<Message> argument, lib.Pointer<Message> result, lib.Pointer<Message> exception)>.isolateLocal((
      lib.Pointer<Executor> self,
      lib.Pointer<Executor> callback,
      lib.Pointer<Message>  argument,
      lib.Pointer<Message>  result,
      lib.Pointer<Message>  exception,
    ) {
      if (result != lib.nullptr) {
        MessageProxy.destruct(result);
      }
      if (exception != lib.nullptr) {
        MessageProxy.destruct(exception);
      }
      return null as Void;
    });
    ExecutorProxy._guard[instance] = (invoke: guardForInvoke, clear: guardForClear);
    instance.ref.invoke = guardForInvoke.nativeFunction;
    instance.ref.clear = guardForClear.nativeFunction;
    return;
  }

  static Void destruct(
    lib.Pointer<Executor> instance,
  ) {
    var guard = ExecutorProxy._guard[instance]!;
    guard.invoke.close();
    guard.clear.close();
    assertTest(ExecutorProxy._guard.remove(instance) != null);
    instance.ref.invoke = lib.nullptr;
    instance.ref.clear = lib.nullptr;
    return;
  }

  // #endregion

}
