import '/common.dart';
import '/bridge/interface.dart' as Interface; // ignore: library_prefixes
import 'dart:io';
import 'dart:ffi' as ffi;

// ----------------

class SymbolTable {
  late ffi.Pointer<ffi.NativeFunction<Interface.version>> version;
  late ffi.Pointer<ffi.NativeFunction<Interface.execute>> execute;
  late ffi.Pointer<ffi.NativeFunction<Interface.prepare>> prepare;
}

class SymbolNameTable {
  static String version = () {
    if (Platform.isWindows) {
      return '?version@Interface@Kernel@TwinStar@@YAPEAUString@123@PEAPEAUSize@123@@Z';
    }
    if (Platform.isLinux || Platform.isMacOS || Platform.isAndroid || Platform.isIOS) {
      return '_ZN8TwinStar6Kernel9Interface7versionEPPNS1_4SizeE';
    }
    throw Exception();
  }();
  static String execute = () {
    if (Platform.isWindows) {
      return '?execute@Interface@Kernel@TwinStar@@YAPEAUString@123@PEAPEAUCallback@123@PEAPEAU4123@PEAPEAUStringList@123@1@Z';
    }
    if (Platform.isLinux || Platform.isMacOS || Platform.isAndroid || Platform.isIOS) {
      return '_ZN8TwinStar6Kernel9Interface7executeEPPNS1_8CallbackEPPNS1_6StringEPPNS1_10StringListES7_';
    }
    throw Exception();
  }();
  static String prepare = () {
    if (Platform.isWindows) {
      return '?prepare@Interface@Kernel@TwinStar@@YAPEAUString@123@XZ';
    }
    if (Platform.isLinux || Platform.isMacOS || Platform.isAndroid || Platform.isIOS) {
      return '_ZN8TwinStar6Kernel9Interface7prepareEv';
    }
    throw Exception();
  }();
}
