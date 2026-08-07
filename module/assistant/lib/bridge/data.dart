import '/common.dart';
import 'dart:ffi' as lib;

// ----------------

final class Message extends lib.Struct {
  external lib.Pointer<lib.Uint8> data;
  @lib.Size()
  external Integer                size;
}

final class Executor extends lib.Struct {
  external lib.Pointer<lib.NativeFunction<lib.Void Function(lib.Pointer<Executor> self, lib.Pointer<Executor> callback, lib.Pointer<Message> argument, lib.Pointer<Message> result, lib.Pointer<Message> exception)>> invoke;
  external lib.Pointer<lib.NativeFunction<lib.Void Function(lib.Pointer<Executor> self, lib.Pointer<Executor> callback, lib.Pointer<Message> argument, lib.Pointer<Message> result, lib.Pointer<Message> exception)>> clear;
}
