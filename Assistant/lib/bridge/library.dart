import '/common.dart';
import '/bridge/service.dart';
import 'dart:io';
import 'dart:ffi' as ffi;

// ----------------

class Library {

  // #region structor

  ffi.DynamicLibrary? _handle;

  ffi.Pointer<Service> _symbol;

  // ----------------

  Library(
  ) :
    _handle = null,
    _symbol = ffi.nullptr;

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
    if (Platform.isWindows) {
      path += '.';
    }
    var handle = ffi.DynamicLibrary.open(path);
    var symbol = ffi.nullptr.cast<Service>();
    try {
      symbol = handle.lookup<Service>(() {
        if (Platform.isWindows) {
          return '?service@Interface@Kernel@Twinning@@3UService@123@A';
        }
        if (Platform.isLinux || Platform.isMacOS || Platform.isAndroid || Platform.isIOS) {
          return '_ZN8Twinning6Kernel9Interface7serviceE';
        }
        throw UnimplementedError();
      }());
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
    assertTest(this.state());
    this._handle!.close();
    this._handle = null;
    this._symbol = ffi.nullptr;
    return;
  }

  // ----------------

  Service symbol(
  ) {
    assertTest(this.state());
    return this._symbol.ref;
  }

  // #endregion

}
