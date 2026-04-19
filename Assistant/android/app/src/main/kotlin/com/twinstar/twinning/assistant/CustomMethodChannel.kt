package com.twinstar.twinning.assistant

import android.content.ComponentName
import android.content.Intent
import android.net.Uri
import android.os.Environment
import android.provider.DocumentsContract
import android.provider.Settings
import androidx.core.net.toUri
import androidx.documentfile.provider.DocumentFile
import io.flutter.embedding.android.FlutterActivity
import io.flutter.embedding.engine.FlutterEngine
import io.flutter.plugin.common.MethodCall
import io.flutter.plugin.common.MethodChannel
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.channels.Channel
import kotlinx.coroutines.launch
import kotlinx.coroutines.runBlocking
import java.io.File
import java.io.FileOutputStream
import java.util.Date

class CustomMethodChannel {

  // region variable

  private val host: FlutterActivity

  private var continuation: Channel<Any?>

  // endregion

  // region construct

  public constructor(
    host: FlutterActivity,
  ) {
    this.host = host
    this.continuation = Channel()
    return
  }

  // endregion

  // region register

  public fun register_configureFlutterEngine(
    flutterEngine: FlutterEngine,
  ): Unit {
    MethodChannel(
      flutterEngine.dartExecutor.binaryMessenger,
      "${this.queryApplicationIdentifier()}.CustomMethodChannel",
    ).setMethodCallHandler { call, result ->
      CoroutineScope(Dispatchers.Main).launch { this@CustomMethodChannel.handle(call, result) }
      return@setMethodCallHandler
    }
    return
  }

  public fun register_onActivityResult(
    requestCode: Int,
    resultCode: Int,
    data: Intent?,
  ): Unit {
    when (requestCode) {
      this.requestCodeForPickStorageItem -> {
        runBlocking {
          this@CustomMethodChannel.continuation.send(data?.data)
        }
      }
      this.requestForRequestExternalStoragePermission -> {
        runBlocking {
          this@CustomMethodChannel.continuation.send(null)
        }
      }
    }
    return
  }

  // endregion

  // region handle

  private suspend fun handle(
    call: MethodCall,
    result: MethodChannel.Result,
  ): Unit {
    try {
      val argumentMap = call.arguments as Map<*, *>
      val resultMap = mutableMapOf<Any?, Any?>()
      when (call.method) {
        "check_storage_permission" -> {
          val detail = this.handleCheckStoragePermission(
            argumentMap["mode"] as String,
          )
          resultMap["state"] = detail
        }
        "query_storage_item" -> {
          val detail = this.handleQueryStorageItem(
            argumentMap["type"] as String,
          )
          resultMap["target"] = detail
        }
        "reveal_storage_item" -> {
          val detail = this.handleRevealStorageItem(
            argumentMap["target"] as String,
          )
        }
        "pick_storage_item" -> {
          val detail = this.handlePickStorageItem(
            argumentMap["type"] as String,
            argumentMap["location"] as String,
            argumentMap["name"] as String,
          )
          resultMap["target"] = detail
        }
        "resolve_content_uri" -> {
          val detail = this.handleResolveContentUri(
            argumentMap["uri"] as String,
            argumentMap["fallback"] as String?,
          )
          resultMap["path"] = detail
        }
        else -> throw Exception("invalid method")
      }
      result.success(resultMap)
    }
    catch (e: Exception) {
      result.error("", e.stackTraceToString(), null)
    }
    return
  }

  private suspend fun handleCheckStoragePermission(
    mode: String,
  ): Boolean {
    check(mode == "check" || mode == "request")
    if (mode == "request") {
      this.host.startActivityForResult(Intent(Settings.ACTION_MANAGE_APP_ALL_FILES_ACCESS_PERMISSION, "package:${this.host.packageName}".toUri()), this.requestForRequestExternalStoragePermission)
      this.continuation.receive()
    }
    return Environment.isExternalStorageManager()
  }

  private suspend fun handleQueryStorageItem(
    type: String,
  ): String {
    check(type == "user_home" || type == "application_shared" || type == "application_cache")
    var target = null as String?
    if (type == "user_home") {
      target = "${Environment.getExternalStorageDirectory().absolutePath}"
    }
    if (type == "application_shared") {
      target = "${this.host.getExternalFilesDir(null)!!.absolutePath}"
    }
    if (type == "application_cache") {
      target = "${this.host.getCacheDir().absolutePath}"
    }
    return target!!
  }

