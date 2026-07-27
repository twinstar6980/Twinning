
import '/common.dart';
import '/utility/storage_path.dart';

// ----------------

class Option {
  Boolean?                    parallelForward;
  Boolean?                    enableFilter;
  Boolean?                    enableBatch;
  List<({StoragePath path})>? resource;
  Option({
    this.parallelForward,
    this.enableFilter,
    this.enableBatch,
    this.resource,
  });
}
