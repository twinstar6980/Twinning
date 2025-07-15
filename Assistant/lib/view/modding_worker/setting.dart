import '/common.dart';

// ----------------

class Setting {
  String       kernel;
  String       script;
  List<String> argument;
  Boolean      immediateLaunch;
  List<String> messageFont;
  Setting({
    required this.kernel,
    required this.script,
    required this.argument,
    required this.immediateLaunch,
    required this.messageFont,
  });
}
