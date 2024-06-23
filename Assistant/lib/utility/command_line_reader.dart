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
    return this.nextString().selfLet((it) => Boolean.parse(it));
  }

  Integer nextInteger(
  ) {
    return this.nextString().selfLet((it) => Integer.parse(it));
  }

  Floater nextFloater(
  ) {
    return this.nextString().selfLet((it) => Floater.parse(it));
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
