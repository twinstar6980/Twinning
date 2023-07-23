// ignore_for_file: unused_import, library_prefixes, unnecessary_cast

import '/common.dart';
import 'dart:io';
import 'dart:isolate' as isolate;
import 'dart:ffi' as ffi;
import 'package:ffi/ffi.dart' as ffi;
import 'package:async/async.dart';
import '/bridge/interface.dart' as Interface;
import '/bridge/converter.dart';
import '/bridge/invoker.dart';
import '/bridge/library.dart';
import '/bridge/host.dart';

// ----------------

class Launcher {

  static
  late isolate.SendPort _mainSendPort;

  static
  List<String>
  _callbackProxy(
    List<String> argument,
  ) {
    var state = ffi.calloc<ffi.Bool>();
    var argumentStructure = ffi.calloc<Interface.StringList>();
    var resultStructure = ffi.calloc<Interface.StringList>();
    var exceptionStructure = ffi.calloc<Interface.String>();
    state.value = false;
    Converter.constructStringList(argumentStructure.ref, argument);
    _mainSendPort.send([state.address, argumentStructure.address, resultStructure.address, exceptionStructure.address]);
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
      throw exception;
    }
    return result;
  }

  static
  Void
  _sub(
    isolate.SendPort mainSendPort,
  ) async {
    var subPort = isolate.ReceivePort();
    var subEvent = StreamQueue<dynamic>(subPort);
    _mainSendPort = mainSendPort;
    mainSendPort.send(subPort.sendPort);
    var kernel = await subEvent.next as String;
    var script = await subEvent.next as String;
    var argument = await subEvent.next as List<String>;
    var result = null as String?;
    var exception = null as String?;
    try {
      var library = Library(kernel);
      Invoker.version(library);
      Invoker.prepare(library);
      result = Invoker.execute(library, _callbackProxy, script, argument);
    } catch (e) {
      exception = e.toString();
    }
    await subEvent.cancel();
    mainSendPort.send(null);
    mainSendPort.send(exception);
    mainSendPort.send(result);
    return;
  }

  static
  Future<String>
  _main(
    Host         host,
    String       library,
    String       script,
    List<String> argument,
  ) async {
    await host.start();
    var mainPort = isolate.ReceivePort();
    var mainEvent = StreamQueue<dynamic>(mainPort);
    await isolate.Isolate.spawn(_sub, mainPort.sendPort);
    var subSendPort = await mainEvent.next as isolate.SendPort;
    subSendPort.send(library);
    subSendPort.send(script);
    subSendPort.send(argument);
    var result = null as String?;
    var exception = null as String?;
    while (await mainEvent.hasNext) {
      var message = await mainEvent.next;
      if (message == null) {
        exception = await mainEvent.next;
        result = await mainEvent.next;
        break;
      }
      assert(message is List<int> && message.length == 4);
      var callbackState = ffi.Pointer<ffi.Bool>.fromAddress(message[0]);
      var callbackArgumentStructure = ffi.Pointer<Interface.StringList>.fromAddress(message[1]);
      var callbackResultStructure = ffi.Pointer<Interface.StringList>.fromAddress(message[2]);
      var callbackExceptionStructure = ffi.Pointer<Interface.String>.fromAddress(message[3]);
      try {
        var callbackResult = await host.execute(Converter.parseStringList(callbackArgumentStructure.ref));
        Converter.constructStringList(callbackResultStructure.ref, callbackResult);
        Converter.constructString(callbackExceptionStructure.ref, '');
      } catch (e) {
        Converter.constructStringList(callbackResultStructure.ref, []);
        Converter.constructString(callbackExceptionStructure.ref, e.toString());
      }
      callbackState.value = true;
    }
    await mainEvent.cancel();
    await host.finish();
    if (exception != null) {
      throw exception;
    }
    return result!;
  }

  // ----------------

  static
  Future<String>
  launch(
    Host         host,
    String       library,
    String       script,
    List<String> argument,
  ) async {
    return await _main(host, library, script, argument);
  }

}
