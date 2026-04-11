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
    assertTest(other._type == .detached);
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
    if (text.length >= 2 && ConvertHelper.isLetter(text[0]) && ConvertHelper.isPathVolumeSeparator(text[1])) {
      position += 2;
      if (position < text.length && ConvertHelper.isPathDirectorySeparator(text[position])) {
        this._type = .absolute;
        this._root = text.substring(0, position);
        position += 1;
      }
      else {
        this._type = .relative;
        this._root = text.substring(0, position);
      }
    }
    else if (text.length >= 2 && ConvertHelper.isPathDirectorySeparator(text[0]) && ConvertHelper.isPathDirectorySeparator(text[1])) {
      position += 2;
      for (; position < text.length; position++) {
        if (ConvertHelper.isPathDirectorySeparator(text[position])) {
          break;
        }
      }
      this._type = .absolute;
      this._root = '//' + text.substring(2, position);
      if (position < text.length && ConvertHelper.isPathDirectorySeparator(text[position])) {
        position += 1;
      }
    }
    else if (text.length >= 1 && ConvertHelper.isPathDirectorySeparator(text[position])) {
      position += 1;
      this._type = .absolute;
      this._root = '';
    }
    else if (text.length >= 2 && ConvertHelper.isPathDot(text[0]) && ConvertHelper.isPathDot(text[1])) {
      position += 2;
      if (position == text.length || ConvertHelper.isPathDirectorySeparator(text[position])) {
        this._type = .relative;
        this._root = '';
      }
      position = 0;
    }
    else if (text.length >= 1 && ConvertHelper.isPathDot(text[0])) {
      position += 1;
      if (position == text.length || ConvertHelper.isPathDirectorySeparator(text[position])) {
        this._type = .relative;
        this._root = '';
        if (position < text.length) {
          position += 1;
        }
      }
      else {
        position = 0;
      }
    }
    var location = position;
    for (; position <= text.length; position++) {
      if (position == text.length || ConvertHelper.isPathDirectorySeparator(text[position])) {
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
    var pathDot = '.';
    var pathDirectorySeparator = style == .posix ? '/' : '\\';
    if (this._type != .detached) {
      for (var index = 0; index < this._root!.length; index++) {
        text.write(!ConvertHelper.isPathDirectorySeparator(this._root![index]) ? this._root![index] : pathDirectorySeparator);
      }
      if (this._type == .relative) {
        text.write(pathDot);
      }
      text.write(pathDirectorySeparator);
    }
    for (var segmentIndex = 0; segmentIndex < this._segment.length; segmentIndex++) {
      var segment = this._segment[segmentIndex];
      if (segmentIndex != 0) {
        text.write(pathDirectorySeparator);
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
