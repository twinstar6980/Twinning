import '/common.dart';
import '/setting.dart';
import '/utility/command_line_reader.dart';
import '/utility/command_line_writer.dart';
import '/utility/storage_helper.dart';
import '/utility/convert_helper.dart';
import '/widget/export.dart';
import '/view/home/module_page.dart';
import '/view/animation_viewer/model.dart' as model;
import '/view/animation_viewer/visual_helper.dart';
import 'dart:math';
import 'dart:async';
import 'package:collection/collection.dart';
import 'package:flutter/widgets.dart';
import 'package:provider/provider.dart';

// ----------------

class MainPage extends StatefulWidget {

  const MainPage({
    super.key,
    required this.option,
  });

  // ----------------

  final List<String> option;

  // ----------------

  @override
  createState() => _MainPageState();

}

class _MainPageState extends State<MainPage> with SingleTickerProviderStateMixin implements ModulePageState {

  late Boolean                                         _immediateSelect;
  late Boolean                                         _automaticPlay;
  late Boolean                                         _repeatPlay;
  late Boolean                                         _keepSpeed;
  late Boolean                                         _showBoundary;
  late String?                                         _animationFile;
  late model.Animation?                                _animation;
  late Map<String, (ImageProvider, Integer, Integer)>? _texture;
  late List<Boolean>?                                  _imageFilter;
  late List<Boolean>?                                  _spriteFilter;
  late (Boolean, Integer)?                             _activeTarget;
  late model.Sprite?                                   _activeSprite;
  late List<(String, Integer, Integer)>?               _activeFrameLabel;
  late (Integer, Integer)?                             _activeFrameRange;
  late Floater?                                        _activeFrameSpeed;
  late StreamController                                _activeProgressIndexStream;
  late StreamController                                _activeProgressStateStream;
  late Boolean                                         _activeProgressChangingContinue;
  late AnimationController                             _animationController;
  late Widget?                                         _animationVisual;
  late ScrollController                                _stageHorizontalScrollSontroller;
  late ScrollController                                _stageVerticalScrollSontroller;

  Boolean get _loaded => this._animation != null;

  Boolean get _activated => this._activeTarget != null;

  Future<Void> _load(
    String animationFile,
  ) async {
    assertTest(!this._loaded && !this._activated);
    var animation = await VisualHelper.loadAnimation(animationFile);
    var texture = await VisualHelper.loadTexture(StorageHelper.parent(animationFile), animation);
    this._animationFile = animationFile;
    this._animation = animation;
    this._texture = texture;
    this._imageFilter = List.filled(this._animation!.image.length, true);
    this._spriteFilter = List.filled(this._animation!.sprite.length, true);
    await refreshState(this.setState);
    return;
  }

  Future<Void> _unload(
  ) async {
    assertTest(this._loaded && !this._activated);
    this._animationFile = null;
    this._animation = null;
    this._texture = null;
    this._imageFilter = null;
    this._spriteFilter = null;
    await refreshState(this.setState);
    return;
  }

  Future<Void> _activate(
    (Boolean, Integer)  target,
    (Integer, Integer)? frameRange,
    Floater?            frameSpeed,
    Integer?            progressIndex,
    Boolean?            progressState,
  ) async {
    assertTest(this._loaded && !this._activated);
    var activeSprite = null as model.Sprite?;
    if (!target.$1) {
      var originalTarget = VisualHelper.selectImage(this._animation!, target.$2);
      activeSprite = .new(
        name: VisualHelper.parseImageFileName(originalTarget.name),
        frameRate: null,
        workArea: (0, 0),
        frame: [
          .new(
            label: null,
            stop: false,
            command: [],
            remove: [],
            append: [
              .new(
                index: 0,
                name: null,
                resource: target.$2,
                sprite: false,
                additive: false,
                preloadFrame: 0,
                timeScale: 1.0,
              ),
            ],
            change: [
              .new(
                index: 0,
                transform: .new(value: model.TranslateTransform(x: 0.0, y: 0.0)),
                color: null,
                spriteFrameNumber: null,
                sourceRectangle: null,
              ),
            ],
          ),
        ],
      );
    }
    else {
      var originalTarget = VisualHelper.selectSprite(this._animation!, target.$2);
      activeSprite = originalTarget;
      assertTest(activeSprite.frame.length != 0);
    }
    this._activeTarget = target;
    this._activeSprite = activeSprite;
    this._activeFrameLabel = VisualHelper.parseSpriteFrameLabel(this._activeSprite!);
    this._activeFrameRange = (0, 0);
    this._activeFrameSpeed = 0.0;
    this._animationVisual = VisualHelper.visualizeSprite(this._animationController, this._animation!, this._texture!, this._activeSprite!, this._imageFilter!, this._spriteFilter!);
    await this._changeFrameRange(frameRange ?? (0, this._activeSprite!.frame.length - 1));
    await this._changeFrameSpeed(frameSpeed ?? this._activeSprite!.frameRate ?? this._animation!.frameRate.toDouble());
    await this._changeProgressIndex(progressIndex ?? 0);
    await this._changeProgressState(progressState ?? this._automaticPlay);
    await refreshState(this.setState);
    return;
  }

