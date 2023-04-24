package com.twinstar.toolkit.shell_gui

import android.content.Intent
import android.os.Bundle
import android.os.Environment
import io.flutter.embedding.android.FlutterActivity
import io.flutter.embedding.engine.FlutterEngine
import io.flutter.plugin.common.MethodChannel
import io.flutter.plugins.GeneratedPluginRegistrant

class MainActivity: FlutterActivity() {

	override fun onCreate(savedInstanceState: Bundle?) {
		super.onCreate(savedInstanceState)
		this.command = listOf()
		if (this.intent.action == "${this.packageName}.action.LAUNCH") {
			this.command = this.intent.extras!!.getStringArray("command")!!.map { it -> it }
		}
	}

	override fun configureFlutterEngine(flutterEngine: FlutterEngine) {
		GeneratedPluginRegistrant.registerWith(flutterEngine)
		MethodChannel(flutterEngine.dartExecutor.binaryMessenger, "${this.packageName}.flutter_method_channel")
			.setMethodCallHandler { call, result ->
				when (call.method) {
					"getCommand" -> {
						result.success(this.command)
					}
					else -> throw Exception("invalid method")
				}
			}
	}

	private lateinit var command: List<String>

}
