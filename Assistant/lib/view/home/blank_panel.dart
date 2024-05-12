import '/common.dart';
import 'package:flutter/material.dart';

// ----------------

class BlankPanel extends StatelessWidget {

  const BlankPanel({
    super.key,
    required this.showLauncherPanel,
  });

  // ----------------

  final Void Function() showLauncherPanel;

  // ----------------

  @override
  build(context) {
    return Container(
      padding: const EdgeInsets.fromLTRB(16, 16, 16, 16),
      alignment: Alignment.center,
      child: SizedBox(
        width: 256,
        height: 256,
        child: FloatingActionButton.large(
          elevation: 0,
          focusElevation: 0,
          hoverElevation: 0,
          highlightElevation: 0,
          disabledElevation: 0,
          onPressed: () async {
            this.showLauncherPanel();
          },
          child: const Icon(
            IconSymbols.widgets,
            size: 128,
          ),
        ),
      ),
    );
  }

}
