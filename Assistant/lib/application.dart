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
          builder: (lightColor, darkColor) {
            // fix for dynamic color error on flutter 3.22+, see https://github.com/material-foundation/flutter-packages/issues/582#issuecomment-2209591668
            if (SystemChecker.isAndroid) {
              if (lightColor != null) {
                lightColor = ColorScheme.fromSeed(seedColor: lightColor.primary, brightness: Brightness.light);
              }
              if (darkColor != null) {
                darkColor = ColorScheme.fromSeed(seedColor: darkColor.primary, brightness: Brightness.dark);
              }
            }
            return MaterialApp(
              navigatorKey: setting.state.applicationNavigatorKey,
              theme: ThemeData(
                materialTapTargetSize: MaterialTapTargetSize.shrinkWrap,
                visualDensity: VisualDensity.standard,
                brightness: Brightness.light,
                colorScheme: !setting.data.themeColorState ? lightColor : ColorScheme.fromSeed(seedColor: setting.data.themeColorLight, brightness: Brightness.light),
                fontFamily: '',
                fontFamilyFallback: [...setting.state.themeFontFamliy],
                tooltipTheme: TooltipTheme.of(context).copyWith(
                  waitDuration: Duration(milliseconds: 1000),
                ),
                progressIndicatorTheme: ProgressIndicatorThemeData(
                  year2023: false, // ignore: deprecated_member_use
                ),
                sliderTheme: SliderThemeData(
                  year2023: false, // ignore: deprecated_member_use
                ),
              ),
              darkTheme: ThemeData(
                materialTapTargetSize: MaterialTapTargetSize.shrinkWrap,
                visualDensity: VisualDensity.standard,
                brightness: Brightness.dark,
                colorScheme: !setting.data.themeColorState ? darkColor : ColorScheme.fromSeed(seedColor: setting.data.themeColorDark, brightness: Brightness.dark),
                fontFamily: '',
                fontFamilyFallback: [...setting.state.themeFontFamliy],
                tooltipTheme: TooltipTheme.of(context).copyWith(
                  waitDuration: Duration(milliseconds: 1000),
                ),
                progressIndicatorTheme: ProgressIndicatorThemeData(
                  year2023: false, // ignore: deprecated_member_use
                ),
                sliderTheme: SliderThemeData(
                  year2023: false, // ignore: deprecated_member_use
                ),
              ),
              themeMode: setting.data.themeMode,
              scrollBehavior: MaterialScrollBehavior().copyWith(scrollbars: false),
              title: ApplicationInformation.name,
              home: home.MainPage(),
            );
          },
        ),
      ),
    );
  }

}
