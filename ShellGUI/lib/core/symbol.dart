// ignore_for_file: unused_import, library_prefixes

import '/common.dart';
import 'dart:io';
import 'dart:ffi' as ffi;
import '/core/interface.dart' as Interface;

// ----------------

class SymbolTable {
  late ffi.Pointer<ffi.NativeFunction<Interface.version>> version;
  late ffi.Pointer<ffi.NativeFunction<Interface.execute>> execute;
  late ffi.Pointer<ffi.NativeFunction<Interface.prepare>> prepare;
}

class SymbolNameTable {
  static String version = () {
    if (Platform.isWindows) {
      return '?version@Interface@Core@TwinStar@@YAPEBUSize@123@XZ';
    }
    if (Platform.isLinux || Platform.isMacOS || Platform.isAndroid || Platform.isIOS) {
      return '_ZN8TwinStar4Core9Interface7versionEv';
    }
    throw Exception();
  }();
  static String execute = () {
    if (Platform.isWindows) {
      return '?execute@Interface@Core@TwinStar@@YAPEBUString@123@PEBUCallback@123@PEBU4123@PEBUStringList@123@@Z';
    }
    if (Platform.isLinux || Platform.isMacOS || Platform.isAndroid || Platform.isIOS) {
      return '_ZN8TwinStar4Core9Interface7executeEPKNS1_8CallbackEPKNS1_6StringEPKNS1_10StringListE';
    }
    throw Exception();
  }();
  static String prepare = () {
    if (Platform.isWindows) {
      return '?prepare@Interface@Core@TwinStar@@YAPEBUString@123@XZ';
    }
    if (Platform.isLinux || Platform.isMacOS || Platform.isAndroid || Platform.isIOS) {
      return '_ZN8TwinStar4Core9Interface7prepareEv';
    }
    throw Exception();
  }();
}
