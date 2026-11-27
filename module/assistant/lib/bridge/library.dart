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

  Future<Void> open(
    StoragePath path,
  ) async {
    assertTest(!this.state());
    var pathString = path.emitNative();
    if (SystemChecker.isWindows) {
      pathString += '.';
    }
    var handle = lib.DynamicLibrary.open(pathString);
    var symbol = lib.Pointer<Service>.fromAddress(0);
    try {
      symbol = handle.lookup<Service>('_ZN8Twinning6Kernel9Interface7serviceE');
    }
    catch (e) {
      handle.close();
      rethrow;
    }
    this._handle = handle;
    this._symbol = symbol;
    try {
      this._symbol!.ref.initialize.asFunction<Void Function()>()();
    }
    catch (e) {
      // ignored
    }
    return;
  }

  Future<Void> close(
  ) async {
    assertTest(this.state());
    try {
      this._symbol!.ref.finalize.asFunction<Void Function()>()();
    }
    catch (e) {
      // ignored
    }
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
