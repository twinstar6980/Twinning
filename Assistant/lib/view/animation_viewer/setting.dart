import '/common.dart';

// ----------------

class Setting {
  Boolean mImmediateSelect;
  Boolean mAutomaticPlay;
  Boolean mRepeatPlay;
  Boolean mRemainFrameRate;
  Boolean mShowSpriteBoundary;
  String  mImageFilterRule;
  String  mSpriteFilterRule;
  Setting({
    required this.mImmediateSelect,
    required this.mAutomaticPlay,
    required this.mRepeatPlay,
    required this.mRemainFrameRate,
    required this.mShowSpriteBoundary,
    required this.mImageFilterRule,
    required this.mSpriteFilterRule,
  });
}
