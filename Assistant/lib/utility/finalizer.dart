import '/common.dart';

// ----------------

class Finalizer {

  // #region constructor

  Future<Void> Function() _action;

  // ----------------

  Finalizer(
    Future<Void> Function() action,
  ) :
    this._action = action;

  // #endregion

  // #region dispose

  Future<Void> dispose(
  ) async {
    await this._action();
    return;
  }

  // #endregion

}
