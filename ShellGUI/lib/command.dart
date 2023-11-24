import '/common.dart';
import 'package:flutter/widgets.dart';

// ----------------

class Command {

  List<String>? mAdditionalArgument;

  // ----------------

  Command(
    this.mAdditionalArgument,
  );

  Command.init(
  ) : this(
    null,
  );

}

class CommandProvider with ChangeNotifier {

  Command data;

  // ----------------

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
    Command data,
  ) {
    this.data = data;
    this.notify();
    return;
  }

}
