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

// ----------------

class ConfigurationHelper {

  // #region convert

  static Configuration parseDataFromJson(
    dynamic json,
  ) {
    return Configuration(
      game: (json['game'] as List<dynamic>).map((jsonGroup) => GameConfiguration(
        identifier: (jsonGroup['identifier'] as String),
        name: (jsonGroup['name'] as String),
        packageIdentifierOnSteam: (jsonGroup['package_identifier_on_steam'] as String?),
        packageIdentifierOnAndroid: (jsonGroup['package_identifier_on_android'] as String?),
      )).toList(),
    );
  }

  // #endregion

}
