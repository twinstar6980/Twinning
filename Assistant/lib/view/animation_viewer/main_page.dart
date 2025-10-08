import '/common.dart';
import '/setting.dart';
import '/utility/command_line_reader.dart';
import '/utility/command_line_writer.dart';
import '/utility/storage_helper.dart';
import '/utility/convert_helper.dart';
import '/utility/control_helper.dart';
import '/view/home/common.dart';
import '/view/animation_viewer/model.dart' as model;
import '/view/animation_viewer/visual_helper.dart';
import 'dart:math';
import 'dart:async';
import 'package:collection/collection.dart';
import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import 'package:single_child_two_dimensional_scroll_view/single_child_two_dimensional_scroll_view.dart';

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

class _MainPageState extends State<MainPage> with SingleTickerProviderStateMixin implements CustomModulePageState {

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
      activeSprite = model.Sprite(
        name: VisualHelper.parseImageFileName(originalTarget.name),
        frameRate: null,
        workArea: (0, 0),
        frame: [
          model.Frame(
            label: null,
            stop: false,
            command: [],
            remove: [],
            append: [
              model.LayerAppend(
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
              model.LayerChange(
                index: 0,
                transform: model.VariantTransform(value: model.TranslateTransform(x: 0.0, y: 0.0)),
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
    this._animationController.duration = Duration.zero;
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
    // TODO
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
    this._animationController.duration = Duration(milliseconds: (this._activeSprite!.frame.length * 1000 / this._activeFrameSpeed!).ceil());
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
    ControlHelper.postTask(() async {
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
    var theme = Theme.of(context);
    return CustomModulePageRegion(
      onDropFile: (item) async {
        if (item.length != 1) {
          await ControlHelper.showSnackBar(context, 'Source is multiply.');
          return;
        }
        if (!await StorageHelper.existFile(item.first)) {
          await ControlHelper.showSnackBar(context, 'Source is not a file.');
          return;
        }
        await this._applyLoad(item.first, null, null, null, null, null, null, null);
        return;
      },
      content: Column(
        children: [
          Card.outlined(
            clipBehavior: Clip.antiAliasWithSaveLayer,
            margin: EdgeInsets.fromLTRB(16, 12, 16, 12),
            child: Column(
              children: [
                Container(
                  color: theme.colorScheme.surfaceContainer,
                  child: LayoutBuilder(
                    builder: (context, constraints) => SingleChildTwoDimensionalScrollView(
                      clipBehavior: Clip.antiAliasWithSaveLayer,
                      padding: EdgeInsets.symmetric(
                        horizontal: max(0, (constraints.maxWidth - (this._animation?.size.$1 ?? 0.0)) / 2.0),
                        vertical: max(0, (constraints.maxHeight - (this._animation?.size.$2 ?? 0.0)) / 2.0),
                      ),
                      horizontalController: this._stageHorizontalScrollSontroller,
                      verticalController: this._stageVerticalScrollSontroller,
                      child: Container(
                        color: !this._showBoundary ? null : theme.colorScheme.surfaceContainerHighest,
                        child: !this._loaded
                          ? SizedBox()
                          : SizedBox.fromSize(
                            size: Size(this._animation!.size.$1, this._animation!.size.$2),
                            child: UnconstrainedBox(
                              child: SizedOverflowBox(
                                alignment: AlignmentDirectional.topStart,
                                size: Size(this._animation!.size.$1, this._animation!.size.$2),
                                child: this._animationVisual,
                              ),
                            ),
                          ),
                      ),
                    ).withScrollbar(
                      controller: this._stageVerticalScrollSontroller,
                    ).withScrollbar(
                      controller: this._stageHorizontalScrollSontroller,
                    ),
                  ),
                ).withExpanded(),
                Divider(height: 0),
                SizedBox(height: 8),
                Row(
                  children: [
                    SizedBox(width: 12),
                    LayoutBuilder(
                      builder: (context, constraints) => IntrinsicHeight(
                        child: OverflowBox(
                          maxWidth: constraints.maxWidth + 16,
                          child: StreamBuilder(
                            stream: this._activeProgressIndexStream.stream,
                            builder: (context, snapshot) => Slider(
                              min: 1.0,
                              max: !this._activated ? 1.0 : (this._activeSprite!.frame.length + 1.0e-9),
                              value: !this._activated ? 1.0 : (this._queryProgressIndex() + 1),
                              onChanged: !this._activated
                                ? null
                                : (value) async {
                                  await this._changeProgressIndex(value.round() - 1);
                                  this._activeProgressIndexStream.sink.add(null);
                                },
                              onChangeStart: !this._activated
                                ? null
                                : (value) async {
                                  this._activeProgressChangingContinue = this._queryProgressState();
                                  if (this._activeProgressChangingContinue) {
                                    await this._changeProgressState(false);
                                  }
                                },
                              onChangeEnd: !this._activated
                                ? null
                                : (value) async {
                                  if (this._activeProgressChangingContinue) {
                                    await this._changeProgressState(true);
                                  }
                                  this._activeProgressChangingContinue = false;
                                },
                            ).withTooltip(
                              message: !this._activated ? '' : '${this._queryProgressIndex() + 1}',
                            ),
                          ),
                        ),
                      ),
                    ).withExpanded(),
                    SizedBox(width: 12),
                  ],
                ),
                SizedBox(height: 4),
                Row(
                  children: [
                    SizedBox(width: 16),
                    TextButton.icon(
                      iconAlignment: IconAlignment.start,
                      icon: Icon(IconSymbols.linear_scale),
                      label: Row(
                        children: [
                          Text(
                            !this._activated ? '[ 0 - 0 ]' : '[ ${this._activeFrameRange!.$1 + 1} - ${this._activeFrameRange!.$2 + 1} ]',
                            overflow: TextOverflow.ellipsis,
                            textAlign: TextAlign.start,
                          ).withExpanded(),
                        ],
                      ),
                      onPressed: !this._activated
                        ? null
                        : () async {
                          var currentValue = this._activeFrameRange!;
                          await ControlHelper.showDialogAsModal<Void>(context, CustomModalDialog(
                            title: 'Frame Range',
                            contentBuilder: (context, setStateForPanel) => [
                              CustomTextField(
                                keyboardType: TextInputType.numberWithOptions(signed: false, decimal: false),
                                inputFormatters: [],
                                decoration: InputDecoration(
                                  contentPadding: EdgeInsets.fromLTRB(12, 16, 12, 16),
                                  filled: false,
                                  border: OutlineInputBorder(),
                                  prefixIcon: Icon(IconSymbols.line_start_circle),
                                  suffixIcon: CustomTextFieldSuffixRegion(
                                    children: [
                                      PopupMenuButton(
                                        tooltip: 'Preset',
                                        position: PopupMenuPosition.under,
                                        icon: Icon(IconSymbols.more_vert),
                                        itemBuilder: (context) => [
                                          (1, 'whole'),
                                          null,
                                          ...this._activeFrameLabel!.map((value) => (value.$2 + 1, value.$1)),
                                        ].map((value) => value == null
                                          ? PopupMenuDivider().as<PopupMenuEntry<Object>>()
                                          : PopupMenuItem(
                                            value: value.$1,
                                            child: ListTile(
                                              contentPadding: EdgeInsets.zero,
                                              dense: true,
                                              title: Text(
                                                value.$2,
                                                overflow: TextOverflow.ellipsis,
                                              ),
                                              trailing: Text(
                                                ConvertHelper.makeIntegerToString(value.$1, false),
                                                overflow: TextOverflow.ellipsis,
                                                style: theme.textTheme.labelSmall!.copyWith(
                                                  color: theme.colorScheme.onSurfaceVariant,
                                                ),
                                              ),
                                            ),
                                          )).toList(),
                                        onSelected: (value) async {
                                          value as Integer;
                                          value -= 1;
                                          currentValue = (value, max(value, currentValue.$2));
                                          await refreshState(setStateForPanel);
                                        },
                                      ),
                                    ],
                                  ),
                                ),
                                value: ConvertHelper.makeIntegerToString(currentValue.$1 + 1, false),
                                onChanged: (text) async {
                                  var value = Integer.tryParse(text);
                                  if (value != null && value >= 1 && value <= this._activeSprite!.frame.length) {
                                    value -= 1;
                                    currentValue = (value, max(value, currentValue.$2));
                                  }
                                  await refreshState(setStateForPanel);
                                },
                              ),
                              SizedBox(height: 12),
                              CustomTextField(
                                keyboardType: TextInputType.numberWithOptions(signed: false, decimal: false),
                                inputFormatters: [],
                                decoration: InputDecoration(
                                  contentPadding: EdgeInsets.fromLTRB(12, 16, 12, 16),
                                  filled: false,
                                  border: OutlineInputBorder(),
                                  prefixIcon: Icon(IconSymbols.line_end_circle),
                                  suffixIcon: CustomTextFieldSuffixRegion(
                                    children: [
                                      PopupMenuButton(
                                        tooltip: 'Preset',
                                        position: PopupMenuPosition.under,
                                        icon: Icon(IconSymbols.more_vert),
                                        itemBuilder: (context) => [
                                          (this._activeSprite!.frame.length, 'whole'),
                                          null,
                                          ...this._activeFrameLabel!.map((value) => (value.$3 + 1, value.$1)),
                                        ].map((value) => value == null
                                          ? PopupMenuDivider().as<PopupMenuEntry<Object>>()
                                          : PopupMenuItem(
                                            value: value.$1,
                                            child: ListTile(
                                              contentPadding: EdgeInsets.zero,
                                              dense: true,
                                              title: Text(
                                                value.$2,
                                                overflow: TextOverflow.ellipsis,
                                              ),
                                              trailing: Text(
                                                ConvertHelper.makeIntegerToString(value.$1, false),
                                                overflow: TextOverflow.ellipsis,
                                                style: theme.textTheme.labelSmall!.copyWith(
                                                  color: theme.colorScheme.onSurfaceVariant,
                                                ),
                                              ),
                                            ),
                                          )).toList(),
                                        onSelected: (value) async {
                                          value as Integer;
                                          value -= 1;
                                          currentValue = (min(value, currentValue.$1), value);
                                          await refreshState(setStateForPanel);
                                        },
                                      ),
                                    ],
                                  ),
                                ),
                                value: ConvertHelper.makeIntegerToString(currentValue.$2 + 1, false),
                                onChanged: (text) async {
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
                    ).withTooltip(
                      message: !this._activated ? '' : 'Frame Range',
                    ).withExpanded(),
                    SizedBox(width: 12),
                    IconButton(
                      tooltip: !this._activated ? '' : 'Previous',
                      isSelected: true,
                      icon: Icon(IconSymbols.arrow_back),
                      onPressed: !this._activated
                        ? null
                        : () async {
                          await this._changeProgressIndex(max(this._queryProgressIndex() - 1, 0));
                        },
                    ),
                    SizedBox(width: 8),
                    StreamBuilder(
                      stream: this._activeProgressStateStream.stream,
                      builder: (context, snapshot) => IconButton.filled(
                        tooltip: !this._activated ? '' : !this._queryProgressState() ? 'Resume' : 'Pause',
                        isSelected: true,
                        icon: Icon(!this._activated ? IconSymbols.play_arrow : !this._queryProgressState() ? IconSymbols.play_arrow : IconSymbols.pause, fill: 1),
                        onPressed: !this._activated
                          ? null
                          : () async {
                            await this._changeProgressState(!this._queryProgressState());
                          },
                      ),
                    ),
                    SizedBox(width: 8),
                    IconButton(
                      tooltip: !this._activated ? '' : 'Next',
                      isSelected: true,
                      icon: Icon(IconSymbols.arrow_forward),
                      onPressed: !this._activated
                        ? null
                        : () async {
                          await this._changeProgressIndex(min(this._queryProgressIndex() + 1, this._activeSprite!.frame.length - 1));
                        },
                    ),
                    SizedBox(width: 12),
                    TextButton.icon(
                      iconAlignment: IconAlignment.end,
                      icon: Icon(IconSymbols.speed),
                      label: Row(
                        children: [
                          Text(
                            !this._activated ? '0.0' : ConvertHelper.makeFloaterToString(this._activeFrameSpeed!, false),
                            overflow: TextOverflow.ellipsis,
                            textAlign: TextAlign.end,
                          ).withExpanded(),
                        ],
                      ),
                      onPressed: !this._activated
                        ? null
                        : () async {
                          var currentValue = this._activeFrameSpeed!;
                          var normalSpeed = this._activeSprite?.frameRate ?? this._animation!.frameRate.toDouble();
                          await ControlHelper.showDialogAsModal<Void>(context, CustomModalDialog(
                            title: 'Frame Speed',
                            contentBuilder: (context, setStateForPanel) => [
                              CustomTextField(
                                keyboardType: TextInputType.numberWithOptions(signed: false, decimal: true),
                                inputFormatters: [],
                                decoration: InputDecoration(
                                  contentPadding: EdgeInsets.fromLTRB(12, 16, 12, 16),
                                  filled: false,
                                  border: OutlineInputBorder(),
                                  prefixIcon: Icon(IconSymbols.speed),
                                  suffixIcon: CustomTextFieldSuffixRegion(
                                    children: [
                                      PopupMenuButton(
                                        tooltip: 'Preset',
                                        position: PopupMenuPosition.under,
                                        icon: Icon(IconSymbols.more_vert),
                                        itemBuilder: (context) => [
                                          (normalSpeed / 2.0, 'Slow'),
                                          (normalSpeed, 'Normal'),
                                          (normalSpeed * 2.0, 'Fast'),
                                        ].map((value) => PopupMenuItem(
                                          value: value.$1,
                                          child: ListTile(
                                            contentPadding: EdgeInsets.zero,
                                            dense: true,
                                            title: Text(
                                              value.$2,
                                              overflow: TextOverflow.ellipsis,
                                            ),
                                            trailing: Text(
                                              ConvertHelper.makeFloaterToString(value.$1, false),
                                              overflow: TextOverflow.ellipsis,
                                              style: theme.textTheme.labelSmall!.copyWith(
                                                color: theme.colorScheme.onSurfaceVariant,
                                              ),
                                            ),
                                          ),
                                        )).toList(),
                                        onSelected: (value) async {
                                          currentValue = value;
                                          await refreshState(setStateForPanel);
                                        },
                                      ),
                                    ],
                                  ),
                                ),
                                value: ConvertHelper.makeFloaterToString(currentValue, false),
                                onChanged: (text) async {
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
                    ).withTooltip(
                      message: !this._activated ? '' : 'Frame Speed',
                    ).withExpanded(),
                    SizedBox(width: 16),
                  ],
                ),
                SizedBox(height: 12),
                Divider(height: 0),
                SizedBox(height: 12),
                Row(
                  children: [
                    SizedBox(width: 16),
                    TextButton.icon(
                      iconAlignment: IconAlignment.start,
                      icon: Icon(IconSymbols.imagesmode),
                      label: Row(
                        children: [
                          Text(
                            !this._loaded ? '0 - 0' : '${this._animation!.image.length} - ${this._imageFilter!.where((value) => value).length}',
                            overflow: TextOverflow.ellipsis,
                            textAlign: TextAlign.start,
                          ).withExpanded(),
                        ],
                      ),
                      onPressed: !this._loaded
                        ? null
                        : () async {
                          var currentValue = [...this._imageFilter!];
                          await ControlHelper.showDialogAsModal<Void>(context, CustomModalDialog(
                            title: 'Image',
                            contentBuilder: (context, setStateForPanel) => [
                              ...this._animation!.image.mapIndexed((index, item) => ListTile(
                                dense: true,
                                leading: Checkbox(
                                  value: currentValue[index],
                                  onChanged: (value) async {
                                  },
                                ).withIgnorePointer(
                                ),
                                title: Text(
                                  VisualHelper.parseImageFileName(item.name),
                                  overflow: TextOverflow.ellipsis,
                                ).withTooltip(
                                  message: VisualHelper.parseImageFileName(item.name),
                                ),
                                trailing: Row(
                                  mainAxisSize: MainAxisSize.min,
                                  children: [
                                    Text(
                                      '${item.size?.$1 ?? 0} x ${item.size?.$2 ?? 0}',
                                      overflow: TextOverflow.ellipsis,
                                    ),
                                  ],
                                ),
                                onTap: () async {
                                  currentValue[index] = !currentValue[index];
                                  await refreshState(setStateForPanel);
                                },
                                onLongPress: () async {
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
                    ).withTooltip(
                      message: !this._loaded ? '' : 'Image',
                    ).withExpanded(flex: 3),
                    SizedBox(width: 12),
                    FilledButton(
                      style: ButtonStyle(
                        padding: WidgetStatePropertyAll(EdgeInsets.fromLTRB(8, 8, 8, 8)),
                      ),
                      child: Row(
                        children: [
                          Icon(IconSymbols.arrow_back_ios_new),
                          SizedBox(width: 8),
                          if (!this._activated)
                            Icon(IconSymbols.power_settings_new).withExpanded()
                          else
                            Text(
                              this._activeSprite!.name ?? '',
                              overflow: TextOverflow.ellipsis,
                              textAlign: TextAlign.center,
                            ).withExpanded(),
                          SizedBox(width: 8),
                          Icon(IconSymbols.arrow_forward_ios),
                        ],
                      ),
                      onPressed: !this._loaded
                        ? null
                        : () async {
                          if (this._activated) {
                            await this._deactivate();
                          }
                          else {
                            if (this._animation!.mainSprite == null) {
                              await ControlHelper.showSnackBar(context, 'The animation does not contain main sprite.');
                            }
                            else {
                              await this._activate((true, this._animation!.sprite.length), null, null, null, null);
                            }
                          }
                          await refreshState(this.setState);
                        },
                    ).withTooltip(
                      message: !this._loaded ? '' : !this._activated ? 'Activate' : 'Deactivate',
                    ).withExpanded(flex: 5),
                    SizedBox(width: 12),
                    TextButton.icon(
                      iconAlignment: IconAlignment.end,
                      icon: Icon(IconSymbols.thread_unread),
                      label: Row(
                        children: [
                          Text(
                            !this._loaded ? '0 - 0' : '${this._spriteFilter!.where((value) => value).length} - ${this._animation!.sprite.length}',
                            overflow: TextOverflow.ellipsis,
                            textAlign: TextAlign.end,
                          ).withExpanded(),
                        ],
                      ),
                      onPressed: !this._loaded
                        ? null
                        : () async {
                          var currentValue = [...this._spriteFilter!];
                          await ControlHelper.showDialogAsModal<Void>(context, CustomModalDialog(
                            title: 'Sprite',
                            contentBuilder: (context, setStateForPanel) => [
                              ...this._animation!.sprite.mapIndexed((index, item) => ListTile(
                                dense: true,
                                leading: Checkbox(
                                  value: currentValue[index],
                                  onChanged: (value) async {
                                  },
                                ).withIgnorePointer(
                                ),
                                title: Text(
                                  item.name ?? '',
                                  overflow: TextOverflow.ellipsis,
                                ).withTooltip(
                                  message: item.name ?? '',
                                ),
                                trailing: Row(
                                  mainAxisSize: MainAxisSize.min,
                                  children: [
                                    Text(
                                      '${item.frame.length} / ${item.frameRate == null ? '?' : ConvertHelper.makeFloaterToString(item.frameRate!, false)}',
                                      overflow: TextOverflow.ellipsis,
                                    ),
                                  ],
                                ),
                                onTap: () async {
                                  currentValue[index] = !currentValue[index];
                                  await refreshState(setStateForPanel);
                                },
                                onLongPress: () async {
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
                    ).withTooltip(
                      message: !this._loaded ? '' : 'Sprite',
                    ).withExpanded(flex: 3),
                    SizedBox(width: 16),
                  ],
                ),
                SizedBox(height: 12),
              ],
            ),
          ).withExpanded(),
        ],
      ),
      bottom: CustomBottomBarContent(
        primary: FloatingActionButton(
          tooltip: 'Source',
          elevation: 0,
          focusElevation: 0,
          hoverElevation: 0,
          highlightElevation: 0,
          disabledElevation: 0,
          child: Icon(IconSymbols.description),
          onPressed: () async {
            await ControlHelper.showDialogAsModal<Void>(context, CustomModalDialog(
              title: 'Source',
              contentBuilder: (context, setStateForPanel) => [
                CustomTextField(
                  keyboardType: TextInputType.none,
                  inputFormatters: [],
                  decoration: InputDecoration(
                    contentPadding: EdgeInsets.fromLTRB(12, 16, 12, 16),
                    filled: false,
                    border: OutlineInputBorder(),
                    prefixIcon: Icon(IconSymbols.attachment),
                  ),
                  value: !this._loaded ? '' : this._animationFile!,
                  onChanged: null,
                ),
                SizedBox(height: 12),
                Row(
                  children: [
                    OutlinedButton.icon(
                      icon: Icon(IconSymbols.clear_all),
                      label: Text(
                        'Clear',
                        overflow: TextOverflow.ellipsis,
                      ),
                      onPressed: !this._loaded
                        ? null
                        : () async {
                          if (this._activated) {
                            await this._deactivate();
                          }
                          await this._unload();
                          await refreshState(setStateForPanel);
                        },
                    ).withExpanded(),
                    SizedBox(width: 8),
                    FilledButton.icon(
                      icon: Icon(IconSymbols.open_in_new),
                      label: Text(
                        'Pick',
                        overflow: TextOverflow.ellipsis,
                      ),
                      onPressed: () async {
                        var animationFile = await StorageHelper.pickLoadFile(context, '@AnimationViewer.AnimationFile');
                        if (animationFile != null) {
                          Navigator.pop(context);
                          await this._applyLoad(animationFile, null, null, null, null, null, null, null);
                        }
                      },
                    ).withExpanded(),
                  ],
                ),
              ],
              actionBuilder: null,
            ));
          },
        ),
        secondary: [
          IconButton.filledTonal(
            tooltip: 'Immediate Select',
            isSelected: this._immediateSelect,
            icon: Icon(IconSymbols.ads_click),
            selectedIcon: Icon(IconSymbols.ads_click, fill: 1),
            onPressed: () async {
              this._immediateSelect = !this._immediateSelect;
              await refreshState(this.setState);
            },
          ),
          SizedBox(width: 8),
          IconButton.filledTonal(
            tooltip: 'Automatic Play',
            isSelected: this._automaticPlay,
            icon: Icon(IconSymbols.autoplay),
            selectedIcon: Icon(IconSymbols.autoplay, fill: 1),
            onPressed: () async {
              this._automaticPlay = !this._automaticPlay;
              await refreshState(this.setState);
            },
          ),
          SizedBox(width: 8),
          IconButton.filledTonal(
            tooltip: 'Repeat Play',
            isSelected: this._repeatPlay,
            icon: Icon(IconSymbols.repeat),
            selectedIcon: Icon(IconSymbols.repeat, fill: 1),
            onPressed: () async {
              this._repeatPlay = !this._repeatPlay;
              await refreshState(this.setState);
            },
          ),
          SizedBox(width: 8),
          IconButton.filledTonal(
            tooltip: 'Keep Speed',
            isSelected: this._keepSpeed,
            icon: Icon(IconSymbols.lock_reset),
            selectedIcon: Icon(IconSymbols.lock_reset, fill: 1),
            onPressed: () async {
              this._keepSpeed = !this._keepSpeed;
              await refreshState(this.setState);
            },
          ),
          SizedBox(width: 8),
          IconButton.filledTonal(
            tooltip: 'Show Boundary',
            isSelected: this._showBoundary,
            icon: Icon(IconSymbols.frame_source),
            selectedIcon: Icon(IconSymbols.frame_source, fill: 1),
            onPressed: () async {
              this._showBoundary = !this._showBoundary;
              await refreshState(this.setState);
            },
          ),
        ],
      ),
    );
  }

}
