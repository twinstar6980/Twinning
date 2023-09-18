import '/common.dart';
import '/bridge/interface.dart' as Interface; // ignore: library_prefixes
import '/bridge/symbol.dart';
import 'dart:ffi' as ffi;

// ----------------

class Library {

  late SymbolTable        _symbol;
  late ffi.DynamicLibrary _handle;

  // ----------------

  Library(
    String path,
  ) {
    this._handle = ffi.DynamicLibrary.open(normalizeLibraryPath(path));
    this._symbol = SymbolTable();
    this._symbol.version = this._handle.lookup<ffi.NativeFunction<Interface.version>>(SymbolNameTable.version);
    this._symbol.execute = this._handle.lookup<ffi.NativeFunction<Interface.execute>>(SymbolNameTable.execute);
    this._symbol.prepare = this._handle.lookup<ffi.NativeFunction<Interface.prepare>>(SymbolNameTable.prepare);
  }

  // ----------------

  ffi.Pointer<Interface.String>
  version(
    ffi.Pointer<ffi.Pointer<Interface.Size>> number,
  ) {
    return this._symbol.version.asFunction<Interface.version>()(number);
  }

  ffi.Pointer<Interface.String>
  execute(
    ffi.Pointer<ffi.Pointer<Interface.Callback>>   callback,
    ffi.Pointer<ffi.Pointer<Interface.String>>     script,
    ffi.Pointer<ffi.Pointer<Interface.StringList>> argument,
    ffi.Pointer<ffi.Pointer<Interface.String>>     result,
  ) {
    return this._symbol.execute.asFunction<Interface.execute>()(callback, script, argument, result);
  }

  ffi.Pointer<Interface.String>
  prepare(
  ) {
    return this._symbol.prepare.asFunction<Interface.prepare>()();
  }

}
