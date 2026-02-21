import '/common.dart';
import '/setting.dart';
import '/utility/storage_helper.dart';
import '/widget/export.dart';
import 'package:flutter/widgets.dart';
import 'package:provider/provider.dart';

// ----------------

class OnboardingPanel extends StatelessWidget {

  const OnboardingPanel({
    super.key,
    required this.onDone,
  });

  // ----------------

  final Void Function() onDone;

  // ----------------

  @override
  build(context) {
    var setting = Provider.of<SettingProvider>(context);
    return FlexContainer.vertical([
      StyledListTile.standard(
        leading: IconView.of(IconSet.settings),
        content: StyledText.inherit('Import Setting'),
        onPressed: (context) async {
          var target = await StorageHelper.pickLoadFile(context, '@application.setting_file');
          if (target != null) {
            await setting.load(file: target);
            await setting.save();
            await StyledSnackExtension.show(context, 'Done!');
            this.onDone();
          }
        },
      ),
      StyledListTile.standard(
        leading: IconView.of(IconSet.build_circle),
        content: StyledText.inherit('Quick Setup'),
        onPressed: (context) async {
          var target = await StorageHelper.pickLoadDirectory(context, '@application.home_directory');
          if (target != null) {
            await setting.quickSetup(target);
            await setting.save();
            await StyledSnackExtension.show(context, 'Done!');
            this.onDone();
          }
        },
      ),
    ]);
  }

}
