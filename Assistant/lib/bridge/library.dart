import '/common.dart';
import '/bridge/service.dart';
import 'dart:ffi' as ffi;

// ----------------

class Library {

  // #region structor

  ffi.DynamicLibrary? _handle;

  ffi.Pointer<Service>? _symbol;

  // ----------------

  Library(
  ) :
    _handle = null,
    _symbol = null;

  // #endregion

  // #region access

  Boolean state(
  ) {
    return this._handle != null;
  }

  // ----------------

  Void open(
    String path,
  ) {
    assertTest(!this.state());
    if (SystemChecker.isWindows) {
      path += '.';
    }
    var handle = ffi.DynamicLibrary.open(path);
    var symbol = null as ffi.Pointer<Service>?;
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
    this._symbol = ffi.nullptr;
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
