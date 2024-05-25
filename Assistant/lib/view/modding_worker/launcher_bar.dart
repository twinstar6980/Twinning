import '/common.dart';
import '/setting.dart';
import '/utility/convert_helper.dart';
import '/utility/control_helper.dart';
import 'package:flutter/material.dart';
import 'package:provider/provider.dart';

// ----------------

class LauncherBar extends StatefulWidget {

  const LauncherBar({
    super.key,
    required this.additionalArgument,
    required this.onLaunch,
  });

  @override
  createState() => _LauncherBarState();

  // ----------------

  final List<String>    additionalArgument;
  final Void Function() onLaunch;

}

class _LauncherBarState extends State<LauncherBar> {

  // ----------------

  @override
  initState() {
    super.initState();
    return;
  }

  @override
  dispose() {
    super.dispose();
    return;
  }

  @override
  build(context) {
    var setting = Provider.of<SettingProvider>(context);
    var theme = Theme.of(context);
    return Row(
      children: [
        Badge.count(
          count: this.widget.additionalArgument.length,
          textStyle: theme.textTheme.labelSmall?.copyWith(
            fontFamily: '',
            fontFamilyFallback: [...setting.state.mModdingWorkerMessageFontFamily, ...setting.state.mThemeFontFamliy],
          ),
          child: SizedBox(
            width: 56,
            height: 40,
            child: IconButton.filledTonal(
              tooltip: 'Additional Argument',
              icon: const Icon(IconSymbols.attach_file),
              onPressed: () async {
                await ControlHelper.showCustomModalDialog(
                  context: context,
                  title: 'Additional Argument',
                  contentBuilder: (context, setState) => [
                    TextFormField(
                      keyboardType: TextInputType.multiline,
                      maxLines: null,
                      inputFormatters: const [],
                      decoration: const InputDecoration(
                        isDense: true,
                      ),
                      style: theme.textTheme.bodyLarge?.copyWith(
                        fontFamily: '',
                        fontFamilyFallback: [...setting.state.mModdingWorkerMessageFontFamily, ...setting.state.mThemeFontFamliy],
                      ),
                      initialValue: ConvertHelper.makeStringListToStringWithLine(this.widget.additionalArgument),
                      onChanged: (value) async {
                        this.widget.additionalArgument.clear();
                        this.widget.additionalArgument.addAll(ConvertHelper.parseStringListFromStringWithLine(value));
                        this.setState(() {});
                        setState(() {});
                      },
                    ),
                  ],
                );
              },
            ),
          ),
        ),
        const SizedBox(width: 16),
        const Expanded(child: SizedBox()),
        const SizedBox(width: 16),
        FloatingActionButton(
          tooltip: 'Launch',
          elevation: 0,
          focusElevation: 0,
          hoverElevation: 0,
          highlightElevation: 0,
          disabledElevation: 0,
          onPressed: () async {
            this.widget.onLaunch();
          },
          child: const Icon(IconSymbols.play_circle),
        ),
      ],
    );
  }

}
