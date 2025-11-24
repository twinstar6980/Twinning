import '/common.dart';
import 'dart:typed_data';

// ----------------

class ByteStreamView {

  // #region constructor

  ByteData _view;

  Integer _position;

  Endian _endian;

  // ----------------

  ByteStreamView(
    ByteData view,
  ) :
    this._view = view,
    this._position = 0,
    this._endian = .little;

  // #endregion

  // #region action

  ByteData get view => this._view;

  Integer get size => this._view.lengthInBytes;

  // ----------------

  Integer get position => this._position;
  set position(Integer value) => this._position = value;

  // #endregion

  // #region access

  Integer getIntegerU8(
  ) {
    var value = this._view.getUint8(this._position);
    this._position += 1;
    return value;
  }

  Void setIntegerU8(
    Integer value,
  ) {
    this._view.setUint8(this._position, value);
    this._position += 1;
    return;
  }

  // ----------------

  Integer getIntegerU16(
  ) {
    var value = this._view.getUint16(this._position, this._endian);
    this._position += 2;
    return value;
  }

  Void setIntegerU16(
    Integer value,
  ) {
    this._view.setUint16(this._position, value, this._endian);
    this._position += 2;
    return;
  }

  // ----------------

  Integer getIntegerU32(
  ) {
    var value = this._view.getUint32(this._position, this._endian);
    this._position += 4;
    return value;
  }

  Void setIntegerU32(
    Integer value,
  ) {
    this._view.setUint32(this._position, value, this._endian);
    this._position += 4;
    return;
  }

  // ----------------

  Integer getInteger8(
  ) {
    var value = this._view.getInt8(this._position);
    this._position += 1;
    return value;
  }

  Void setInteger8(
    Integer value,
  ) {
    this._view.setInt8(this._position, value);
    this._position += 1;
    return;
  }

  // ----------------

  Integer getInteger16(
  ) {
    var value = this._view.getInt16(this._position, this._endian);
    this._position += 2;
    return value;
  }

  Void setInteger16(
    Integer value,
  ) {
    this._view.setInt16(this._position, value, this._endian);
    this._position += 2;
    return;
  }

  // ----------------

  Integer getInteger32(
  ) {
    var value = this._view.getInt32(this._position, this._endian);
    this._position += 4;
    return value;
  }

  Void setInteger32(
    Integer value,
  ) {
    this._view.setInt32(this._position, value, this._endian);
    this._position += 4;
    return;
  }

  // ----------------

  Floater getFloater32(
  ) {
    var value = this._view.getFloat32(this._position, this._endian);
    this._position += 4;
    return value;
  }

  Void setFloater32(
    Floater value,
  ) {
    this._view.setFloat32(this._position, value, this._endian);
    this._position += 4;
    return;
  }

  // ----------------

  Floater getFloater64(
  ) {
    var value = this._view.getFloat64(this._position, this._endian);
    this._position += 8;
    return value;
  }

  Void setFloater64(
    Floater value,
  ) {
    this._view.setFloat64(this._position, value, this._endian);
    this._position += 8;
    return;
  }

  // #endregion

}
