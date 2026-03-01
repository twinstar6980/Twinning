import '/common.dart';
import 'dart:ffi' as ffi;

// ----------------

final class Message extends ffi.Struct {
  external ffi.Pointer<ffi.Uint8> data;
  @ffi.Size()
  external Integer                size;
}

final class Executor extends ffi.Struct {
  external ffi.Pointer<ffi.NativeFunction<ffi.Void Function(ffi.Pointer<Executor> self, ffi.Pointer<Executor> callback, ffi.Pointer<Message> argument, ffi.Pointer<Message> result, ffi.Pointer<Message> exception)>> invoke;
  external ffi.Pointer<ffi.NativeFunction<ffi.Void Function(ffi.Pointer<Executor> self, ffi.Pointer<Executor> callback, ffi.Pointer<Message> argument, ffi.Pointer<Message> result, ffi.Pointer<Message> exception)>> clear;
}
