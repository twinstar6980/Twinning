import '/common.dart';
import '/utility/convert_helper.dart';
import 'package:collection/collection.dart';
import 'package:flutter/foundation.dart';

// ----------------

enum StoragePathType {
  nothing,
  relative,
  absolute,
}

enum StoragePathStyle {
  generic,
  native,
  posix,
  windows,
}

// ----------------

class StoragePath {

  // #region constructor

  StoragePathType _type;

  String? _root;

  List<String> _part;

  // ----------------

  StoragePath(
  ) :
    this._type = .nothing,
    this._root = null,
    this._part = [];

  StoragePath.copy(
    StoragePath other,
  ) :
    this._type = other._type,
    this._root = other._root,
    this._part = other._part.toList();

  StoragePath.by(
    StoragePathType type,
  ) :
    this._type = type,
    this._root = null,
    this._part = [];

  StoragePath.of(
    String text,
  ) :
    this._type = .nothing,
    this._root = null,
    this._part = [] {
    this.parse(text);
    return;
  }

  // #endregion

  // #region equality

  @override
  operator ==(other) => other is StoragePath && this._type == other._type && this._root == other._root && listEquals(this._part, other._part);

  @override
  get hashCode => Object.hashAll([this._type, this._root, ...this._part]);

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

  List<String> part(
  ) {
    return this._part;
  }

  // #endregion

  // #region segment

  StoragePath? parent(
  ) {
    var result = null as StoragePath?;
    if (this._type != .nothing && !this._part.isEmpty) {
      result = .new();
      result._type = this._type;
      result._root = this._root;
      result._part.addAll(this._part.slice(0, this._part.length - 1));
    }
    return result;
  }

  String? name(
  ) {
    var result = null as String?;
    if (this._type != .nothing && !this._part.isEmpty) {
      result = this._part.last;
    }
    return result;
  }

  // ----------------

  String? stem(
  ) {
    var result = null as String?;
    if (this._type != .nothing && !this._part.isEmpty) {
      var position = this._part.last.lastIndexOf('.');
      if (position != -1) {
        result = this._part.last.substring(0, position);
      }
      else {
        result = this._part.last;
      }
    }
    return result;
  }

  String? extension(
  ) {
    var result = null as String?;
    if (this._type != .nothing && !this._part.isEmpty) {
      var position = this._part.last.lastIndexOf('.');
      if (position != -1) {
        result = this._part.last.substring(position + 1);
      }
    }
    return result;
  }

  // #endregion

  // #region compose

  StoragePath join(
    String other,
  ) {
    assertTest(this._type != .nothing);
    var result = StoragePath();
    result._type = this._type;
    result._root = this._root;
    result._part.addAll(this._part);
    result._part.add(other);
    return result;
  }

  StoragePath push(
    StoragePath other,
  ) {
    assertTest(this._type != .nothing);
    assertTest(other._type == .relative);
    var result = StoragePath();
    result._type = this._type;
    result._root = this._root;
    result._part.addAll(this._part);
    result._part.addAll(other._part);
    return result;
  }

  // #endregion

  // #region convert

  Void parse(
    String text,
  ) {
    this._type = .nothing;
    this._root = null;
    this._part = [];
    if (!text.isEmpty) {
      var position = 0;
      if (text.length >= 2 && text[1] == ':' && ConvertHelper.isLetter(text[0])) {
        this._root = text.substring(0, 2);
        position += 2;
      }
      if (text.length > position && ConvertHelper.isPathSeparator(text[position])) {
        this._type = .absolute;
        position += 1;
      }
      else {
        this._type = .relative;
      }
      var location = position;
      while (true) {
        if (position == text.length || ConvertHelper.isPathSeparator(text[position])) {
          var segment = text.substring(location, position);
          if (segment == '' || segment == '.') {
          }
          else if (segment == '..' && !this._part.isEmpty && this._part.last != '..') {
            this._part.removeLast();
          }
          else {
            this._part.add(segment);
          }
          if (position == text.length) {
            break;
          }
          position += 1;
          location = position;
          continue;
        }
        position += 1;
      }
    }
    return;
  }

  String emit({
    StoragePathStyle style = .generic,
  }) {
    var text = '';
    if (this._type != .nothing) {
      var mappedStyle = style;
      if (style == .generic) {
        mappedStyle = .posix;
      }
      if (style == .native) {
        if (SystemChecker.isWindows) {
          mappedStyle = .windows;
        }
        if (SystemChecker.isLinux || SystemChecker.isMacintosh || SystemChecker.isAndroid || SystemChecker.isIphone) {
          mappedStyle = .posix;
        }
      }
      var separator = mappedStyle == .posix ? '/' : '\\';
      if (this._root != null) {
        text += this._root!;
      }
      if (this._type == .relative) {
        text += '.';
      }
      if (this._part.isEmpty) {
        text += separator;
      }
      for (var segment in this._part) {
        text += separator;
        if (mappedStyle == .posix) {
          text += segment;
        }
        if (mappedStyle == .windows) {
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
          text += segment.substring(0, segmentSize);
        }
      }
    }
    return text;
  }

  // ----------------

  String emitGeneric(
  ) {
    return this.emit(style: .generic);
  }

  String emitNative(
  ) {
    return this.emit(style: .native);
  }

  String emitPosix(
  ) {
    return this.emit(style: .posix);
  }

  String emitWindows(
  ) {
    return this.emit(style: .windows);
  }

  // #endregion

}
