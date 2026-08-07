import '/common.dart';
import '/module.dart';
import '/utility/json_type.dart';
import '/utility/storage_path.dart';
import '/utility/storage_helper.dart';
import '/widget/export.dart';
import '/view/popcap_animation_viewer/main_page.dart';
import '/view/popcap_animation_viewer/setting_panel.dart';
import '/view/popcap_animation_viewer/setting.dart';
import '/view/popcap_animation_viewer/configuration.dart';
import '/view/popcap_animation_viewer/option.dart';
import '/view/popcap_animation_viewer/visual_helper.dart';

// ----------------

final ModuleDescriptor kModuleDescriptor = ModuleHelper.createDescriptor<MainPage, SettingPanel, Setting, Configuration, Option>(
  draft: false,
  type: .popcapAnimationViewer,
  name: 'PopCap Animation Viewer',
  icon: IconSet.thread_unread,
  buildMainPage: (key, setting, configuration, option) => .new(key: key, setting: setting, configuration: configuration, option: option),
  buildSettingPanel: (data, onUpdate) => .new(data: data, onUpdate: onUpdate),
  makeSetting: (data) => {
    'immediate_select': data.immediateSelect,
    'automatic_play': data.automaticPlay,
    'repeat_play': data.repeatPlay,
    'reverse_play': data.reversePlay,
    'keep_speed': data.keepSpeed,
    'show_boundary': data.showBoundary,
  },
  generateDefaultSetting: () => Setting(
    immediateSelect: true,
    automaticPlay: true,
    repeatPlay: true,
    reversePlay: false,
    keepSpeed: false,
    showBoundary: false,
  ),
  parseSetting: (json) => Setting(
    immediateSelect: json.jsonIn('immediate_select').jsonBoolean(),
    automaticPlay: json.jsonIn('automatic_play').jsonBoolean(),
    repeatPlay: json.jsonIn('repeat_play').jsonBoolean(),
    reversePlay: json.jsonIn('reverse_play').jsonBoolean(),
    keepSpeed: json.jsonIn('keep_speed').jsonBoolean(),
    showBoundary: json.jsonIn('show_boundary').jsonBoolean(),
  ),
  parseConfiguration: (json) => Configuration(
  ),
  makeOption: (data) => {
    'immediate_select': data.immediateSelect,
    'automatic_play': data.automaticPlay,
    'repeat_play': data.repeatPlay,
    'reverse_play': data.reversePlay,
    'keep_speed': data.keepSpeed,
    'show_boundary': data.showBoundary,
    'animation_file': data.animationFile?.emit(),
    'image_filter': data.imageFilter,
    'sprite_filter': data.spriteFilter,
    'active_target': data.activeTarget?.selfLet((data) => [
      data.type,
      data.index,
    ]),
    'active_frame_range': data.activeFrameRange?.selfLet((data) => [
      data.begin,
      data.end,
    ]),
    'active_frame_speed': data.activeFrameSpeed,
    'active_progress_index': data.activeProgressIndex,
    'active_progress_state': data.activeProgressState,
  },
  parseOption: (json) => Option(
    immediateSelect: json.jsonInOr('immediate_select')?.jsonBoolean(),
    automaticPlay: json.jsonInOr('automatic_play')?.jsonBoolean(),
    repeatPlay: json.jsonInOr('repeat_play')?.jsonBoolean(),
    reversePlay: json.jsonInOr('reverse_play')?.jsonBoolean(),
    keepSpeed: json.jsonInOr('keep_speed')?.jsonBoolean(),
    showBoundary: json.jsonInOr('show_boundary')?.jsonBoolean(),
    animationFile: json.jsonInOr('animation_file')?.jsonString().selfLet((it) => StoragePath.of(it)),
    imageFilter: json.jsonInOr('image_filter')?.jsonArray().cast<Integer>(),
    spriteFilter: json.jsonInOr('sprite_filter')?.jsonArray().cast<Integer>(),
    activeTarget: json.jsonInOr('active_target')?.jsonArray().selfLet((data) => (
      type: data.jsonAt(0).jsonBoolean(),
      index: data.jsonAt(1).jsonInteger(),
    )),
    activeFrameRange: json.jsonInOr('active_frame_range')?.jsonArray().selfLet((data) => (
      begin: data.jsonAt(0).jsonInteger(),
      end: data.jsonAt(1).jsonInteger(),
    )),
    activeFrameSpeed: json.jsonInOr('active_frame_speed')?.jsonFloater(),
    activeProgressIndex: json.jsonInOr('active_progress_index')?.jsonInteger(),
    activeProgressState: json.jsonInOr('active_progress_state')?.jsonBoolean(),
  ),
  generateDefaultOption: () => Option(
    immediateSelect: null,
    automaticPlay: null,
    repeatPlay: null,
    reversePlay: null,
    keepSpeed: null,
    showBoundary: null,
    animationFile: null,
    imageFilter: null,
    spriteFilter: null,
    activeTarget: null,
    activeFrameRange: null,
    activeFrameSpeed: null,
    activeProgressIndex: null,
    activeProgressState: null,
  ),
  generateForwardOption: (resource) async {
    if (resource.length != 1) {
      return null;
    }
    var animationFile = null as StoragePath?;
    if (await StorageHelper.existFile(resource.first)) {
      animationFile = await VisualHelper.checkAnimationFilePath(resource.first);
    }
    if (await StorageHelper.existDirectory(resource.first)) {
      animationFile = await VisualHelper.checkAnimationDirectoryPath(resource.first);
    }
    if (animationFile == null) {
      return null;
    }
    return .new(
      animationFile: animationFile,
    );
  },
);
