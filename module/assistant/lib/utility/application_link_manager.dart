import '/common.dart';
import '/utility/platform_integration_manager.dart';

// ----------------

class ApplicationLinkManager {

  // #region singleton

  static final ApplicationLinkManager instance = ._();

  // #endregion

  // #region constructor

  Boolean _initialized;

  Stream<String>? _stream;

  Integer _count;

  Future<Void> Function(Uri link)? _handler;

  // ----------------

  ApplicationLinkManager._(
  ) :
    this._initialized = false,
    this._stream = null,
    this._count = 0,
    this._handler = null;

  // #endregion

  // #region utility

  Future<Void> initialize(
  ) async {
    assertTest(!this._initialized);
    this._stream = PlatformIntegrationManager.instance.getStreamForLink();
    this._initialized = true;
    return;
  }

  // ----------------

  Future<Void> listen(
    Future<Void> Function(Uri link) handler,
  ) async {
    assertTest(this._initialized);
    this._handler = handler;
    this._stream!.listen((link) {
      this._count += 1;
      this._handler!.call(.parse(link));
      return;
    });
    return;
  }

  Future<Integer> count(
  ) async {
    assertTest(this._initialized);
    return this._count;
  }

  // #endregion

}
