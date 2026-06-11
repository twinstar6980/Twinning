import '/common.dart';
import '/utility/convert_helper.dart';
import '/utility/storage_path.dart';
import '/utility/storage_helper.dart';
import '/utility/process_helper.dart';
import '/utility/vdf_helper.dart';
import '/utility/platform_integration_manager.dart';
import 'dart:ui';
import 'dart:async';
import 'dart:typed_data';
import 'package:collection/collection.dart';
import 'package:archive/archive_io.dart' as lib;

// ----------------

enum GameProgramState {
  none,
  original,
  modified,
}

enum GameRecordState {
  none,
  original,
  decrypted,
  invalid,
}

enum GamePackageType {
  windowsSteam,
  windowsStandalone,
  android,
}

class GameInformation {
  GamePackageType  packageType;
  StoragePath      packagePath;
  String?          packageIdentifier;
  String?          packageVersion;
  String           packageName;
  Image?           packageIcon;
  String?          key; // TODO: rename
  Uint8List?       keyEx; // TODO: rename
  GameProgramState programState;
  List<String>     programBackup;
  GameRecordState  recordState;
  List<String>     recordBackup;
  GameInformation(
  ) :
    this.packageType = .windowsSteam,
    this.packagePath = .new(),
    this.packageIdentifier = null,
    this.packageVersion = null,
    this.packageName = '',
    this.packageIcon = null,
    this.key = null,
    this.keyEx = null,
    this.programState = .none,
    this.programBackup = [],
    this.recordState = .none,
    this.recordBackup = [];
}

class GameRecordHelper {

  // #region common

  static Future<StoragePath> _queryOriginalDirectory(
    GameInformation game,
  ) async {
    var result = null as StoragePath?;
    if (game.packageType == .windowsSteam || game.packageType == .windowsStandalone) {
      result = game.packagePath.join('saves').join(game.key ?? '0');
    }
    result!;
    return result;
  }

  // ----------------

  static Future<List<StoragePath>> _listContent(
    StoragePath recordDirectory,
  ) async {
    return (await StorageHelper.listDirectory(recordDirectory, 1, true, false, true, false))
      .where((it) => RegExp(r'^\d{4,4}(_backup)?$').hasMatch(it.name()!))
      .toList();
  }

  // #endregion

  // #region state

  static Future<GameRecordState> detectState(
    StoragePath recordDirectory,
    Uint8List?  key,
  ) async {
    var state = GameRecordState.invalid;
    var itemList = await GameRecordHelper._listContent(recordDirectory);
    if (itemList.isEmpty) {
      state = .none;
    }
    else if (itemList.firstWhereOrNull((it) => it.name()! == '0000') != null) {
      var itemFile = recordDirectory.join('0000');
      var itemData = await StorageHelper.readFileLimited(itemFile, 8);
      if (itemData.length == 8) {
        if (itemData.buffer.asUint32List().first == 0x00000000) {
          state = .decrypted;
        }
        else if (key != null) {
          GameRecordHelper._encryptData(itemData, key);
          if (itemData.buffer.asUint32List().first == 0x00000000) {
            state = .original;
          }
        }
      }
    }
    return state;
  }

  // #endregion

  // #region encrypt

  static Void _encryptData(
    Uint8List  data,
    Uint8List? key,
  ) {
    if (key != null) {
      assertTest(!key.isEmpty);
      for (var index = 0; index < data.length; index++) {
        data[index] ^= key[index % key.length];
      }
    }
    return;
  }

  static Future<Void> _encryptFile(
    StoragePath sourceFile,
    StoragePath destinationFile,
    Uint8List?  key,
  ) async {
    var data = await StorageHelper.readFile(sourceFile);
    GameRecordHelper._encryptData(data, key);
    if (!await StorageHelper.existFile(destinationFile)) {
      await StorageHelper.createFile(destinationFile);
    }
    await StorageHelper.writeFile(destinationFile, data);
    return;
  }

  static Future<Void> _encryptDirectory(
    StoragePath recordDirectory,
    Uint8List?  key,
  ) async {
    var itemList = await GameRecordHelper._listContent(recordDirectory);
    for (var item in itemList) {
      await GameRecordHelper._encryptFile(recordDirectory.push(item), recordDirectory.push(item), key);
    }
    return;
  }

