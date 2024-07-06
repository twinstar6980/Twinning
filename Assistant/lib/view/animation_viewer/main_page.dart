import '/common.dart';
import '/setting.dart';
import '/utility/command_line_reader.dart';
import '/utility/storage_helper.dart';
import '/utility/control_helper.dart';
import '/view/home/common.dart';
import '/view/animation_viewer/model.dart' as model;
import '/view/animation_viewer/visual.dart';
import 'dart:math';
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

  late Boolean              _immediateSelect;
  late Boolean              _automaticPlay;
  late Boolean              _repeatPlay;
  late Boolean              _remainFrameRate;
  late Boolean              _showSpriteBoundary;
  late String               _imageFilterRule;
  late String               _spriteFilterRule;
  late String?              _animationFile;
  late String?              _imageDirectory;
  late model.Animation?     _animation;
  late List<ImageProvider>? _imageSource;
  late List<Boolean>?       _imageFilter;
  late List<Boolean>?       _spriteFilter;
  late Integer?             _workingSpriteIndex;
  late (Integer, Integer)?  _workingSpriteFrameRange;
  late Floater?             _workingSpriteFrameRate;
  late Boolean?             _workingSpriteState;
  late AnimationController  _animationController;
  late Widget?              _animationVisual;
  late ScrollController     _stageHorizontalScrollSontroller;
  late ScrollController     _stageVerticalScrollSontroller;

  Boolean get _loaded => this._animationFile != null;

  Boolean get _working => this._workingSpriteIndex != null;

  Future<Void> _load(
    String animationFile,
    String imageDirectory,
  ) async {
    assertTest(!this._loaded && !this._working);
    var animationData = await VisualHelper.loadAnimation(animationFile);
    var imageSourceData = await VisualHelper.loadImageSource(imageDirectory, animationData);
    this._animationFile = animationFile;
    this._imageDirectory = imageDirectory;
    this._animation = animationData;
    this._imageSource = imageSourceData;
    this._imageFilter = List.filled(this._animation!.image.length, true);
    this._spriteFilter = List.filled(this._animation!.image.length, true);
    this.setState(() {});
    return;
  }

  Future<Void> _unload(
  ) async {
    assertTest(this._loaded && !this._working);
    this._animationFile = null;
    this._imageDirectory = null;
    this._animation = null;
    this._imageSource = null;
    this._imageFilter = null;
    this._spriteFilter = null;
    this.setState(() {});
    return;
  }

  Future<Void> _loadWorkingSprite(
    Integer index,
  ) async {
    assertTest(this._loaded && !this._working);
    var workingSprite = VisualHelper.selectSprite(this._animation!, index);
    this._workingSpriteIndex = index;
    this._workingSpriteFrameRange = (0, workingSprite.frame.length);
    this._workingSpriteFrameRate = workingSprite.frame_rate ?? this._animation!.frame_rate.toDouble();
    this._workingSpriteState = true;
    this._animationController.duration = Duration(seconds: (workingSprite.frame.length / 30.0).toInt());
    this._animationVisual = VisualHelper.visualizeSprite(this._animation!, this._imageSource!, this._workingSpriteIndex!, this._animationController!);
    this._animationVisual = SizedBox.fromSize(
      size: Size(this._animation!.size.$1, this._animation!.size.$2),
      child: UnconstrainedBox(
        child: SizedOverflowBox(
          alignment: AlignmentDirectional.topStart,
          size: Size(this._animation!.size.$1, this._animation!.size.$2),
          child: this._animationVisual,
        ),
      ),
    );
    this._animationController.repeat();
    this.setState(() {});
    return;
  }

  Future<Void> _unloadWorkingSprite(
  ) async {
    assertTest(this._loaded && this._working);
    this._workingSpriteIndex = null;
    this._workingSpriteFrameRange = null;
    this._workingSpriteFrameRate = null;
    this._workingSpriteState = null;
    this._animationController.duration = Duration.zero;
    this._animationController.reset();
    this._animationVisual = null;
    this.setState(() {});
    return;
  }

  Future<Void> _applyLoad(
    String  animationFile,
    String? imageDirectory,
  ) async {
    imageDirectory ??= StorageHelper.parent(animationFile);
    if (this._loaded) {
      if (this._working) {
        await this._unloadWorkingSprite();
      }
      await this._unload();
    }
    await this._load(animationFile, imageDirectory);
    //TODO
    await this._loadWorkingSprite(this._animation!.sprite.length);
    return;
  }

  // ----------------

  @override
  modulePageApplyOption(optionView) async {
    var option = CommandLineReader(optionView);
    // TODO
    assertTest(option.done());
    this.setState(() {});
    return;
  }

  @override
  modulePageCollectOption() async {
    return [];
  }

  @override
  modulePageRequestClose() async {
    return true;
  }

  // ----------------

  @override
  initState() {
    super.initState();
    var setting = Provider.of<SettingProvider>(this.context, listen: false);
    this._immediateSelect = setting.data.mAnimationViewer.mImmediateSelect;
    this._automaticPlay = setting.data.mAnimationViewer.mAutomaticPlay;
    this._repeatPlay = setting.data.mAnimationViewer.mRepeatPlay;
    this._remainFrameRate = setting.data.mAnimationViewer.mRemainFrameRate;
    this._showSpriteBoundary = setting.data.mAnimationViewer.mShowSpriteBoundary;
    this._imageFilterRule = setting.data.mAnimationViewer.mImageFilterRule;
    this._spriteFilterRule = setting.data.mAnimationViewer.mSpriteFilterRule;
    this._animationFile = null;
    this._imageDirectory = null;
    this._animation = null;
    this._imageSource = null;
    this._imageFilter = null;
    this._spriteFilter = null;
    this._workingSpriteIndex = null;
    this._workingSpriteFrameRange = null;
    this._workingSpriteFrameRate = null;
    this._workingSpriteState = null;
    this._animationController = AnimationController(vsync: this);
    this._animationVisual = null;
    this._stageHorizontalScrollSontroller = ScrollController();
    this._stageVerticalScrollSontroller = ScrollController();
    ControlHelper.postTask(() async {
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
    this._animationController.dispose();
    this._stageHorizontalScrollSontroller.dispose();
    this._stageVerticalScrollSontroller.dispose();
    super.dispose();
    return;
  }

  @override
  build(context) {
    var theme = Theme.of(context);
    return CustomModulePageLayout(
      onDropFile: (item) async {
        await this._applyLoad(item.first, null);
      },
      content: Column(
        children: [
          const SizedBox(height: 12),
          Expanded(
            child: Card.outlined(
              clipBehavior: Clip.antiAliasWithSaveLayer,
              margin: const EdgeInsets.fromLTRB(16, 0, 16, 0),
              child: Column(
                children: [
                  Expanded(
                    child: Container(
                      color: theme.colorScheme.surfaceContainer,
                      child: Scrollbar(
                        interactive: true,
                        controller: this._stageHorizontalScrollSontroller,
                        child: Scrollbar(
                          interactive: true,
                          controller: this._stageVerticalScrollSontroller,
                          child: LayoutBuilder(
                            builder: (context, constraints) => SingleChildTwoDimensionalScrollView(
                              padding: EdgeInsets.symmetric(
                                horizontal: max(0, (constraints.maxWidth - (this._animation?.size.$1 ?? 0)) / 2.0),
                                vertical: max(0, (constraints.maxHeight - (this._animation?.size.$2 ?? 0)) / 2.0),
                              ),
                              clipBehavior: Clip.none,
                              horizontalController: this._stageHorizontalScrollSontroller,
                              verticalController: this._stageVerticalScrollSontroller,
                              child: Container(
                                color: theme.colorScheme.surfaceContainerHighest,
                                child: this._animationVisual ?? const SizedBox.shrink(),
                              ),
                            ),
                          ),
                        ),
                      ),
                    ),
                  ),
                  Divider(height: 0),
                const SizedBox(height: 4),
                Row(
                  children: [
                    const SizedBox(width: 16),
                    // SizedBox(
                    //   width: 64,
                    //   child: TextField(
                    //     decoration: InputDecoration(
                    //       contentPadding: EdgeInsets.fromLTRB(12, 16, 12, 16),
                    //       filled: true,
                    //       border: UnderlineInputBorder(),
                    //     ),
                    //     textAlign: TextAlign.start,
                    //   ),
                    // ),
                    // const SizedBox(width: 12),
                    Expanded(
                      child: Container(
                        alignment: AlignmentDirectional.centerStart,
                        child: TextButton.icon(
                          iconAlignment: IconAlignment.start,
                          icon: const Icon(IconSymbols.speed),
                          label: Text(
                            '30.0',
                            overflow: TextOverflow.ellipsis,
                            textAlign: TextAlign.start,
                          ),
                          onPressed: () async {
                          },
                        ),
                      ),
                    ),
                    const SizedBox(width: 12),
                    IconButton(
                      isSelected: true,
                      icon: const Icon(IconSymbols.arrow_back),
                      onPressed: () async {
                        // TODO
                      },
                    ),
                    const SizedBox(width: 4),
                    IconButton(
                      isSelected: true,
                      icon: Icon(IconSymbols.pause, size: 36),
                      onPressed: () async {
                        // TODO
                      },
                    ),
                    const SizedBox(width: 4),
                    IconButton(
                      isSelected: true,
                      icon: const Icon(IconSymbols.arrow_forward),
                      onPressed: () async {
                        // TODO
                      },
                    ),
                    const SizedBox(width: 12),
                    Expanded(
                      child: Container(
                        alignment: AlignmentDirectional.centerEnd,
                        child: TextButton.icon(
                          iconAlignment: IconAlignment.end,
                          icon: const Icon(IconSymbols.sell),
                          label: Text(
                            'attack',
                            overflow: TextOverflow.ellipsis,
                            textAlign: TextAlign.end,
                          ),
                          onPressed: () async {
                          },
                        ),
                      ),
                    ),
                    // const SizedBox(width: 12),
                    // SizedBox(
                    //   width: 64,
                    //   child: TextField(
                    //     decoration: InputDecoration(
                    //       contentPadding: EdgeInsets.fromLTRB(12, 16, 12, 16),
                    //       filled: true,
                    //       border: UnderlineInputBorder(),
                    //     ),
                    //     textAlign: TextAlign.end,
                    //   ),
                    // ),
                    const SizedBox(width: 16),
                  ],
                ),
                const SizedBox(height: 4),
                Row(
                  children: [
                    Expanded(
                      child: LayoutBuilder(
                        builder: (context, constraints) => IntrinsicHeight(
                          child: OverflowBox(
                            maxWidth: constraints.maxWidth + 16,
                            child: Slider(
                              value: 0.5,
                              onChanged: (value) {
                              },
                            ),
                          ),
                        ),
                      ),
                    ),
                  ],
                ),
                const SizedBox(height: 8),
                ],
              ),
            ),
          ),
          const SizedBox(height: 12),
        ],
      ),
      bottom: Row(
        children: [
          IconButton.filledTonal(
            tooltip: 'Immediate Select',
            isSelected: this._immediateSelect,
            icon: const Icon(IconSymbols.ads_click),
            selectedIcon: const Icon(IconSymbols.ads_click, fill: 1),
            onPressed: () async {
              this._immediateSelect = !this._immediateSelect;
              this.setState(() {});
            },
          ),
          const SizedBox(width: 8),
          IconButton.filledTonal(
            tooltip: 'Automatic Play',
            isSelected: this._automaticPlay,
            icon: const Icon(IconSymbols.line_end_arrow_notch),
            selectedIcon: const Icon(IconSymbols.line_end_arrow_notch, fill: 1),
            onPressed: () async {
              this._automaticPlay = !this._automaticPlay;
              this.setState(() {});
            },
          ),
          const SizedBox(width: 8),
          IconButton.filledTonal(
            tooltip: 'Repeat Play',
            isSelected: this._repeatPlay,
            icon: const Icon(IconSymbols.repeat),
            selectedIcon: const Icon(IconSymbols.repeat, fill: 1),
            onPressed: () async {
              this._repeatPlay = !this._repeatPlay;
              this.setState(() {});
            },
          ),
          const SizedBox(width: 8),
          IconButton.filledTonal(
            tooltip: 'Remain Frame Rate',
            isSelected: this._remainFrameRate,
            icon: const Icon(IconSymbols.keep),
            selectedIcon: const Icon(IconSymbols.keep, fill: 1),
            onPressed: () async {
              this._remainFrameRate = !this._remainFrameRate;
              this.setState(() {});
            },
          ),
          const SizedBox(width: 16),
          const Expanded(child: SizedBox()),
          const SizedBox(width: 16),
          FloatingActionButton(
            // TODO
            tooltip: 'Source',
            elevation: 0,
            focusElevation: 0,
            hoverElevation: 0,
            highlightElevation: 0,
            disabledElevation: 0,
            onPressed: () async {
              var animationFile = await StorageHelper.pickLoadFile(context, 'AnimationViewer.AnimationFile');
              if (animationFile != null) {
                await this._applyLoad(animationFile, null);
              }
              // TODO
            },
            child: const Icon(IconSymbols.open_in_new),
          ),
        ],
      ),
    );
  }

}
