import '/common.dart';
import '/bridge/interface.dart' as Interface; // ignore: library_prefixes
import '/bridge/converter.dart';
import '/bridge/library.dart';
import 'dart:ffi' as ffi;
import 'package:ffi/ffi.dart' as ffi;

// ----------------

class Invoker {

  static List<String> Function(List<String>)? _callbackImplement = null;
  static ffi.Pointer<Interface.StringList>?   _callbackResultHandler = null;
  static ffi.Pointer<Interface.String>?       _callbackExceptionHandler = null;

  static ffi.Pointer<Interface.String> _callbackProxy(
    ffi.Pointer<ffi.Pointer<Interface.StringList>> argument,
    ffi.Pointer<ffi.Pointer<Interface.StringList>> result,
  ) {
    Converter.destructStringList(_callbackResultHandler!.ref);
    Converter.destructString(_callbackExceptionHandler!.ref);
    result.value = _callbackResultHandler!;
    try {
      var argumentValue = Converter.parseStringList(argument.value.ref);
      var resultValue = _callbackImplement!(argumentValue);
      Converter.constructStringList(_callbackResultHandler!.ref, resultValue);
      return ffi.nullptr;
    }
    catch (e) {
      var exceptionValue = e.toString();
      Converter.constructString(_callbackExceptionHandler!.ref, exceptionValue);
      return _callbackExceptionHandler!;
    }
  }

  // ----------------

  static String execute(
    Library                             library,
    List<String> Function(List<String>) callback,
    String                              script,
    List<String>                        argument,
  ) {
    {
      _callbackImplement = callback;
      _callbackResultHandler = ffi.calloc.call<Interface.StringList>();
      _callbackExceptionHandler = ffi.calloc.call<Interface.String>();
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
      ffi.calloc.free(_callbackResultHandler!);
      ffi.calloc.free(_callbackExceptionHandler!);
      _callbackImplement = null;
      _callbackResultHandler = null;
      _callbackExceptionHandler = null;
    }
    if (exceptionPointer != ffi.nullptr) {
      var exception = Converter.parseString(exceptionPointer.ref);
      throw exception;
    }
    return result;
  }

}
