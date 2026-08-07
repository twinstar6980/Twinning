import '/common.dart';
import '/utility/json_type.dart';

// ----------------

class TranslateTransform {
  Floater x;
  Floater y;
  TranslateTransform({
    required this.x,
    required this.y,
  });
}

class RotateTranslateTransform {
  Floater angle;
  Floater x;
  Floater y;
  RotateTranslateTransform({
    required this.angle,
    required this.x,
    required this.y,
  });
}

class MatrixTranslateTransform {
  Floater a;
  Floater b;
  Floater c;
  Floater d;
  Floater x;
  Floater y;
  MatrixTranslateTransform({
    required this.a,
    required this.b,
    required this.c,
    required this.d,
    required this.x,
    required this.y,
  });
}

class VariantTransform {
  Object value;
  VariantTransform({
    required this.value,
  });
}

class Color {
  Floater red;
  Floater green;
  Floater blue;
  Floater alpha;
  Color({
    required this.red,
    required this.green,
    required this.blue,
    required this.alpha,
  });
}

class Rectangle {
  ({Floater x, Floater y})          position;
  ({Floater width, Floater height}) size;
  Rectangle({
    required this.position,
    required this.size,
  });
}

class Command {
  String command;
  String argument;
  Command({
    required this.command,
    required this.argument,
  });
}

class LayerRemove {
  Integer index;
  LayerRemove({
    required this.index,
  });
}

class LayerAppend {
  Integer index;
  String? name;
  Integer resource;
  Boolean sprite;
  Boolean additive;
  Integer preloadFrame;
  Floater timeScale;
  LayerAppend({
    required this.index,
    required this.name,
    required this.resource,
    required this.sprite,
    required this.additive,
    required this.preloadFrame,
    required this.timeScale,
  });
}

class LayerChange {
  Integer          index;
  VariantTransform transform;
  Color?           color;
  Integer?         spriteFrameNumber;
  Rectangle?       sourceRectangle;
  LayerChange({
    required this.index,
    required this.transform,
    required this.color,
    required this.spriteFrameNumber,
    required this.sourceRectangle,
  });
}

class Frame {
  String?           label;
  Boolean           stop;
  List<Command>     command;
  List<LayerRemove> remove;
  List<LayerAppend> append;
  List<LayerChange> change;
  Frame({
    required this.label,
    required this.stop,
    required this.command,
    required this.remove,
    required this.append,
    required this.change,
  });
}

class WorkArea {
  Integer start;
  Integer duration;
  WorkArea({
    required this.start,
    required this.duration,
  });
}

class Sprite {
  String?     name;
  Floater?    frameRate;
  WorkArea?   workArea;
  List<Frame> frame;
  Sprite({
    required this.name,
    required this.frameRate,
    required this.workArea,
    required this.frame,
  });
}

class Image {
  String                             name;
  ({Integer width, Integer height})? size;
  VariantTransform                   transform;
  Image({
    required this.name,
    required this.size,
    required this.transform,
  });
}

class Animation {
  Integer            frameRate;
  (Floater, Floater) position;
  (Floater, Floater) size;
  List<Image>        image;
  List<Sprite>       sprite;
  Sprite?            mainSprite;
  Animation({
    required this.frameRate,
    required this.position,
    required this.size,
    required this.image,
    required this.sprite,
    required this.mainSprite,
  });
}

// ----------------

class ModelHelper {

  // #region utility

  static VariantTransform parseVariantTransformFromList(
    List<Floater> list,
  ) {
    return VariantTransform(value: switch (list.length) {
      2 => TranslateTransform(x: list[0], y: list[1]),
      3 => RotateTranslateTransform(angle: list[0], x: list[1], y: list[2]),
      6 => MatrixTranslateTransform(a: list[0], b: list[1], c: list[2], d: list[3], x: list[4], y: list[5]),
      _ => throw Exception(),
    });
  }

  // #endregion

  // #region convert

