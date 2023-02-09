// ignore_for_file: unused_import

import '/common.dart';
import 'dart:io';
import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import '/page/console/page.dart';
import '/page/setting/page.dart';
import '/platform_method.dart';
import '/command.dart';

// ----------------

class HomePage extends StatefulWidget {

  const HomePage({
    super.key,
  });

  @override
  State<HomePage> createState() => _HomePageState();

  // ----------------

}

class _HomePageState extends State<HomePage> {

  final List<Widget> _page = [const ConsolePage(), const SettingPage()];

  Integer _pageIndex = 0;
  
  Void
  _pageIndexSet(
    Integer index,
  ) {
    this._pageIndex = index;
    this.setState(() {});
  }

  // ----------------

  @override
  void initState() {
    super.initState();
    if (Platform.isAndroid) {
      Future.microtask(() async {
        var methodArgument = <String, dynamic>{};
        var methodResult = await gPlatformMethod!.invokeMethod('getCommand', methodArgument) as List<Object?>?;
        if (methodResult != null) {
          var commandFromAndroid = methodResult.map((e) => e as String).toList();
          var command = Provider.of<CommandProvider>(this.context);
          command.set(['', ...commandFromAndroid]);
        }
      });
    }
    return;
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text(kApplicationName),
      ),
      body: IndexedStack(
        index: this._pageIndex,
        children: this._page,
      ),
      bottomNavigationBar: NavigationBar(
        selectedIndex: this._pageIndex,
        labelBehavior: NavigationDestinationLabelBehavior.alwaysShow,
        onDestinationSelected: this._pageIndexSet,
        destinations: const [
          NavigationDestination(
            icon: Icon(Icons.terminal),
            label: '控制台',
          ),
          NavigationDestination(
            icon: Icon(Icons.settings),
            label: '设置',
          ),
        ],
      ),
    );
  }

}
