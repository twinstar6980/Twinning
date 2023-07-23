// ignore_for_file: unused_import, library_prefixes

import '/common.dart';
import 'dart:convert' as convert;
import 'dart:ffi' as ffi;
import 'package:ffi/ffi.dart' as ffi;
import '/bridge/interface.dart' as Interface;

// ----------------

class Converter {

  static
  Integer
  parseSize(
    Interface.Size structure,
  ) {
    return structure.value;
  }

  static
  Void
  constructSize(
    Interface.Size structure,
    Integer        value,
  ) {
    structure.value = value;
    return;
  }

  static
  Void
  destructSize(
    Interface.Size structure,
  ) {
    structure.value = 0;
    return;
  }

  // ----------------

  static
  String
  parseString(
    Interface.String structure,
  ) {
    return structure.data == ffi.nullptr ? '' : structure.data.cast<ffi.Utf8>().toDartString(length: parseSize(structure.size));
  }

  static
  Void
  constructString(
    Interface.String structure,
    String           value,
  ) {
    var value8 = convert.utf8.encode(value);
    structure.data = ffi.calloc.allocate<Interface.Character>(value8.length);
    var valueData = structure.data.cast<ffi.Uint8>().asTypedList(value8.length);
    valueData.setAll(0, value8);
    constructSize(structure.size, value8.length);
    constructSize(structure.capacity, value8.length);
    return;
  }

  static
  Void
  destructString(
    Interface.String structure,
  ) {
    ffi.calloc.free(structure.data);
    structure.data = ffi.nullptr;
    destructSize(structure.size);
    destructSize(structure.capacity);
    return;
  }

  // ----------------

  static
  List<String>
  parseStringList(
    Interface.StringList structure,
  ) {
    var count = parseSize(structure.size);
    var result = List<String>.empty(growable: true);
    for (var index = 0; index < count; ++index) {
      result.add(parseString(structure.data[index]));
    }
    return result;
  }

  static
  Void
  constructStringList(
    Interface.StringList structure,
    List<String>         value,
  ) {
    var count = value.length;
    structure.data = ffi.calloc.allocate<Interface.String>(ffi.sizeOf<Interface.String>() * value.length);
    for (var index = 0; index < count; ++index) {
      constructString(structure.data.elementAt(index).ref, value[index]);
    }
    constructSize(structure.size, count);
    constructSize(structure.capacity, count);
    return;
  }

  static
  Void
  destructStringList(
    Interface.StringList structure,
  ) {
    var count = parseSize(structure.size);
    for (var index = 0; index < count; ++index) {
      destructString(structure.data.elementAt(index).ref);
    }
    ffi.calloc.free(structure.data);
    structure.data = ffi.nullptr;
    destructSize(structure.size);
    destructSize(structure.capacity);
    return;
  }

  // ----------------

  static
  ffi.Pointer<ffi.NativeFunction<ffi.Pointer<Interface.String> Function(ffi.Pointer<ffi.Pointer<Interface.StringList>>, ffi.Pointer<ffi.Pointer<Interface.StringList>>)>>
  parseCallback(
    Interface.Callback structure,
  ) {
    return structure.value;
  }

  static
  Void
  constructCallback(
    Interface.Callback                                                                                                                                                      structure,
    ffi.Pointer<ffi.NativeFunction<ffi.Pointer<Interface.String> Function(ffi.Pointer<ffi.Pointer<Interface.StringList>>, ffi.Pointer<ffi.Pointer<Interface.StringList>>)>> value,
  ) {
    structure.value = value;
    return;
  }

  static
  Void
  destructCallback(
    Interface.Callback structure,
  ) {
    structure.value = ffi.Pointer.fromAddress(0);
    return;
  }

}
