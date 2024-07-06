import '/common.dart';
import '/view/home/common.dart';
import '/view/animation_viewer/setting.dart';
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
            enabled: true,
            icon: IconSymbols.ads_click,
            label: 'Immediate Select',
            content: [
              Text(
                !this.data.mImmediateSelect ? 'Disabled' : 'Enabled',
                overflow: TextOverflow.ellipsis,
                style: theme.textTheme.bodyMedium,
              ),
            ],
            onTap: null,
            panelBuilder: (context, setStateForPanel) => [
              ListTile(
                contentPadding: EdgeInsets.zero,
                leading: Switch(
                  value: this.data.mImmediateSelect,
                  onChanged: (value) async {
                    this.data.mImmediateSelect = value;
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
            enabled: true,
            icon: IconSymbols.line_end_arrow_notch,
            label: 'Automatic Play',
            content: [
              Text(
                !this.data.mAutomaticPlay ? 'Disabled' : 'Enabled',
                overflow: TextOverflow.ellipsis,
                style: theme.textTheme.bodyMedium,
              ),
            ],
            onTap: null,
            panelBuilder: (context, setStateForPanel) => [
              ListTile(
                contentPadding: EdgeInsets.zero,
                leading: Switch(
                  value: this.data.mAutomaticPlay,
                  onChanged: (value) async {
                    this.data.mAutomaticPlay = value;
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
            enabled: true,
            icon: IconSymbols.repeat,
            label: 'Repeat Play',
            content: [
              Text(
                !this.data.mRepeatPlay ? 'Disabled' : 'Enabled',
                overflow: TextOverflow.ellipsis,
                style: theme.textTheme.bodyMedium,
              ),
            ],
            onTap: null,
            panelBuilder: (context, setStateForPanel) => [
              ListTile(
                contentPadding: EdgeInsets.zero,
                leading: Switch(
                  value: this.data.mRepeatPlay,
                  onChanged: (value) async {
                    this.data.mRepeatPlay = value;
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
            enabled: true,
            icon: IconSymbols.keep,
            label: 'Remain Frame Rate',
            content: [
              Text(
                !this.data.mRemainFrameRate ? 'Disabled' : 'Enabled',
                overflow: TextOverflow.ellipsis,
                style: theme.textTheme.bodyMedium,
              ),
            ],
            onTap: null,
            panelBuilder: (context, setStateForPanel) => [
              ListTile(
                contentPadding: EdgeInsets.zero,
                leading: Switch(
                  value: this.data.mRemainFrameRate,
                  onChanged: (value) async {
                    this.data.mRemainFrameRate = value;
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
            enabled: true,
            icon: IconSymbols.frame_source,
            label: 'Show Sprite Boundary',
            content: [
              Text(
                !this.data.mShowSpriteBoundary ? 'Disabled' : 'Enabled',
                overflow: TextOverflow.ellipsis,
                style: theme.textTheme.bodyMedium,
              ),
            ],
            onTap: null,
            panelBuilder: (context, setStateForPanel) => [
              ListTile(
                contentPadding: EdgeInsets.zero,
                leading: Switch(
                  value: this.data.mShowSpriteBoundary,
                  onChanged: (value) async {
                    this.data.mShowSpriteBoundary = value;
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
            enabled: true,
            icon: IconSymbols.filter_alt,
            label: 'Image Filter Rule',
            content: [
              Text(
                this.data.mImageFilterRule.isEmpty ? 'Empty' : 'Defined',
                overflow: TextOverflow.ellipsis,
                style: theme.textTheme.bodyMedium,
              ),
            ],
            onTap: null,
            panelBuilder: (context, setStateForPanel) => [
              ListTile(
                contentPadding: EdgeInsets.zero,
                title: CustomTextFieldWithFocus(
                  keyboardType: TextInputType.text,
                  inputFormatters: const [],
                  decoration: const InputDecoration(
                    contentPadding: EdgeInsets.fromLTRB(12, 16, 12, 16),
                    filled: false,
                    border: OutlineInputBorder(),
                    suffixIcon: CustomTextFieldSuffixWidget(
                      children: [
                      ],
                    ),
                  ),
                  value: this.data.mImageFilterRule,
                  onChanged: (value) async {
                    this.data.mImageFilterRule = value;
                    setStateForPanel(() {});
                    setState(() {});
                    this.onUpdate();
                  },
                ),
              ),
            ],
          ),
          CustomSettingItem(
            enabled: true,
            icon: IconSymbols.filter_alt,
            label: 'Sprite Filter Rule',
            content: [
              Text(
                this.data.mSpriteFilterRule.isEmpty ? 'Empty' : 'Defined',
                overflow: TextOverflow.ellipsis,
                style: theme.textTheme.bodyMedium,
              ),
            ],
            onTap: null,
            panelBuilder: (context, setStateForPanel) => [
              ListTile(
                contentPadding: EdgeInsets.zero,
                title: CustomTextFieldWithFocus(
                  keyboardType: TextInputType.text,
                  inputFormatters: const [],
                  decoration: const InputDecoration(
                    contentPadding: EdgeInsets.fromLTRB(12, 16, 12, 16),
                    filled: false,
                    border: OutlineInputBorder(),
                    suffixIcon: CustomTextFieldSuffixWidget(
                      children: [
                      ],
                    ),
                  ),
                  value: this.data.mSpriteFilterRule,
                  onChanged: (value) async {
                    this.data.mSpriteFilterRule = value;
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
