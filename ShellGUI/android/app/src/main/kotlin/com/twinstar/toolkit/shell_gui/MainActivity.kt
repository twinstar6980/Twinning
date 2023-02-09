package com.twinstar.toolkit.shell_gui

import android.annotation.SuppressLint
import android.content.ContentResolver
import android.content.ContentUris
import android.content.Intent
import android.net.Uri
import android.os.Build
import android.os.Bundle
import android.os.Environment
import android.os.Parcelable
import android.provider.DocumentsContract
import android.provider.MediaStore
import androidx.annotation.RequiresApi
import io.flutter.embedding.android.FlutterActivity
import io.flutter.embedding.engine.FlutterEngine
import io.flutter.plugin.common.MethodChannel
import io.flutter.plugins.GeneratedPluginRegistrant

class MainActivity: FlutterActivity() {

	override fun onCreate(savedInstanceState: Bundle?) {
		super.onCreate(savedInstanceState)
		val intent = this.intent
		intent.addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION)
		if (intent.action == Intent.ACTION_SEND && intent.type != null) {
			var uri = intent.getParcelableExtra<Parcelable>(Intent.EXTRA_STREAM)!! as Uri
			this.command = listOf(uri).map { parseUri(it) ?: "?" }
		}
		if (intent.action == Intent.ACTION_SEND_MULTIPLE && intent.type != null) {
			var uriList = intent.getParcelableArrayListExtra<Parcelable>(Intent.EXTRA_STREAM)!! as List<Uri>
			this.command = uriList.map { parseUri(it) ?: "?" }
		}
	}

	override fun configureFlutterEngine(flutterEngine: FlutterEngine) {
		GeneratedPluginRegistrant.registerWith(flutterEngine)
		MethodChannel(flutterEngine.dartExecutor.binaryMessenger, "com.twinstar.toolkit.shell_gui.flutter_method_channel")
			.setMethodCallHandler { call, result ->
				when (call.method) {
					"getCommand" -> {
						result.success(this.command)
						this.command = null
					}
					else -> throw Exception("invalid method")
				}
			}
	}

	private var command: List<String>? = null

	@RequiresApi(Build.VERSION_CODES.KITKAT)
	private fun parseUri(uri: Uri): String? {
		if (uri.scheme == ContentResolver.SCHEME_FILE) {
			return uri.path
		}
		if (uri.scheme == ContentResolver.SCHEME_CONTENT) {
			when (uri.authority) {
				"media" -> {
					return getDataColumn(uri)!!
				}
				else -> {
					return uri.path
				}
			}
		}
		return null
	}

	@SuppressLint("Range")
	private fun getDataColumn(uri: Uri): String? {
		val cursor = this.contentResolver.query(uri, listOf(MediaStore.MediaColumns.DATA).toTypedArray(), null, null, null, null)
		if (cursor == null) {
			return null;
		}
		if (!cursor.moveToFirst()) {
			return null;
		}
		if (cursor.columnCount != 1) {
			return null
		}
		val path = cursor.getString(0)
		return path
	}

}
