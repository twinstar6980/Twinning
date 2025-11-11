import '/common.dart'; // ignore: unused_import
import '/widget/export.dart';
import 'package:flutter/widgets.dart';

// ----------------

enum MessageType {
  verbosity,
  information,
  warning,
  error,
  success,
  input,
}

extension MessageTypeExtension on MessageType {

  Color color(
    BuildContext context,
  ) {
    return [
      StyledColorExtension.value(context, StyledColor.onSurface),
      ColorSet.blue,
      ColorSet.yellow,
      ColorSet.red,
      ColorSet.green,
      ColorSet.purple,
    ][this.index];
  }

}
