import '/common.dart';

// ----------------

abstract class Client {

  // #region life

  Future<Void> start(
  );

  Future<Void> finish(
  );

  // ----------------

  Future<List<String>> callback(
    List<String> argument,
  );

  // #endregion

}
