import '/common.dart';

// ----------------

class GameConfiguration {
  String  identifier;
  String  name;
  String? packageIdentifierOnSteam;
  String? packageIdentifierOnAndroid;
  GameConfiguration({
    required this.identifier,
    required this.name,
    required this.packageIdentifierOnSteam,
    required this.packageIdentifierOnAndroid,
  });
}

class Configuration {
  List<GameConfiguration> game;
  Configuration({
    required this.game,
  });
}
