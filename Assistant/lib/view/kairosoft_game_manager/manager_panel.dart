import 'package:assistant/view/kairosoft_game_manager/game_card.dart';

import '/common.dart';
import '/module.dart';
import '/utility/convert_helper.dart';
import '/utility/kairosoft_game_helper.dart';
import '/widget/export.dart';
import 'package:flutter/widgets.dart';

// ----------------

class ManagerPanel extends StatelessWidget {

  const ManagerPanel({
    super.key,
    required this.information,
  });

  // ----------------

  final List<GameInformation> information;

  // ----------------

  @override
  build(context) {
    return StatefulBuilder(
      builder: (context, setState) => ListContainer.of(
        itemCount: this.information.length,
        itemBuilder: (context, index) => GameCard(
          information: this.information[index],
          onX: () {}
        ),
      ),
    );
  }

}
