package com.twinstar.twinning.assistant

import android.app.ComponentCaller
import android.content.Intent
import android.os.Bundle
import io.flutter.embedding.android.FlutterActivity
import io.flutter.embedding.engine.FlutterEngine
import io.flutter.plugins.GeneratedPluginRegistrant

class MainActivity : FlutterActivity() {

  public override fun configureFlutterEngine(
    flutterEngine: FlutterEngine,
  ): Unit {
    super.configureFlutterEngine(flutterEngine)
    GeneratedPluginRegistrant.registerWith(flutterEngine)
    PlatformIntegrationManager.instance().inject_MainActivity_configureFlutterEngine(this, flutterEngine)
    return
  }

  public override fun onCreate(
    savedInstanceState: Bundle?,
  ): Unit {
    super.onCreate(savedInstanceState)
    PlatformIntegrationManager.instance().inject_MainActivity_onCreate(this, savedInstanceState)
    return
  }

  public override fun onNewIntent(
    intent: Intent,
    caller: ComponentCaller,
  ) {
    super.onNewIntent(intent, caller)
    PlatformIntegrationManager.instance().inject_MainActivity_onNewIntent(this, intent, caller)
    return
  }

  public override fun onActivityResult(
    requestCode: Int,
    resultCode: Int,
    data: Intent?,
  ): Unit {
    super.onActivityResult(requestCode, resultCode, data)
    PlatformIntegrationManager.instance().inject_MainActivity_onActivityResult(this, requestCode, resultCode, data)
    return
  }

}
