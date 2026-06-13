import '/common.dart';
import '/module.dart';
import '/utility/convert_helper.dart';
import '/utility/storage_path.dart';
import '/utility/storage_helper.dart';
import '/utility/process_helper.dart';
import '/utility/vdf_helper.dart';
import '/utility/platform_integration_manager.dart';
import '/view/core_task_worker/forward_helper.dart' as core_task_worker;
import 'dart:ui';
import 'dart:async';
import 'dart:typed_data';
import 'package:collection/collection.dart';
import 'package:archive/archive_io.dart' as lib;
import 'package:flutter/widgets.dart' show BuildContext;

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
}

class GameInformation {
  GamePackageType  packageType;
  StoragePath      packagePath;
  String           packageIdentifier;
  String           packageVersion;
  String           packageName;
  Image?           packageIcon;
  String?          userIdentifier;
  Uint8List?       userKey;
  GameProgramState programState;
  GameRecordState  recordState;
  List<String>     recordBackup;
  GameInformation(
  ) :
    this.packageType = .windowsSteam,
    this.packagePath = .new(),
    this.packageIdentifier = '',
    this.packageVersion = '',
    this.packageName = '',
    this.packageIcon = null,
    this.userIdentifier = null,
    this.userKey = null,
    this.programState = .none,
    this.recordState = .none,
    this.recordBackup = [];
}

class GameRecordHelper {

  // #region common

