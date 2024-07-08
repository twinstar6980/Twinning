import '/common.dart'; // ignore: unused_import

// ----------------

class Wrapper<TValue> {

  // #region structor

  _WrapperData _data;

  // ----------------

  Wrapper._of(
    _WrapperData data,
  ) : this._data = data;

  Wrapper(
    TValue value,
  ) : this._of(_WrapperData(value));

  // #endregion

  // #region query

  TValue get value => this._data.value as TValue;
  set value(TValue value) => this._data.value = value;

  // ----------------

  Wrapper<TTarget> cast<TTarget>(
  ) {
    return Wrapper<TTarget>._of(this._data);
  }

  // #endregion

}

class _WrapperData {
  Object? value;
  _WrapperData(
    this.value,
  );
}
