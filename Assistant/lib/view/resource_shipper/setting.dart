import '/common.dart';

// ----------------

class Setting {
  String  optionConfiguration;
  Boolean parallelForward;
  Boolean enableFilter;
  Boolean enableBatch;
  Setting({
    required this.optionConfiguration,
    required this.parallelForward,
    required this.enableFilter,
    required this.enableBatch,
  });
}
