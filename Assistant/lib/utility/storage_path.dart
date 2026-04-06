import '/common.dart';
import '/utility/convert_helper.dart';
import 'package:collection/collection.dart';
import 'package:flutter/foundation.dart';

// ----------------

enum StoragePathType {
  detached,
  relative,
  absolute,
}

enum StoragePathStyle {
  posix,
  windows,
}

// ----------------

class StoragePath {

  // #region constructor

  StoragePathType _type;

  String? _root;

  List<String> _segment;

  // ----------------

  StoragePath(
  ) :
    this._type = .detached,
    this._root = null,
    this._segment = [];

  StoragePath.copy(
    StoragePath other,
  ) :
    this._type = other._type,
    this._root = other._root,
    this._segment = other._segment.toList();

  StoragePath.by(
    StoragePathType type,
  ) :
    this._type = type,
    this._root = null,
    this._segment = [];

  StoragePath.of(
    String text,
  ) :
    this._type = .detached,
    this._root = null,
    this._segment = [] {
    this.parse(text);
    return;
  }

  // #endregion

  // #region equality

  @override
  operator ==(other) => other is StoragePath && this._type == other._type && this._root == other._root && listEquals(this._segment, other._segment);

  @override
  get hashCode => Object.hashAll([this._type, this._root, ...this._segment]);

  // #endregion

  // #region query

  StoragePathType type(
  ) {
    return this._type;
  }

  String? root(
  ) {
    return this._root;
  }

  List<String> segment(
  ) {
    return this._segment;
  }

  // #endregion

  // #region part

  StoragePath? parent(
  ) {
    var result = null as StoragePath?;
    if (!this._segment.isEmpty) {
      result = .new();
      result._type = this._type;
      result._root = this._root;
      result._segment.addAll(this._segment.slice(0, this._segment.length - 1));
    }
    return result;
  }

  String? name(
  ) {
    var result = null as String?;
    if (!this._segment.isEmpty) {
      result = this._segment.last;
    }
    return result;
  }

  // ----------------

  String? stem(
  ) {
    var result = null as String?;
    if (!this._segment.isEmpty) {
      var segment = this._segment.last;
      var position = segment.lastIndexOf('.');
      if (position != -1) {
        result = segment.substring(0, position);
      }
      else {
        result = segment;
      }
    }
    return result;
  }

  String? extension(
  ) {
    var result = null as String?;
    if (!this._segment.isEmpty) {
      var segment = this._segment.last;
      var position = segment.lastIndexOf('.');
      if (position != -1) {
        result = segment.substring(position + 1);
      }
    }
    return result;
  }

  // #endregion

  // #region compose

  StoragePath join(
    String other,
  ) {
    var result = StoragePath();
    result._type = this._type;
    result._root = this._root;
    result._segment.addAll(this._segment);
    result._segment.add(other);
    return result;
  }

  StoragePath push(
    StoragePath other,
  ) {
    assertTest(other._type != .absolute);
    var result = StoragePath();
    result._type = this._type;
    result._root = this._root;
    result._segment.addAll(this._segment);
    result._segment.addAll(other._segment);
    return result;
  }

  // #endregion

  // #region convert

  Void parse(
    String text,
  ) {
    this._type = .detached;
    this._root = null;
    this._segment = [];
    var position = 0;
    if (text.length >= 2 && text[1] == ':' && ConvertHelper.isLetter(text[0])) {
      this._root = text.substring(0, 2);
      position += 2;
    }
    if (text.length > position && ConvertHelper.isPathDot(text[position])) {
      var isRelative = false;
      var isParent = false;
      var offset = 1;
      if (text.length > position + offset && ConvertHelper.isPathDot(text[position + offset])) {
        isParent = true;
        offset += 1;
      }
      if (text.length == position + offset) {
        isRelative = true;
      }
      else if (ConvertHelper.isPathSeparator(text[position + offset])) {
        isRelative = true;
        offset += 1;
      }
      if (isRelative) {
        this._type = .relative;
        if (!isParent) {
          position += offset;
        }
      }
    }
    else if (text.length > position && ConvertHelper.isPathSeparator(text[position])) {
      this._type = .absolute;
      position += 1;
    }
    var location = position;
    for (; position <= text.length; position++) {
      if (position == text.length || ConvertHelper.isPathSeparator(text[position])) {
        var segment = text.substring(location, position);
        if (!segment.isEmpty) {
          this._segment.add(segment);
        }
        location = position + 1;
      }
    }
    return;
  }

  String emit({
    StoragePathStyle style = .posix,
    Boolean          rectify = false,
  }) {
    var text = StringBuffer();
    var dot = '.';
    var separator = style == .posix ? '/' : '\\';
    if (this._root != null) {
      text.write(this._root!);
    }
    if (this._type == .relative) {
      text.write(dot);
      text.write(separator);
    }
    if (this._type == .absolute) {
      text.write(separator);
    }
    for (var segmentIndex = 0; segmentIndex < this._segment.length; segmentIndex++) {
      var segment = this._segment[segmentIndex];
      if (segmentIndex != 0) {
        text.write(separator);
      }
      if (!rectify) {
        text.write(segment);
      }
      else {
        if (style == .posix) {
          text.write(segment);
        }
        if (style == .windows) {
          var segmentSize = segment.length;
          while (segmentSize != 0) {
            if (segment[segmentSize - 1] == ' ') {
              segmentSize -= 1;
              continue;
            }
            if (segment[segmentSize - 1] == '.' && !((segmentSize == 1) || (segmentSize == 2 && segment[0] == '.'))) {
              segmentSize -= 1;
              continue;
            }
            break;
          }
          text.write(segment.substring(0, segmentSize));
        }
      }
    }
    return text.toString();
  }

  // ----------------

  String emitPosix({
    Boolean rectify = false,
  }) {
    return this.emit(style: .posix, rectify: rectify);
  }

  String emitWindows({
    Boolean rectify = false,
  }) {
    return this.emit(style: .windows, rectify: rectify);
  }

  String emitNative(
  ) {
    var mappedStyle = null as StoragePathStyle?;
    if (SystemChecker.isWindows) {
      mappedStyle = .windows;
    }
    if (SystemChecker.isLinux || SystemChecker.isMacintosh || SystemChecker.isAndroid || SystemChecker.isIphone) {
      mappedStyle = .posix;
    }
    return this.emit(style: mappedStyle!, rectify: true);
  }

  // #endregion

}
