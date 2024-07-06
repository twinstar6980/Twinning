import '/common.dart';
import '/bridge/data.dart';
import '/bridge/proxy.dart';
import '/bridge/service.dart';
import '/bridge/library.dart';
import '/bridge/client.dart';
import 'dart:io';
import 'dart:isolate';
import 'dart:ffi' as ffi;
import 'package:ffi/ffi.dart' as ffi;
import 'package:async/async.dart';

// ----------------

class Launcher {

  // #region function

  static Future<List<String>> launch(
    Client       client,
    Library      library,
    String       script,
    List<String> argument,
  ) async {
    var subWorker = (
      SendPort sendPort,
    ) async {
      var portMessage = null as List<dynamic>?;
      var receivePort = ReceivePort();
      var streamQueue = StreamQueue<dynamic>(receivePort);
      sendPort.send(receivePort.sendPort);
      portMessage = await streamQueue.next as List<dynamic>;
      var callbackProxy = (
        ExecutorProxy callbackProxy,
        MessageProxy  argumentProxy,
        MessageProxy  resultProxy,
      ) {
        var state = ffi.calloc<ffi.Bool>();
        state.value = false;
        var exceptionProxy = MessageProxy(null);
        var argument = ffi.calloc<Message>();
        var result = ffi.calloc<Message>();
        var exception = ffi.calloc<Message>();
        MessageProxy.construct(argument, argumentProxy);
        sendPort.send([state, argument, result, exception]);
        while (!state.value) {
          sleep(const Duration(milliseconds: 10));
        }
        resultProxy.value = MessageProxy.parse(result).value;
        exceptionProxy.value = MessageProxy.parse(exception).value;
        MessageProxy.destruct(argument);
        MessageProxy.destruct(result);
        MessageProxy.destruct(exception);
        ffi.calloc.free(argument);
        ffi.calloc.free(result);
        ffi.calloc.free(exception);
        ffi.calloc.free(state);
        if (exceptionProxy.value.isNotEmpty) {
          throw exceptionProxy.value.first;
        }
        return null as Void;
      };
      var result = null as List<String>?;
      var exception = null as Object?;
      var service = portMessage[0] as Service;
      var script = portMessage[1] as String;
      var argument = portMessage[2] as List<String>;
      try {
        var executorCallback = ExecutorProxy(callbackProxy);
        var executorArgument = MessageProxy(['execute', script, ...argument]);
        var executorResult = MessageProxy(null);
        ExecutorProxy.parse(service.executor).value(executorCallback, executorArgument, executorResult);
        result = executorResult.value;
      }
      catch (e) {
        exception = e;
      }
      await streamQueue.cancel();
      sendPort.send(null);
      sendPort.send([result, exception]);
      return;
    };
    var mainWorker = (
    ) async {
      var portMessage = null as List<dynamic>?;
      var receivePort = ReceivePort();
      var streamQueue = StreamQueue<dynamic>(receivePort);
      await Isolate.spawn(subWorker, receivePort.sendPort);
      var sendPort = await streamQueue.next as SendPort;
      var result = null as List<String>?;
      var exception = null as Object?;
      await client.start();
      sendPort.send([library.symbol(), script, argument]);
      while (await streamQueue.hasNext) {
        portMessage = await streamQueue.next as List<dynamic>?;
        if (portMessage == null) {
          portMessage = await streamQueue.next as List<dynamic>;
          result = portMessage[0];
          exception = portMessage[1];
          break;
        }
        else {
          var callbackState = portMessage[0] as ffi.Pointer<ffi.Bool>;
          var callbackArgument = portMessage[1] as ffi.Pointer<Message>;
          var callbackResult = portMessage[2] as ffi.Pointer<Message>;
          var callbackException = portMessage[3] as ffi.Pointer<Message>;
          try {
            var callbackResultProxy = await client.callback(MessageProxy.parse(callbackArgument).value);
            MessageProxy.construct(callbackResult, MessageProxy(callbackResultProxy));
            MessageProxy.construct(callbackException, MessageProxy([]));
          }
          catch (e) {
            MessageProxy.construct(callbackResult, MessageProxy([]));
            MessageProxy.construct(callbackException, MessageProxy([e.toString()]));
          }
          callbackState.value = true;
        }
      }
      await client.finish();
      await streamQueue.cancel();
      if (exception != null) {
        throw exception;
      }
      return result!;
    };
    return await mainWorker();
  }

  // #endregion

}
