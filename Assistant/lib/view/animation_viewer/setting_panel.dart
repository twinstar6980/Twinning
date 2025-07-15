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
          SizedBox(height: 8),
          CustomSettingItem(
            icon: IconSymbols.ads_click,
            label: 'Immediate Select',
            content: [
              Text(
                !this.data.immediateSelect ? 'Disabled' : 'Enabled',
                overflow: TextOverflow.ellipsis,
                style: theme.textTheme.bodyMedium,
              ),
            ],
            onTap: null,
            panelBuilder: (context, setStateForPanel) => [
              ListTile(
                contentPadding: EdgeInsets.zero,
                leading: Switch(
                  value: this.data.immediateSelect,
                  onChanged: (value) async {
                    this.data.immediateSelect = value;
                    await refreshState(setStateForPanel);
                    await refreshState(setState);
                    this.onUpdate();
                  },
                ),
                title: Text(
                  'Enable',
                  overflow: TextOverflow.ellipsis,
                ),
              ),
            ],
          ),
          CustomSettingItem(
            icon: IconSymbols.autoplay,
            label: 'Automatic Play',
            content: [
              Text(
                !this.data.automaticPlay ? 'Disabled' : 'Enabled',
                overflow: TextOverflow.ellipsis,
                style: theme.textTheme.bodyMedium,
              ),
            ],
            onTap: null,
            panelBuilder: (context, setStateForPanel) => [
              ListTile(
                contentPadding: EdgeInsets.zero,
                leading: Switch(
                  value: this.data.automaticPlay,
                  onChanged: (value) async {
                    this.data.automaticPlay = value;
                    await refreshState(setStateForPanel);
                    await refreshState(setState);
                    this.onUpdate();
                  },
                ),
                title: Text(
                  'Enable',
                  overflow: TextOverflow.ellipsis,
                ),
              ),
            ],
          ),
          CustomSettingItem(
            icon: IconSymbols.repeat,
            label: 'Repeat Play',
            content: [
              Text(
                !this.data.repeatPlay ? 'Disabled' : 'Enabled',
                overflow: TextOverflow.ellipsis,
                style: theme.textTheme.bodyMedium,
              ),
            ],
            onTap: null,
            panelBuilder: (context, setStateForPanel) => [
              ListTile(
                contentPadding: EdgeInsets.zero,
                leading: Switch(
                  value: this.data.repeatPlay,
                  onChanged: (value) async {
                    this.data.repeatPlay = value;
                    await refreshState(setStateForPanel);
                    await refreshState(setState);
                    this.onUpdate();
                  },
                ),
                title: Text(
                  'Enable',
                  overflow: TextOverflow.ellipsis,
                ),
              ),
            ],
          ),
          CustomSettingItem(
            icon: IconSymbols.lock_reset,
            label: 'Keep Speed',
            content: [
              Text(
                !this.data.keepSpeed ? 'Disabled' : 'Enabled',
                overflow: TextOverflow.ellipsis,
                style: theme.textTheme.bodyMedium,
              ),
            ],
            onTap: null,
            panelBuilder: (context, setStateForPanel) => [
              ListTile(
                contentPadding: EdgeInsets.zero,
                leading: Switch(
                  value: this.data.keepSpeed,
                  onChanged: (value) async {
                    this.data.keepSpeed = value;
                    await refreshState(setStateForPanel);
                    await refreshState(setState);
                    this.onUpdate();
                  },
                ),
                title: Text(
                  'Enable',
                  overflow: TextOverflow.ellipsis,
                ),
              ),
            ],
          ),
          CustomSettingItem(
            icon: IconSymbols.frame_source,
            label: 'Show Boundary',
            content: [
              Text(
                !this.data.showBoundary ? 'Disabled' : 'Enabled',
                overflow: TextOverflow.ellipsis,
                style: theme.textTheme.bodyMedium,
              ),
            ],
            onTap: null,
            panelBuilder: (context, setStateForPanel) => [
              ListTile(
                contentPadding: EdgeInsets.zero,
                leading: Switch(
                  value: this.data.showBoundary,
                  onChanged: (value) async {
                    this.data.showBoundary = value;
                    await refreshState(setStateForPanel);
                    await refreshState(setState);
                    this.onUpdate();
                  },
                ),
                title: Text(
                  'Enable',
                  overflow: TextOverflow.ellipsis,
                ),
              ),
            ],
          ),
          SizedBox(height: 8),
        ],
      ),
    );
  }

}
