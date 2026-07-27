import '/common.dart';
import '/utility/convert_helper.dart';
import '/bridge/data.dart';
import '/bridge/proxy.dart';
import '/bridge/service.dart';
import '/bridge/library.dart';
import '/bridge/client.dart';
import 'dart:io' as lib;
import 'dart:isolate' as lib;
import 'dart:ffi' as lib;
import 'package:ffi/ffi.dart' as lib;
import 'package:async/async.dart' as lib;

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
      lib.SendPort sendPort,
    ) async {
      var portMessage = null as List<Object?>?;
      var receivePort = lib.ReceivePort();
      var streamQueue = lib.StreamQueue<Object?>(receivePort);
      sendPort.send(receivePort.sendPort);
      portMessage = (await streamQueue.next)!.as<List<Object?>>();
      var callbackProxy = (
        ExecutorProxy callbackProxy,
        MessageProxy  argumentProxy,
        MessageProxy  resultProxy,
      ) {
        var state = lib.calloc<lib.Bool>();
        state.value = false;
        var exceptionProxy = MessageProxy(null);
        var argument = lib.calloc<Message>();
        var result = lib.calloc<Message>();
        var exception = lib.calloc<Message>();
        MessageProxy.construct(argument, argumentProxy);
        sendPort.send([state, argument, result, exception]);
        while (!state.value) {
          lib.sleep(.new(milliseconds: 10));
        }
        resultProxy.value = MessageProxy.parse(result).value;
        exceptionProxy.value = MessageProxy.parse(exception).value;
        MessageProxy.destruct(argument);
        MessageProxy.destruct(result);
        MessageProxy.destruct(exception);
        lib.calloc.free(argument);
        lib.calloc.free(result);
        lib.calloc.free(exception);
        lib.calloc.free(state);
        if (!exceptionProxy.value.isEmpty) {
          throw exceptionProxy.value.first;
        }
        return null as Void;
      };
      var service = portMessage[0]!.as<Service>();
      var script = portMessage[1]!.as<String>();
      var argument = portMessage[2]!.as<List<String>>();
      var result = null as List<String>?;
      var exception = null as ({Object exception, StackTrace stack})?;
      try {
        var executorCallback = ExecutorProxy(callbackProxy);
        var executorArgument = MessageProxy(['execute', script, ...argument]);
        var executorResult = MessageProxy(null);
        ExecutorProxy.parse(service.executor).value(executorCallback, executorArgument, executorResult);
        result = executorResult.value;
      }
      catch (e, s) {
        exception = (exception: e, stack: s);
      }
      await streamQueue.cancel(immediate: true);
      receivePort.close();
      sendPort.send(null);
      sendPort.send([result, exception]);
      return;
    };
    var mainWorker = (
    ) async {
      var portMessage = null as List<Object?>?;
      var receivePort = lib.ReceivePort();
      var streamQueue = lib.StreamQueue<Object?>(receivePort);
      var isolate = await lib.Isolate.spawn(subWorker, receivePort.sendPort);
      var sendPort = await streamQueue.next as lib.SendPort;
      var result = null as List<String>?;
      var exception = null as ({Object exception, StackTrace stack})?;
      await client.start();
      sendPort.send([library.symbol(), script, argument]);
      while (await streamQueue.hasNext) {
        portMessage = (await streamQueue.next)?.as<List<Object?>>();
        if (portMessage == null) {
          portMessage = (await streamQueue.next)!.as<List<Object?>>();
          result = portMessage[0]?.as<List<String>>();
          exception = portMessage[1]?.as<({Object exception, StackTrace stack})>();
          break;
        }
        else {
          var callbackState = portMessage[0]!.as<lib.Pointer<lib.Bool>>();
          var callbackArgument = portMessage[1]!.as<lib.Pointer<Message>>();
          var callbackResult = portMessage[2]!.as<lib.Pointer<Message>>();
          var callbackException = portMessage[3]!.as<lib.Pointer<Message>>();
          try {
            var callbackResultProxy = await client.handle(MessageProxy.parse(callbackArgument).value);
            MessageProxy.construct(callbackResult, .new(callbackResultProxy));
            MessageProxy.construct(callbackException, .new([]));
          }
          catch (e, s) {
            MessageProxy.construct(callbackException, .new([ConvertHelper.generateExceptionMessage(e, s).join('\n')]));
            MessageProxy.construct(callbackResult, .new([]));
          }
          callbackState.value = true;
        }
      }
      await client.finish();
      await streamQueue.cancel(immediate: true);
      receivePort.close();
      isolate.kill(priority: lib.Isolate.immediate);
      if (exception != null) {
        Error.throwWithStackTrace(exception.exception, exception.stack);
      }
      return result!;
    };
    return await mainWorker();
  }

  // #endregion

}
