package com.twinstar.twinning.assistant

import android.app.Activity
import android.content.ClipData
import android.content.Intent
import android.net.Uri
import android.os.Bundle
import android.widget.Toast

class ForwarderActivity : Activity() {

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

	private fun encodePercentString(
		source: String,
	): String {
		val data = source.encodeToByteArray()
		val destination = StringBuilder(data.size * 3)
		for (character in data) {
			if (('0'.code <= character && character <= '9'.code) ||
				('a'.code <= character && character <= 'z'.code) ||
				('A'.code <= character && character <= 'Z'.code) ||
				(character == '-'.code.toByte()) ||
				(character == '.'.code.toByte()) ||
				(character == '_'.code.toByte()) ||
				(character == '~'.code.toByte())) {
				destination.append(Char(character.toInt()))
			}
			else {
				destination.append('%')
				destination.append((character / 0x10).toString(16))
				destination.append((character % 0x10).toString(16))
			}
		}
		return destination.toString()
	}

	private fun openLink(
		link: Uri,
		attachment: List<Uri>,
	): Unit {
		this.startActivity(Intent().also { intent ->
			intent.setAction(Intent.ACTION_VIEW)
			intent.setData(link)
			intent.setClipData(ClipData.newPlainText("", "").also { clip ->
				attachment.forEach() { item -> clip.addItem(ClipData.Item(item)) }
			})
			intent.addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION)
			intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK)
			intent.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP)
		})
		return
	}

	private fun forwardResource(
		resource: List<Uri>,
	): Unit {
		val command = mutableListOf<String>()
		command.add("-forward")
		command.addAll(resource.map() { item -> item.toString() })
		val link = Uri.parse("twinstar.twinning.assistant:/application?${command.joinToString("&") { item -> "command=${this.encodePercentString(item)}" }}")
		this.openLink(link, resource)
		return
	}

	private fun showException(
		exception: Exception,
	): Unit {
		Toast.makeText(this, exception.toString(), Toast.LENGTH_LONG).show()
		return
	}

	// endregion

}