  private suspend fun handleRevealStorageItem(
    target: String,
  ): Unit {
    val primaryDirectory = Environment.getExternalStorageDirectory().absolutePath
    if (!target.startsWith(primaryDirectory) || !(target.length == primaryDirectory.length || target[primaryDirectory.length] == '/')) {
      throw UnsupportedOperationException()
    }
    val targetSegment = target.substring(primaryDirectory.length + if (target.length == primaryDirectory.length) 0 else 1)
    val intent = Intent().also {
      it.setAction(Intent.ACTION_VIEW)
      it.setComponent(ComponentName("com.android.documentsui", "com.android.documentsui.files.FilesActivity"))
      it.setData("content://com.android.externalstorage.documents/document/primary%3A${Uri.encode(targetSegment)}".toUri())
      it.addFlags(Intent.FLAG_ACTIVITY_SINGLE_TOP)
    }
    this.host.startActivity(intent)
    return
  }

  private suspend fun handlePickStorageItem(
    type: String,
    location: String,
    name: String,
  ): String? {
    check(type == "load_file" || type == "load_directory" || type == "save_file")
    val intent = Intent()
    if (type == "load_file") {
      intent.setAction(Intent.ACTION_OPEN_DOCUMENT)
      intent.addCategory(Intent.CATEGORY_OPENABLE)
      intent.setType("*/*")
    }
    if (type == "load_directory") {
      intent.setAction(Intent.ACTION_OPEN_DOCUMENT_TREE)
    }
    if (type == "save_file") {
      intent.setAction(Intent.ACTION_CREATE_DOCUMENT)
      intent.addCategory(Intent.CATEGORY_OPENABLE)
      intent.setType("*/*")
    }
    intent.putExtra(Intent.EXTRA_LOCAL_ONLY, true)
    val primaryDirectory = Environment.getExternalStorageDirectory().absolutePath
    if (!location.startsWith(primaryDirectory) || !(location.length == primaryDirectory.length || location[primaryDirectory.length] == '/')) {
      throw UnsupportedOperationException()
    }
    val locationSegment = location.substring(primaryDirectory.length + if (location.length == primaryDirectory.length) 0 else 1)
    intent.putExtra(DocumentsContract.EXTRA_INITIAL_URI, "content://com.android.externalstorage.documents/document/primary%3A${Uri.encode(locationSegment)}".toUri())
    if (type == "save_file") {
      intent.putExtra(Intent.EXTRA_TITLE, name)
    }
    val timeBeforePick = Date().time
    this.host.startActivityForResult(intent, this.requestCodeForPickStorageItem)
    val targetUri = this.continuation.receive() as Uri?
    if (type == "save_file" && targetUri != null) {
      val targetDocument = DocumentFile.fromSingleUri(this.host, targetUri)!!
      if (targetDocument.lastModified() > timeBeforePick) {
        check(targetDocument.delete())
      }
    }
    val target = if (targetUri == null) null else this.resolveContentUri(targetUri, null)!!
    return target
  }

  private suspend fun handleResolveContentUri(
    uri: String,
    fallback: String?,
  ): String? {
    return this.resolveContentUri(uri.toUri(), fallback)
  }

  // endregion

  // region utility

  private val requestCodeForPickStorageItem: Int = 1001

  private val requestForRequestExternalStoragePermission: Int = 1002

  // ----------------

  private fun queryApplicationIdentifier(
  ): String {
    return this.host.packageName
  }

  // ----------------

  private fun resolveContentUri(
    uri: Uri,
    fallback: String?,
  ): String? {
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
    if (result == null && fallback != null) {
      val document = if (!DocumentsContract.isTreeUri(uri)) DocumentFile.fromSingleUri(this.host, uri)!! else DocumentFile.fromTreeUri(this.host, uri)!!
      if (document.exists()) {
        val placementName = document.name!!
        var placementSuffix = 0
        var placement = "${fallback}/${placementName}"
        while (File(placement).exists()) {
          placementSuffix += 1
          placement = "${fallback}/${placementName}.${placementSuffix}"
        }
        this.duplicateContentUri(uri, placement)
        result = placement
      }
    }
    return result
  }

  private fun duplicateContentUri(
    target: Uri,
    placement: String,
  ): Unit {
    check(!File(placement).exists())
    val destinationParent = File(placement).parentFile
    if (destinationParent != null && !destinationParent.isDirectory()) {
      destinationParent.mkdirs()
    }
    val targetDocument = if (!DocumentsContract.isTreeUri(target)) DocumentFile.fromSingleUri(this.host, target)!! else DocumentFile.fromTreeUri(this.host, target)!!
    if (targetDocument.isFile) {
      File(placement).createNewFile()
      this.host.contentResolver.openInputStream(target)!!.use { input ->
        FileOutputStream(placement, false).use { output ->
          input.copyTo(output)
        }
      }
    }
    else if (targetDocument.isDirectory) {
      File(placement).mkdirs()
      for (item in targetDocument.listFiles()) {
        this.duplicateContentUri(item.uri, "${placement}/${item.name}")
      }
    }
    else {
      throw UnsupportedOperationException()
    }
    return
  }

  // endregion

}
