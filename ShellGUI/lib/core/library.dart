// ignore_for_file: unused_import, library_prefixes

import '/common.dart';
import 'dart:ffi' as ffi;
import '/core/interface.dart' as Interface;

// ----------------

abstract class Library {

  ffi.Pointer<Interface.String>
  version(
    ffi.Pointer<ffi.Pointer<Interface.Size>> number,
  );

  ffi.Pointer<Interface.String>
  execute(
    ffi.Pointer<ffi.Pointer<Interface.Callback>>   callback,
    ffi.Pointer<ffi.Pointer<Interface.String>>     script,
    ffi.Pointer<ffi.Pointer<Interface.StringList>> argument,
    ffi.Pointer<ffi.Pointer<Interface.String>>     result,
  );

  ffi.Pointer<Interface.String>
  prepare(
  );

}
