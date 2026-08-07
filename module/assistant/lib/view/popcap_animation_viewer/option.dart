import '/common.dart';
import '/utility/storage_path.dart';

// ----------------

class Option {
  Boolean?                         immediateSelect;
  Boolean?                         automaticPlay;
  Boolean?                         repeatPlay;
  Boolean?                         reversePlay;
  Boolean?                         keepSpeed;
  Boolean?                         showBoundary;
  StoragePath?                     animationFile;
  List<Integer>?                   imageFilter;
  List<Integer>?                   spriteFilter;
  ({Boolean type, Integer index})? activeTarget;
  ({Integer begin, Integer end})?  activeFrameRange;
  Floater?                         activeFrameSpeed;
  Integer?                         activeProgressIndex;
  Boolean?                         activeProgressState;
  Option({
    this.immediateSelect,
    this.automaticPlay,
    this.repeatPlay,
    this.reversePlay,
    this.keepSpeed,
    this.showBoundary,
    this.animationFile,
    this.imageFilter,
    this.spriteFilter,
    this.activeTarget,
    this.activeFrameRange,
    this.activeFrameSpeed,
    this.activeProgressIndex,
    this.activeProgressState,
  }
  );
}
