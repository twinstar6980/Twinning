import '/common.dart';

// ----------------

class ConvertHelper {

  // #region string list

  static String convertStringListToTextWithLine(
    List<String> value,
  ) {
    return value.join('\n') + (value.isNotEmpty && value.last.isEmpty ? '\n' : '');
  }

  static List<String> convertStringListFromTextWithLine(
    String text,
  ) {
    var value = text.split('\n').map((value) => (!value.endsWith('\r') ? value : value.substring(0, value.length - 1))).toList();
    if (value.isNotEmpty && value.last.isEmpty) {
      value.removeLast();
    }
    return value;
  }

  // #endregion

}
