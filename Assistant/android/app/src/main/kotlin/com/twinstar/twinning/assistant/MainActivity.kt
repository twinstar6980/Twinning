package com.twinstar.twinning.assistant

import android.content.Intent
import io.flutter.embedding.android.FlutterActivity
import io.flutter.embedding.engine.FlutterEngine
import io.flutter.plugins.GeneratedPluginRegistrant

class MainActivity : FlutterActivity() {

	private val customMethodChannel: CustomMethodChannel = CustomMethodChannel(this)

	public override fun configureFlutterEngine(
		flutterEngine: FlutterEngine,
	): Unit {
		super.configureFlutterEngine(flutterEngine)
		GeneratedPluginRegistrant.registerWith(flutterEngine)
		this.customMethodChannel.register_onConfigureFlutterEngine(flutterEngine)
		return
	}

	public override fun onActivityResult(
		requestCode: Int,
		resultCode: Int,
		data: Intent?,
	): Unit {
		super.onActivityResult(requestCode, resultCode, data)
		this.customMethodChannel.register_onActivityResult(requestCode, resultCode, data)
		return
	}

	public override fun onRequestPermissionsResult(
		requestCode: Int,
		permissions: Array<out String>,
		grantResults: IntArray,
	): Unit {
		super.onRequestPermissionsResult(requestCode, permissions, grantResults)
		this.customMethodChannel.register_onRequestPermissionsResult(requestCode, permissions, grantResults)
		return
	}

}
