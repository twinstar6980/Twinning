package com.twinstar.twinning.assistant

import android.content.Intent
import android.os.Bundle
import io.flutter.embedding.android.FlutterActivity
import io.flutter.embedding.engine.FlutterEngine
import io.flutter.plugins.GeneratedPluginRegistrant

class MainActivity : FlutterActivity() {

  private val customMethodChannel: CustomMethodChannel = CustomMethodChannel(this)

  protected override fun onCreate(
    savedInstanceState: Bundle?,
  ): Unit {
    super.onCreate(savedInstanceState)
    this.customMethodChannel.register_onCreate(savedInstanceState)
    return
  }

  protected override fun onActivityResult(
    requestCode: Int,
    resultCode: Int,
    data: Intent?,
  ): Unit {
    super.onActivityResult(requestCode, resultCode, data)
    this.customMethodChannel.register_onActivityResult(requestCode, resultCode, data)
    return
  }

  public override fun configureFlutterEngine(
    flutterEngine: FlutterEngine,
  ): Unit {
    super.configureFlutterEngine(flutterEngine)
    GeneratedPluginRegistrant.registerWith(flutterEngine)
    this.customMethodChannel.register_configureFlutterEngine(flutterEngine)
    return
  }

}
