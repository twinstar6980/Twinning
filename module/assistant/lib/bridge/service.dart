import '/common.dart'; // ignore: unused_import
import '/bridge/data.dart';
import 'dart:ffi' as lib;

// ----------------

final class Service extends lib.Struct {
  external lib.Pointer<Executor>                                executor;
  external lib.Pointer<lib.NativeFunction<lib.Void Function()>> initialize;
  external lib.Pointer<lib.NativeFunction<lib.Void Function()>> finalize;
}
