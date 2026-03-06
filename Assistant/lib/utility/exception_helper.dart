import '/common.dart';
import 'package:flutter/widgets.dart';
import 'package:stack_trace/stack_trace.dart' as lib;

// ----------------

class ExceptionHelper {

  // #region utility

  static Boolean _initialized = false;

  static Void Function(Object exception, StackTrace? stack)? _handler = null;

  // ----------------

  static Void initialize(
  ) {
    assertTest(!ExceptionHelper._initialized);
    WidgetsBinding.instance.platformDispatcher.onError = (exception, stack) {
      ExceptionHelper._handler?.call(exception, stack);
      return true;
    };
    FlutterError.onError = (details) {
      FlutterError.presentError(details);
      ExceptionHelper._handler?.call(details.exception, details.stack);
      return;
    };
    ExceptionHelper._initialized = true;
    return;
  }

  static Void listen(
    Void Function(Object exception, StackTrace? stack)? handler,
  ) {
    assertTest(ExceptionHelper._initialized);
    ExceptionHelper._handler = handler;
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