  Future<Void> _deactivate(
  ) async {
    assertTest(this._loaded && this._activated);
    this._activeTarget = null;
    this._activeSprite = null;
    this._activeFrameLabel = null;
    this._activeFrameRange = null;
    this._activeFrameSpeed = null;
    this._animationController.duration = .zero;
    this._animationController.reset();
    this._animationVisual = null;
    await refreshState(this.setState);
    return;
  }

  Future<Void> _changeElementFilter(
    List<Boolean>? imageFilter,
    List<Boolean>? spriteFilter,
  ) async {
    assertTest(this._loaded);
    assertTest(imageFilter == null || imageFilter.length == this._animation!.image.length);
    assertTest(spriteFilter == null || spriteFilter.length == this._animation!.sprite.length);
    if (imageFilter != null) {
      this._imageFilter = imageFilter;
    }
    if (spriteFilter != null) {
      this._spriteFilter = spriteFilter;
    }
    if (this._activated) {
      this._animationVisual = VisualHelper.visualizeSprite(this._animationController, this._animation!, this._texture!, this._activeSprite!, this._imageFilter!, this._spriteFilter!);
    }
    await refreshState(this.setState);
    return;
  }

  Future<Void> _changeFrameRange(
    (Integer, Integer) frameRange,
  ) async {
    assertTest(this._loaded && this._activated);
    assertTest(0 <= frameRange.$1 && frameRange.$1 < this._activeSprite!.frame.length);
    assertTest(0 <= frameRange.$2 && frameRange.$2 < this._activeSprite!.frame.length);
    assertTest(frameRange.$1 <= frameRange.$2);
    this._activeFrameRange = frameRange;
    // TODO: unimplemented
    await refreshState(this.setState);
    return;
  }

  Future<Void> _changeFrameSpeed(
    Floater frameSpeed,
  ) async {
    assertTest(this._loaded && this._activated);
    assertTest(frameSpeed > 0.0);
    var currentState = this._queryProgressState();
    if (currentState) {
      this._animationController.stop();
    }
    this._activeFrameSpeed = frameSpeed;
    this._animationController.duration = .new(milliseconds: (this._activeSprite!.frame.length * 1000 / this._activeFrameSpeed!).ceil());
    if (currentState) {
      this._animationController.forward();
    }
    await refreshState(this.setState);
    return;
  }

  Integer _queryProgressIndex(
  ) {
    assertTest(this._loaded && this._activated);
    return (this._animationController.value * this._activeSprite!.frame.length).floor();
  }

  Future<Void> _changeProgressIndex(
    Integer index,
  ) async {
    assertTest(this._loaded && this._activated);
    assertTest(index < this._activeSprite!.frame.length);
    var currentState = this._queryProgressState();
    if (currentState) {
      this._animationController.stop();
    }
    this._animationController.value = index / this._activeSprite!.frame.length;
    if (currentState) {
      this._animationController.forward();
    }
    return;
  }

  Boolean _queryProgressState(
  ) {
    assertTest(this._loaded && this._activated);
    return this._animationController.isAnimating;
  }

  Future<Void> _changeProgressState(
    Boolean state,
  ) async {
    assertTest(this._loaded && this._activated);
    if (!state) {
      this._animationController.stop();
    }
    else {
      if (this._animationController.isCompleted) {
        this._animationController.reset();
      }
      this._animationController.forward();
    }
    this._activeProgressStateStream.sink.add(null);
    return;
  }

  Future<Void> _applyLoad(
    String              animationFile,
    List<Integer>?      imageFilter,
    List<Integer>?      spriteFilter,
    (Boolean, Integer)? activeTarget,
    (Integer, Integer)? activeFrameRange,
    Floater?            activeFrameSpeed,
    Integer?            activeProgressIndex,
    Boolean?            activeProgressState,
  ) async {
    if (this._loaded) {
      if (this._activated) {
        await this._deactivate();
      }
      await this._unload();
    }
    await this._load(animationFile);
    await this._changeElementFilter(
      imageFilter == null ? null : this._animation!.image.mapIndexed((index, value) => !imageFilter.contains(index)).toList(),
      spriteFilter == null ? null : this._animation!.sprite.mapIndexed((index, value) => !spriteFilter.contains(index)).toList(),
    );
    if (activeTarget == null && this._immediateSelect && this._animation!.mainSprite != null) {
      activeTarget = (true, this._animation!.sprite.length);
    }
    if (activeTarget != null) {
      await this._activate(activeTarget, activeFrameRange, activeFrameSpeed, activeProgressIndex, activeProgressState);
    }
    return;
  }

