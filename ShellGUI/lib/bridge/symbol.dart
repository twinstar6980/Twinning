import '/common.dart';
import '/bridge/interface.dart' as Interface; // ignore: library_prefixes
import 'dart:io';
import 'dart:ffi' as ffi;

// ----------------

class SymbolTable {
  late ffi.Pointer<ffi.NativeFunction<Interface.execute>> execute;
}

class SymbolNameTable {
  static String execute = () {
    if (Platform.isWindows) {
      return '?execute@Interface@Kernel@TwinStar@@YAPEAUString@123@PEAPEAUCallback@123@PEAPEAU4123@PEAPEAUStringList@123@1@Z';
    }
    if (Platform.isLinux || Platform.isMacOS || Platform.isAndroid || Platform.isIOS) {
      return '_ZN8TwinStar6Kernel9Interface7executeEPPNS1_8CallbackEPPNS1_6StringEPPNS1_10StringListES7_';
    }
    throw Exception();
  }();
}
