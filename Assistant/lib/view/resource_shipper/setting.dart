import '/common.dart';

// ----------------

class Setting {
  String  mOptionConfiguration;
  Boolean mParallelForward;
  Boolean mEnableFilter;
  Boolean mEnableBatch;
  Setting({
    required this.mOptionConfiguration,
    required this.mParallelForward,
    required this.mEnableFilter,
    required this.mEnableBatch,
  });
}
