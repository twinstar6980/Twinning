import '/common.dart';
import '/utility/storage_helper.dart';
import '/utility/json_helper.dart';
import '/view/animation_viewer/model.dart' as model;
import 'dart:ui';
import 'dart:math';
import 'dart:collection';
import 'package:flutter/material.dart';

// ----------------

class VisualHelper {

  // #region utility

  static String parseImageFileName(
    String value,
  ) {
    var result = value;
    var indexA1 = result.indexOf('(');
    var indexA2 = result.indexOf(')');
    if (indexA1 != -1 || indexA2 != -1) {
      assertTest(indexA1 != -1 && indexA2 != -1 && indexA1 < indexA2);
      result = result.substring(0, indexA1) + result.substring(indexA2 + 1, null);
    }
    var indexB1 = result.indexOf('\$');
    if (indexB1 != -1) {
      assertTest(indexA1 == -1 && indexA2 == -1);
      result = result.substring(indexB1 + 1, null);
    }
    var indexC1 = result.indexOf('[');
    var indexC2 = result.indexOf(']');
    if (indexC1 != -1 || indexC2 != -1) {
      assertTest(indexC1 != -1 && indexC2 != -1 && indexC1 < indexC2);
      result = result.substring(0, indexC1) + result.substring(indexC2 + 1, null);
    }
    var indexD1 = result.indexOf('|');
    if (indexD1 != -1) {
      result = result.substring(0, indexD1);
    }
    return result;
  }

  static List<(String, Integer, Integer)> parseSpriteFrameLabel(
    model.Sprite sprite,
  ) {
    var result = <(String, Integer, Integer)>[];
    var currentFrameLabel = <(String, Integer)>[];
    for (var frameIndex = 0; frameIndex < sprite.frame.length; frameIndex++) {
      var frame = sprite.frame[frameIndex];
      if (frame.label != null) {
        currentFrameLabel.add((frame.label!, frameIndex));
      }
      if (frame.stop) {
        for (var item in currentFrameLabel) {
          result.add((item.$1, item.$2, frameIndex - item.$2 + 1));
        }
        currentFrameLabel.clear();
      }
    }
    return result;
  }

  // ----------------

  static model.Image selectImage(
    model.Animation animation,
    Integer         index,
  ) {
    var result = null as model.Image?;
    if (0 <= index && index < animation.image.length) {
      result = animation.image[index];
    }
    else {
      throw Exception();
    }
    return result;
  }

  static model.Sprite selectSprite(
    model.Animation animation,
    Integer         index,
  ) {
    var result = null as model.Sprite?;
    if (0 <= index && index < animation.sprite.length) {
      result = animation.sprite[index];
    }
    else if (index == animation.sprite.length) {
      result = animation.main_sprite!;
    }
    else {
      throw Exception();
    }
    return result;
  }

  // #endregion

  // #region visualize

  static Matrix4 _makeMatrix(
    model.VariantTransform variant,
  ) {
    var result = null as Matrix4?;
    if (variant.value is model.TranslateTransform) {
      var value = variant.value as model.TranslateTransform;
      result = Matrix4(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        value.x, value.y, 0, 1,
      );
    }
    if (variant.value is model.RotateTranslateTransform) {
      var value = variant.value as model.RotateTranslateTransform;
      var valueCos = cos(value.angle);
      var valueSin = sin(value.angle);
      result = Matrix4(
        valueCos, valueSin, 0, 0,
        -valueSin, valueCos, 0, 0,
        0, 0, 1, 0,
        value.x, value.y, 0, 1,
      );
    }
    if (variant.value is model.MatrixTranslateTransform) {
      var value = variant.value as model.MatrixTranslateTransform;
      result = Matrix4(
        value.a, value.b, 0, 0,
        value.c, value.d, 0, 0,
        0, 0, 1, 0,
        value.x, value.y, 0, 1,
      );
    }
    return result!;
  }

  static ColorFilter _makeColor(
    model.Color value,
  ) {
    var result = ColorFilter.mode(Color.fromARGB(
      (255.0 * value.alpha).toInt(),
      (255.0 * value.red).toInt(),
      (255.0 * value.green).toInt(),
      (255.0 * value.blue).toInt(),
    ), BlendMode.modulate);
    return result;
  }

  // ----------------

