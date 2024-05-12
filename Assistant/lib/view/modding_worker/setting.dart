import '/common.dart';

// ----------------

class Setting {
  String       mKernel;
  String       mScript;
  List<String> mArgument;
  Boolean      mImmediateLaunch;
  List<String> mMessageFont;
  Setting({
    required this.mKernel,
    required this.mScript,
    required this.mArgument,
    required this.mImmediateLaunch,
    required this.mMessageFont,
  });
}
