// ignore_for_file: unused_import, library_prefixes

import '/common.dart';
import 'dart:ffi' as ffi;
import '/core/interface.dart' as Interface;

// ----------------

abstract class Library {

  ffi.Pointer<Interface.Size>
  version(
  );

  ffi.Pointer<Interface.String>
  execute(
    ffi.Pointer<Interface.Callback>   callback,
    ffi.Pointer<Interface.String>     script,
    ffi.Pointer<Interface.StringList> argument,
  );

  ffi.Pointer<Interface.String>
  prepare(
  );

}