  // ----------------

  static Future<Void> encrypt(
    GameInformation game,
  ) async {
    var recordDirectory = await _queryOriginalDirectory(game);
    await _encryptDirectory(recordDirectory, game.keyEx);
    game.recordState = await detectState(recordDirectory, game.keyEx);
    return;
  }

  // #endregion

  // #region backup

  static Future<StoragePath> _queryBackupDirectory(
    GameInformation game,
    String?         identifier,
  ) async {
    var result = await GameRepositoryHelper.queryPackageDirectory(game);
    result = result.join('record');
    if (identifier != null) {
      result = result.join(identifier);
    }
    return result;
  }

  // ----------------

  static Future<List<String>> listBackup(
    GameInformation game,
  ) async {
    var backupRootDirectory = await _queryBackupDirectory(game, null);
    var backupNameList = <String>[];
    if (await StorageHelper.existDirectory(backupRootDirectory)) {
      backupNameList = (await StorageHelper.listDirectory(backupRootDirectory, 1, false, false, false, true)).map((it) => it.name()!).toList();
    }
    return backupNameList;
  }

  static Future<Void> createBackup(
    GameInformation game,
    String          identifier,
  ) async {
    var backupDirectory = await _queryBackupDirectory(game, identifier);
    assertTest(!await StorageHelper.exist(backupDirectory));
    var originalDirectory = await _queryOriginalDirectory(game);
    var contentFileList = await _listContent(originalDirectory);
    await StorageHelper.createDirectory(backupDirectory);
    for (var contentFile in contentFileList) {
      await StorageHelper.copy(originalDirectory.push(contentFile), backupDirectory.push(contentFile), false);
    }
    game.recordBackup.add(identifier);
    return;
  }

  static Future<Void> deleteBackup(
    GameInformation game,
    String          identifier,
  ) async {
    var backupDirectory = await _queryBackupDirectory(game, identifier);
    assertTest(await StorageHelper.existDirectory(backupDirectory));
    await StorageHelper.remove(backupDirectory);
    game.recordBackup.remove(identifier);
    return;
  }

  static Future<Void> renameBackup(
    GameInformation game,
    String          identifier,
    String          newIdentifier,
  ) async {
    var backupDirectory = await _queryBackupDirectory(game, identifier);
    assertTest(await StorageHelper.existDirectory(backupDirectory));
    if (newIdentifier != identifier) {
      var newBackupDirectory = await _queryBackupDirectory(game, newIdentifier);
      assertTest(!await StorageHelper.exist(newBackupDirectory));
      await StorageHelper.rename(backupDirectory, newBackupDirectory);
      game.recordBackup[game.recordBackup.indexOf(identifier)] = newIdentifier;
    }
    return;
  }

  static Future<Void> restoreBackup(
    GameInformation game,
    String          identifier,
  ) async {
    var backupDirectory = await _queryBackupDirectory(game, identifier);
    assertTest(await StorageHelper.existDirectory(backupDirectory));
    var originalDirectory = await _queryOriginalDirectory(game);
    if (await StorageHelper.exist(originalDirectory)) {
      await StorageHelper.remove(originalDirectory);
    }
    await StorageHelper.copy(backupDirectory, originalDirectory, false);
    return;
  }

  static Future<Void> exportBackup(
    StoragePath targetDirectory,
    StoragePath archiveFile,
    Uint8List?  key,
  ) async {
    var archiveDirectory = await StorageHelper.temporary();
    await StorageHelper.createDirectory(archiveDirectory);
    // TODO
    await StorageHelper.createFile(archiveDirectory.join('configuration'));
    await StorageHelper.writeFileText(archiveDirectory.join('configuration'), '');
    await StorageHelper.createDirectory(archiveDirectory.join('data'));
    for (var dataFile in await GameRecordHelper._listContent(targetDirectory)) {
      await GameRecordHelper._encryptFile(targetDirectory.push(dataFile), archiveDirectory.join('data').push(dataFile), key);
    }
    if (await StorageHelper.exist(archiveFile)) {
      await StorageHelper.remove(archiveFile);
    }
    var archive = lib.createArchiveFromDirectory(.new(archiveDirectory.emit()));
    var archiveData = lib.ZipEncoder().encodeBytes(archive, level: lib.DeflateLevel.bestCompression);
    if (!await StorageHelper.existFile(archiveFile)) {
      await StorageHelper.createFile(archiveFile);
    }
    await StorageHelper.writeFile(archiveFile, archiveData);
    await StorageHelper.remove(archiveDirectory);
    return;
  }

