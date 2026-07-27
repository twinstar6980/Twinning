import '/common.dart';

// ----------------

class Finalizer {

  // #region constructor

  Future<Void> Function() _action;

  Boolean _disposed;

  // ----------------

  Finalizer(
    Future<Void> Function() action,
  ) :
    this._action = action,
    this._disposed = false;

  // #endregion

  // #region dispose

  Future<Void> dispose(
  ) async {
    assertTest(!this._disposed);
    this._disposed = true;
    await this._action();
    return;
  }

  // #endregion

}
