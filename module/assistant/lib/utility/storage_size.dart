import '/common.dart';
import '/utility/convert_helper.dart';
import 'dart:math' as lib;

// ----------------

enum StorageSizeExponent {
  b,
  k,
  m,
  g,
}

// ----------------

class StorageSize {

  // #region constructor

  Floater _count;

  StorageSizeExponent _exponent;

  // ----------------

  StorageSize(
  ) :
    this._count = 0.0,
    this._exponent = .b;

  StorageSize.copy(
    StorageSize other,
  ) :
    this._count = other._count,
    this._exponent = other._exponent;

  StorageSize.by(
    Floater             count,
    StorageSizeExponent exponent,
  ) :
    this._count = count,
    this._exponent = exponent {
    return;
  }

  StorageSize.of(
    String text,
  ) :
    this._count = 0.0,
    this._exponent = .b {
    this.parse(text);
    return;
  }

  // #endregion

  // #region equality

  @override
  operator ==(other) => other is StorageSize && this._count == other._count && this._exponent == other._exponent;

  @override
  get hashCode => Object.hashAll([this._count, this._exponent]);

  // #endregion

  // #region query

  Floater count(
  ) {
    return this._count;
  }

  StorageSizeExponent exponent(
  ) {
    return this._exponent;
  }

  // #endregion

  // #region value

  Integer value(
  ) {
    var countBig = this._count.truncateToDouble();
    var countLittle = this._count - countBig;
    return countBig.toInt() * lib.pow(1024, this._exponent.index).toInt() + (countLittle * lib.pow(1024, this._exponent.index)).truncate();
  }

  // #endregion

  // #region convert

  Void parse(
    String text,
  ) {
    this._count = 0;
    this._exponent = StorageSizeExponent.b;
    var count = ConvertHelper.parseFloaterFromString(text.substring(0, text.length - 1));
    assertTest(count.isFinite);
    var exponent = ['b', 'k', 'm', 'g'].indexOf(text.substring(text.length - 1));
    assertTest(exponent != -1);
    this._count = count;
    this._exponent = StorageSizeExponent.values[exponent];
    return;
  }

  String emit(
  ) {
    return '${ConvertHelper.makeFloaterToString(this._count)}${['b', 'k', 'm', 'g'][this._exponent.index]}';
  }

  // #endregion

}