  static Future<Void> importBackup(
    StoragePath targetDirectory,
    StoragePath archiveFile,
    Uint8List?  key,
  ) async {
    var archiveDirectory = await StorageHelper.temporary();
    await StorageHelper.createDirectory(archiveDirectory);
    var archiveData = await StorageHelper.readFile(archiveFile);
    var archive = lib.ZipDecoder().decodeBytes(archiveData);
    await lib.extractArchiveToDisk(archive, archiveDirectory.emit());
    if (await StorageHelper.exist(targetDirectory)) {
      await StorageHelper.remove(targetDirectory);
    }
    // TODO
    var configuration = StorageHelper.readFileText(archiveDirectory.join('configuration'));
    await StorageHelper.createDirectory(targetDirectory);
    for (var dataFile in await GameRecordHelper._listContent(archiveDirectory.join('data'))) {
      await GameRecordHelper._encryptFile(archiveDirectory.join('configuration').push(dataFile), targetDirectory.push(dataFile), key);
    }
    await StorageHelper.remove(archiveDirectory);
    return;
  }

  // #endregion

}

class GameProgramHelper {

  // #region common

  static Future<StoragePath> _queryOriginalFile(
    GameInformation game,
  ) async {
    var result = null as StoragePath?;
    if (game.packageType == .windowsSteam || game.packageType == .windowsStandalone) {
      result = game.packagePath.join('GameAssembly.dll');
    }
    result!;
    return result;
  }

  // #endregion

  // #region modify

  static Future<Void> modify(
    GameInformation game,
    Boolean         disableRecordEncryption,
    Boolean         enableDebugMode,
  ) async {
    var originalFile = await _queryOriginalFile(game);
    assertTest(await StorageHelper.existFile(originalFile));
    // TODO
    return;
  }

  // #endregion

  // #region backup

  static Future<StoragePath> _queryBackupDirectory(
    GameInformation game,
    String?         identifier,
  ) async {
    var result = await GameRepositoryHelper.queryPackageDirectory(game);
    result = result.join('program');
    if (identifier != null) {
      result = result.join(identifier);
    }
    return result;
  }

  // ----------------

  static Future<List<String>> listBackup(
    GameInformation game,
  ) async {
    var backupRootDirectory = await _queryBackupDirectory(game, null);
    var backupNameList = <String>[];
    if (await StorageHelper.existDirectory(backupRootDirectory)) {
      backupNameList = (await StorageHelper.listDirectory(backupRootDirectory, 1, false, false, false, true)).map((it) => it.name()!).toList();
    }
    return backupNameList;
  }

  static Future<Void> createBackup(
    GameInformation game,
    String          identifier,
  ) async {
    var backupDirectory = await _queryBackupDirectory(game, identifier);
    assertTest(!await StorageHelper.exist(backupDirectory));
    var originalFile = await _queryOriginalFile(game);
    assertTest(await StorageHelper.existFile(originalFile));
    await StorageHelper.createDirectory(backupDirectory);
    await StorageHelper.copy(originalFile, backupDirectory.join(originalFile.name()!), false);
    game.programBackup.add(identifier);
    return;
  }

  static Future<Void> deleteBackup(
    GameInformation game,
    String          identifier,
  ) async {
    var backupDirectory = await _queryBackupDirectory(game, identifier);
    assertTest(await StorageHelper.existDirectory(backupDirectory));
    await StorageHelper.remove(backupDirectory);
    game.programBackup.remove(identifier);
    return;
  }

  static Future<Void> renameBackup(
    GameInformation game,
    String          identifier,
    String          newIdentifier,
  ) async {
    var backupDirectory = await _queryBackupDirectory(game, identifier);
    assertTest(await StorageHelper.existDirectory(backupDirectory));
    if (newIdentifier != identifier) {
      var newBackupDirectory = await _queryBackupDirectory(game, newIdentifier);
      assertTest(!await StorageHelper.exist(newBackupDirectory));
      await StorageHelper.rename(backupDirectory, newBackupDirectory);
      game.programBackup[game.programBackup.indexOf(identifier)] = newIdentifier;
    }
    return;
  }

