import '/common.dart';
import '/setting.dart';
import '/command.dart';
import '/page/home.dart';
import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import 'package:dynamic_color/dynamic_color.dart';

// ----------------

class Application extends StatelessWidget {

  const Application({
    super.key,
    required this.setting,
    required this.command,
  });

  // ----------------

  final Setting setting;
  final Command command;

  // ----------------

  @override
  build(context) {
    return MultiProvider(
      providers: [
        ChangeNotifierProvider.value(value: SettingProvider(this.setting)),
        ChangeNotifierProvider.value(value: CommandProvider(this.command)),
      ],
      child: Consumer<SettingProvider>(
        builder: (context, setting, _) {
          return DynamicColorBuilder(builder: (lightDynamic, darkDynamic) {
            return MaterialApp(
              title: kApplicationName,
              theme: ThemeData(
                useMaterial3: true,
                brightness: Brightness.light,
                colorScheme: setting.data.mThemeColorInheritFromSystem ? lightDynamic : null,
                colorSchemeSeed: !setting.data.mThemeColorInheritFromSystem ? setting.data.mThemeColorLight : null,
                scaffoldBackgroundColor: setting.data.mThemeColorInheritFromSystem ? lightDynamic?.background : null,
                fontFamily: '',
                fontFamilyFallback: gPrimaryFontFamliy,
                appBarTheme: AppBarTheme(
                  centerTitle: false,
                  elevation: 3,
                  scrolledUnderElevation: 3,
                  shadowColor: lightDynamic?.shadow,
                ),
              ),
              darkTheme: ThemeData(
                useMaterial3: true,
                brightness: Brightness.dark,
                colorScheme: setting.data.mThemeColorInheritFromSystem ? darkDynamic : null,
                colorSchemeSeed: !setting.data.mThemeColorInheritFromSystem ? setting.data.mThemeColorDark : null,
                scaffoldBackgroundColor: setting.data.mThemeColorInheritFromSystem ? darkDynamic?.background : null,
                fontFamily: '',
                fontFamilyFallback: gPrimaryFontFamliy,
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
