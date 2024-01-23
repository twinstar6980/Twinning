package com.twinstar.toolkit.shell_gui

import android.content.Intent
import android.os.Bundle
import io.flutter.embedding.android.FlutterActivity
import io.flutter.embedding.engine.FlutterEngine
import io.flutter.plugins.GeneratedPluginRegistrant

class MainActivity: FlutterActivity() {

	override fun onCreate(savedInstanceState: Bundle?) {
		super.onCreate(savedInstanceState)
		this.customMethodChannel.register_onCreate(savedInstanceState)
		return
	}

	override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
		super.onActivityResult(requestCode, resultCode, data)
		this.customMethodChannel.register_onActivityResult(requestCode, resultCode, data)
		return
	}

	override fun onRequestPermissionsResult(requestCode: Int, permissions: Array<out String>, grantResults: IntArray) {
		super.onRequestPermissionsResult(requestCode, permissions, grantResults)
		this.customMethodChannel.register_onRequestPermissionsResult(requestCode, permissions, grantResults)
		return
	}

	override fun configureFlutterEngine(flutterEngine: FlutterEngine) {
		GeneratedPluginRegistrant.registerWith(flutterEngine)
		this.customMethodChannel.register_onConfigureFlutterEngine(flutterEngine)
		return
	}

	private val customMethodChannel = CustomMethodChannel(this)

}
