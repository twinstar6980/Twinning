import '/common.dart';
import '/bridge/data.dart';
import 'dart:convert';
import 'dart:ffi' as ffi;
import 'package:ffi/ffi.dart' as ffi;

// ----------------

class MessageProxy {

  // #region structor

  List<String> value;

  // ----------------

  MessageProxy(
    List<String>? value,
  ) :
    value = value ?? [];

  // #endregion

  // #region convert

  static MessageProxy parse(
    ffi.Pointer<Message> instance,
  ) {
    var proxy = MessageProxy(null);
    var value = proxy.value;
    var dataPosition = 0;
    var valueSize = (instance.ref.data + dataPosition).cast<ffi.Uint32>().value;
    dataPosition += ffi.sizeOf<ffi.Uint32>();
    for (var valueIndex = 0; valueIndex < valueSize; valueIndex++) {
      var valueItemSize = (instance.ref.data + dataPosition).cast<ffi.Uint32>().value;
      dataPosition += ffi.sizeOf<ffi.Uint32>();
      var valueItem = utf8.decode((instance.ref.data + dataPosition).cast<ffi.Uint8>().asTypedList(valueItemSize));
      dataPosition += ffi.sizeOf<ffi.Uint8>() * valueItemSize;
      value.add(valueItem);
      var dataPadding = dataPosition % ffi.sizeOf<ffi.Uint32>();
      if (dataPadding != 0) {
        dataPadding = ffi.sizeOf<ffi.Uint32>() - dataPadding;
      }
      dataPosition += dataPadding;
    }
    assertTest(dataPosition == instance.ref.size);
    return proxy;
  }

  static Void construct(
    ffi.Pointer<Message> instance,
    MessageProxy         proxy,
  ) {
    var value = proxy.value.map((value) => utf8.encode(value)).toList();
    var dataSize = 0;
    dataSize += ffi.sizeOf<ffi.Uint32>();
    for (var valueItem in value) {
      dataSize += ffi.sizeOf<ffi.Uint32>();
      dataSize += ffi.sizeOf<ffi.Uint8>() * valueItem.length;
      var dataPadding = dataSize % ffi.sizeOf<ffi.Uint32>();
      if (dataPadding != 0) {
        dataPadding = ffi.sizeOf<ffi.Uint32>() - dataPadding;
      }
      dataSize += dataPadding;
    }
    instance.ref.data = ffi.calloc.call<ffi.Uint8>(dataSize);
    instance.ref.size = dataSize;
    var dataPosition = 0;
    var valueSize = value.length;
    (instance.ref.data + dataPosition).cast<ffi.Uint32>().value = valueSize;
    dataPosition += ffi.sizeOf<ffi.Uint32>();
    for (var valueIndex = 0; valueIndex < valueSize; valueIndex++) {
      var valueItem = value[valueIndex];
      var valueItemSize = valueItem.length;
      (instance.ref.data + dataPosition).cast<ffi.Uint32>().value = valueItemSize;
      dataPosition += ffi.sizeOf<ffi.Uint32>();
      (instance.ref.data + dataPosition).cast<ffi.Uint8>().asTypedList(valueItemSize).setAll(0, valueItem);
      dataPosition += ffi.sizeOf<ffi.Uint8>() * valueItemSize;
      var dataPadding = dataPosition % ffi.sizeOf<ffi.Uint32>();
      if (dataPadding != 0) {
        dataPadding = ffi.sizeOf<ffi.Uint32>() - dataPadding;
      }
      dataPosition += dataPadding;
    }
    assertTest(dataPosition == instance.ref.size);
    return;
  }

  static Void destruct(
    ffi.Pointer<Message> instance,
  ) {
    ffi.calloc.free(instance.ref.data);
    instance.ref.data = ffi.nullptr;
    instance.ref.size = 0;
    return;
  }

  // #endregion

}

class ExecutorProxy {

  // #region structor

  Void Function(ExecutorProxy, MessageProxy, MessageProxy) value;

  // ----------------

  ExecutorProxy(
    Void Function(ExecutorProxy, MessageProxy, MessageProxy)? value,
  ) :
    value = value ?? ((_, __, ___) => throw UnimplementedError());

  // #endregion

  // #region convert

  static Map<ffi.Pointer<Executor>, (ffi.NativeCallable, ffi.NativeCallable)> _guard = {};

  // ----------------

