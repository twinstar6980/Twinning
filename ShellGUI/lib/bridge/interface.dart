import '/common.dart' as builtin;
import 'dart:ffi' as ffi;

// ----------------

final class Size extends ffi.Struct {
  @ffi.Size()
  external builtin.Integer value;
}

final class Character extends ffi.Struct {
  @ffi.Uint8()
  external builtin.Integer value;
}

final class String extends ffi.Struct {
  external ffi.Pointer<Character> data;
  external Size                   size;
  external Size                   capacity;
}

final class StringList extends ffi.Struct {
  external ffi.Pointer<String> data;
  external Size                size;
  external Size                capacity;
}

final class Callback extends ffi.Struct {
  external ffi.Pointer<ffi.NativeFunction<ffi.Pointer<String> Function(ffi.Pointer<ffi.Pointer<StringList>>, ffi.Pointer<ffi.Pointer<StringList>>)>> value;
}

// ----------------

// ignore: camel_case_types
typedef execute = ffi.Pointer<String> Function(
  ffi.Pointer<ffi.Pointer<Callback>>   callback,
  ffi.Pointer<ffi.Pointer<String>>     script,
  ffi.Pointer<ffi.Pointer<StringList>> argument,
  ffi.Pointer<ffi.Pointer<String>>     result,
);