  // ----------------

  @override
  modulePageOpenView() async {
    return;
  }

  @override
  modulePageCloseView() async {
    return true;
  }

  @override
  modulePageEnterView() async {
    return;
  }

  @override
  modulePageExitView() async {
    return;
  }

  @override
  modulePageApplyOption(optionView) async {
    var optionImmediateSelect = null as Boolean?;
    var optionAutomaticPlay = null as Boolean?;
    var optionRepeatPlay = null as Boolean?;
    var optionKeepSpeed = null as Boolean?;
    var optionShowBoundary = null as Boolean?;
    var optionAnimationFile = null as String?;
    var optionImageFilter = null as List<Integer>?;
    var optionSpriteFilter = null as List<Integer>?;
    var optionActiveTarget = null as (Boolean, Integer)?;
    var optionActiveFrameRange = null as (Integer, Integer)?;
    var optionActiveFrameSpeed = null as Floater?;
    var optionActiveProgressIndex = null as Integer?;
    var optionActiveProgressState = null as Boolean?;
    var option = CommandLineReader(optionView);
    if (option.check('-immediate_select')) {
      optionImmediateSelect = option.nextBoolean();
    }
    if (option.check('-automatic_play')) {
      optionAutomaticPlay = option.nextBoolean();
    }
    if (option.check('-repeat_play')) {
      optionRepeatPlay = option.nextBoolean();
    }
    if (option.check('-keep_speed')) {
      optionKeepSpeed = option.nextBoolean();
    }
    if (option.check('-show_boundary')) {
      optionShowBoundary = option.nextBoolean();
    }
    if (option.check('-animation_file')) {
      optionAnimationFile = option.nextString();
    }
    if (option.check('-image_filter')) {
      optionImageFilter = option.nextString().split(',').where((value) => value.isNotEmpty).map(Integer.parse).toList();
    }
    if (option.check('-sprite_filter')) {
      optionSpriteFilter = option.nextString().split(',').where((value) => value.isNotEmpty).map(Integer.parse).toList();
    }
    if (option.check('-active_target')) {
      optionActiveTarget = (
        option.nextBoolean(),
        option.nextInteger(),
      );
    }
    if (option.check('-active_frame_range')) {
      optionActiveFrameRange = (
        option.nextInteger(),
        option.nextInteger(),
      );
    }
    if (option.check('-active_frame_speed')) {
      optionActiveFrameSpeed = option.nextFloater();
    }
    if (option.check('-active_progress_index')) {
      optionActiveProgressIndex = option.nextInteger();
    }
    if (option.check('-active_progress_state')) {
      optionActiveProgressState = option.nextBoolean();
    }
    if (!option.done()) {
      throw Exception('too many option \'${option.nextStringList().join(' ')}\'');
    }
    if (optionImmediateSelect != null) {
      this._immediateSelect = optionImmediateSelect;
    }
    if (optionAutomaticPlay != null) {
      this._automaticPlay = optionAutomaticPlay;
    }
    if (optionRepeatPlay != null) {
      this._repeatPlay = optionRepeatPlay;
    }
    if (optionKeepSpeed != null) {
      this._keepSpeed = optionKeepSpeed;
    }
    if (optionShowBoundary != null) {
      this._showBoundary = optionShowBoundary;
    }
    if (optionAnimationFile != null) {
      await this._applyLoad(
        optionAnimationFile,
        optionImageFilter,
        optionSpriteFilter,
        optionActiveTarget,
        optionActiveFrameRange,
        optionActiveFrameSpeed,
        optionActiveProgressIndex,
        optionActiveProgressState,
      );
    }
    await refreshState(this.setState);
    return;
  }

