import '/common.dart';
import '/bridge/interface.dart' as Interface; // ignore: library_prefixes
import '/bridge/symbol.dart';
import 'dart:io';
import 'dart:ffi' as ffi;

// ----------------

class Library {

  ffi.DynamicLibrary? _handle = null;
  SymbolTable?        _symbol = null;

  // ----------------

  Boolean state(
  ) {
    return this._handle != null;
  }

  Void open(
    String path,
  ) {
    assertAlways(!this.state());
    var pathAbsolute = path;
    if (Platform.isWindows) {
      pathAbsolute += '.';
    }
    var handle = ffi.DynamicLibrary.open(pathAbsolute);
    var symbol = SymbolTable();
    try {
      symbol.execute = handle.lookup<ffi.NativeFunction<Interface.execute>>(SymbolNameTable.execute);
    }
    catch (e) {
      handle.close();
      rethrow;
    }
    this._handle = handle;
    this._symbol = symbol;
    return;
  }

  Void close(
  ) {
    assertAlways(this.state());
    this._handle!.close();
    this._handle = null;
    this._symbol = null;
    return;
  }

  // ----------------

  ffi.Pointer<Interface.String> execute(
    ffi.Pointer<ffi.Pointer<Interface.Callback>>   callback,
    ffi.Pointer<ffi.Pointer<Interface.String>>     script,
    ffi.Pointer<ffi.Pointer<Interface.StringList>> argument,
    ffi.Pointer<ffi.Pointer<Interface.String>>     result,
  ) {
    assertAlways(this.state());
    return this._symbol!.execute.asFunction<Interface.execute>()(callback, script, argument, result);
  }

}
