package com.twinstar.twinning.assistant

import android.content.ComponentName
import android.content.Intent
import android.net.Uri
import android.os.Environment
import android.provider.DocumentsContract
import android.provider.OpenableColumns
import android.provider.Settings
import androidx.core.database.getFloatOrNull
import androidx.core.database.getLongOrNull
import androidx.core.database.getStringOrNull
import androidx.core.net.toUri
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
      "${this.getApplicationIdentifier()}.CustomMethodChannel",
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
      when (call.method) {
        "copy_storage_file" -> {
          val detail = this.handleCopyStorageFile(
            call.argument<String>("target")!!,
            call.argument<String>("location")!!,
          )
          result.success(mapOf<Any?, Any?>(
            "placement" to detail,
          ))
        }
        "reveal_storage_file" -> {
          val detail = this.handleRevealPickStorageItem(
            call.argument<String>("target")!!,
          )
          result.success(mapOf<Any?, Any?>(
          ))
        }
        "pick_storage_item" -> {
          val detail = this.handlePickStorageItem(
            call.argument<String>("type")!!,
            call.argument<String>("location")!!,
            call.argument<String>("name")!!,
          )
          result.success(mapOf<Any?, Any?>(
            "target" to detail,
          ))
        }
        "check_external_storage_permission" -> {
          val detail = this.handleCheckExternalStoragePermission(
            call.argument<String>("mode")!!,
          )
          result.success(mapOf<Any?, Any?>(
            "state" to detail,
          ))
        }
        "query_external_storage_path" -> {
          val detail = this.handleQueryExternalStoragePath(
          )
          result.success(mapOf<Any?, Any?>(
            "path" to detail,
          ))
        }
        else -> {
          result.notImplemented()
        }
      }
    }
    catch (e: Exception) {
      result.error("", e.stackTraceToString(), null)
    }
    return
  }

  private suspend fun handleCopyStorageFile(
    target: String,
    location: String,
  ): String {
    check(File(location).isDirectory)
    val targetUri = target.toUri()
    check(targetUri.scheme == "content")
    val placementName = this.queryDatabaseOfContentUri<String>(targetUri, OpenableColumns.DISPLAY_NAME)
    check(placementName != null)
    var placement = "${location}/${placementName}"
    var placementSuffix = 0
    while (File(placement).exists()) {
      placementSuffix += 1
      placement = "${location}/${placementName}.${placementSuffix}"
    }
    this.host.contentResolver.openInputStream(targetUri)!!.use { input ->
      FileOutputStream(placement, false).use { output ->
        input.copyTo(output)
      }
    }
    return placement
  }

  private suspend fun handleRevealPickStorageItem(
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
    if (type == "save_file") {
      if (targetUri != null && queryDatabaseOfContentUri<Long>(targetUri, DocumentsContract.Document.COLUMN_LAST_MODIFIED)!! > timeBeforePick) {
        check(DocumentsContract.deleteDocument(this.host.contentResolver, targetUri))
      }
    }
    val target = targetUri?.toString()
    return target
  }

  private suspend fun handleCheckExternalStoragePermission(
    mode: String,
  ): Boolean {
    check(mode == "check" || mode == "request")
    if (mode == "request") {
      this.host.startActivityForResult(Intent(Settings.ACTION_MANAGE_APP_ALL_FILES_ACCESS_PERMISSION, "package:${this.host.packageName}".toUri()), this.requestForRequestExternalStoragePermission)
      this.continuation.receive()
    }
    return Environment.isExternalStorageManager()
  }

  private suspend fun handleQueryExternalStoragePath(
  ): String {
    return Environment.getExternalStorageDirectory().absolutePath
  }

  // endregion

  // region utility

  private val requestCodeForPickStorageItem: Int = 1001

  private val requestForRequestExternalStoragePermission: Int = 1002

  // ----------------

  private fun getApplicationIdentifier(
  ): String {
    return this.host.packageName
  }

  // ----------------

  private inline fun <reified TValue> queryDatabaseOfContentUri(
    uri: Uri,
    name: String,
  ): TValue? {
    var value = null as TValue?
    this.host.contentResolver.query(uri, arrayOf(name), null, null, null, null).use { cursor ->
      check(cursor != null)
      check(cursor.columnCount == 1)
      check(cursor.moveToFirst())
      value = when (TValue::class) {
        Long::class -> cursor.getLongOrNull(0) as TValue?
        Float::class -> cursor.getFloatOrNull(0) as TValue?
        String::class -> cursor.getStringOrNull(0) as TValue?
        else -> throw Exception()
      }
    }
    return value
  }

  // endregion

}