  @override
  modulePageCollectOption() async {
    var option = CommandLineWriter();
    if (option.check('-immediate_select')) {
      option.nextBoolean(this._immediateSelect);
    }
    if (option.check('-automatic_play')) {
      option.nextBoolean(this._automaticPlay);
    }
    if (option.check('-repeat_play')) {
      option.nextBoolean(this._repeatPlay);
    }
    if (option.check('-keep_speed')) {
      option.nextBoolean(this._keepSpeed);
    }
    if (option.check('-show_boundary')) {
      option.nextBoolean(this._showBoundary);
    }
    if (option.check('-animation_file', state: this._loaded)) {
      option.nextString(this._animationFile!);
    }
    if (option.check('-image_filter', state: this._loaded)) {
      option.nextString(this._imageFilter!.mapIndexed((index, value) => value ? null : ConvertHelper.makeIntegerToString(index, false)).nonNulls.join(','));
    }
    if (option.check('-sprite_filter', state: this._loaded)) {
      option.nextString(this._spriteFilter!.mapIndexed((index, value) => value ? null : ConvertHelper.makeIntegerToString(index, false)).nonNulls.join(','));
    }
    if (option.check('-active_target', state: this._activated)) {
      option.nextBoolean(this._activeTarget!.$1);
      option.nextInteger(this._activeTarget!.$2);
    }
    if (option.check('-active_frame_range', state: this._activated)) {
      option.nextInteger(this._activeFrameRange!.$1);
      option.nextInteger(this._activeFrameRange!.$2);
    }
    if (option.check('-active_frame_speed', state: this._activated)) {
      option.nextFloater(this._activeFrameSpeed!);
    }
    if (option.check('-active_progress_index', state: this._activated)) {
      option.nextInteger(this._queryProgressIndex());
    }
    if (option.check('-active_progress_state', state: this._activated)) {
      option.nextBoolean(this._queryProgressState());
    }
    return option.done();
  }

  // ----------------

  @override
  initState() {
    super.initState();
    var setting = Provider.of<SettingProvider>(this.context, listen: false);
    this._immediateSelect = setting.data.animationViewer.immediateSelect;
    this._automaticPlay = setting.data.animationViewer.automaticPlay;
    this._repeatPlay = setting.data.animationViewer.repeatPlay;
    this._keepSpeed = setting.data.animationViewer.keepSpeed;
    this._showBoundary = setting.data.animationViewer.showBoundary;
    this._animationFile = null;
    this._animation = null;
    this._texture = null;
    this._imageFilter = null;
    this._spriteFilter = null;
    this._activeTarget = null;
    this._activeSprite = null;
    this._activeFrameLabel = null;
    this._activeFrameRange = null;
    this._activeFrameSpeed = null;
    this._activeProgressIndexStream = StreamController();
    this._activeProgressStateStream = StreamController();
    this._activeProgressChangingContinue = false;
    this._animationController = AnimationController(lowerBound: 0.0, upperBound: 1.0 - 1.0e-9, vsync: this);
    this._animationController.addListener(() async {
      this._activeProgressIndexStream.sink.add(null);
      return;
    });
    this._animationController.addStatusListener((status) async {
      if (status == AnimationStatus.completed) {
        if (!this._repeatPlay) {
          this._activeProgressStateStream.sink.add(null);
        }
        else {
          this._changeProgressState(true);
        }
      }
      return;
    });
    this._animationVisual = null;
    this._stageHorizontalScrollSontroller = ScrollController();
    this._stageVerticalScrollSontroller = ScrollController();
    postTask(() async {
      await this.modulePageOpenView();
      await this.modulePageApplyOption(this.widget.option);
    });
    return;
  }

  @override
  didUpdateWidget(oldWidget) {
    super.didUpdateWidget(oldWidget);
    return;
  }

  @override
  dispose() {
    this._activeProgressIndexStream.close();
    this._activeProgressStateStream.close();
    this._animationController.dispose();
    this._stageHorizontalScrollSontroller.dispose();
    this._stageVerticalScrollSontroller.dispose();
    super.dispose();
    return;
  }

