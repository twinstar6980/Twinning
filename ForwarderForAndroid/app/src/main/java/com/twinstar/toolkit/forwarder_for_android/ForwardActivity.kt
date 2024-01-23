package com.twinstar.toolkit.forwarder_for_android

import android.app.Activity
import android.content.ClipData
import android.content.Intent
import android.net.Uri
import android.os.Bundle
import android.widget.Toast

class ForwardActivity : Activity() {

	override fun onCreate(savedInstanceState: Bundle?) {
		super.onCreate(savedInstanceState)
		try {
			val uri = mutableListOf<Uri>()
			if (this.intent.action == Intent.ACTION_SEND) {
				uri.add(this.intent.getParcelableExtra(Intent.EXTRA_STREAM)!!)
			}
			if (this.intent.action == Intent.ACTION_SEND_MULTIPLE) {
				uri.addAll(this.intent.getParcelableArrayListExtra(Intent.EXTRA_STREAM)!!)
			}
			val command = listOf("-additional_argument", *uri.map { item -> item.toString() }.toTypedArray())
			val intent = Intent().also { intent ->
				intent.setAction(Intent.ACTION_VIEW)
				intent.setData(Uri.parse("twinstar.toolkit.shell-gui:/run?${command.joinToString("&") { item -> "command=${Uri.encode(item)}" }}"))
				intent.setClipData(ClipData.newPlainText("", "").also { clip ->
					for (item in uri) {
						clip.addItem(ClipData.Item(item))
					}
				})
				intent.addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION)
			}
			this.startActivity(intent)
		}
		catch (e: Exception) {
			Toast.makeText(this, e.toString(), Toast.LENGTH_LONG).show()
		}
		this.finish()
		return
	}

}
