import '/common.dart';
import 'package:flutter/material.dart';

// ----------------

enum MessageType {
  verbosity,
  information,
  warning,
  error,
  success,
  input,
  ;

  static MessageType fromString(
    String name,
  ) {
    late MessageType value;
    switch (name) {
      case 'v': {
        value = MessageType.verbosity;
        break;
      }
      case 'i': {
        value = MessageType.information;
        break;
      }
      case 'w': {
        value = MessageType.warning;
        break;
      }
      case 'e': {
        value = MessageType.error;
        break;
      }
      case 's': {
        value = MessageType.success;
        break;
      }
      case 't': {
        value = MessageType.input;
        break;
      }
      default: {
        throw Exception('invalid message type string : ${name}');
      }
    }
    return value;
  }

  static Color color(
    MessageType type,
    ThemeData   theme,
  ) {
    return [
      theme.colorScheme.onSurface,
      Colors.blue,
      Colors.yellow,
      Colors.red,
      Colors.green,
      Colors.purple,
    ][type.index];
  }

}
