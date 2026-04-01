import '/common.dart';
import 'package:app_links/app_links.dart' as lib;

// ----------------

class ApplicationLinkManager {

  // #region singleton

  static final ApplicationLinkManager instance = ._();

  // #endregion

  // #region constructor

  Boolean _initialized;

  lib.AppLinks? _plugin;

  Future<Void> Function(Uri link)? _handler;

  // ----------------

  ApplicationLinkManager._(
  ) :
    this._initialized = false,
    this._plugin = null,
    this._handler = null;

  // #endregion

  // #region utility

  Future<Void> initialize(
  ) async {
    assertTest(!this._initialized);
    this._plugin = lib.AppLinks();
    this._initialized = true;
    return;
  }

  Future<Void> listen(
    Future<Void> Function(Uri link) handler,
  ) async {
    assertTest(this._initialized);
    this._handler = handler;
    this._plugin!.uriLinkStream.listen((link) {
      this._handler!.call(link);
      return;
    });
    return;
  }

  // ----------------

  Future<Uri?> getFirst(
  ) async {
    assertTest(this._initialized);
    return await this._plugin!.getInitialLink();
  }

  Future<Uri?> getLast(
  ) async {
    assertTest(this._initialized);
    return await this._plugin!.getLatestLink();
  }

  // #endregion

}
