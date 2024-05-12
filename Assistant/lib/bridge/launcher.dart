import '/common.dart';
import '/bridge/data.dart';
import '/bridge/proxy.dart';
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
    String       library,
    String       script,
    List<String> argument,
  ) async {
    var subWorker = (
      SendPort sendPort,
    ) async {
      var receivePort = ReceivePort();
      var streamQueue = StreamQueue<dynamic>(receivePort);
      sendPort.send(receivePort.sendPort);
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
        sendPort.send([state.address, argument.address, result.address, exception.address]);
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
      var exception = null as String?;
      var currentMessage = await streamQueue.next as List<dynamic>;
      var kernel = currentMessage[0] as String;
      var script = currentMessage[1] as String;
      var argument = currentMessage[2] as List<String>;
      var library = Library();
      try {
        library.open(kernel);
        var executorCallback = ExecutorProxy(callbackProxy);
        var executorArgument = MessageProxy(['execute', script, ...argument]);
        var executorResult = MessageProxy(null);
        ExecutorProxy.parse(library.symbol().executor).value(executorCallback, executorArgument, executorResult);
        result = executorResult.value;
      }
      catch (e) {
        exception = e.toString();
      }
      if (library.state()) {
        library.close();
      }
      await streamQueue.cancel();
      sendPort.send(null);
      sendPort.send([result, exception]);
      return;
    };
    var mainWorker = (
    ) async {
      var receivePort = ReceivePort();
      var streamQueue = StreamQueue<dynamic>(receivePort);
      await Isolate.spawn(subWorker, receivePort.sendPort);
      var sendPort = await streamQueue.next as SendPort;
      var result = null as List<String>?;
      var exception = null as String?;
      await client.start();
      sendPort.send([library, script, argument]);
      while (await streamQueue.hasNext) {
        var currentMessage = await streamQueue.next as List<dynamic>?;
        if (currentMessage == null) {
          currentMessage = await streamQueue.next as List<dynamic>;
          result = currentMessage[0];
          exception = currentMessage[1];
          break;
        }
        else {
          var callbackState = ffi.Pointer<ffi.Bool>.fromAddress(currentMessage[0]);
          var callbackArgument = ffi.Pointer<Message>.fromAddress(currentMessage[1]);
          var callbackResult = ffi.Pointer<Message>.fromAddress(currentMessage[2]);
          var callbackException = ffi.Pointer<Message>.fromAddress(currentMessage[3]);
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
