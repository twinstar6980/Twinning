import '/common.dart';
import '/bridge/interface.dart' as Interface; // ignore: library_prefixes
import '/bridge/converter.dart';
import '/bridge/invoker.dart';
import '/bridge/library.dart';
import '/bridge/host.dart';
import 'dart:io';
import 'dart:isolate' as isolate;
import 'dart:ffi' as ffi;
import 'package:ffi/ffi.dart' as ffi;
import 'package:async/async.dart';

// ----------------

class Launcher {

  static isolate.SendPort? _mainSendPort = null;

  static List<String> _callbackProxy(
    List<String> argument,
  ) {
    var state = ffi.calloc<ffi.Bool>();
    state.value = false;
    var argumentStructure = ffi.calloc<Interface.StringList>();
    var resultStructure = ffi.calloc<Interface.StringList>();
    var exceptionStructure = ffi.calloc<Interface.String>();
    Converter.constructStringList(argumentStructure.ref, argument);
    _mainSendPort!.send([state.address, argumentStructure.address, resultStructure.address, exceptionStructure.address]);
    while (!state.value) {
      sleep(const Duration(milliseconds: 10));
    }
    var result = Converter.parseStringList(resultStructure.ref);
    var exception = Converter.parseString(exceptionStructure.ref);
    Converter.destructStringList(argumentStructure.ref);
    Converter.destructStringList(resultStructure.ref);
    Converter.destructString(exceptionStructure.ref);
    ffi.calloc.free(argumentStructure);
    ffi.calloc.free(resultStructure);
    ffi.calloc.free(exceptionStructure);
    ffi.calloc.free(state);
    if (exception.isNotEmpty) {
      throw exception.substring(1);
    }
    return result;
  }

  static Void _sub(
    isolate.SendPort mainSendPort,
  ) async {
    _mainSendPort = mainSendPort;
    var subReceivePort = isolate.ReceivePort();
    var subStreamQueue = StreamQueue<dynamic>(subReceivePort);
    mainSendPort.send(subReceivePort.sendPort);
    var result = null as String?;
    var exception = null as String?;
    var subEvent = await subStreamQueue.next as List<dynamic>;
    var kernel = subEvent[0] as String;
    var script = subEvent[1] as String;
    var argument = subEvent[2] as List<String>;
    var library = Library();
    try {
      library.open(kernel);
      result = Invoker.execute(library, _callbackProxy, script, argument);
    }
    catch (e) {
      exception = e.toString();
    }
    if (library.state()) {
      library.close();
    }
    await subStreamQueue.cancel();
    mainSendPort.send(null);
    mainSendPort.send([result, exception]);
    _mainSendPort = null;
    return;
  }

  static Future<String> _main(
    Host         host,
    String       library,
    String       script,
    List<String> argument,
  ) async {
    var mainReceivePort = isolate.ReceivePort();
    var mainStreamQueue = StreamQueue<dynamic>(mainReceivePort);
    await isolate.Isolate.spawn(_sub, mainReceivePort.sendPort);
    var subSendPort = await mainStreamQueue.next as isolate.SendPort;
    var result = null as String?;
    var exception = null as String?;
    await host.start();
    subSendPort.send([library, script, argument]);
    while (await mainStreamQueue.hasNext) {
      var mainEvent = await mainStreamQueue.next as List<dynamic>?;
      if (mainEvent == null) {
        mainEvent = await mainStreamQueue.next as List<dynamic>;
        result = mainEvent[0];
        exception = mainEvent[1];
        break;
      }
      else {
        var callbackState = ffi.Pointer<ffi.Bool>.fromAddress(mainEvent[0]);
        var callbackArgumentStructure = ffi.Pointer<Interface.StringList>.fromAddress(mainEvent[1]);
        var callbackResultStructure = ffi.Pointer<Interface.StringList>.fromAddress(mainEvent[2]);
        var callbackExceptionStructure = ffi.Pointer<Interface.String>.fromAddress(mainEvent[3]);
        try {
          var callbackResult = await host.execute(Converter.parseStringList(callbackArgumentStructure.ref));
          Converter.constructStringList(callbackResultStructure.ref, callbackResult);
          Converter.constructString(callbackExceptionStructure.ref, '');
        }
        catch (e) {
          var callbackException = e.toString();
          Converter.constructStringList(callbackResultStructure.ref, []);
          Converter.constructString(callbackExceptionStructure.ref, '!${callbackException}');
        }
        callbackState.value = true;
      }
    }
    await host.finish();
    await mainStreamQueue.cancel();
    if (exception != null) {
      throw exception;
    }
    return result!;
  }

  // ----------------

  static Future<String> launch(
    Host         host,
    String       library,
    String       script,
    List<String> argument,
  ) async {
    return await _main(host, library, script, argument);
  }

}
