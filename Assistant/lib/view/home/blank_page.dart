import '/common.dart'; // ignore: unused_import
import '/setting.dart';
import '/widget/export.dart';
import '/view/home/module_page.dart';
import 'package:flutter/widgets.dart';
import 'package:provider/provider.dart';

// ----------------

class BlankPage extends StatelessWidget {

  const BlankPage({
    super.key,
  });

  // ----------------

  // ----------------

  @override
  build(context) {
    return ModulePageRegion(
      onDropFile: null,
      content: BoxContainer.of(
        padding: .fromLTRB(16, 16, 16, 16),
        align: .center,
        child: BoxContainer.of(
          constraints: .tightFor(width: 256, height: 256),
          child: StyledFloatingButton.larger(
            tooltip: 'Launcher',
            icon: IconView.of(IconSet.widgets, size: 128),
            onPressed: (context) async {
              Provider.of<SettingProvider>(context, listen: false).state.homeShowLauncherPanel!();
            },
          ),
        ),
      ),
      bottom: StyledBottomBar.standard(
        primary: null,
        secondary: [],
      ),
    );
  }

}
