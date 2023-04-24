package com.twinstar.toolkit.forwarder_for_android

import android.annotation.SuppressLint
import android.app.Activity
import android.content.ComponentName
import android.content.ContentResolver
import android.content.Intent
import android.net.Uri
import android.os.Bundle
import android.os.Parcelable
import android.provider.MediaStore

class ForwardActivity : Activity() {
	
	override fun onCreate(savedInstanceState: Bundle?) {
		super.onCreate(savedInstanceState)
		val intent = this.intent
		intent.addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION)
		var command = mutableListOf<String>("-additional")
		if (intent.action == Intent.ACTION_SEND && intent.type != null) {
			var uri = intent.getParcelableExtra<Parcelable>(Intent.EXTRA_STREAM)!! as Uri
			command.addAll(listOf(uri).map { parseUri(it) ?: "?" })
		}
		if (intent.action == Intent.ACTION_SEND_MULTIPLE && intent.type != null) {
			var uriList = intent.getParcelableArrayListExtra<Parcelable>(Intent.EXTRA_STREAM)!! as List<Uri>
			command.addAll(uriList.map { parseUri(it) ?: "?" })
		}
		val forwardIntent = Intent().apply {
			component = ComponentName("${externalPackageNameOfShellGUI}", "${externalPackageNameOfShellGUI}.MainActivity")
			action = "${externalPackageNameOfShellGUI}.action.LAUNCH"
			putExtra("command", command.toTypedArray())
		}
		this.startActivity(forwardIntent)
		this.finish()
	}
	
	private val externalPackageNameOfShellGUI: String = "com.twinstar.toolkit.shell_gui"
	
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
