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

// ----------------

Future<TResult> withFinalizerScope<TResult>(
  Future<TResult> Function(List<Future<Void> Function()> finalizer) action,
) async {
  var finalizer = <Future<Void> Function()>[];
  try {
    return await action(finalizer);
  }
  finally {
    for (var finalizerAction in finalizer.reversed) {
      await finalizerAction();
    }
  }
}

TResult withFinalizerScopeSync<TResult>(
  TResult Function(List<Void Function()> finalizer) action,
) {
  var finalizer = <Void Function()>[];
  try {
    return action(finalizer);
  }
  finally {
    for (var finalizerAction in finalizer.reversed) {
      finalizerAction();
    }
  }
}
