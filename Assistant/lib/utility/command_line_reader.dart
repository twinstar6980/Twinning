import '/common.dart';
import 'package:collection/collection.dart';

// ----------------

class CommandLineReader {

  // #region structor

  List<String> _view;

  Integer _position;

  // ----------------

  CommandLineReader(
    List<String> view,
  ) :
    _view = view,
    _position = 0;

  // #endregion

  // #region query

  Boolean done(
  ) {
    return !(0 <= this._position && this._position < this._view.length);
  }

  Boolean check(
    String name,
  ) {
    var result = false;
    if (!this.done() && this._view[this._position] == name) {
      result = true;
      this._position++;
    }
    return result;
  }

  // ----------------

  Boolean nextBoolean(
  ) {
    return Boolean.parse(this.nextString());
  }

  Integer nextInteger(
  ) {
    return Integer.parse(this.nextString());
  }

  Floater nextFloater(
  ) {
    return Floater.parse(this.nextString());
  }

  String nextString(
  ) {
    assertTest(!this.done());
    var result = this._view[this._position];
    this._position++;
    return result;
  }

  List<String> nextStringList(
  ) {
    var result = this._view.slice(this._position);
    this._position = this._view.length;
    return result;
  }

  // #endregion

}
