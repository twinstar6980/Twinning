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
          child: SizedBox(
            width: 56,
            child: IconButton.filledTonal(
              icon: const Icon(IconSymbols.attachment),
              onPressed: () async {
                await ControlHelper.showCustomModalDialog(
                  context: context,
                  title: 'Additional Argument',
                  contentBuilder: (context, setState) => [
                    TextFormField(
                      style: theme.textTheme.bodyLarge?.copyWith(fontFamily: '', fontFamilyFallback: [...setting.state.mConsoleFontFamliy, ...setting.state.mThemeFontFamliy]),
                      decoration: const InputDecoration(
                        isDense: true,
                      ),
                      keyboardType: TextInputType.multiline,
                      maxLines: null,
                      inputFormatters: const [],
                      initialValue: ConvertHelper.convertStringListToTextWithLine(this.widget.additionalArgument),
                      onChanged: (value) async {
                        this.widget.additionalArgument.clear();
                        this.widget.additionalArgument.addAll(ConvertHelper.convertStringListFromTextWithLine(value));
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
