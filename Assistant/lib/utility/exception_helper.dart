import '/common.dart';
import 'package:flutter/widgets.dart';
import 'package:stack_trace/stack_trace.dart';

// ----------------

class ExceptionHelper {

  // #region utility

  static Void registerGlobalHandler(
    Void Function(Object, StackTrace?) handler,
  ) {
      WidgetsBinding.instance.platformDispatcher.onError = (error, stack) {
        handler(error, stack);
        return true;
      };
      FlutterError.onError = (details) {
        FlutterError.presentError(details);
        handler(details.exception, details.stack);
        return;
      };
      return;
  }

  // ----------------

  static String generateMessage(
    Object      exception,
    StackTrace? stack,
  ) {
    var result = '${exception}';
    if (stack != null) {
      var trace = Trace.from(stack);
      for (var frame in trace.frames) {
        result += '\n@ ${frame.library.selfLet((it) => it.startsWith('package:assistant') ? it.substring('package:'.length) : it)}:${frame.line ?? '?'}:${frame.column ?? '?'} ${frame.member ?? '?'}';
      }
    }
    return result;
  }

  // #endregion

}
