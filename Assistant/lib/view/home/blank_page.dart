import '/common.dart';
import '/setting.dart';
import '/view/home/common.dart';
import 'package:flutter/material.dart';
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
    return CustomModulePageRegion(
      onDropFile: null,
      content: Container(
        alignment: Alignment.center,
        padding: const EdgeInsets.fromLTRB(16, 16, 16, 16),
        child: SizedBox(
          width: 256,
          height: 256,
          child: FloatingActionButton.large(
            tooltip: 'Launch',
            elevation: 0,
            focusElevation: 0,
            hoverElevation: 0,
            highlightElevation: 0,
            disabledElevation: 0,
            child: const Icon(IconSymbols.widgets, size: 128),
            onPressed: () async {
              Provider.of<SettingProvider>(context, listen: false).state.mHomeShowLauncherPanel!();
            },
          ),
        ),
      ),
      bottom: const CustomBottomBarContent(
        primary: null,
        secondary: [],
      ),
    );
  }

}
