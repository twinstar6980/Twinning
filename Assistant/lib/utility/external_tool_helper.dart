import '/common.dart';
import '/utility/storage_helper.dart';
import '/utility/process_helper.dart';

// ----------------

class ExternalToolHelper {

  // #region special

  static Future<List<String>> runIl2cppdumper(
    String programFile,
    String metadataFile,
  ) async {
    var dumpDirectory = await StorageHelper.temporary();
    await StorageHelper.createDirectory(dumpDirectory);
    var processResult = await ProcessHelper.runProcess(
      await ProcessHelper.searchProgramEnsure('dotnet', true),
      [
        await ProcessHelper.searchProgramEnsure('Il2CppDumper.dll', false),
        programFile,
        metadataFile,
        dumpDirectory,
      ],
      null,
    );
    assertTest(processResult.$2.replaceAll('\r\n', '\n').endsWith('Done!\nPress any key to exit...\n'));
    var result = (await StorageHelper.readFileText('${dumpDirectory}/dump.cs')).split('\n');
    await StorageHelper.remove(dumpDirectory);
    return result;
  }

  static List<(Integer, String, Boolean, String)> doIl2cppdumperSearchFieldFromDumpData(
    List<String> source,
    String       className,
    String       fieldName,
  ) {
    var result = <(Integer, String, Boolean, String)>[];
    var classRule = RegExp(r'^(private|protected|public) class ([^ ]+)');
    var fieldRule = RegExp(r'^\t(private|protected|public)( static)? (.+) (.+); \/\/ 0x(.+)$');
    for (var index = 0; index < source.length; index++) {
      var classMatch = classRule.firstMatch(source[index]);
      if (classMatch == null || classMatch[2] != className) {
        continue;
      }
      for (; index < source.length; index++) {
        if (source[index] == '}') {
          break;
        }
        var fieldMatch = fieldRule.firstMatch(source[index]);
        if (fieldMatch == null || fieldMatch[4] != fieldName) {
          continue;
        }
        result.add((
          Integer.parse(fieldMatch[5]!, radix: 16),
          fieldMatch[1]!,
          fieldMatch[2] != null,
          fieldMatch[3]!,
        ));
      }
      break;
    }
    return result;
  }

  static List<(Integer, String, Boolean, String, String)> doIl2cppdumperSearchMethodFromDumpData(
    List<String> source,
    String       className,
    String       methodName,
  ) {
    var result = <(Integer, String, Boolean, String, String)>[];
    var classRule = RegExp(r'^(private|protected|public) class ([^ ]+)');
    var methodRule = RegExp(r'^\t(private|protected|public)( static)? (.+) (.+)\((.*)\) \{ \}$');
    var commentRule = RegExp(r'^\t\/\/ RVA: 0x(.+) Offset: 0x(.+) VA: 0x(.+)$');
    for (var index = 0; index < source.length; index++) {
      var classMatch = classRule.firstMatch(source[index]);
      if (classMatch == null || classMatch[2] != className) {
        continue;
      }
      for (; index < source.length; index++) {
        if (source[index] == '}') {
          break;
        }
        var methodMatch = methodRule.firstMatch(source[index]);
        if (methodMatch == null || methodMatch[4] != methodName) {
          continue;
        }
        var commentMatch = commentRule.firstMatch(source[index - 1])!;
        result.add((
          Integer.parse(commentMatch[2]!, radix: 16),
          methodMatch[1]!,
          methodMatch[2] != null,
          methodMatch[3]!,
          methodMatch[5]!,
        ));
      }
      break;
    }
    return result;
  }

  // ----------------

  static Future<Void> runZipalign(
    String zipFile,
  ) async {
    var alignedFile = await StorageHelper.temporary();
    await StorageHelper.createFile(alignedFile);
    var processResult = await ProcessHelper.runProcess(
      await ProcessHelper.searchProgramEnsure('zipalign', true),
      [
        '-P', '16',
        '-f',
        '4',
        '${zipFile}',
        '${alignedFile}',
      ],
      null,
    );
    assertTest(processResult.$1 == 0);
    await StorageHelper.copy(alignedFile, zipFile);
    await StorageHelper.remove(alignedFile);
    return;
  }

  // ----------------

  static Future<Void> runApksigner(
    String apkFile,
    String apkCertificateFile,
    String apkCertificatePassword,
  ) async {
    var processResult = await ProcessHelper.runProcess(
      await ProcessHelper.searchProgramEnsure('apksigner', true),
      [
        'sign',
        '--v1-signing-enabled', 'true',
        '--v2-signing-enabled', 'true',
        '--v3-signing-enabled', 'true',
        '--v4-signing-enabled', 'false',
        '--ks', '${apkCertificateFile}',
        '--ks-pass', 'pass:${apkCertificatePassword}',
        '${apkFile}',
      ],
      null,
    );
    assertTest(processResult.$1 == 0);
    return;
  }

  // #endregion

}
