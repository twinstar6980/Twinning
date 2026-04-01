import '/common.dart';
import 'package:flutter/widgets.dart';

// ----------------

class ApplicationExceptionManager {

  // #region singleton

  static final ApplicationExceptionManager instance = ._();

  // #endregion

  // #region constructor

  Boolean _initialized;

  Future<Void> Function(Object exception, StackTrace? stack)? _handler;

  // ----------------

  ApplicationExceptionManager._(
  ) :
    _initialized = false,
    _handler = null;

  // #endregion

  // #region utility

  Future<Void> initialize(
  ) async {
    assertTest(!this._initialized);
    WidgetsBinding.instance.platformDispatcher.onError = (exception, stack) {
      this._handler?.call(exception, stack);
      return true;
    };
    FlutterError.onError = (details) {
      FlutterError.presentError(details);
      this._handler?.call(details.exception, details.stack);
      return;
    };
    this._initialized = true;
    return;
  }

  Future<Void> listen(
    Future<Void> Function(Object exception, StackTrace? stack)? handler,
  ) async {
    assertTest(this._initialized);
    this._handler = handler;
    return;
  }

  // #endregion

}
