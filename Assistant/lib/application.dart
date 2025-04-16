import '/common.dart';
import '/setting.dart';
import '/view/home/main_page.dart' as home;
import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import 'package:dynamic_color/dynamic_color.dart';

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
        builder: (context, setting, child) => DynamicColorBuilder(
          builder: (lightColor, darkColor) => MaterialApp(
            navigatorKey: setting.state.mApplicationNavigatorKey,
            theme: ThemeData(
              materialTapTargetSize: MaterialTapTargetSize.shrinkWrap,
              visualDensity: VisualDensity.standard,
              brightness: Brightness.light,
              colorScheme: !setting.data.mThemeColorState ? lightColor : ColorScheme.fromSeed(seedColor: setting.data.mThemeColorLight, brightness: Brightness.light),
              fontFamily: '',
              fontFamilyFallback: [...setting.state.mThemeFontFamliy],
              tooltipTheme: TooltipTheme.of(context).copyWith(
                waitDuration: const Duration(milliseconds: 1000),
              ),
              progressIndicatorTheme: const ProgressIndicatorThemeData(
                year2023: false,
              ),
              sliderTheme: const SliderThemeData(
                year2023: false,
              ),
            ),
            darkTheme: ThemeData(
              materialTapTargetSize: MaterialTapTargetSize.shrinkWrap,
              visualDensity: VisualDensity.standard,
              brightness: Brightness.dark,
              colorScheme: !setting.data.mThemeColorState ? darkColor : ColorScheme.fromSeed(seedColor: setting.data.mThemeColorDark, brightness: Brightness.dark),
              fontFamily: '',
              fontFamilyFallback: [...setting.state.mThemeFontFamliy],
              tooltipTheme: TooltipTheme.of(context).copyWith(
                waitDuration: const Duration(milliseconds: 1000),
              ),
              progressIndicatorTheme: const ProgressIndicatorThemeData(
                year2023: false,
              ),
              sliderTheme: const SliderThemeData(
                year2023: false,
              ),
            ),
            themeMode: setting.data.mThemeMode,
            scrollBehavior: const MaterialScrollBehavior().copyWith(scrollbars: false),
            title: kApplicationName,
            home: const home.MainPage(),
          ),
        ),
      ),
    );
  }

}
