import '/common.dart';
import '/bridge/interface.dart' as Interface; // ignore: library_prefixes
import '/bridge/symbol.dart';
import 'dart:ffi' as ffi;

// ----------------

class Library {

  ffi.DynamicLibrary? _handle = null;
  SymbolTable?        _symbol = null;

  // ----------------

  Boolean
  state(
  ) {
    return this._handle != null;
  }

  Void
  open(
    String path,
  ) {
    assert(!this.state());
    var handle = ffi.DynamicLibrary.open(normalizeLibraryPath(path));
    var symbol = SymbolTable();
    try {
      symbol.version = handle.lookup<ffi.NativeFunction<Interface.version>>(SymbolNameTable.version);
      symbol.execute = handle.lookup<ffi.NativeFunction<Interface.execute>>(SymbolNameTable.execute);
      symbol.prepare = handle.lookup<ffi.NativeFunction<Interface.prepare>>(SymbolNameTable.prepare);
    } catch (e) {
      handle.close();
      rethrow;
    }
    this._handle = handle;
    this._symbol = symbol;
    return;
  }

  Void
  close(
  ) {
    assert(this.state());
    this._handle!.close();
    this._handle = null;
    this._symbol = null;
    return;
  }

  // ----------------

  ffi.Pointer<Interface.String>
  version(
    ffi.Pointer<ffi.Pointer<Interface.Size>> number,
  ) {
    assert(this.state());
    return this._symbol!.version.asFunction<Interface.version>()(number);
  }

  ffi.Pointer<Interface.String>
  execute(
    ffi.Pointer<ffi.Pointer<Interface.Callback>>   callback,
    ffi.Pointer<ffi.Pointer<Interface.String>>     script,
    ffi.Pointer<ffi.Pointer<Interface.StringList>> argument,
    ffi.Pointer<ffi.Pointer<Interface.String>>     result,
  ) {
    assert(this.state());
    return this._symbol!.execute.asFunction<Interface.execute>()(callback, script, argument, result);
  }

  ffi.Pointer<Interface.String>
  prepare(
  ) {
    assert(this.state());
    return this._symbol!.prepare.asFunction<Interface.prepare>()();
  }

}
