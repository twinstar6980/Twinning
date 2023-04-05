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

  static ffi.Pointer<Interface.String>?       _callbackExceptionHandler;

  static ffi.Pointer<Interface.StringList>?   _callbackResultHandler;

  static
  ffi.Pointer<Interface.String>
  _callbackProxy(
    ffi.Pointer<ffi.Pointer<Interface.StringList>> argument,
    ffi.Pointer<ffi.Pointer<Interface.StringList>> result,
  ) {
    Converter.destructString(_callbackExceptionHandler!.ref);
    Converter.destructStringList(_callbackResultHandler!.ref);
    result.value = _callbackResultHandler!;
    try {
      var argumentValue = Converter.parseStringList(argument.value.ref);
      var resultValue = _callbackTarget!(argumentValue);
      Converter.constructStringList(_callbackResultHandler!.ref, resultValue);
      return ffi.nullptr;
    } catch (e) {
      var exceptionValue = e.toString();
      Converter.constructString(_callbackExceptionHandler!.ref, exceptionValue);
      return _callbackExceptionHandler!;
    }
  }

  // ----------------

  static
  Integer
  version(
    Library library,
  ) {
    var numberPointer = ffi.Pointer<ffi.Pointer<Interface.Size>>.fromAddress(0);
    {
      numberPointer = ffi.calloc.call<ffi.Pointer<Interface.Size>>();
    }
    {
    }
    var exceptionPointer = library.version(numberPointer);
    {
    }
    var number = Converter.parseSize(numberPointer.value.ref);
    {
      ffi.calloc.free(numberPointer);
    }
    if (exceptionPointer != ffi.nullptr) {
      var exception = Converter.parseString(exceptionPointer.ref);
      throw exception;
    }
    return number;
  }

  static
  String
  execute(
    Library                             library,
    List<String> Function(List<String>) callback,
    String                              script,
    List<String>                        argument,
  ) {
    {
      _callbackTarget = callback;
      _callbackExceptionHandler = ffi.calloc.call<Interface.String>();
      _callbackResultHandler = ffi.calloc.call<Interface.StringList>();
      Converter.constructStringList(_callbackResultHandler!.ref, []);
      Converter.constructString(_callbackExceptionHandler!.ref, '');
    }
    var callbackPointer = ffi.Pointer<ffi.Pointer<Interface.Callback>>.fromAddress(0);
    var scriptPointer = ffi.Pointer<ffi.Pointer<Interface.String>>.fromAddress(0);
    var argumentPointer = ffi.Pointer<ffi.Pointer<Interface.StringList>>.fromAddress(0);
    var resultPointer = ffi.Pointer<ffi.Pointer<Interface.String>>.fromAddress(0);
    {
      callbackPointer = ffi.calloc.call<ffi.Pointer<Interface.Callback>>();
      scriptPointer = ffi.calloc.call<ffi.Pointer<Interface.String>>();
      argumentPointer = ffi.calloc.call<ffi.Pointer<Interface.StringList>>();
      resultPointer = ffi.calloc.call<ffi.Pointer<Interface.String>>();
    }
    {
      callbackPointer.value = ffi.calloc.call<Interface.Callback>();
      scriptPointer.value = ffi.calloc.call<Interface.String>();
      argumentPointer.value = ffi.calloc.call<Interface.StringList>();
      Converter.constructCallback(callbackPointer.value.ref, ffi.Pointer.fromFunction(_callbackProxy));
      Converter.constructString(scriptPointer.value.ref, script);
      Converter.constructStringList(argumentPointer.value.ref, argument);
    }
    var exceptionPointer = library.execute(callbackPointer, scriptPointer, argumentPointer, resultPointer);
    {
      Converter.destructCallback(callbackPointer.value.ref);
      Converter.destructString(scriptPointer.value.ref);
      Converter.destructStringList(argumentPointer.value.ref);
      ffi.calloc.free(callbackPointer.value);
      ffi.calloc.free(scriptPointer.value);
      ffi.calloc.free(argumentPointer.value);
    }
    var result = Converter.parseString(resultPointer.value.ref);
    {
      ffi.calloc.free(callbackPointer);
      ffi.calloc.free(scriptPointer);
      ffi.calloc.free(argumentPointer);
      ffi.calloc.free(resultPointer);
    }
    {
      Converter.destructStringList(_callbackResultHandler!.ref);
      Converter.destructString(_callbackExceptionHandler!.ref);
      ffi.calloc.free(_callbackExceptionHandler!);
      ffi.calloc.free(_callbackResultHandler!);
      _callbackTarget = null;
    }
    if (exceptionPointer != ffi.nullptr) {
      var exception = Converter.parseString(exceptionPointer.ref);
      throw exception;
    }
    return result;
  }

  static
  Void
  prepare(
    Library library,
  ) {
    {
    }
    {
    }
    var exceptionPointer = library.prepare();
    {
    }
    {
    }
    if (exceptionPointer != ffi.nullptr) {
      var exception = Converter.parseString(exceptionPointer.ref);
      throw exception;
    }
    return;
  }

}
