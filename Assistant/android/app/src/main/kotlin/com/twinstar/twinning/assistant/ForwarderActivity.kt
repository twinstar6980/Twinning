package com.twinstar.twinning.assistant

import android.app.Activity
import android.content.Intent
import android.net.Uri
import android.os.Bundle
import android.os.Environment
import android.widget.Toast

class ForwarderActivity : Activity() {

  // region variable

  // endregion

  // region implement Activity

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

  private fun queryApplicationIdentifier(
  ): String {
    return this.packageName
  }

  // ----------------

  private fun resolveContentUri(
    uri: Uri,
  ): String {
    var result = null as String?
    check(uri.scheme == "content")
    val provider = uri.authority
    var path = Uri.decode(uri.path)
    when (provider) {
      // AOSP DocumentsUI
      "com.android.externalstorage.documents" -> {
        // /document/primary:<path-relative-external-storage>
        if (path.startsWith("/document/primary:")) {
          result = "${Environment.getExternalStorageDirectory()}/${path.substring("/document/primary:".length)}"
        }
        // /tree/primary:<path-relative-external-storage>
        if (path.startsWith("/tree/primary:")) {
          result = "${Environment.getExternalStorageDirectory()}/${path.substring("/tree/primary:".length)}"
        }
      }
      // Material Files
      "me.zhanghai.android.files.file_provider" -> {
        path = Uri.decode(path)
        // /file://<path-absolute>
        if (path.startsWith("/file://")) {
          result = Uri.decode(Uri.parse(path.substring("/".length)).path)
        }
      }
      // Root Explorer
      "com.speedsoftware.rootexplorer.fileprovider" -> {
        // /root/<path-relative-root>
        if (path.startsWith("/root/")) {
          result = path.substring("/root".length)
        }
      }
      // Solid Explorer
      "pl.solidexplorer2.files" -> {
        result = path
      }
      // MT Manager
      "bin.mt.plus.fp" -> {
        result = path
      }
      // NMM
      "in.mfile.files" -> {
        result = path
      }
    }
    if (result == null) {
      throw UnsupportedOperationException()
    }
    return result
  }

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
  ): Unit {
    this.startActivity(Intent().also { intent ->
      intent.setAction(Intent.ACTION_VIEW)
      intent.setData(link)
      intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK)
      intent.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP)
    })
    return
  }

  // ----------------

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
    command.add("-forward")
    command.addAll(resource.map({ item -> this.resolveContentUri(item) }))
    val link = Uri.parse("${this.queryApplicationIdentifier()}:/application?${command.joinToString("&", transform = { item -> "command=${this.encodePercentString(item)}" })}")
    this.openLink(link)
    return
  }

  // endregion

}
