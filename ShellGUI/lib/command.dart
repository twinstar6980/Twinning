// ignore_for_file: unused_import

import '/common.dart';
import 'package:flutter/widgets.dart';

// ----------------

class Command {

  String       core;
  String       script;
  List<String> argument;

  // ----------------

  Command(
    this.core,
    this.script,
    this.argument,
  );

}

class CommandProvider with ChangeNotifier {

  Command? data;

  CommandProvider(
    this.data,
  );

  // ----------------

  Void
  notify(
  ) async {
    this.notifyListeners();
    return;
  }

  Void
  set(
    Command? data,
  ) {
    this.data = data;
    this.notify();
    return;
  }

}
