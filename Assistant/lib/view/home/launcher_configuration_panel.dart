import '/common.dart';
import '/module.dart';
import '/utility/json_type.dart';
import '/utility/json_helper.dart';
import '/widget/export.dart';

// ----------------

class LauncherConfigurationPanel extends StatelessWidget {

  const LauncherConfigurationPanel({
    super.key,
    required this.data,
    required this.onUpdate,
  });

  // ----------------

  final ModuleLauncherConfiguration data;
  final Void Function()             onUpdate;

  // ----------------

  @override
  build(context) {
    return StatefulBuilder(
      builder: (context, setState) => FlexContainer.vertical([
        Gap.vertical(8),
        SettingListItem(
          icon: IconSet.text_fields,
          label: 'Title',
          comment: [
            StyledText.inherit(this.data.title).withFlexExpanded(),
          ],
          onPressed: null,
          panelBuilder: (context, setStateForPanel) => [
            StyledListTile.standardTight(
              content: StyledInput.outlined(
                type: .text,
                format: null,
                hint: null,
                prefix: null,
                suffix: null,
                value: this.data.title,
                onChanged: (context, value) async {
                  this.data.title = value;
                  await refreshState(setStateForPanel);
                  await refreshState(setState);
                  this.onUpdate();
                },
              ),
            ),
          ],
        ),
        SettingListItem(
          icon: IconSet.label,
          label: 'Type',
          comment: [
            StyledText.inherit(ModuleHelper.query(this.data.type).name).withFlexExpanded(),
          ],
          onPressed: null,
          panelBuilder: (context, setStateForPanel) => [
            ...ModuleType.values.map((item) => StyledListTile.standardTight(
              leading: StyledRadio.standard(
                value: this.data.type == item,
                onChanged: (context) async {
                  this.data.type = item;
                  this.data.option = ModuleHelper.query(this.data.type).generateDefaultOption();
                  await refreshState(setStateForPanel);
                  await refreshState(setState);
                  this.onUpdate();
                },
              ),
              content: StyledText.inherit(ModuleHelper.query(item).name),
            )),
          ],
        ),
        SettingListItem(
          icon: IconSet.format_list_bulleted,
          label: 'Option',
          comment: [
            StyledText.inherit('${ModuleHelper.query(this.data.type).makeOption(this.data.option).length}').withFlexExpanded(),
          ],
          onPressed: null,
          panelBuilder: (context, setStateForPanel) => [
            StyledListTile.standardTight(
              content: StyledInput.outlined(
                type: .multiline,
                format: null,
                hint: null,
                prefix: null,
                suffix: null,
                value: JsonHelper.encodeText(ModuleHelper.query(this.data.type).makeOption(this.data.option)),
                onChanged: (context, value) async {
                  try {
                    this.data.option = ModuleHelper.query(this.data.type).parseOption(JsonHelper.decodeText(value).jsonObject());
                  }
                  catch (e) {
                    // ignored
                  }
                  await refreshState(setStateForPanel);
                  await refreshState(setState);
                  this.onUpdate();
                },
              ),
            ),
          ],
        ),
        Gap.vertical(8),
      ]),
    );
  }

}
