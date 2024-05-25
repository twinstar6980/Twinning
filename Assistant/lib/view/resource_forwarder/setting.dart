import '/common.dart';

// ----------------

class Setting {
  String       mOptionConfiguration;
  Boolean      mParallelExecute;
  Boolean      mEnableFilter;
  Boolean      mEnableBatch;
  Boolean      mRemainInput;
  Setting({
    required this.mOptionConfiguration,
    required this.mParallelExecute,
    required this.mEnableFilter,
    required this.mEnableBatch,
    required this.mRemainInput,
  });
}
