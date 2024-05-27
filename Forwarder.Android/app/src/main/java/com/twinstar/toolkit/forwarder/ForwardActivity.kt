package com.twinstar.toolkit.forwarder

import android.app.Activity
import android.content.ClipData
import android.content.Intent
import android.net.Uri
import android.os.Bundle
import android.widget.Toast

class ForwardActivity : Activity() {

	// region implement - Activity

	protected override fun onCreate(
		savedInstanceState: Bundle?,
	): Unit {
		super.onCreate(savedInstanceState)
		try {
			val resource = mutableListOf<Uri>()
			if (this.intent.action == Intent.ACTION_SEND) {
				resource.add(this.intent.getParcelableExtra(Intent.EXTRA_STREAM)!!)
			}
			if (this.intent.action == Intent.ACTION_SEND_MULTIPLE) {
				resource.addAll(this.intent.getParcelableArrayListExtra(Intent.EXTRA_STREAM)!!)
			}
			this.forwardResource(resource)
		}
		catch (e: Exception) {
			this.showException(e)
		}
		this.finish()
		return
	}

	// endregion

	// region utility

	private fun showException(
		exception: Exception,
	): Unit {
		Toast.makeText(this, exception.toString(), Toast.LENGTH_LONG).show()
		return
	}

	private fun forwardResource(
		resource: List<Uri>,
	): Unit {
		val command = mutableListOf<String>()
		command.add("-initial_tab")
		command.add("Resource Forwarder")
		command.add("resource_forwarder")
		command.add("-input")
		command.addAll(resource.map() { item -> item.toString() })
		val link = "twinstar.toolkit.assistant:/run?${command.joinToString("&") { item -> "command=${Uri.encode(item)}" }}"
		this.startActivity(Intent().also { intent ->
			intent.setAction(Intent.ACTION_VIEW)
			intent.setData(Uri.parse(link))
			intent.setClipData(ClipData.newPlainText("", "").also { clip ->
				resource.forEach() { item -> clip.addItem(ClipData.Item(item)) }
			})
			intent.addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION)
			intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK)
			intent.addFlags(Intent.FLAG_ACTIVITY_MULTIPLE_TASK)
		})
		return
	}

	// endregion

}