  static Future<StoragePath> _queryOriginalDirectory(
    GameInformation game,
  ) async {
    var result = null as StoragePath?;
    if (game.packageType == .windowsSteam) {
      result = game.packagePath.join('saves').join(game.userIdentifier ?? '0');
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

  static Future<Void> detectState(
    GameInformation game,
  ) async {
    game.recordState = .none;
    var originalDirectory = await GameRecordHelper._queryOriginalDirectory(game);
    if (await StorageHelper.existDirectory(originalDirectory)) {
      var contentItemList = await GameRecordHelper._listContent(originalDirectory);
      if (!contentItemList.isEmpty) {
        var contentItemFile = contentItemList.firstWhereOrNull((it) => it.name()! == '0000');
        if (contentItemFile == null) {
          game.recordState = .invalid;
        }
        else {
          var contentItemData = await StorageHelper.readFileLimited(originalDirectory.push(contentItemFile), 8);
          if (contentItemData.length == 8) {
            if (contentItemData.buffer.asUint32List().first == 0x00000000) {
              game.recordState = .decrypted;
            }
            else if (game.userKey != null) {
              GameRecordHelper._encryptData(contentItemData, game.userKey!);
              if (contentItemData.buffer.asUint32List().first == 0x00000000) {
                game.recordState = .original;
              }
            }
          }
        }
      }
    }
    return;
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
    StoragePath targetDirectory,
    Uint8List?  key,
  ) async {
    var contentItemList = await GameRecordHelper._listContent(targetDirectory);
    for (var contentItem in contentItemList) {
      await GameRecordHelper._encryptFile(targetDirectory.push(contentItem), targetDirectory.push(contentItem), key);
    }
    return;
  }

  // ----------------

  static Future<Void> encrypt(
    GameInformation game,
  ) async {
    var originalDirectory = await GameRecordHelper._queryOriginalDirectory(game);
    await GameRecordHelper._encryptDirectory(originalDirectory, game.userKey);
    await GameRecordHelper.detectState(game);
    return;
  }

  // #endregion

  // #region backup

  static Future<StoragePath> _queryBackupDirectory(
    GameInformation game,
    String?         identifier,
  ) async {
    var result = await GameRepositoryHelper.queryStoreDirectory(game);
    result = result.join('record');
    if (identifier != null) {
      result = result.join(identifier);
    }
    return result;
  }

  // ----------------

  static Future<Void> listBackup(
    GameInformation game,
  ) async {
    var backupRootDirectory = await _queryBackupDirectory(game, null);
    game.recordBackup = [];
    if (await StorageHelper.existDirectory(backupRootDirectory)) {
      game.recordBackup = (await StorageHelper.listDirectory(backupRootDirectory, 1, false, false, false, true)).map((it) => it.name()!).toList();
    }
    return;
  }

  static Future<Void> createBackup(
    GameInformation game,
    String          identifier,
  ) async {
    var backupDirectory = await GameRecordHelper._queryBackupDirectory(game, identifier);
    assertTest(!await StorageHelper.exist(backupDirectory));
    var originalDirectory = await GameRecordHelper._queryOriginalDirectory(game);
    var contentFileList = await GameRecordHelper._listContent(originalDirectory);
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
    var backupDirectory = await GameRecordHelper._queryBackupDirectory(game, identifier);
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
    var backupDirectory = await GameRecordHelper._queryBackupDirectory(game, identifier);
    assertTest(await StorageHelper.existDirectory(backupDirectory));
    if (newIdentifier != identifier) {
      var newBackupDirectory = await GameRecordHelper._queryBackupDirectory(game, newIdentifier);
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
    var backupDirectory = await GameRecordHelper._queryBackupDirectory(game, identifier);
    assertTest(await StorageHelper.existDirectory(backupDirectory));
    var originalDirectory = await GameRecordHelper._queryOriginalDirectory(game);
    if (await StorageHelper.exist(originalDirectory)) {
      await StorageHelper.remove(originalDirectory);
    }
    await StorageHelper.copy(backupDirectory, originalDirectory, false);
    await GameRecordHelper.detectState(game);
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
    if (game.packageType == .windowsSteam) {
      result = game.packagePath.join('GameAssembly.dll');
    }
    result!;
    return result;
  }

  // #endregion

  // #region state

  static Future<Void> detectState(
    GameInformation game,
  ) async {
    game.programState = .none;
    var originalFile = await GameProgramHelper._queryOriginalFile(game);
    if (await StorageHelper.existFile(originalFile)) {
      var backupDirectory = await GameProgramHelper._queryBackupDirectory(game);
      if (!await StorageHelper.existDirectory(backupDirectory)) {
        game.programState = .original;
      }
      else {
        game.programState = .modified;
      }
    }
    return;
  }

  // #endregion

  // #region modify

  static Future<Void> modify(
    GameInformation game,
    Boolean         disableRecordEncryption,
    Boolean         enableDebugMode,
    BuildContext    context,
  ) async {
    if (game.programState == .modified) {
      await GameProgramHelper.restoreBackup(game);
    }
    await GameProgramHelper.createBackup(game);
    var originalFile = await GameProgramHelper._queryOriginalFile(game);
    assertTest(await StorageHelper.existFile(originalFile));
    var taskResult = await core_task_worker.ForwardHelper.execute(context, core_task_worker.ForwardHelper.makeArgumentForCommand(
      null,
      'kairosoft.game.program.modify',
      {
        'target': game.packagePath.emit(),
        'disable_record_encryption': disableRecordEncryption,
        'enable_debug_mode': enableDebugMode,
      },
    ));
    assertTest(taskResult.first == 's');
    return;
  }

  // #endregion

  // #region backup

  static Future<StoragePath> _queryBackupDirectory(
    GameInformation game,
  ) async {
    var result = null as StoragePath?;
    if (game.packageType == .windowsSteam) {
      result = game.packagePath.join('.${ApplicationInformation.identifier}').join('backup_${game.packageVersion}');
    }
    result!;
    return result;
  }

  // ----------------

  static Future<Void> createBackup(
    GameInformation game,
  ) async {
    var backupDirectory = await GameProgramHelper._queryBackupDirectory(game);
    assertTest(!await StorageHelper.exist(backupDirectory));
    var originalFile = await GameProgramHelper._queryOriginalFile(game);
    assertTest(await StorageHelper.existFile(originalFile));
    await StorageHelper.createDirectory(backupDirectory);
    await StorageHelper.copy(originalFile, backupDirectory.join(originalFile.name()!), false);
    game.programState = .modified;
    return;
  }

  static Future<Void> restoreBackup(
    GameInformation game,
  ) async {
    var backupDirectory = await GameProgramHelper._queryBackupDirectory(game);
    assertTest(await StorageHelper.existDirectory(backupDirectory));
    var originalFile = await GameProgramHelper._queryOriginalFile(game);
    if (await StorageHelper.exist(originalFile)) {
      await StorageHelper.remove(originalFile);
    }
    await StorageHelper.copy(backupDirectory.join(originalFile.name()!), originalFile, false);
    await StorageHelper.remove(backupDirectory);
    game.programState = .original;
    return;
  }

  // #endregion

}

class GameRepositoryHelper {

  // #region common

  static Future<StoragePath> queryStoreDirectory(
    GameInformation game,
  ) async {
    return (await StorageHelper.query(.applicationPersistent))
      .join(ModuleHelper.query(.kairosoftGameManager).identifier)
      .join(game.packageName);
  }

  static Future<Image?> extractWindowsProgramIcon(
    StoragePath programFile,
  ) async {
    var icon = await PlatformIntegrationManager.instance.invokeOnWindowsExtractAssociatedIcon(programFile.emitNative());
    return await ConvertHelper.parseImageFromData(icon.data, width: icon.width, height: icon.height, isRawBgra: true);
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
    information.packageType = .windowsSteam;
    information.packagePath = gameDirectory;
    information.packageIdentifier = gameIdentifier;
    information.packageVersion = gameManifest.entries.first.value!.as<Map<Object?, Object?>>()['buildid']!.as<Integer>().toString();
    information.packageName = gameManifest.entries.first.value!.as<Map<Object?, Object?>>()['name']!.as<String>();
    information.packageIcon = await GameRepositoryHelper.extractWindowsProgramIcon(gameDirectory.join('KairoGames.exe'));
    information.userIdentifier = gameManifest.entries.first.value!.as<Map<Object?, Object?>>()['LastOwner']!.as<Integer>().toString();
    information.userKey = GameRepositoryHelper.makeKeyFromWindowsSteamUser(information.userIdentifier!);
    await GameProgramHelper.detectState(information);
    await GameRecordHelper.detectState(information);
    await GameRecordHelper.listBackup(information);
    return information;
  }

  static Future<List<GameInformation>> loadWindowsSteamRepository(
    StoragePath repositoryDirectory,
  ) async {
    assertTest(await GameRepositoryHelper.checkWindowsSteamRepository(repositoryDirectory));
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
    var newInformation = null as GameInformation?;
    if (game.packageType == .windowsSteam) {
      newInformation = await GameRepositoryHelper.loadWindowsSteamGame(game.packagePath.parent()!.parent()!.parent()!, game.packageIdentifier);
    }
    newInformation!;
    game.packageType = newInformation.packageType;
    game.packagePath = newInformation.packagePath;
    game.packageIdentifier = newInformation.packageIdentifier;
    game.packageVersion = newInformation.packageVersion;
    game.packageName = newInformation.packageName;
    game.packageIcon = newInformation.packageIcon;
    game.userIdentifier = newInformation.userIdentifier;
    game.userKey = newInformation.userKey;
    game.programState = newInformation.programState;
    game.recordState = newInformation.recordState;
    game.recordBackup = newInformation.recordBackup;
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
    if (game.packageType == .windowsSteam) {
      await ProcessHelper.runProcess(game.packagePath.join('KairoGames.exe'), [], null, null);
    }
    return;
  }

  // #endregion

}
