import '/common.dart';
import '/utility/convert_helper.dart';

// ----------------

class CommandLineWriter {

  // #region structor

  List<String> _view;

  // ----------------

  CommandLineWriter(
  ) :
    _view = [];

  // #endregion

  // #region query

  List<String> done(
  ) {
    return this._view;
  }

  Boolean check(
    String name, {
    Boolean state = true,
  }) {
    if (state) {
      this.nextString(name);
    }
    return state;
  }

  // ----------------

  Void nextBoolean(
    Boolean value,
  ) {
    return this.nextString(value.selfLet((it) => ConvertHelper.makeBooleanToString(it)));
  }

  Void nextInteger(
    Integer value,
  ) {
    return this.nextString(value.selfLet((it) => ConvertHelper.makeIntegerToString(it, false)));
  }

  Void nextFloater(
    Floater value,
  ) {
    return this.nextString(value.selfLet((it) => ConvertHelper.makeFloaterToString(it, false)));
  }

  Void nextString(
    String value,
  ) {
    this._view.add(value);
    return;
  }

  Void nextStringList(
    List<String> value,
  ) {
    this._view.addAll(value);
    return;
  }

  // #endregion

}