  static Widget visualizeImage(
    AnimationController                            animationController,
    model.Animation                                animation,
    Map<String, (ImageProvider, Integer, Integer)> texture,
    model.Image                                    image,
  ) {
    var textureData = texture[image.name];
    return Transform(
      transform: _makeMatrix(image.transform),
      child: textureData == null
        ? null // TODO
        : Image(
          image: textureData.$1,
          width: (image.size?.$1 ?? textureData.$2).toDouble(),
          height: (image.size?.$2 ?? textureData.$3).toDouble(),
          fit: BoxFit.fill,
        ),
    );
  }

  static Widget visualizeSprite(
    AnimationController                            animationController,
    model.Animation                                animation,
    Map<String, (ImageProvider, Integer, Integer)> texture,
    model.Sprite                                   sprite,
  ) {
    var layerList = SplayTreeMap<Integer, _VisualLayer>();
    var frameIndex = 0;
    for (var frame in sprite.frame) {
      for (var action in frame.remove) {
        var layer = layerList[action.index]!;
        assertTest(!layer.isRemoved);
        layer.isRemoved = true;
      }
      for (var action in frame.append) {
        assertTest(!layerList.containsKey(action.index));
        var layer = layerList[action.index] = _VisualLayer();
        var subView = !action.sprite
          ? visualizeImage(animationController, animation, texture, selectImage(animation, action.resource))
          : visualizeSprite(animationController, animation, texture, selectSprite(animation, action.resource));
        var subController = animationController.drive(StepTween(begin: 0, end: sprite.frame.length));
        layer.view = AnimatedBuilder(
          animation: subController,
          builder: (context, child) {
            var index = subController.value;
            var property = layer.property[index];
            if (property == null) {
              return const SizedBox();
            }
            return ColorFiltered(
              colorFilter: property.$2,
              child: Transform(
                transform: property.$1,
                child: subView,
              ),
            );
          },
        );
        layer.property = List.filled(sprite.frame.length, null);
        layer.property[frameIndex] = (
          _makeMatrix(model.VariantTransform(value: model.TranslateTransform(x: 0.0, y: 0.0))),
          _makeColor(model.Color(red: 1.0, green: 1.0, blue: 1.0, alpha: 1.0)),
        );
        layer.isRemoved = false;
        layer.isChanged = true;
      }
      for (var action in frame.change) {
        var layer = layerList[action.index]!;
        assertTest(!layer.isRemoved);
        if (layer.isChanged) {
          layer.property[frameIndex] = (
            _makeMatrix(action.transform),
            action.color != null ? _makeColor(action.color!) : layer.property[frameIndex]!.$2,
          );
        }
        else {
          layer.property[frameIndex] = (
            _makeMatrix(action.transform),
            action.color != null ? _makeColor(action.color!) : layer.property[frameIndex - 1]!.$2,
          );
        }
        layer.isChanged = true;
      }
      for (var layer in layerList.values) {
        if (layer.isRemoved) {
          continue;
        }
        if (layer.isChanged) {
          layer.isChanged = false;
          continue;
        }
        layer.property[frameIndex] = layer.property[frameIndex - 1]!;
      }
      frameIndex++;
    }
    return Stack(
      fit: StackFit.passthrough,
      children: layerList.values.map((value) => value.view).toList(),
    );
  }

  // #endregion

  // #region load

  static Future<model.Animation> loadAnimation(
    String file,
  ) async {
    return model.ModelHelper.parseDataFromJson(await JsonHelper.deserializeFile(file));
  }

  static Future<Map<String, (ImageProvider, Integer, Integer)>> loadTexture(
    String          directory,
    model.Animation animation,
  ) async {
    var result = <String, (ImageProvider, Integer, Integer)>{};
    for (var image in animation.image) {
      var file = '${directory}/${parseImageFileName(image.name)}.png';
      if (await StorageHelper.existFile(file)) {
        var data = MemoryImage(await StorageHelper.readFile(file));
        var descriptor = await ImageDescriptor.encoded(await ImmutableBuffer.fromUint8List(data.bytes));
        result[image.name] = (data, descriptor.width, descriptor.height);
      }
    }
    return result;
  }

  // #endregion

}

class _VisualLayer {
  late Widget                        view;
  late List<(Matrix4, ColorFilter)?> property;
  late Boolean                       isRemoved;
  late Boolean                       isChanged;
}
