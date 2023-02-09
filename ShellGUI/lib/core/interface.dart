// ignore_for_file: unused_import, library_prefixes, camel_case_types

import '/common.dart' as builtin;
import 'dart:ffi' as ffi;

// ----------------

class Size extends ffi.Struct {
  @ffi.Size()
  external builtin.Integer value;
}

class Character extends ffi.Struct {
  @ffi.Uint8()
  external builtin.Integer value;
}

class String extends ffi.Struct {
  external ffi.Pointer<Character> data;
  external Size size;
  external Size capacity;
}

class StringList extends ffi.Struct {
  external ffi.Pointer<String> data;
  external Size size;
  external Size capacity;
}

class Callback extends ffi.Struct {
  external ffi.Pointer<ffi.NativeFunction<ffi.Pointer<StringList> Function(ffi.Pointer<StringList>)>> value;
}

typedef version = ffi.Pointer<Size> Function();

typedef execute = ffi.Pointer<String> Function(ffi.Pointer<Callback>, ffi.Pointer<String>, ffi.Pointer<StringList>);

typedef prepare = ffi.Pointer<String> Function();
