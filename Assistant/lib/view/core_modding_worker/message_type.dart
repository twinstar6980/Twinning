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
    return switch (this) {
      .verbosity   => StyledColor.onSurface.query(context),
      .information => ColorSet.blue,
      .warning     => ColorSet.yellow,
      .error       => ColorSet.red,
      .success     => ColorSet.green,
      .input       => ColorSet.purple,
    };
  }

}