  static Future<Void> restoreBackup(
    GameInformation game,
    String          identifier,
  ) async {
    var backupDirectory = await _queryBackupDirectory(game, identifier);
    assertTest(await StorageHelper.existDirectory(backupDirectory));
    var originalFile = await _queryOriginalFile(game);
    if (await StorageHelper.exist(originalFile)) {
      await StorageHelper.remove(originalFile);
    }
    await StorageHelper.copy(backupDirectory.join(originalFile.name()!), originalFile, false);
    return;
  }

  // #endregion

}

class GameRepositoryHelper {

  // #region common

  static Future<StoragePath> queryPackageDirectory(
    GameInformation game,
  ) async {
    return (await StorageHelper.query(.applicationShared)).join('kr').join(game.packageName);
  }

  static Future<Image?> extractProgramIcon(
    StoragePath programFile,
  ) async {
    var icon = await PlatformIntegrationManager.instance.invokeOnWindowsExtractAssociatedIcon(programFile.emitNative());
    return await ConvertHelper.parseImageFromData(icon.data, width: icon.width, height: icon.height, isRawBgra: true);
  }

  static Future<({GameProgramState program, GameRecordState record})> checkGameState(
    StoragePath gameDirectory,
    String?     version,
    String      user,
  ) async {
    var programState = GameProgramState.none;
    var recordState = GameRecordState.none;
    if (await StorageHelper.existFile(gameDirectory.join('KairoGames.exe'))) {
      programState = !await StorageHelper.existFile(gameDirectory.join('KairoGames.exe.${version ?? '0'}.bak'))
        ? .original
        : .modified;
    }
    if (await StorageHelper.existDirectory(gameDirectory.join('saves').join('${user}'))) {
      recordState = await GameRecordHelper.detectState(gameDirectory.join('saves').join('${user}'), GameRepositoryHelper.makeKeyFromWindowsSteamUser(user));
    }
    return (program: programState, record: recordState);
  }

  // #endregion

  // #region windows standalone

  static Future<GameInformation?> loadWindowsStandaloneGame(
    StoragePath gameDirectory,
  ) async {
    if (!await StorageHelper.existFile(gameDirectory.join('KairoGames.exe'))) {
      return null;
    }
    var information = GameInformation();
    information.packagePath = gameDirectory;
    information.packageIdentifier = null;
    information.packageVersion = null;
    information.packageName = gameDirectory.name()!;
    information.packageIcon = await GameRepositoryHelper.extractProgramIcon(gameDirectory.join('KairoGames.exe'));
    information.key = '0';
    if (!await StorageHelper.existDirectory(gameDirectory.join('saves'))) {
      information.key = (await StorageHelper.listDirectory(gameDirectory.join('saves'), 1, true, false, false, true)).firstWhereOrNull((it) => RegExp(r'^\d+$').hasMatch(it.name()!))?.name()! ?? '0';
    }
    var gameState = await GameRepositoryHelper.checkGameState(gameDirectory, information.packageVersion, information.key!);
    information.programState = gameState.program;
    information.recordState = gameState.record;
    return information;
  }

  static Future<List<GameInformation>> loadWindowsStandaloneRepository(
    StoragePath repositoryDirectory,
  ) async {
    var libraryList = await StorageHelper.listDirectory(repositoryDirectory, 1, true, false, false, true);
    var result = <GameInformation>[];
    for (var library in libraryList) {
      var libraryDirectory = repositoryDirectory.push(library);
      var gameConfiguration = await GameRepositoryHelper.loadWindowsStandaloneGame(libraryDirectory);
      if (gameConfiguration != null) {
        result.add(gameConfiguration);
      }
    }
    return result;
  }

  static Future<Boolean> checkWindowsStandaloneRepository(
    StoragePath repositoryDirectory,
  ) async {
    return await StorageHelper.existDirectory(repositoryDirectory);
  }

  // #endregion

  // #region windows steam

  static Uint8List makeKeyFromWindowsSteamUser(
    String user,
  ) {
    var keyValue = Integer.parse(user);
    var key = Uint64List.fromList([keyValue]).buffer.asUint8List();
    return key;
  }

