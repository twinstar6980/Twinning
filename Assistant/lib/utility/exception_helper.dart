import '/common.dart';
import 'package:flutter/widgets.dart';
import 'package:stack_trace/stack_trace.dart' as lib;

// ----------------

class ExceptionHelper {

  // #region utility

  static Boolean _initialized = false;

  static Void Function(Object, StackTrace?)? _handler = null;

  // ----------------

  static Void initialize(
  ) {
    assertTest(!_initialized);
    WidgetsBinding.instance.platformDispatcher.onError = (error, stack) {
      _handler?.call(error, stack);
      return true;
    };
    FlutterError.onError = (details) {
      FlutterError.presentError(details);
      _handler?.call(details.exception, details.stack);
      return;
    };
    _initialized = true;
    return;
  }

  static Void listen(
    Void Function(Object, StackTrace?)? handler,
  ) {
    assertTest(_initialized);
    _handler = handler;
    return;
  }

  // ----------------

  static String generateMessage(
    Object      exception,
    StackTrace? stack,
  ) {
    var result = '${exception}';
    if (stack != null) {
      var trace = lib.Trace.from(stack);
      for (var frame in trace.frames) {
        result += '\n@ ${frame.library.selfLet((it) => it.startsWith('package:assistant') ? it.substring('package:'.length) : it)}:${frame.line ?? '?'}:${frame.column ?? '?'} ${frame.member ?? '?'}';
      }
    }
    return result;
  }

  // #endregion

}
