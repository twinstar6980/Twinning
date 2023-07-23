// ignore_for_file: unused_import, library_prefixes

import '/common.dart';

// ----------------

abstract class Host {

  Future<Void>
  start(
  );

  Future<Void>
  finish(
  );

  Future<List<String>>
  execute(
    List<String> argument,
  );

}
