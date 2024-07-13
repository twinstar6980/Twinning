import '/common.dart';
import '/utility/convert_helper.dart';
import '/utility/storage_helper.dart';
import '/view/home/common.dart';
import '/view/modding_worker/setting.dart';
import 'package:flutter/material.dart';

// ----------------

class SettingPanel extends StatelessWidget {

  const SettingPanel({
    super.key,
    required this.data,
    required this.onUpdate,
  });

  // ----------------

  final Setting         data;
  final Void Function() onUpdate;

  // ----------------

  @override
  build(context) {
    var theme = Theme.of(context);
    return StatefulBuilder(
      builder: (context, setState) => Column(
        children: [
          const SizedBox(height: 8),
          CustomSettingItem(
            icon: IconSymbols.adjust,
            label: 'Kernel',
            content: [
              Text(
                !StorageHelper.existFileSync(this.data.mKernel) ? 'Invalid' : 'Available',
                overflow: TextOverflow.ellipsis,
                style: theme.textTheme.bodyMedium,
              ),
            ],
            onTap: null,
            panelBuilder: (context, setStateForPanel) => [
              ListTile(
                contentPadding: EdgeInsets.zero,
                title: CustomTextField(
                  keyboardType: TextInputType.text,
                  inputFormatters: const [],
                  decoration: InputDecoration(
                    contentPadding: const EdgeInsets.fromLTRB(12, 16, 12, 16),
                    filled: false,
                    border: const OutlineInputBorder(),
                    suffixIcon: CustomTextFieldSuffixRegion(
                      children: [
                        IconButton(
                          tooltip: 'Pick',
                          icon: const Icon(IconSymbols.open_in_new),
                          onPressed: () async {
                            var target = await StorageHelper.pickLoadFile(context, 'ModdingWorker.Kernel');
                            if (target != null) {
                              this.data.mKernel = target;
                              setStateForPanel(() {});
                              setState(() {});
                              this.onUpdate();
                            }
                          },
                        ),
                      ],
                    ),
                  ),
                  value: this.data.mKernel,
                  onChanged: (value) async {
                    this.data.mKernel = StorageHelper.regularize(value);
                    setStateForPanel(() {});
                    setState(() {});
                    this.onUpdate();
                  },
                ),
              ),
            ],
          ),
          CustomSettingItem(
            icon: IconSymbols.data_object,
            label: 'Script',
            content: [
              Text(
                !StorageHelper.existFileSync(this.data.mScript) ? 'Invalid' : 'Available',
                overflow: TextOverflow.ellipsis,
                style: theme.textTheme.bodyMedium,
              ),
            ],
            onTap: null,
            panelBuilder: (context, setStateForPanel) => [
              ListTile(
                contentPadding: EdgeInsets.zero,
                title: CustomTextField(
                  keyboardType: TextInputType.text,
                  inputFormatters: const [],
                  decoration: InputDecoration(
                    contentPadding: const EdgeInsets.fromLTRB(12, 16, 12, 16),
                    filled: false,
                    border: const OutlineInputBorder(),
                    suffixIcon: CustomTextFieldSuffixRegion(
                      children: [
                        IconButton(
                          tooltip: 'Pick',
                          icon: const Icon(IconSymbols.open_in_new),
                          onPressed: () async {
                            var target = await StorageHelper.pickLoadFile(context, 'ModdingWorker.Script');
                            if (target != null) {
                              this.data.mScript = target;
                              setStateForPanel(() {});
                              setState(() {});
                              this.onUpdate();
                            }
                          },
                        ),
                      ],
                    ),
                  ),
                  value: this.data.mScript,
                  onChanged: (value) async {
                    this.data.mScript = StorageHelper.regularize(value);
                    setStateForPanel(() {});
                    setState(() {});
                    this.onUpdate();
                  },
                ),
              ),
            ],
          ),
          CustomSettingItem(
            icon: IconSymbols.format_list_bulleted,
            label: 'Argument',
            content: [
              Text(
                !this.data.mArgument.isNotEmpty ? 'Empty' : 'Defined',
                overflow: TextOverflow.ellipsis,
                style: theme.textTheme.bodyMedium,
              ),
            ],
            onTap: null,
            panelBuilder: (context, setStateForPanel) => [
              ListTile(
                contentPadding: EdgeInsets.zero,
                title: CustomTextField(
                  keyboardType: TextInputType.multiline,
                  inputFormatters: const [],
                  decoration: const InputDecoration(
                    contentPadding: EdgeInsets.fromLTRB(12, 16, 12, 16),
                    filled: false,
                    border: OutlineInputBorder(),
                    suffixIcon: CustomTextFieldSuffixRegion(
                      children: [
                      ],
                    ),
                  ),
                  value: ConvertHelper.makeStringListToStringWithLine(this.data.mArgument),
                  onChanged: (value) async {
                    this.data.mArgument = ConvertHelper.parseStringListFromStringWithLine(value);
                    setStateForPanel(() {});
                    setState(() {});
                    this.onUpdate();
                  },
                ),
              ),
            ],
          ),
          CustomSettingItem(
            icon: IconSymbols.touch_app,
            label: 'Immediate Launch',
            content: [
              Text(
                !this.data.mImmediateLaunch ? 'Disabled' : 'Enabled',
                overflow: TextOverflow.ellipsis,
                style: theme.textTheme.bodyMedium,
              ),
            ],
            onTap: null,
            panelBuilder: (context, setStateForPanel) => [
              ListTile(
                contentPadding: EdgeInsets.zero,
                leading: Switch(
                  value: this.data.mImmediateLaunch,
                  onChanged: (value) async {
                    this.data.mImmediateLaunch = value;
                    setStateForPanel(() {});
                    setState(() {});
                    this.onUpdate();
                  },
                ),
                title: const Text(
                  'Enable',
                  overflow: TextOverflow.ellipsis,
                ),
              ),
            ],
          ),
          CustomSettingItem(
            icon: IconSymbols.text_fields,
            label: 'Message Font',
            content: [
              Text(
                !this.data.mMessageFont.isNotEmpty ? 'Default' : 'Custom',
                overflow: TextOverflow.ellipsis,
                style: theme.textTheme.bodyMedium,
              ),
            ],
            onTap: null,
            panelBuilder: (context, setStateForPanel) => [
              ListTile(
                contentPadding: EdgeInsets.zero,
                title: CustomTextField(
                  keyboardType: TextInputType.multiline,
                  inputFormatters: const [],
                  decoration: InputDecoration(
                    contentPadding: const EdgeInsets.fromLTRB(12, 16, 12, 16),
                    filled: false,
                    border: const OutlineInputBorder(),
                    suffixIcon: CustomTextFieldSuffixRegion(
                      children: [
                        IconButton(
                          tooltip: 'Pick',
                          icon: const Icon(IconSymbols.open_in_new),
                          onPressed: () async {
                            var target = await StorageHelper.pickLoadFile(context, 'ModdingWorker.MessageFont');
                            if (target != null) {
                              this.data.mMessageFont = this.data.mMessageFont + [target];
                              setStateForPanel(() {});
                              setState(() {});
                              this.onUpdate();
                            }
                          },
                        ),
                      ],
                    ),
                  ),
                  value: ConvertHelper.makeStringListToStringWithLine(this.data.mMessageFont),
                  onChanged: (value) async {
                    this.data.mMessageFont = ConvertHelper.parseStringListFromStringWithLine(value);
                    setStateForPanel(() {});
                    setState(() {});
                    this.onUpdate();
                  },
                ),
              ),
            ],
          ),
          const SizedBox(height: 8),
        ],
      ),
    );
  }

}
