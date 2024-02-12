import '/common.dart';
import '/page/console/page.dart';
import '/page/setting/page.dart';
import 'package:flutter/material.dart';

// ----------------

class HomePage extends StatefulWidget {

  const HomePage({
    super.key,
  });

  @override
  createState() => _HomePageState();

  // ----------------

}

class _HomePageState extends State<HomePage> {

  List<Widget> _page = [const ConsolePage(), const SettingPage()];
  Integer      _pageIndex = 0;

  Void _pageIndexSet(
    Integer value,
  ) {
    this._pageIndex = value;
    this.setState(() {});
    return;
  }

  // ----------------

  @override
  build(context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text(kApplicationName),
      ),
      body: IndexedStack(
        index: this._pageIndex,
        children: this._page,
      ),
      bottomNavigationBar: NavigationBar(
        labelBehavior: NavigationDestinationLabelBehavior.alwaysShow,
        selectedIndex: this._pageIndex,
        onDestinationSelected: this._pageIndexSet,
        destinations: const [
          NavigationDestination(
            icon: Icon(Icons.terminal),
            label: 'Console',
          ),
          NavigationDestination(
            icon: Icon(Icons.settings),
            label: 'Setting',
          ),
        ],
      ),
    );
  }

}
