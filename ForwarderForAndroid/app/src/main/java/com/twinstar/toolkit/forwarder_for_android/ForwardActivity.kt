package com.twinstar.toolkit.forwarder_for_android

import android.app.Activity
import android.content.ComponentName
import android.content.Intent
import android.net.Uri
import android.os.Bundle

class ForwardActivity : Activity() {
	
	override fun onCreate(savedInstanceState: Bundle?) {
		super.onCreate(savedInstanceState)
		val intent = this.intent
		intent.addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION)
		val command = mutableListOf<String>()
		command.add("-additional_argument")
		if (intent.action == Intent.ACTION_SEND && intent.type != null) {
			val uri = intent.getParcelableExtra<Uri>(Intent.EXTRA_STREAM)!!
			command.add(uri.toString())
		}
		if (intent.action == Intent.ACTION_SEND_MULTIPLE && intent.type != null) {
			val uri = intent.getParcelableArrayListExtra<Uri>(Intent.EXTRA_STREAM)!!
			command.addAll(uri.map { it.toString() })
		}
		val forwardIntent = Intent().also {
			it.setComponent(ComponentName(TARGET_PACKAGE_NAME, "${TARGET_PACKAGE_NAME}.MainActivity"))
			it.setAction("${TARGET_PACKAGE_NAME}.action.LAUNCH")
			it.putExtra("command", command.toTypedArray())
		}
		this.startActivity(forwardIntent)
		this.finish()
		return
	}
	
	companion object {
		
		private const val TARGET_PACKAGE_NAME: String = "com.twinstar.toolkit.shell_gui"
		
	}
	
}