  @override
  build(context) {
    return ModulePageRegion(
      onDropFile: (item) async {
        if (item.length != 1) {
          await StyledSnackExtension.show(context, 'Source is multiply.');
          return;
        }
        if (!await StorageHelper.existFile(item.first)) {
          await StyledSnackExtension.show(context, 'Source is not a file.');
          return;
        }
        await this._applyLoad(item.first, null, null, null, null, null, null, null);
        return;
      },
      content: FlexContainer.vertical([
        StyledCard.outlined(
          margin: .fromLTRB(16, 12, 16, 12),
          content: FlexContainer.vertical([
            BoxContainer.of(
              color: StyledColor.surfaceContainer.query(context),
              child: LayoutBuilder(
                builder: (context, constraints) => BoxContainer.of(
                  constraints: .tightFor(
                    width: this._animation?.size.$1 ?? 0.0,
                    height: this._animation?.size.$2 ?? 0.0,
                  ),
                  margin: .symmetric(
                    horizontal: max(0, (constraints.maxWidth - (this._animation?.size.$1 ?? 0.0)) / 2.0),
                    vertical: max(0, (constraints.maxHeight - (this._animation?.size.$2 ?? 0.0)) / 2.0),
                  ),
                  color: !this._showBoundary ? null : StyledColor.surfaceContainerHighest.query(context),
                  child: this._animationVisual,
                ).withScrollableArea(
                  horizontal: this._stageHorizontalScrollSontroller,
                  vertical: this._stageVerticalScrollSontroller,
                ).withStyledScrollBar(
                  controller: this._stageHorizontalScrollSontroller,
                ).withStyledScrollBar(
                  controller: this._stageVerticalScrollSontroller,
                ),
              ),
            ).withFlexExpanded(),
            StyledDivider.minimal(),
            Gap.vertical(8),
            FlexContainer.horizontal([
              Gap.horizontal(4),
              StreamBuilder(
                stream: this._activeProgressIndexStream.stream,
                builder: (context, snapshot) => StyledSlider.standard(
                  enabled: this._activated,
                  tooltip: !this._activated ? '' : '${this._queryProgressIndex() + 1}',
                  minimum: 1.0,
                  maximum: !this._activated ? 1.0 : (this._activeSprite!.frame.length + 1.0e-9),
                  value: !this._activated ? 1.0 : (this._queryProgressIndex() + 1),
                  onChanged: (context, value) async {
                    await this._changeProgressIndex(value.round() - 1);
                    this._activeProgressIndexStream.sink.add(null);
                  },
                  onChangeStart: (context, value) async {
                    this._activeProgressChangingContinue = this._queryProgressState();
                    if (this._activeProgressChangingContinue) {
                      await this._changeProgressState(false);
                    }
                  },
                  onChangeEnd: (context, value) async {
                    if (this._activeProgressChangingContinue) {
                      await this._changeProgressState(true);
                    }
                    this._activeProgressChangingContinue = false;
                  },
                ),
              ).withFlexExpanded(),
              Gap.horizontal(4),
            ]),
            Gap.vertical(4),
            FlexContainer.horizontal([
              Gap.horizontal(16),
              StyledButton.text(
                enabled: this._activated,
                tooltip: 'Frame Range',
                iconAlign: .start,
                icon: Icon(IconSet.linear_scale),
                content: FlexContainer.horizontal([
                  StyledText.custom(
                    !this._activated ? '[ 0 - 0 ]' : '[ ${this._activeFrameRange!.$1 + 1} - ${this._activeFrameRange!.$2 + 1} ]',
                    align: .start,
                  ).withFlexExpanded(),
                ]),
                onPressed: (context) async {
                  var currentValue = this._activeFrameRange!;
                  await StyledModalDialogExtension.show<Void>(context, StyledModalDialog.standard(
                    title: 'Frame Range',
                    contentBuilder: (context, setStateForPanel) => [
                      StyledInput.outlined(
                        type: .numberWithOptions(signed: false, decimal: false),
                        format: null,
                        hint: null,
                        prefix: Icon(IconSet.line_start_circle),
                        suffix: [
                          StyledIconButton.standard(
                            tooltip: 'Preset',
                            icon: Icon(IconSet.more_vert),
                            onPressed: (context) async {
                              var value = await StyledMenuExtension.show<Integer>(context, StyledMenu.standard(
                                position: .under,
                                children: [
                                  (1, 'whole'),
                                  null,
                                  ...this._activeFrameLabel!.map((value) => (value.$2 + 1, value.$1)),
                                ].map((value) => value == null ? null : StyledMenuItem.standard(
                                  value: value.$1,
                                  content: StyledText.inherit(value.$2),
                                  trailing: StyledText.inherit(ConvertHelper.makeIntegerToString(value.$1, false)),
                                )),
                              ));
                              if (value != null) {
                                var selectedValue = value;
                                selectedValue -= 1;
                                currentValue = (selectedValue, max(selectedValue, currentValue.$2));
                                await refreshState(setStateForPanel);
                              }
                            },
                          ),
                        ],
                        value: ConvertHelper.makeIntegerToString(currentValue.$1 + 1, false),
                        onChanged: (context, text) async {
                          var value = Integer.tryParse(text);
                          if (value != null && value >= 1 && value <= this._activeSprite!.frame.length) {
                            value -= 1;
                            currentValue = (value, max(value, currentValue.$2));
                          }
                          await refreshState(setStateForPanel);
                        },
                      ),
                      Gap.vertical(12),
                      StyledInput.outlined(
                        type: .numberWithOptions(signed: false, decimal: false),
                        format: null,
                        hint: null,
                        prefix: Icon(IconSet.line_end_circle),
                        suffix: [
                          StyledIconButton.standard(
                            tooltip: 'Preset',
                            icon: Icon(IconSet.more_vert),
                            onPressed: (context) async {
                              var value = await StyledMenuExtension.show<Integer>(context, StyledMenu.standard(
                                position: .under,
                                children: [
                                  (this._activeSprite!.frame.length, 'whole'),
                                  null,
                                  ...this._activeFrameLabel!.map((value) => (value.$3 + 1, value.$1)),
                                ].map((value) => value == null ? null : StyledMenuItem.standard(
                                  value: value.$1,
                                  content: StyledText.inherit(value.$2),
                                  trailing: StyledText.inherit(ConvertHelper.makeIntegerToString(value.$1, false)),
                                )),
                              ));
                              if (value != null) {
                                var selectedValue = value;
                                selectedValue -= 1;
                                currentValue = (min(selectedValue, currentValue.$1), selectedValue);
                                await refreshState(setStateForPanel);
                              }
                            },
                          ),
                        ],
                        value: ConvertHelper.makeIntegerToString(currentValue.$2 + 1, false),
                        onChanged: (context, text) async {
                          var value = Integer.tryParse(text);
                          if (value != null && value >= 1 && value <= this._activeSprite!.frame.length) {
                            value -= 1;
                            currentValue = (min(value, currentValue.$1), value);
                          }
                          await refreshState(setStateForPanel);
                        },
                      ),
                    ],
                    actionBuilder: null,
                  ));
                  await this._changeFrameRange(currentValue);
                },
              ).withFlexExpanded(),
              Gap.horizontal(12),
              StyledIconButton.standard(
                enabled: this._activated,
                tooltip: 'Previous',
                selected: true,
                icon: Icon(IconSet.arrow_back),
                onPressed: (context) async {
                  await this._changeProgressIndex(max(this._queryProgressIndex() - 1, 0));
                },
              ),
              Gap.horizontal(8),
              StreamBuilder(
                stream: this._activeProgressStateStream.stream,
                builder: (context, snapshot) => StyledIconButton.filled(
                  enabled: this._activated,
                  tooltip: !this._activated ? null : !this._queryProgressState() ? 'Resume' : 'Pause',
                  selected: true,
                  icon: Icon(!this._activated ? IconSet.play_arrow : !this._queryProgressState() ? IconSet.play_arrow : IconSet.pause, fill: 1),
                  onPressed: (context) async {
                    await this._changeProgressState(!this._queryProgressState());
                  },
                ),
              ),
              Gap.horizontal(8),
              StyledIconButton.standard(
                enabled: this._activated,
                tooltip: 'Next',
                selected: true,
                icon: Icon(IconSet.arrow_forward),
                onPressed: (context) async {
                  await this._changeProgressIndex(min(this._queryProgressIndex() + 1, this._activeSprite!.frame.length - 1));
                },
              ),
              Gap.horizontal(12),
              StyledButton.text(
                enabled: this._activated,
                tooltip: 'Frame Speed',
                iconAlign: .end,
                icon: Icon(IconSet.speed),
                content: FlexContainer.horizontal([
                  StyledText.custom(
                    !this._activated ? '0.0' : ConvertHelper.makeFloaterToString(this._activeFrameSpeed!, false),
                    align: .end,
                  ).withFlexExpanded(),
                ]),
                onPressed: (context) async {
                  var currentValue = this._activeFrameSpeed!;
                  var normalSpeed = this._activeSprite?.frameRate ?? this._animation!.frameRate.toDouble();
                  await StyledModalDialogExtension.show<Void>(context, StyledModalDialog.standard(
                    title: 'Frame Speed',
                    contentBuilder: (context, setStateForPanel) => [
                      StyledInput.outlined(
                        type: .numberWithOptions(signed: false, decimal: true),
                        format: null,
                        hint: null,
                        prefix: Icon(IconSet.speed),
                        suffix: [
                          StyledIconButton.standard(
                            tooltip: 'Preset',
                            icon: Icon(IconSet.more_vert),
                            onPressed: (context) async {
                              var value = await StyledMenuExtension.show<Floater>(context, StyledMenu.standard(
                                position: .under,
                                children: [
                                  (normalSpeed / 2.0, 'Slow'),
                                  (normalSpeed, 'Normal'),
                                  (normalSpeed * 2.0, 'Fast'),
                                ].map((value) => StyledMenuItem.standard(
                                  value: value.$1,
                                  content: StyledText.inherit(value.$2),
                                  trailing: StyledText.inherit(ConvertHelper.makeFloaterToString(value.$1, false)),
                                )),
                              ));
                              if (value != null) {
                                currentValue = value;
                                await refreshState(setStateForPanel);
                              }
                            },
                          ),
                        ],
                        value: ConvertHelper.makeFloaterToString(currentValue, false),
                        onChanged: (context, text) async {
                          var value = Floater.tryParse(text);
                          if (value != null && value.isFinite && value > 0.0) {
                            currentValue = value;
                          }
                          await refreshState(setStateForPanel);
                        },
                      ),
                    ],
                    actionBuilder: null,
                  ));
                  await this._changeFrameSpeed(currentValue);
                },
              ).withFlexExpanded(),
              Gap.horizontal(16),
            ]),
            Gap.vertical(12),
            StyledDivider.minimal(),
            Gap.vertical(12),
            FlexContainer.horizontal([
              Gap.horizontal(16),
              StyledButton.text(
                enabled: this._loaded,
                tooltip: 'Image',
                iconAlign: .start,
                icon: Icon(IconSet.imagesmode),
                content: FlexContainer.horizontal([
                  StyledText.custom(
                    !this._loaded ? '0 - 0' : '${this._animation!.image.length} - ${this._imageFilter!.where((value) => value).length}',
                    align: .start,
                  ).withFlexExpanded(),
                ]),
                onPressed: (context) async {
                  var currentValue = [...this._imageFilter!];
                  await StyledModalDialogExtension.show<Void>(context, StyledModalDialog.standard(
                    title: 'Image',
                    contentBuilder: (context, setStateForPanel) => [
                      ...this._animation!.image.mapIndexed((index, item) => StyledListTile.standard(
                        dense: true,
                        leading: StyledCheck.standard(
                          value: currentValue[index],
                          onChanged: (context, value) async {
                          },
                        ).withImpenetrableArea(
                        ),
                        content: StyledText.inherit(tooltip: true, VisualHelper.parseImageFileName(item.name)),
                        trailing: StyledText.inherit('${item.size?.$1 ?? 0} x ${item.size?.$2 ?? 0}'),
                        onPressed: (context) async {
                          currentValue[index] = !currentValue[index];
                          await refreshState(setStateForPanel);
                        },
                        onLongPressed: (context) async {
                          var newFrameSpeed = !this._keepSpeed ? null : this._activeFrameSpeed;
                          if (this._activated) {
                            await this._deactivate();
                          }
                          await this._activate((false, index), null, newFrameSpeed, null, null);
                          Navigator.pop(context);
                        },
                      )),
                    ],
                    actionBuilder: null,
                  ));
                  await this._changeElementFilter(currentValue, null);
                },
              ).withFlexExpanded(weight: 3),
              Gap.horizontal(12),
              StyledButton.filled(
                enabled: this._loaded,
                tooltip: !this._loaded ? '' : !this._activated ? 'Activate' : 'Deactivate',
                content: !this._activated
                  ? Icon(IconSet.power_settings_new)
                  : StyledText.custom(
                    this._activeSprite!.name ?? '',
                    align: .center,
                  ),
                onPressed: (context) async {
                  if (this._activated) {
                    await this._deactivate();
                  }
                  else {
                    if (this._animation!.mainSprite == null) {
                      await StyledSnackExtension.show(context, 'The animation does not contain main sprite.');
                    }
                    else {
                      await this._activate((true, this._animation!.sprite.length), null, null, null, null);
                    }
                  }
                  await refreshState(this.setState);
                },
              ).withFlexExpanded(weight: 5),
              Gap.horizontal(12),
              StyledButton.text(
                enabled: this._loaded,
                tooltip: 'Sprite',
                iconAlign: .end,
                icon: Icon(IconSet.thread_unread),
                content: FlexContainer.horizontal([
                  StyledText.custom(
                    !this._loaded ? '0 - 0' : '${this._spriteFilter!.where((value) => value).length} - ${this._animation!.sprite.length}',
                    align: .end,
                  ).withFlexExpanded(),
                ]),
                onPressed: (context) async {
                  var currentValue = [...this._spriteFilter!];
                  await StyledModalDialogExtension.show<Void>(context, StyledModalDialog.standard(
                    title: 'Sprite',
                    contentBuilder: (context, setStateForPanel) => [
                      ...this._animation!.sprite.mapIndexed((index, item) => StyledListTile.standard(
                        dense: true,
                        leading: StyledCheck.standard(
                          value: currentValue[index],
                          onChanged: (context, value) async {
                          },
                        ).withImpenetrableArea(
                        ),
                        content: StyledText.inherit(tooltip: true, item.name ?? ''),
                        trailing: StyledText.inherit('${item.frame.length} / ${item.frameRate == null ? '?' : ConvertHelper.makeFloaterToString(item.frameRate!, false)}'),
                        onPressed: (context) async {
                          currentValue[index] = !currentValue[index];
                          await refreshState(setStateForPanel);
                        },
                        onLongPressed: (context) async {
                          var newFrameSpeed = !this._keepSpeed ? null : this._activeFrameSpeed;
                          if (this._activated) {
                            await this._deactivate();
                          }
                          await this._activate((true, index), null, newFrameSpeed, null, null);
                          Navigator.pop(context);
                        },
                      )),
                    ],
                    actionBuilder: null,
                  ));
                  await this._changeElementFilter(null, currentValue);
                },
              ).withFlexExpanded(weight: 3),
              Gap.horizontal(16),
            ]),
            Gap.vertical(12),
          ]),
        ).withFlexExpanded(),
      ]),
      bottom: StyledBottomBar.standard(
        primary: StyledFloatingButton.standard(
          tooltip: 'Source',
          icon: Icon(IconSet.description),
          onPressed: (context) async {
            await StyledModalDialogExtension.show<Void>(context, StyledModalDialog.standard(
              title: 'Source',
              contentBuilder: (context, setStateForPanel) => [
                StyledInput.outlined(
                  type: .none,
                  format: null,
                  hint: null,
                  prefix: Icon(IconSet.attachment),
                  suffix: null,
                  value: !this._loaded ? '' : this._animationFile!,
                  onChanged: (context, value) async {},
                ),
                Gap.vertical(12),
                FlexContainer.horizontal([
                  StyledButton.outlined(
                    enabled: this._loaded,
                    icon: Icon(IconSet.clear_all),
                    content: StyledText.inherit('Clear'),
                    onPressed: (context) async {
                      if (this._activated) {
                        await this._deactivate();
                      }
                      await this._unload();
                      await refreshState(setStateForPanel);
                    },
                  ).withFlexExpanded(),
                  Gap.horizontal(8),
                  StyledButton.filled(
                    icon: Icon(IconSet.open_in_new),
                    content: StyledText.inherit('Pick'),
                    onPressed: (context) async {
                      var animationFile = await StorageHelper.pickLoadFile(context, '@AnimationViewer.AnimationFile');
                      if (animationFile != null) {
                        Navigator.pop(context);
                        await this._applyLoad(animationFile, null, null, null, null, null, null, null);
                      }
                    },
                  ).withFlexExpanded(),
                ]),
              ],
              actionBuilder: null,
            ));
          },
        ),
        secondary: [
          StyledIconButton.filledTonal(
            tooltip: 'Immediate Select',
            selected: this._immediateSelect,
            icon: Icon(IconSet.ads_click),
            iconOnSelected: Icon(IconSet.ads_click, fill: 1),
            onPressed: (context) async {
              this._immediateSelect = !this._immediateSelect;
              await refreshState(this.setState);
            },
          ),
          Gap.horizontal(8),
          StyledIconButton.filledTonal(
            tooltip: 'Automatic Play',
            selected: this._automaticPlay,
            icon: Icon(IconSet.autoplay),
            iconOnSelected: Icon(IconSet.autoplay, fill: 1),
            onPressed: (context) async {
              this._automaticPlay = !this._automaticPlay;
              await refreshState(this.setState);
            },
          ),
          Gap.horizontal(8),
          StyledIconButton.filledTonal(
            tooltip: 'Repeat Play',
            selected: this._repeatPlay,
            icon: Icon(IconSet.repeat),
            iconOnSelected: Icon(IconSet.repeat, fill: 1),
            onPressed: (context) async {
              this._repeatPlay = !this._repeatPlay;
              await refreshState(this.setState);
            },
          ),
          Gap.horizontal(8),
          StyledIconButton.filledTonal(
            tooltip: 'Keep Speed',
            selected: this._keepSpeed,
            icon: Icon(IconSet.lock_reset),
            iconOnSelected: Icon(IconSet.lock_reset, fill: 1),
            onPressed: (context) async {
              this._keepSpeed = !this._keepSpeed;
              await refreshState(this.setState);
            },
          ),
          Gap.horizontal(8),
          StyledIconButton.filledTonal(
            tooltip: 'Show Boundary',
            selected: this._showBoundary,
            icon: Icon(IconSet.frame_source),
            iconOnSelected: Icon(IconSet.frame_source, fill: 1),
            onPressed: (context) async {
              this._showBoundary = !this._showBoundary;
              await refreshState(this.setState);
            },
          ),
        ],
      ),
    );
  }

}
