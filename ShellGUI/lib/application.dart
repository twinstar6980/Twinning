import '/common.dart';
import '/setting.dart';
import '/page/home.dart';
import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import 'package:dynamic_color/dynamic_color.dart';

// ----------------

class Application extends StatelessWidget {

  const Application({
    super.key,
    required this.setting,
    required this.navigator,
  });

  // ----------------

  final SettingProvider           setting;
  final GlobalKey<NavigatorState> navigator;

  // ----------------

  @override
  build(context) {
    return ChangeNotifierProvider.value(
      value: setting,
      child: Consumer<SettingProvider>(
        builder: (context, setting, _) {
          return DynamicColorBuilder(builder: (lightDynamic, darkDynamic) {
            return MaterialApp(
              navigatorKey: navigator,
              title: kApplicationName,
              theme: ThemeData(
                brightness: Brightness.light,
                colorScheme: !setting.data.mThemeColorInheritFromSystem ? null : lightDynamic,
                colorSchemeSeed: setting.data.mThemeColorInheritFromSystem ? null : setting.data.mThemeColorLight,
                scaffoldBackgroundColor: !setting.data.mThemeColorInheritFromSystem ? null : lightDynamic?.background,
                fontFamily: '',
                fontFamilyFallback: [...setting.state.mPrimaryFontFamliy],
                appBarTheme: AppBarTheme(
                  centerTitle: false,
                  elevation: 3,
                  scrolledUnderElevation: 3,
                  shadowColor: lightDynamic?.shadow,
                ),
              ),
              darkTheme: ThemeData(
                brightness: Brightness.dark,
                colorScheme: !setting.data.mThemeColorInheritFromSystem ? null : darkDynamic,
                colorSchemeSeed: setting.data.mThemeColorInheritFromSystem ? null : setting.data.mThemeColorDark,
                scaffoldBackgroundColor: !setting.data.mThemeColorInheritFromSystem ? null : darkDynamic?.background,
                fontFamily: '',
                fontFamilyFallback: [...setting.state.mPrimaryFontFamliy],
                appBarTheme: AppBarTheme(
                  centerTitle: false,
                  elevation: 3,
                  scrolledUnderElevation: 3,
                  shadowColor: darkDynamic?.shadow,
                ),
              ),
              themeMode: setting.data.mThemeMode,
              home: const HomePage(),
            );
          });
        },
      ),
    );
  }

}