  // ----------------

  static Future<GameInformation?> loadWindowsSteamGame(
    StoragePath libraryDirectory,
    String      gameIdentifier,
  ) async {
    var gameManifestFile = libraryDirectory.join('steamapps').join('appmanifest_${gameIdentifier}.acf');
    if (!await StorageHelper.existFile(gameManifestFile)) {
      return null;
    }
    var gameManifest = await VdfHelper.decodeFile(gameManifestFile);
    assertTest(gameManifest.length == 1);
    assertTest(gameManifest.entries.first.key == 'AppState');
    assertTest(gameManifest.entries.first.value!.as<Map<Object?, Object?>>()['appid']!.as<Integer>().toString() == gameIdentifier);
    var gameDirectory = libraryDirectory.join('steamapps').join('common').join(gameManifest.entries.first.value!.as<Map<Object?, Object?>>()['installdir']!.as<String>());
    if (!await StorageHelper.existFile(gameDirectory.join('KairoGames.exe'))) {
      return null;
    }
    var information = GameInformation();
    information.packagePath = gameDirectory;
    information.packageIdentifier = gameIdentifier;
    information.packageVersion = gameManifest.entries.first.value!.as<Map<Object?, Object?>>()['buildid']!.as<Integer>().toString();
    information.packageName = gameManifest.entries.first.value!.as<Map<Object?, Object?>>()['name']!.as<String>();
    information.packageIcon = await GameRepositoryHelper.extractProgramIcon(gameDirectory.join('KairoGames.exe'));
    information.key = gameManifest.entries.first.value!.as<Map<Object?, Object?>>()['LastOwner']!.as<Integer>().toString();
    information.keyEx = GameRepositoryHelper.makeKeyFromWindowsSteamUser(information.key!);
    var gameState = await GameRepositoryHelper.checkGameState(gameDirectory, information.packageVersion, information.key!);
    information.programState = gameState.program;
    information.programBackup = await GameProgramHelper.listBackup(information);
    information.recordState = gameState.record;
    information.recordBackup = await GameRecordHelper.listBackup(information);
    return information;
  }

  static Future<List<GameInformation>> loadWindowsSteamRepository(
    StoragePath repositoryDirectory,
  ) async {
    var libraryList = await VdfHelper.decodeFile(repositoryDirectory.join('steamapps').join('libraryfolders.vdf'));
    assertTest(libraryList.length == 1);
    assertTest(libraryList.entries.first.key == 'libraryfolders');
    var result = <GameInformation>[];
    for (var library in libraryList['libraryfolders']!.as<Map<Object?, Object?>>().entries) {
      var libraryDirectory = StoragePath.of(library.value!.as<Map<Object?, Object?>>()['path']!.as<String>().replaceAll('\\\\', '/'));
      for (var game in library.value!.as<Map<Object?, Object?>>()['apps']!.as<Map<Object?, Object?>>().entries) {
        var gameIdentifier = game.key!.as<String>();
        var gameConfiguration = await GameRepositoryHelper.loadWindowsSteamGame(libraryDirectory, gameIdentifier);
        if (gameConfiguration != null) {
          result.add(gameConfiguration);
        }
      }
    }
    return result;
  }

  static Future<Boolean> checkWindowsSteamRepository(
    StoragePath repositoryDirectory,
  ) async {
    return await StorageHelper.existFile(repositoryDirectory.join('steam.exe'));
  }

  // #endregion

}

class GameActionHelper {

  // #region common

  static Future<Void> reload(
    GameInformation game,
  ) async {
    if (game.packageType == .windowsSteam) {
      // GameRepositoryHelper.loadWindowsSteamGame(libraryDirectory, gameIdentifier)
    }
    return;
  }

  static Future<Void> reveal(
    GameInformation game,
  ) async {
    await StorageHelper.reveal(game.packagePath);
    return;
  }

  static Future<Void> launch(
    GameInformation game,
  ) async {
    if (game.packageType == .windowsSteam || game.packageType == .windowsStandalone) {
      await ProcessHelper.runProcess(game.packagePath.join('KairoGames.exe'), [], null, null);
    }
    return;
  }

  // #endregion

}
