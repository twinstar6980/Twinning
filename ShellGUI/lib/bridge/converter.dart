import '/common.dart';
import '/bridge/interface.dart' as Interface; // ignore: library_prefixes
import 'dart:convert' as convert;
import 'dart:ffi' as ffi;
import 'package:ffi/ffi.dart' as ffi;

// ----------------

class Converter {

  static Integer parseSize(
    Interface.Size structure,
  ) {
    return structure.value;
  }

  static Void constructSize(
    Interface.Size structure,
    Integer        value,
  ) {
    structure.value = value;
    return;
  }

  static Void destructSize(
    Interface.Size structure,
  ) {
    structure.value = 0;
    return;
  }

  // ----------------

  static String parseString(
    Interface.String structure,
  ) {
    return structure.data == ffi.nullptr ? '' : structure.data.cast<ffi.Utf8>().toDartString(length: parseSize(structure.size));
  }

  static Void constructString(
    Interface.String structure,
    String           value,
  ) {
    var data = convert.utf8.encode(value);
    var size = data.length;
    structure.data = ffi.calloc.allocate<Interface.Character>(size);
    structure.data.cast<ffi.Uint8>().asTypedList(size).setAll(0, data);
    constructSize(structure.size, size);
    constructSize(structure.capacity, size);
    return;
  }

  static Void destructString(
    Interface.String structure,
  ) {
    ffi.calloc.free(structure.data);
    structure.data = ffi.nullptr;
    destructSize(structure.size);
    destructSize(structure.capacity);
    return;
  }

  // ----------------

  static List<String> parseStringList(
    Interface.StringList structure,
  ) {
    var size = parseSize(structure.size);
    var value = <String>[];
    for (var index = 0; index < size; index++) {
      value.add(parseString(structure.data[index]));
    }
    return value;
  }

  static Void constructStringList(
    Interface.StringList structure,
    List<String>         value,
  ) {
    var size = value.length;
    structure.data = ffi.calloc.allocate<Interface.String>(ffi.sizeOf<Interface.String>() * size);
    for (var index = 0; index < size; index++) {
      constructString(structure.data.elementAt(index).ref, value[index]);
    }
    constructSize(structure.size, size);
    constructSize(structure.capacity, size);
    return;
  }

  static Void destructStringList(
    Interface.StringList structure,
  ) {
    var capacity = parseSize(structure.capacity);
    for (var index = 0; index < capacity; index++) {
      destructString(structure.data.elementAt(index).ref);
    }
    ffi.calloc.free(structure.data);
    structure.data = ffi.nullptr;
    destructSize(structure.size);
    destructSize(structure.capacity);
    return;
  }

  // ----------------

  static ffi.Pointer<ffi.NativeFunction<ffi.Pointer<Interface.String> Function(ffi.Pointer<ffi.Pointer<Interface.StringList>>, ffi.Pointer<ffi.Pointer<Interface.StringList>>)>> parseCallback(
    Interface.Callback structure,
  ) {
    return structure.value;
  }

  static Void constructCallback(
    Interface.Callback                                                                                                                                                      structure,
    ffi.Pointer<ffi.NativeFunction<ffi.Pointer<Interface.String> Function(ffi.Pointer<ffi.Pointer<Interface.StringList>>, ffi.Pointer<ffi.Pointer<Interface.StringList>>)>> value,
  ) {
    structure.value = value;
    return;
  }

  static Void destructCallback(
    Interface.Callback structure,
  ) {
    structure.value = ffi.Pointer.fromAddress(0);
    return;
  }

}