  static ExecutorProxy parse(
    ffi.Pointer<Executor> instance,
  ) {
    var proxy = ExecutorProxy(null);
    proxy.value = (callbackProxy, argumentProxy, resultProxy) {
      var exceptionProxy = MessageProxy(null);
      var callback = ffi.Pointer<Executor>.fromAddress(0);
      var argument = ffi.Pointer<Message>.fromAddress(0);
      var result = ffi.Pointer<Message>.fromAddress(0);
      var exception = ffi.Pointer<Message>.fromAddress(0);
      {
        callback = ffi.calloc.call<Executor>();
        argument = ffi.calloc.call<Message>();
        result = ffi.calloc.call<Message>();
        exception = ffi.calloc.call<Message>();
      }
      {
        ExecutorProxy.construct(callback, callbackProxy);
        MessageProxy.construct(argument, argumentProxy);
      }
      {
        instance.ref.invoke.asFunction<Void Function(ffi.Pointer<Executor>, ffi.Pointer<Executor>, ffi.Pointer<Message>, ffi.Pointer<Message>, ffi.Pointer<Message>)>()(instance, callback, argument, result, exception);
        resultProxy.value = MessageProxy.parse(result).value;
        exceptionProxy.value = MessageProxy.parse(exception).value;
        instance.ref.clear.asFunction<Void Function(ffi.Pointer<Executor>, ffi.Pointer<Executor>, ffi.Pointer<Message>, ffi.Pointer<Message>, ffi.Pointer<Message>)>()(instance, callback, argument, result, exception);
      }
      {
        ExecutorProxy.destruct(callback);
        MessageProxy.destruct(argument);
      }
      {
        ffi.calloc.free(callback);
        ffi.calloc.free(argument);
        ffi.calloc.free(result);
        ffi.calloc.free(exception);
      }
      if (exceptionProxy.value.length != 0) {
        throw exceptionProxy.value.first;
      }
      return;
    };
    return proxy;
  }

  static void construct(
    ffi.Pointer<Executor> instance,
    ExecutorProxy         proxy,
  ) {
    assertTest(!ExecutorProxy._guard.containsKey(instance));
    var guardForInvoke = ffi.NativeCallable<ffi.Void Function(ffi.Pointer<Executor>, ffi.Pointer<Executor>, ffi.Pointer<Message>, ffi.Pointer<Message>, ffi.Pointer<Message>)>.isolateLocal((
      ffi.Pointer<Executor> self,
      ffi.Pointer<Executor> callback,
      ffi.Pointer<Message>  argument,
      ffi.Pointer<Message>  result,
      ffi.Pointer<Message>  exception,
    ) {
      try {
        var callbackProxy = ExecutorProxy.parse(callback);
        var argumentProxy = MessageProxy.parse(argument);
        var resultProxy = MessageProxy(null);
        proxy.value(callbackProxy, argumentProxy, resultProxy);
        MessageProxy.construct(result, resultProxy);
        MessageProxy.construct(exception, MessageProxy([]));
      }
      catch (e) {
        MessageProxy.construct(exception, MessageProxy([e.toString()]));
        MessageProxy.construct(result, MessageProxy([]));
      }
      return null as Void;
    });
    var guardForClear = ffi.NativeCallable<ffi.Void Function(ffi.Pointer<Executor>, ffi.Pointer<Executor>, ffi.Pointer<Message>, ffi.Pointer<Message>, ffi.Pointer<Message>)>.isolateLocal((
      ffi.Pointer<Executor> self,
      ffi.Pointer<Executor> callback,
      ffi.Pointer<Message>  argument,
      ffi.Pointer<Message>  result,
      ffi.Pointer<Message>  exception,
    ) {
      if (result != ffi.nullptr) {
        MessageProxy.destruct(result);
      }
      if (exception != ffi.nullptr) {
        MessageProxy.destruct(exception);
      }
      return null as Void;
    });
    ExecutorProxy._guard[instance] = (guardForInvoke, guardForClear);
    instance.ref.invoke = guardForInvoke.nativeFunction;
    instance.ref.clear = guardForClear.nativeFunction;
    return;
  }

  static void destruct(
    ffi.Pointer<Executor> instance,
  ) {
    var guard = ExecutorProxy._guard[instance]!;
    guard.$1.close();
    guard.$2.close();
    assertTest(ExecutorProxy._guard.remove(instance) != null);
    instance.ref.invoke = ffi.nullptr;
    instance.ref.clear = ffi.nullptr;
    return;
  }

  // #endregion

}
