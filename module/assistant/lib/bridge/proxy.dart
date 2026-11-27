import '/common.dart';
import '/utility/finalizer.dart';
import '/utility/convert_helper.dart';
import '/bridge/data.dart';
import 'dart:convert' as lib;
import 'dart:typed_data' as lib;
import 'dart:ffi' as lib;
import 'package:ffi/ffi.dart' as lib;
import 'package:flutter/foundation.dart' as lib;

// ----------------

class MessageProxy {

  // #region constructor

  List<String> value;

  // ----------------

  MessageProxy(
  ) :
    this.value = [];

  MessageProxy.of(
    List<String> value,
  ) :
    this.value = value;

  // #endregion

  // #region convert

  static MessageProxy parse(
    lib.Pointer<Message> instance,
  ) {
    assertTest(instance.ref.data != lib.nullptr && instance.ref.size != 0);
    var proxy = MessageProxy();
    var dataPosition = 0;
    var nextInteger = () {
      var dataSize = lib.sizeOf<lib.Size>();
      assertTest(dataPosition <= dataPosition + dataSize && dataPosition + dataSize <= instance.ref.size);
      var view = (instance.ref.data + dataPosition).asTypedList(dataSize);
      var value = lib.ByteData.view(view.buffer).getUint64(0, .host);
      dataPosition += dataSize;
      return value;
    };
    var nextString = (Integer size) {
      var dataSize = lib.sizeOf<lib.Uint8>() * size;
      assertTest(dataPosition <= dataPosition + dataSize && dataPosition + dataSize <= instance.ref.size);
      var view = (instance.ref.data + dataPosition).asTypedList(dataSize);
      var value = lib.Uint8List.fromList(view);
      dataPosition += dataSize;
      return value;
    };
    var value = <lib.Uint8List>[];
    var valueSize = nextInteger();
    for (var valueIndex = 0; valueIndex < valueSize; valueIndex++) {
      var valueItemSize = nextInteger();
      var valueItem = nextString(valueItemSize);
      value.add(valueItem);
    }
    assertTest(dataPosition == instance.ref.size);
    proxy.value = value.map((it) => lib.utf8.decode(it)).toList();
    return proxy;
  }

  static Void construct(
    lib.Pointer<Message> instance,
    MessageProxy         proxy,
  ) {
    assertTest(instance.ref.data == lib.nullptr && instance.ref.size == 0);
    var dataPosition = 0;
    var nextInteger = (Integer value) {
      var dataSize = lib.sizeOf<lib.Size>();
      assertTest(dataPosition <= dataPosition + dataSize && dataPosition + dataSize <= instance.ref.size);
      var view = (instance.ref.data + dataPosition).asTypedList(dataSize);
      lib.ByteData.view(view.buffer).setUint64(0, value, .host);
      dataPosition += dataSize;
      return null as Void;
    };
    var nextString = (lib.Uint8List value) {
      var dataSize = lib.sizeOf<lib.Uint8>() * value.length;
      assertTest(dataPosition <= dataPosition + dataSize && dataPosition + dataSize <= instance.ref.size);
      var view = (instance.ref.data + dataPosition).asTypedList(dataSize);
      view.setAll(0, value);
      dataPosition += dataSize;
      return null as Void;
    };
    var value = proxy.value.map((it) => lib.utf8.encode(it)).toList();
    var dataSize = 0;
    dataSize += lib.sizeOf<lib.Size>();
    for (var valueItem in value) {
      dataSize += lib.sizeOf<lib.Size>();
      dataSize += lib.sizeOf<lib.Uint8>() * valueItem.length;
    }
    instance.ref.data = lib.calloc.call<lib.Uint8>(dataSize);
    instance.ref.size = dataSize;
    var valueSize = value.length;
    nextInteger(valueSize);
    for (var valueIndex = 0; valueIndex < valueSize; valueIndex++) {
      var valueItem = value[valueIndex];
      var valueItemSize = valueItem.length;
      nextInteger(valueItemSize);
      nextString(valueItem);
    }
    assertTest(dataPosition == instance.ref.size);
    return;
  }

  static Void destruct(
    lib.Pointer<Message> instance,
  ) {
    assertTest(instance.ref.data != lib.nullptr && instance.ref.size != 0);
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
  ) :
    this.value = ((_, _, _) => throw UnimplementedException());

  ExecutorProxy.of(
    Void Function(ExecutorProxy callback, MessageProxy argument, MessageProxy result) value,
  ) :
    this.value = value;

  // #endregion

  // #region convert

  static final Map<lib.Pointer<Executor>, ({lib.NativeCallable invoke, lib.NativeCallable clear})> _guard = {};

  // ----------------

  static ExecutorProxy parse(
    lib.Pointer<Executor> instance,
  ) {
    assertTest(instance.ref.invoke != lib.nullptr && instance.ref.clear != lib.nullptr);
    var proxy = ExecutorProxy();
    proxy.value = (callbackProxy, argumentProxy, resultProxy) => withFinalizerScopeSync((finalizer) {
      var exceptionProxy = MessageProxy();
      var callback = lib.Pointer<Executor>.fromAddress(0);
      var argument = lib.Pointer<Message>.fromAddress(0);
      var result = lib.Pointer<Message>.fromAddress(0);
      var exception = lib.Pointer<Message>.fromAddress(0);
      callback = lib.calloc.call<Executor>();
      argument = lib.calloc.call<Message>();
      result = lib.calloc.call<Message>();
      exception = lib.calloc.call<Message>();
      finalizer.add(() {
        lib.calloc.free(callback);
        lib.calloc.free(argument);
        lib.calloc.free(result);
        lib.calloc.free(exception);
      });
      ExecutorProxy.construct(callback, callbackProxy);
      finalizer.add(() {
        ExecutorProxy.destruct(callback);
      });
      MessageProxy.construct(argument, argumentProxy);
      finalizer.add(() {
        MessageProxy.destruct(argument);
      });
      instance.ref.invoke.asFunction<Void Function(lib.Pointer<Executor> self, lib.Pointer<Executor> callback, lib.Pointer<Message> argument, lib.Pointer<Message> result, lib.Pointer<Message> exception)>()(instance, callback, argument, result, exception);
      finalizer.add(() {
        instance.ref.clear.asFunction<Void Function(lib.Pointer<Executor> self, lib.Pointer<Executor> callback, lib.Pointer<Message> argument, lib.Pointer<Message> result, lib.Pointer<Message> exception)>()(instance, callback, argument, result, exception);
      });
      resultProxy.value = MessageProxy.parse(result).value;
      exceptionProxy.value = MessageProxy.parse(exception).value;
      if (!exceptionProxy.value.isEmpty) {
        throw exceptionProxy.value.first;
      }
      return;
    });
    return proxy;
  }

  static Void construct(
    lib.Pointer<Executor> instance,
    ExecutorProxy         proxy,
  ) {
    assertTest(instance.ref.invoke == lib.nullptr && instance.ref.clear == lib.nullptr);
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
        var resultProxy = MessageProxy();
        proxy.value(callbackProxy, argumentProxy, resultProxy);
        MessageProxy.construct(result, resultProxy);
        MessageProxy.construct(exception, .new());
      }
      catch (e, s) {
        MessageProxy.construct(exception, .of([ConvertHelper.generateExceptionMessage(e, s).join('\n')]));
        MessageProxy.construct(result, .new());
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
    assertTest(instance.ref.invoke != lib.nullptr && instance.ref.clear != lib.nullptr);
    assertTest(ExecutorProxy._guard.containsKey(instance));
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
