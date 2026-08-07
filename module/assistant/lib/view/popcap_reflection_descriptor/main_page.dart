import '/common.dart';
import '/widget/export.dart';
import '/view/home/module_page.dart';
import '/view/popcap_reflection_descriptor/setting.dart';
import '/view/popcap_reflection_descriptor/configuration.dart';
import '/view/popcap_reflection_descriptor/option.dart';

// ----------------

class MainPage extends StatefulWidget {

  const MainPage({
    super.key,
    required this.setting,
    required this.configuration,
    required this.option,
  });

  // ----------------

  final Setting       setting;
  final Configuration configuration;
  final Option        option;

  // ----------------

  @override
  createState() => _MainPageState();

}

class _MainPageState extends State<MainPage> implements ModulePageState {

  // ----------------

  @override
  modulePageOpenView() async {
    return;
  }

  @override
  modulePageCloseView() async {
    return true;
  }

  @override
  modulePageEnterView() async {
    return;
  }

  @override
  modulePageExitView() async {
    return;
  }

  @override
  modulePageApplyOption(option) async {
    option as Option;
    await refreshState(this.setState);
    return;
  }

  @override
  modulePageCollectOption() async {
    var option = Option();
    return option;
  }

  @override
  modulePageExecuteCommand(method, argument) async {
    var result = <String>[];
    switch (method) {
      default: throw UnsupportedException();
    }
    return result; // ignore: dead_code
  }

  // ----------------

  @override
  initState() {
    super.initState();
    postTask(() async {
      await this.modulePageOpenView();
      await this.modulePageApplyOption(this.widget.option);
    });
    return;
  }

  @override
  didUpdateWidget(oldWidget) {
    super.didUpdateWidget(oldWidget);
    return;
  }

  @override
  dispose() {
    super.dispose();
    return;
  }

  @override
  build(context) {
    return ModulePageRegion(
      onStorageDrop: null,
      content: FlexContainer.vertical([
      ]),
      bottom: StyledBottomBar.standard(
        primary: null,
        secondary: [
        ],
      ),
    );
  }

}
