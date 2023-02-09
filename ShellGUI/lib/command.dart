// ignore_for_file: unused_import

import '/common.dart';
import 'dart:io';
import 'dart:convert';
import 'package:flutter/material.dart';
import 'package:path_provider/path_provider.dart';

// ----------------

class CommandProvider with ChangeNotifier {

  List<String>? additionalArgument;

  CommandProvider(
    List<String> command,
  ) {
    this.set(command);
  }

  // ----------------

  Future<Void>
  notify(
  ) async {
    this.notifyListeners();
    return;
  }

  Future<Void>
  set(
    List<String> command,
  ) async {
    if (command.isEmpty) {
      this.additionalArgument = null;
    } else {
      this.additionalArgument = command.sublist(1);
    }
    this.notify();
    return;
  }

}
