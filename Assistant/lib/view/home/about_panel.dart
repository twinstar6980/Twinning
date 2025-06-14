import '/common.dart';
import '/setting.dart';
import '/utility/storage_helper.dart';
import '/utility/control_helper.dart';
import 'package:flutter/material.dart';
import 'package:provider/provider.dart';

// ----------------

class AboutPanel extends StatelessWidget {

  const AboutPanel({
    super.key,
  });

  // ----------------

  // ----------------

  @override
  build(context) {
    var setting = Provider.of<SettingProvider>(context);
    var theme = Theme.of(context);
    return Container(
      padding: EdgeInsets.fromLTRB(24, 16, 24, 16),
      child: Column(
        children: [
          Row(
            children: [
              Expanded(
                child: Text(
                  '${kApplicationName} - v${kApplicationVersion}',
                  overflow: TextOverflow.clip,
                  style: theme.textTheme.titleMedium,
                ),
              ),
            ],
          ),
          SizedBox(height: 8),
          Row(
            children: [
              Expanded(
                child: Text(
                  '© ${kApplicationYear} ${kApplicationDeveloper}. All rights reserved.',
                  overflow: TextOverflow.clip,
                  style: theme.textTheme.bodyMedium,
                ),
              ),
            ],
          ),
          SizedBox(height: 8),
          Divider(),
          SizedBox(height: 8),
          Row(
            crossAxisAlignment: CrossAxisAlignment.start,
            children: [
              SizedBox(width: 4),
              SizedBox(
                height: 34,
                child: Tooltip(
                  message: 'Setting File',
                  child: Icon(IconSymbols.settings),
                ),
              ),
              SizedBox(width: 16),
              Expanded(
                child: Wrap(
                  spacing: 12,
                  runSpacing: 12,
                  children: [
                    ActionChip(
                      avatar: Icon(IconSymbols.refresh),
                      label: Text(
                        'Reload',
                        overflow: TextOverflow.ellipsis,
                      ),
                      onPressed: () async {
                        await setting.load();
                        await setting.save();
                        await ControlHelper.showSnackBar(setting.state.mApplicationNavigatorKey.currentContext!, 'Done!');
                      },
                    ),
                    ActionChip(
                      avatar: Icon(IconSymbols.settings_backup_restore),
                      label: Text(
                        'Reset',
                        overflow: TextOverflow.ellipsis,
                      ),
                      onPressed: () async {
                        if (await ControlHelper.showDialogForConfirm(context)) {
                          await setting.reset();
                          await setting.save();
                          await ControlHelper.showSnackBar(setting.state.mApplicationNavigatorKey.currentContext!, 'Done!');
                        }
                      },
                    ),
                    ActionChip(
                      avatar: Icon(IconSymbols.download),
                      label: Text(
                        'Import',
                        overflow: TextOverflow.ellipsis,
                      ),
                      onPressed: () async {
                        var file = await StorageHelper.pickLoadFile(context, 'Application.SettingFile');
                        if (file != null) {
                          await setting.load(file: file);
                          await setting.save();
                          await ControlHelper.showSnackBar(setting.state.mApplicationNavigatorKey.currentContext!, 'Done!');
                        }
                      },
                    ),
                    ActionChip(
                      avatar: Icon(IconSymbols.upload),
                      label: Text(
                        'Export',
                        overflow: TextOverflow.ellipsis,
                      ),
                      onPressed: () async {
                        var file = await StorageHelper.pickSaveFile(context, 'Application.SettingFile');
                        if (file != null) {
                          await setting.save(file: file, apply: false);
                          await ControlHelper.showSnackBar(setting.state.mApplicationNavigatorKey.currentContext!, 'Done!');
                        }
                      },
                    ),
                  ],
                ),
              ),
            ],
          ),
          SizedBox(height: 12),
          Row(
            crossAxisAlignment: CrossAxisAlignment.start,
            children: [
              SizedBox(width: 4),
              SizedBox(
                height: 34,
                child: Tooltip(
                  message: 'Shared Directory',
                  child: Icon(IconSymbols.folder_special),
                ),
              ),
              SizedBox(width: 16),
              Expanded(
                child: Wrap(
                  spacing: 12,
                  runSpacing: 12,
                  children: [
                    ActionChip(
                      avatar: Icon(IconSymbols.open_in_browser),
                      label: Text(
                        'Reveal',
                        overflow: TextOverflow.ellipsis,
                      ),
                      onPressed: () async {
                        await ControlHelper.showDialogForRevealStoragePath(context, 'Shared Directory', await StorageHelper.queryApplicationSharedDirectory());
                      },
                    ),
                  ],
                ),
              ),
            ],
          ),
          SizedBox(height: 12),
          Row(
            crossAxisAlignment: CrossAxisAlignment.start,
            children: [
              SizedBox(width: 4),
              SizedBox(
                height: 34,
                child: Tooltip(
                  message: 'Cache Directory',
                  child: Icon(IconSymbols.folder_delete),
                ),
              ),
              SizedBox(width: 16),
              Expanded(
                child: Wrap(
                  spacing: 12,
                  runSpacing: 12,
                  children: [
                    ActionChip(
                      avatar: Icon(IconSymbols.delete_forever),
                      label: Text(
                        'Clear',
                        overflow: TextOverflow.ellipsis,
                      ),
                      onPressed: () async {
                        var cacheDirectory = await StorageHelper.queryApplicationCacheDirectory();
                        if (await StorageHelper.exist(cacheDirectory)) {
                          await StorageHelper.remove(cacheDirectory);
                        }
                        await ControlHelper.showSnackBar(setting.state.mApplicationNavigatorKey.currentContext!, 'Done!');
                      },
                    ),
                  ],
                ),
              ),
            ],
          ),
        ],
      ),
    );
  }

}
