// ignore_for_file: unused_import, library_prefixes

import '/common.dart';
import 'dart:ffi' as ffi;
import 'package:ffi/ffi.dart' as ffi;
import '/core/interface.dart' as Interface;
import '/core/converter.dart';
import '/core/library.dart';

// ----------------

class Invoker {

  static List<String> Function(List<String>)? _callbackTarget;

  static ffi.Pointer<Interface.StringList>?   _callbackResult;

  static
  ffi.Pointer<Interface.StringList>
  _callbackProxy(
    ffi.Pointer<Interface.StringList> argument,
  ) {
    var resultValue = List<String>.empty(growable: true);
    try {
      resultValue.clear();
      resultValue.add('');
      resultValue.addAll(_callbackTarget!(Converter.parseStringList(argument.ref)));
    } catch (e) {
      resultValue.clear();
      resultValue.add(e.toString());
    }
    Converter.destructStringList(_callbackResult!.ref);
    Converter.constructStringList(_callbackResult!.ref, resultValue);
    return _callbackResult!;
  }

  // ----------------

  static
  Integer
  version(
    Library library,
  ) {
    var resultStructure = library.version();
    return Converter.parseSize(resultStructure.ref);
  }

  static
  String?
  execute(
    Library                             library,
    List<String> Function(List<String>) callback,
    String                              script,
    List<String>                        argument,
  ) {
    _callbackTarget = callback;
    _callbackResult = ffi.calloc<Interface.StringList>();
    Converter.constructStringList(_callbackResult!.ref, []);
    var callbackStructure = ffi.calloc<Interface.Callback>();
    var scriptStructure = ffi.calloc<Interface.String>();
    var argumentStructure = ffi.calloc<Interface.StringList>();
    Converter.constructCallback(callbackStructure.ref, ffi.Pointer.fromFunction(_callbackProxy));
    Converter.constructString(scriptStructure.ref, script);
    Converter.constructStringList(argumentStructure.ref, argument);
    var resultStructure = library.execute(callbackStructure, scriptStructure, argumentStructure);
    Converter.destructCallback(callbackStructure.ref);
    Converter.destructString(scriptStructure.ref);
    Converter.destructStringList(argumentStructure.ref);
    ffi.calloc.free(callbackStructure);
    ffi.calloc.free(scriptStructure);
    ffi.calloc.free(argumentStructure);
    Converter.destructStringList(_callbackResult!.ref);
    ffi.calloc.free(_callbackResult!);
    _callbackTarget = null;
    return resultStructure == ffi.nullptr ? null : Converter.parseString(resultStructure.ref);
  }

  static
  String?
  prepare(
    Library library,
  ) {
    var resultStructure = library.prepare();
    return resultStructure == ffi.nullptr ? null : Converter.parseString(resultStructure.ref);
  }

}
