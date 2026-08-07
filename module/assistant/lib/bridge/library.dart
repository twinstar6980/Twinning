import '/common.dart';
import '/utility/storage_path.dart';
import '/bridge/service.dart';
import 'dart:ffi' as lib;

// ----------------

class Library {

  // #region constructor

  lib.DynamicLibrary? _handle;

  lib.Pointer<Service>? _symbol;

  // ----------------

  Library(
  ) :
    this._handle = null,
    this._symbol = null;

  // #endregion

  // #region access

  Boolean state(
  ) {
    return this._handle != null;
  }

  // ----------------

  Void open(
    StoragePath path,
  ) {
    assertTest(!this.state());
    var pathString = path.emitNative();
    if (SystemChecker.isWindows) {
      pathString += '.';
    }
    var handle = lib.DynamicLibrary.open(pathString);
    var symbol = null as lib.Pointer<Service>?;
    try {
      symbol = handle.lookup<Service>('_ZN8Twinning6Kernel9Interface7serviceE');
    }
    catch (e) {
      handle.close();
      rethrow;
    }
    this._handle = handle;
    this._symbol = symbol;
    this._symbol!.ref.initialize.asFunction<Void Function()>()();
    return;
  }

  Void close(
  ) {
    assertTest(this.state());
    this._symbol!.ref.finalize.asFunction<Void Function()>()();
    this._symbol = lib.nullptr;
    this._handle!.close();
    this._handle = null;
    return;
  }

  // ----------------

  Service symbol(
  ) {
    assertTest(this.state());
    return this._symbol!.ref;
  }

  // #endregion

}
