// ignore_for_file: unused_import, unnecessary_import, dead_code

import '/common.dart';
import 'dart:io';
import 'dart:ui';
import 'dart:convert';
import 'package:flutter/widgets.dart';
import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import 'package:path_provider/path_provider.dart';
import 'package:window_manager/window_manager.dart';
import 'package:dynamic_color/dynamic_color.dart';
import '/setting.dart';
import '/command.dart';
import '/page/home.dart';

// ----------------

class Application extends StatelessWidget {

  const Application({
    super.key,
    required this.command,
  });

  // ----------------

  final List<String>? command;

  // ----------------

  @override
  Widget build(BuildContext context) {
    return MultiProvider(
      providers: [
        ChangeNotifierProvider.value(value: SettingProvider()),
        ChangeNotifierProvider.value(value: CommandProvider(this.command)),
      ],
      child: Consumer<SettingProvider>(
        builder: (context, setting, _) {
          return DynamicColorBuilder(builder: (ColorScheme? lightDynamic, ColorScheme? darkDynamic) {
            return MaterialApp(
              title: kApplicationName,
              theme: ThemeData(
                useMaterial3: true,
                brightness: Brightness.light,
                colorSchemeSeed: !setting.data.mThemeColorInheritFromSystem ? setting.data.mThemeColorLight : null,
                colorScheme: setting.data.mThemeColorInheritFromSystem ? lightDynamic : null,
                scaffoldBackgroundColor: setting.data.mThemeColorInheritFromSystem ? lightDynamic?.background : null,
                fontFamily: 'Noto Sans Mono CJK SC',
                appBarTheme: AppBarTheme(
                  centerTitle: false,
                  elevation: 4,
                  scrolledUnderElevation: 4,
                  shadowColor: darkDynamic?.shadow,
                ),
              ),
              darkTheme: ThemeData(
                useMaterial3: true,
                brightness: Brightness.dark,
                colorSchemeSeed: !setting.data.mThemeColorInheritFromSystem ? setting.data.mThemeColorDark : null,
                colorScheme: setting.data.mThemeColorInheritFromSystem ? darkDynamic : null,
                scaffoldBackgroundColor: setting.data.mThemeColorInheritFromSystem ? darkDynamic?.background : null,
                fontFamily: 'Noto Sans Mono CJK SC',
                appBarTheme: AppBarTheme(
                  centerTitle: false,
                  elevation: 4,
                  scrolledUnderElevation: 4,
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
