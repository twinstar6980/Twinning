import '/common.dart';
import '/utility/finalizer.dart';
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

  // #region utility

  static Future<List<String>> launch(
    Client       client,
    Library      library,
    String       script,
    List<String> argument,
  ) async {
    var subWorker = (
      lib.SendPort sendPort,
    ) async => withFinalizerScope((finalizer) async {
      var portMessage = null as List<Object?>?;
      var receivePort = lib.ReceivePort();
      finalizer.add(() async {
        receivePort.close();
      });
      var streamQueue = lib.StreamQueue<Object?>(receivePort);
      finalizer.add(() async {
        await streamQueue.cancel(immediate: true);
      });
      sendPort.send(receivePort.sendPort);
      portMessage = (await streamQueue.next)!.as<List<Object?>>();
      var service = portMessage[0]!.as<Service>();
      var script = portMessage[1]!.as<String>();
      var argument = portMessage[2]!.as<List<String>>();
      var result = null as List<String>?;
      var exception = null as ({Object exception, StackTrace stack})?;
      try {
        var executorCallback = ExecutorProxy.of((
          ExecutorProxy callbackProxy,
          MessageProxy  argumentProxy,
          MessageProxy  resultProxy,
        ) => withFinalizerScopeSync((finalizer) {
          var exceptionProxy = MessageProxy();
          var state = lib.Pointer<lib.Bool>.fromAddress(0);
          var argument = lib.Pointer<Message>.fromAddress(0);
          var result = lib.Pointer<Message>.fromAddress(0);
          var exception = lib.Pointer<Message>.fromAddress(0);
          state = lib.calloc<lib.Bool>();
          argument = lib.calloc<Message>();
          result = lib.calloc<Message>();
          exception = lib.calloc<Message>();
          finalizer.add(() {
            lib.calloc.free(state);
            lib.calloc.free(argument);
            lib.calloc.free(result);
            lib.calloc.free(exception);
          });
          state.value = false;
          MessageProxy.construct(argument, argumentProxy);
          finalizer.add(() {
            MessageProxy.destruct(argument);
          });
          sendPort.send([state, argument, result, exception]);
          while (!state.value) {
            lib.sleep(.new(milliseconds: 10));
          }
          finalizer.add(() {
            MessageProxy.destruct(result);
            MessageProxy.destruct(exception);
          });
          resultProxy.value = MessageProxy.parse(result).value;
          exceptionProxy.value = MessageProxy.parse(exception).value;
          if (!exceptionProxy.value.isEmpty) {
            throw exceptionProxy.value.first;
          }
          return null as Void;
        }));
        var executorArgument = MessageProxy.of(['execute', script, ...argument]);
        var executorResult = MessageProxy();
        ExecutorProxy.parse(service.executor).value(executorCallback, executorArgument, executorResult);
        result = executorResult.value;
      }
      catch (e, s) {
        exception = (exception: e, stack: s);
      }
      sendPort.send(null);
      sendPort.send([result, exception]);
      return;
    });
    var mainWorker = (
    ) async => await withFinalizerScope((finalizer) async {
      var portMessage = null as List<Object?>?;
      var receivePort = lib.ReceivePort();
      finalizer.add(() async {
        receivePort.close();
      });
      var streamQueue = lib.StreamQueue<Object?>(receivePort);
      finalizer.add(() async {
        await streamQueue.cancel(immediate: true);
      });
      var isolate = await lib.Isolate.spawn(subWorker, receivePort.sendPort);
      finalizer.add(() async {
        isolate.kill(priority: lib.Isolate.immediate);
      });
      var sendPort = await streamQueue.next as lib.SendPort;
      var result = null as List<String>?;
      var exception = null as ({Object exception, StackTrace stack})?;
      await client.start();
      finalizer.add(() async {
        await client.finish();
      });
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
            MessageProxy.construct(callbackResult, .of(callbackResultProxy));
            MessageProxy.construct(callbackException, .new());
          }
          catch (e, s) {
            MessageProxy.construct(callbackException, .of([ConvertHelper.generateExceptionMessage(e, s).join('\n')]));
            MessageProxy.construct(callbackResult, .new());
          }
          callbackState.value = true;
        }
      }
      if (exception != null) {
        Error.throwWithStackTrace(exception.exception, exception.stack);
      }
      return result!;
    });
    return await mainWorker();
  }

  // #endregion

}
