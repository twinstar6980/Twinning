import '/common.dart';
import '/utility/json_type.dart';

// ----------------

class Option {
  Boolean?                                                                       parallelForward;
  List<({String method, Boolean batch, JsonObject argument, Boolean expanded})>? command;
  Option({
    this.parallelForward,
    this.command,
  });
}
