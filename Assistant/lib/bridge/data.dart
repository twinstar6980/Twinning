import '/common.dart';
import 'dart:ffi' as ffi;

// ----------------

final class Message extends ffi.Struct {
  external ffi.Pointer<ffi.Uint8> data;
  @ffi.Size()
  external Integer                size;
}

final class Executor extends ffi.Struct {
  external ffi.Pointer<ffi.NativeFunction<ffi.Void Function(ffi.Pointer<Executor>, ffi.Pointer<Executor>, ffi.Pointer<Message>, ffi.Pointer<Message>, ffi.Pointer<Message>)>> invoke;
  external ffi.Pointer<ffi.NativeFunction<ffi.Void Function(ffi.Pointer<Executor>, ffi.Pointer<Executor>, ffi.Pointer<Message>, ffi.Pointer<Message>, ffi.Pointer<Message>)>> clear;
}
