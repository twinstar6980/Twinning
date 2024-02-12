import '/common.dart';

// ----------------

abstract class Host {

  Future<Void> start(
  );

  Future<Void> finish(
  );

  // ----------------

  Future<List<String>> execute(
    List<String> argument,
  );

}
