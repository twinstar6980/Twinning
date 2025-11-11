import '/common.dart'; // ignore: unused_import
import '/setting.dart';
import '/widget/export.dart';
import '/view/home/main_page.dart' as home;
import 'package:flutter/widgets.dart';
import 'package:provider/provider.dart';

// ----------------

class Application extends StatelessWidget {

  const Application({
    super.key,
    required this.setting,
  });

  // ----------------

  final SettingProvider setting;

  // ----------------

  @override
  build(context) {
    return ChangeNotifierProvider.value(
      value: this.setting,
      child: Consumer<SettingProvider>(
        builder: (context, setting, child) => StyledApplication.standard(
          navigatorKey: setting.state.applicationNavigatorKey,
          themeMode: setting.data.themeMode,
          themeColorState: setting.data.themeColorState,
          themeColorLight: setting.data.themeColorLight,
          themeColorDark: setting.data.themeColorDark,
          themeFontFamily: setting.state.themeFontFamliy,
          home: home.MainPage(),
        ),
      ),
    );
  }

}
