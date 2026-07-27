import '/common.dart';

// ----------------

abstract class Client {

  // #region constructor

  Client(
  );

  // #endregion

  // #region life

  Future<Void> start(
  );

  Future<Void> finish(
  );

  // #endregion

  // #region handle

  Future<List<String>> handle(
    List<String> argument,
  );

  // #endregion

}
