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
      padding: const EdgeInsets.fromLTRB(24, 16, 24, 16),
      child: Column(
        children: [
          Row(
            children: [
              Expanded(
                child: Text(
                  '${kApplicationName} - ${kApplicationVersion}',
                  overflow: TextOverflow.clip,
                  style: theme.textTheme.titleMedium,
                ),
              ),
            ],
          ),
          const SizedBox(height: 8),
          Row(
            children: [
              Expanded(
                child: Text(
                  '© 2023-2024 TwinStar. All rights reserved.',
                  overflow: TextOverflow.clip,
                  style: theme.textTheme.bodyMedium,
                ),
              ),
            ],
          ),
          const SizedBox(height: 8),
          const Divider(),
          const SizedBox(height: 8),
          Row(
            crossAxisAlignment: CrossAxisAlignment.start,
            children: [
              const SizedBox(width: 4),
              const SizedBox(
                height: 34,
                child: Tooltip(
                  message: 'Setting File',
                  child: Icon(IconSymbols.settings),
                ),
              ),
              const SizedBox(width: 16),
              Expanded(
                child: Wrap(
                  spacing: 12,
                  runSpacing: 12,
                  children: [
                    ActionChip(
                      avatar: const Icon(IconSymbols.refresh),
                      label: const Text(
                        'Reload',
                        overflow: TextOverflow.ellipsis,
                      ),
                      onPressed: () async {
                        await setting.load();
                        await setting.save();
                      },
                    ),
                    ActionChip(
                      avatar: const Icon(IconSymbols.settings_backup_restore),
                      label: const Text(
                        'Reset',
                        overflow: TextOverflow.ellipsis,
                      ),
                      onPressed: () async {
                        if (await ControlHelper.showDialogForConfirm(context)) {
                          await setting.reset();
                          await setting.save();
                        }
                      },
                    ),
                    ActionChip(
                      avatar: const Icon(IconSymbols.download),
                      label: const Text(
                        'Import',
                        overflow: TextOverflow.ellipsis,
                      ),
                      onPressed: () async {
                        var file = await StorageHelper.pickLoadFile(context, 'Application.SettingFile');
                        if (file != null) {
                          await setting.load(file: file);
                          await setting.save();
                        }
                      },
                    ),
                    ActionChip(
                      avatar: const Icon(IconSymbols.upload),
                      label: const Text(
                        'Export',
                        overflow: TextOverflow.ellipsis,
                      ),
                      onPressed: () async {
                        var file = await StorageHelper.pickSaveFile(context, 'Application.SettingFile');
                        if (file != null) {
                          await setting.save(file: file, apply: false);
                        }
                      },
                    ),
                  ],
                ),
              ),
            ],
          ),
          const SizedBox(height: 12),
          Row(
            crossAxisAlignment: CrossAxisAlignment.start,
            children: [
              const SizedBox(width: 4),
              const SizedBox(
                height: 34,
                child: Tooltip(
                  message: 'Shared Directory',
                  child: Icon(IconSymbols.folder_special),
                ),
              ),
              const SizedBox(width: 16),
              Expanded(
                child: Wrap(
                  spacing: 12,
                  runSpacing: 12,
                  children: [
                    ActionChip(
                      avatar: const Icon(IconSymbols.open_in_browser),
                      label: const Text(
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
          const SizedBox(height: 12),
          Row(
            crossAxisAlignment: CrossAxisAlignment.start,
            children: [
              const SizedBox(width: 4),
              const SizedBox(
                height: 34,
                child: Tooltip(
                  message: 'Cache Directory',
                  child: Icon(IconSymbols.folder_delete),
                ),
              ),
              const SizedBox(width: 16),
              Expanded(
                child: Wrap(
                  spacing: 12,
                  runSpacing: 12,
                  children: [
                    ActionChip(
                      avatar: const Icon(IconSymbols.delete_forever),
                      label: const Text(
                        'Clear',
                        overflow: TextOverflow.ellipsis,
                      ),
                      onPressed: () async {
                        await StorageHelper.removeDirectory(await StorageHelper.queryApplicationCacheDirectory());
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