  static Animation parseDataFromJson(
    JsonNode json,
  ) {
    json.jsonObject();
    return Animation(
      frameRate: json.jsonIn('frame_rate').jsonInteger(),
      position: json.jsonIn('position').jsonArray().cast<Floater>().selfAlso((it) => assertTest(it.length == 2)).selfLet((it) => (it[0], it[1])),
      size: json.jsonIn('size').jsonArray().cast<Floater>().selfAlso((it) => assertTest(it.length == 2)).selfLet((it) => (it[0], it[1])),
      image: json.jsonIn('image').jsonArray().map((json) => Image(
        name: json.jsonIn('name').jsonString(),
        size: json.jsonIn('size').jsonArray().cast<Integer>().selfAlso((it) => assertTest(it.length == 2)).selfLet((it) => (width: it[0], height: it[1])),
        transform: json.jsonIn('transform').jsonArray().cast<Floater>().selfLet(ModelHelper.parseVariantTransformFromList),
      )).toList(),
      sprite: json.jsonIn('sprite').jsonArray().map((json) => Sprite(
        name: json.jsonIn('name')?.jsonString(),
        frameRate: json.jsonIn('frame_rate')?.jsonFloater(),
        workArea: json.jsonIn('work_area')?.jsonArray().cast<Integer>().selfAlso((it) => assertTest(it.length == 2)).selfLet((it) => .new(start: it[0], duration: it[1])),
        frame: json.jsonIn('frame').jsonArray().map((json) => Frame(
          label: json.jsonIn('label')?.jsonString(),
          stop: json.jsonIn('stop').jsonBoolean(),
          command: json.jsonIn('command').jsonArray().map((json) => Command(
            command: json.jsonAt(0).jsonString(),
            argument: json.jsonAt(1).jsonString(),
          )).toList(),
          remove: json.jsonIn('remove').jsonArray().map((json) => LayerRemove(
            index: json.jsonIn('index').jsonInteger(),
          )).toList(),
          append: json.jsonIn('append').jsonArray().map((json) => LayerAppend(
            index: json.jsonIn('index').jsonInteger(),
            name: json.jsonIn('name')?.jsonString(),
            resource: json.jsonIn('resource').jsonInteger(),
            sprite: json.jsonIn('sprite').jsonBoolean(),
            additive: json.jsonIn('additive').jsonBoolean(),
            preloadFrame: json.jsonIn('preload_frame').jsonInteger(),
            timeScale: json.jsonIn('time_scale').jsonFloater(),
          )).toList(),
          change: json.jsonIn('change').jsonArray().map((json) => LayerChange(
            index: json.jsonIn('index').jsonInteger(),
            transform: json.jsonIn('transform').jsonArray().cast<Floater>().selfLet(ModelHelper.parseVariantTransformFromList),
            color: json.jsonIn('color')?.jsonArray().cast<Floater>().selfAlso((it) => assertTest(it.length == 4)).selfLet((it) => Color(red: it[0], green: it[1], blue: it[2], alpha: it[3])),
            spriteFrameNumber: json.jsonIn('sprite_frame_number')?.jsonInteger(),
            sourceRectangle: json.jsonIn('source_rectangle')?.jsonObject().selfLet((json) => Rectangle(
              position: json.jsonIn('position').jsonArray().cast<Floater>().selfAlso((it) => assertTest(it.length == 2)).selfLet((it) => (x: it[0], y: it[1])),
              size: json.jsonIn('size').jsonArray().cast<Floater>().selfAlso((it) => assertTest(it.length == 2)).selfLet((it) => (width: it[0], height: it[1])),
            )),
          )).toList(),
        )).toList(),
      )).toList(),
      mainSprite: json.jsonIn('main_sprite')?.jsonObject().selfLet((json) => Sprite(
        name: json.jsonIn('name')?.jsonString(),
        frameRate: json.jsonIn('frame_rate')?.jsonFloater(),
        workArea: json.jsonIn('work_area')?.jsonArray().cast<Integer>().selfAlso((it) => assertTest(it.length == 2)).selfLet((it) => .new(start: it[0], duration: it[1])),
        frame: json.jsonIn('frame').jsonArray().map((json) => Frame(
          label: json.jsonIn('label')?.jsonString(),
          stop: json.jsonIn('stop').jsonBoolean(),
          command: json.jsonIn('command').jsonArray().map((json) => Command(
            command: json.jsonAt(0).jsonString(),
            argument: json.jsonAt(1).jsonString(),
          )).toList(),
          remove: json.jsonIn('remove').jsonArray().map((json) => LayerRemove(
            index: json.jsonIn('index').jsonInteger(),
          )).toList(),
          append: json.jsonIn('append').jsonArray().map((json) => LayerAppend(
            index: json.jsonIn('index').jsonInteger(),
            name: json.jsonIn('name')?.jsonString(),
            resource: json.jsonIn('resource').jsonInteger(),
            sprite: json.jsonIn('sprite').jsonBoolean(),
            additive: json.jsonIn('additive').jsonBoolean(),
            preloadFrame: json.jsonIn('preload_frame').jsonInteger(),
            timeScale: json.jsonIn('time_scale').jsonFloater(),
          )).toList(),
          change: json.jsonIn('change').jsonArray().map((json) => LayerChange(
            index: json.jsonIn('index').jsonInteger(),
            transform: json.jsonIn('transform').jsonArray().cast<Floater>().selfLet(ModelHelper.parseVariantTransformFromList),
            color: json.jsonIn('color')?.jsonArray().cast<Floater>().selfAlso((it) => assertTest(it.length == 4)).selfLet((it) => Color(red: it[0], green: it[1], blue: it[2], alpha: it[3])),
            spriteFrameNumber: json.jsonIn('sprite_frame_number')?.jsonInteger(),
            sourceRectangle: json.jsonIn('source_rectangle')?.jsonObject().selfLet((json) => Rectangle(
              position: json.jsonIn('position').jsonArray().cast<Floater>().selfAlso((it) => assertTest(it.length == 2)).selfLet((it) => (x: it[0], y: it[1])),
              size: json.jsonIn('size').jsonArray().cast<Floater>().selfAlso((it) => assertTest(it.length == 2)).selfLet((it) => (width: it[0], height: it[1])),
            )),
          )).toList(),
        )).toList(),
      )),
    );
  }

  // #endregion

}
