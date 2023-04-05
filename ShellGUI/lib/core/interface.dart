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
  external ffi.Pointer<ffi.NativeFunction<ffi.Pointer<String> Function(ffi.Pointer<ffi.Pointer<StringList>>, ffi.Pointer<ffi.Pointer<StringList>>)>> value;
}

// ----------------

typedef version = ffi.Pointer<String> Function(
  ffi.Pointer<ffi.Pointer<Size>> number,
);

typedef execute = ffi.Pointer<String> Function(
  ffi.Pointer<ffi.Pointer<Callback>>   callback,
  ffi.Pointer<ffi.Pointer<String>>     script,
  ffi.Pointer<ffi.Pointer<StringList>> argument,
  ffi.Pointer<ffi.Pointer<String>>     result,
);

typedef prepare = ffi.Pointer<String> Function(
);
