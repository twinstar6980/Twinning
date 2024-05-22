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
    var state = !this.done() && this._view[this._position] == name;
    if (state) {
      this._position++;
    }
    return state;
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
    var value = this._view[this._position];
    this._position++;
    return value;
  }

  List<String> nextStringList(
  ) {
    var value = this._view.slice(this._position);
    this._position = this._view.length;
    return value;
  }

  // #endregion

}
