import '/common.dart'; // ignore: unused_import
import 'package:flutter/material.dart';

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
    ThemeData theme,
  ) {
    return [
      theme.colorScheme.onSurface,
      Colors.blue,
      Colors.yellow,
      Colors.red,
      Colors.green,
      Colors.purple,
    ][this.index];
  }

}
