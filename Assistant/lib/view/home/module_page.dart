import '/common.dart';
import '/widget/export.dart';
import 'package:flutter/widgets.dart';

// ----------------

class ModulePageRegion extends StatelessWidget {

  const ModulePageRegion({
    super.key,
    required this.onDropFile,
    required this.content,
    required this.bottom,
  });

  // ----------------

  final Void Function(List<String> item)? onDropFile;
  final Widget                            content;
  final Widget                            bottom;

  // ----------------

  @override
  build(context) {
    return FlexContainer.vertical([
      MediaQueryInheritedArea.of(
        data: MediaQuery.of(context).copyWith(
          padding: .zero,
          viewPadding: .zero,
        ),
        child: this.content,
      ).withStyledMaterial(
      ).withFlexExpanded(),
      this.bottom,
    ]).withStorageDropRegion(
      onDrop: this.onDropFile,
    );
  }

}

abstract class ModulePageState {

  // #region life

  Future<Void> modulePageOpenView(
  );

  Future<Boolean> modulePageCloseView(
  );

  // ----------------

  Future<Void> modulePageEnterView(
  );

  Future<Void> modulePageExitView(
  );

  // ----------------

  Future<Void> modulePageApplyOption(
    List<String> optionView,
  );

  Future<List<String>> modulePageCollectOption(
  );

  // #endregion

}
