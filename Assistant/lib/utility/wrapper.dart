import '/common.dart'; // ignore: unused_import

// ----------------

class Wrapper<TValue> {

  // #region constructor

  _WrapperData _data;

  // ----------------

  Wrapper(
  ) :
    this._data = .new(null);

  Wrapper.of(
    TValue value,
  ) :
    this._data = .new(value);

  Wrapper._of(
    _WrapperData data,
  ) :
    this._data = data;

  // #endregion

  // #region query

  TValue get value => this._data.value as TValue;
  set value(TValue value) => this._data.value = value;

  // ----------------

  Wrapper<TTarget> cast<TTarget>(
  ) {
    return ._of(this._data);
  }

  // #endregion

}

class _WrapperData {
  Object? value;
  _WrapperData(
    this.value,
  );
}
