import '/common.dart';
import 'package:flutter/widgets.dart';

// ----------------

class Command {

  String       kernel;
  String       script;
  List<String> argument;

  // ----------------

  Command(
    this.kernel,
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
