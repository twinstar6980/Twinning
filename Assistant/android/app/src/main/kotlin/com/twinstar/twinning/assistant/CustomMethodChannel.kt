package com.twinstar.twinning.assistant

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

  public fun register_onConfigureFlutterEngine(
    flutterEngine: FlutterEngine,
  ): Unit {
    MethodChannel(
      flutterEngine.dartExecutor.binaryMessenger,
      "${this.host.packageName}.CustomMethodChannel",
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
        "copy_storage_file" -> {
          val detail = this.handleCopyStorageFile(
            call.argument<String>("source")!!,
            call.argument<String>("placement")!!,
          )
          result.success(mapOf<Any?, Any?>(
            "destination" to detail,
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
    var locationSafe = ""
    val primaryDirectory = Environment.getExternalStorageDirectory().absolutePath + "/"
    if (location.startsWith(primaryDirectory)) {
      locationSafe = location.substring(primaryDirectory.length)
    }
    intent.putExtra(DocumentsContract.EXTRA_INITIAL_URI, "content://com.android.externalstorage.documents/document/primary%3A${Uri.encode(locationSafe)}".toUri())
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

  private suspend fun handleCopyStorageFile(
    source: String,
    placement: String,
  ): String {
    check(File(placement).isDirectory)
    val sourceUri = source.toUri()
    check(sourceUri.scheme == "content")
    val destinationName = this.queryDatabaseOfContentUri<String>(sourceUri, OpenableColumns.DISPLAY_NAME)
    check(destinationName != null)
    val destination = "${placement}/${destinationName}"
    if (File(destination).exists()) {
      check(File(destination).delete())
    }
    this.host.contentResolver.openInputStream(sourceUri)!!.use { input ->
      FileOutputStream(destination, false).use { output ->
        input.copyTo(output)
      }
    }
    return destination
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
