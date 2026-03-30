import '/common.dart';
import '/utility/storage_path.dart';

// ----------------

class Setting {
  StoragePath       kernel;
  StoragePath       script;
  List<String>      argument;
  Boolean           immediateLaunch;
  List<StoragePath> messageFont;
  Setting({
    required this.kernel,
    required this.script,
    required this.argument,
    required this.immediateLaunch,
    required this.messageFont,
  });
}
