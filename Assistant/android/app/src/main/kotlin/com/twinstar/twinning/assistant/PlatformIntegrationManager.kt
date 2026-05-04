package com.twinstar.twinning.assistant

import android.app.ComponentCaller
import android.app.Notification
import android.app.NotificationChannel
import android.app.NotificationManager
import android.app.PendingIntent
import android.content.ComponentName
import android.content.Context
import android.content.Intent
import android.content.pm.PackageManager
import android.net.Uri
import android.os.Bundle
import android.os.Environment
import android.provider.DocumentsContract
import android.provider.Settings
import androidx.core.database.getFloatOrNull
import androidx.core.database.getLongOrNull
import androidx.core.database.getStringOrNull
import androidx.core.net.toUri
import io.flutter.embedding.engine.FlutterEngine
import io.flutter.plugin.common.MethodCall
import io.flutter.plugin.common.MethodChannel
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.channels.Channel
import kotlinx.coroutines.launch
import kotlinx.coroutines.runBlocking
import java.util.Date
import kotlin.properties.Delegates

class PlatformIntegrationManager {

  // region variable

  private var channel: MethodChannel by Delegates.notNull()

  private var activity: MainActivity by Delegates.notNull()

  private val continuation: Channel<Any?>

  // endregion

  // region construct

  private constructor(
  ) {
    this.continuation = Channel()
    return
  }

  // endregion

  // region singleton

  public companion object {

    private var instance: PlatformIntegrationManager = PlatformIntegrationManager()

    public fun instance(
    ): PlatformIntegrationManager {
      return instance
    }

  }

  // endregion

  // region register

  @Suppress("FunctionName", "LocalVariableName")
  public fun inject_MainActivity_configureFlutterEngine(
    host: MainActivity,
    with_flutterEngine: FlutterEngine,
  ): Unit {
    this.activity = host
    this.channel = MethodChannel(
      with_flutterEngine.dartExecutor.binaryMessenger,
      "${this.queryApplicationIdentifier()}/PlatformIntegrationManager",
    )
    this.channel.setMethodCallHandler({ call, result ->
      CoroutineScope(Dispatchers.Main).launch {
        this@PlatformIntegrationManager.handle(call, result)
      }
      return@setMethodCallHandler
    })
    return
  }

  @Suppress("FunctionName", "LocalVariableName")
  public fun inject_MainActivity_onCreate(
    host: MainActivity,
    with_savedInstanceState: Bundle?,
  ): Unit {
    val notificationManager = this.activity.getSystemService(Context.NOTIFICATION_SERVICE) as NotificationManager
    notificationManager.createNotificationChannel(
      NotificationChannel("main", "Main", NotificationManager.IMPORTANCE_DEFAULT).also {
        it.setShowBadge(false)
      },
    )
    val intent = this.activity.intent
    if (intent.action == Intent.ACTION_VIEW && intent.data != null && intent.data!!.scheme == this.queryApplicationIdentifier()) {
      runBlocking {
        val link = intent.data!!.toString()
        this@PlatformIntegrationManager.invokeReceiveApplicationLink(link)
      }
    }
    return
  }

  @Suppress("FunctionName", "LocalVariableName")
  public fun inject_MainActivity_onNewIntent(
    host: MainActivity,
    with_intent: Intent,
    with_caller: ComponentCaller,
  ): Unit {
    val intent = with_intent
    if (intent.action == Intent.ACTION_VIEW && intent.data != null && intent.data!!.scheme == this.queryApplicationIdentifier()) {
      runBlocking {
        val link = intent.data!!.toString()
        this@PlatformIntegrationManager.invokeReceiveApplicationLink(link)
      }
    }
    return
  }

  @Suppress("FunctionName", "LocalVariableName")
  public fun inject_MainActivity_onActivityResult(
    host: MainActivity,
    with_requestCode: Int,
    with_resultCode: Int,
    with_data: Intent?,
  ): Unit {
    when (with_requestCode) {
      this.requestCodeForPickStorageItem -> {
        runBlocking {
          val value = mutableListOf<Uri>()
          if (with_data != null) {
            if (with_data.data != null) {
              value.add(with_data.data!!)
            }
            else if (with_data.clipData != null) {
              for (index in 0 until with_data.clipData!!.itemCount) {
                value.add(with_data.clipData!!.getItemAt(index).uri)
              }
            }
          }
          this@PlatformIntegrationManager.continuation.send(value)
        }
      }
      this.requestForToggleStoragePermission -> {
        runBlocking {
          this@PlatformIntegrationManager.continuation.send(null)
        }
      }
      this.requestForToggleNotificationPermission -> {
        runBlocking {
          this@PlatformIntegrationManager.continuation.send(null)
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
      val method = call.method
      @Suppress("UNCHECKED_CAST")
      val rawArgument = call.arguments as MutableMap<Any?, Any?>
      val getArgument = { name: String ->
        this.extractFlutterValueMap(rawArgument, name)
      }
      val rawResult = mutableMapOf<Any?, Any?>()
      val setResult = { name: String, value: Any? ->
        this.infuseFlutterValueMap(rawResult, name, value)
      }
      when (method) {
        "check_application_permission" -> {
          val detail = this.handleCheckApplicationPermission(
            this.decodeFlutterValue<String>(getArgument("name")),
          )
          setResult("state", this.encodeFlutterValue(detail))
        }
        "update_application_permission" -> {
          val detail = this.handleUpdateApplicationPermission(
            this.decodeFlutterValue<String>(getArgument("name")),
          )
        }
        "check_application_extension" -> {
          val detail = this.handleCheckApplicationExtension(
            this.decodeFlutterValue<String>(getArgument("name")),
          )
          setResult("state", this.encodeFlutterValue(detail))
        }
        "update_application_extension" -> {
          val detail = this.handleUpdateApplicationExtension(
            this.decodeFlutterValue<String>(getArgument("name")),
            this.decodeFlutterValue<Boolean>(getArgument("state")),
          )
        }
        "query_storage_item" -> {
          val detail = this.handleQueryStorageItem(
            this.decodeFlutterValue<String>(getArgument("type")),
          )
          setResult("target", this.encodeFlutterValue(detail))
        }
        "reveal_storage_item" -> {
          val detail = this.handleRevealStorageItem(
            this.decodeFlutterValue<String>(getArgument("target")),
          )
        }
        "pick_storage_item" -> {
          val detail = this.handlePickStorageItem(
            this.decodeFlutterValue<String>(getArgument("type")),
            this.decodeFlutterValue<Boolean>(getArgument("multiply")),
            this.decodeFlutterValue<String>(getArgument("location")),
            this.decodeFlutterValue<String>(getArgument("name")),
          )
          setResult("target", this.encodeFlutterValue(detail))
        }
        "push_system_notification" -> {
          val detail = this.handlePushSystemNotification(
            this.decodeFlutterValue<String>(getArgument("title")),
            this.decodeFlutterValue<String>(getArgument("description")),
          )
        }
        else -> throw Exception("invalid method")
      }
      result.success(rawResult)
    }
    catch (e: Exception) {
      result.error("", e.stackTraceToString(), null)
    }
    return
  }

  // ----------------

  private suspend fun handleCheckApplicationPermission(
    name: String,
  ): Boolean {
    check(name == "storage" || name == "notification")
    var state = false
    if (name == "storage") {
      state = Environment.isExternalStorageManager()
    }
    if (name == "notification") {
      val notificationManager = this.activity.getSystemService(Context.NOTIFICATION_SERVICE) as NotificationManager
      state = notificationManager.areNotificationsEnabled()
    }
    return state
  }

  private suspend fun handleUpdateApplicationPermission(
    name: String,
  ): Unit {
    check(name == "storage" || name == "notification")
    if (name == "storage") {
      this.activity.startActivityForResult(
        Intent(Settings.ACTION_MANAGE_APP_ALL_FILES_ACCESS_PERMISSION).also {
          it.setData("package:${this.activity.packageName}".toUri())
        },
        this.requestForToggleStoragePermission,
      )
      this.continuation.receive()
    }
    if (name == "notification") {
      this.activity.startActivityForResult(
        Intent(Settings.ACTION_APP_NOTIFICATION_SETTINGS).also {
          it.putExtra(Settings.EXTRA_APP_PACKAGE, this.activity.packageName)
        },
        this.requestForToggleNotificationPermission,
      )
      this.continuation.receive()
    }
    return
  }

  // ----------------

  private suspend fun handleCheckApplicationExtension(
    name: String,
  ): Boolean {
    check(name == "forwarder")
    var state = false
    @Suppress("KotlinConstantConditions")
    if (name == "forwarder") {
      val packageManager = this.activity.context.packageManager
      val componentName = ComponentName(this.activity.context, ForwarderActivity::class.qualifiedName!!)
      state = when (packageManager.getComponentEnabledSetting(componentName)) {
        PackageManager.COMPONENT_ENABLED_STATE_ENABLED -> true
        PackageManager.COMPONENT_ENABLED_STATE_DISABLED -> false
        else -> packageManager.getActivityInfo(componentName, PackageManager.MATCH_DISABLED_COMPONENTS).enabled
      }
    }
    return state
  }

  private suspend fun handleUpdateApplicationExtension(
    name: String,
    state: Boolean,
  ): Unit {
    check(name == "forwarder")
    @Suppress("KotlinConstantConditions")
    if (name == "forwarder") {
      val packageManager = this.activity.context.packageManager
      val componentName = ComponentName(this.activity.context, ForwarderActivity::class.qualifiedName!!)
      val newState = if (state) PackageManager.COMPONENT_ENABLED_STATE_ENABLED else PackageManager.COMPONENT_ENABLED_STATE_DISABLED
      packageManager.setComponentEnabledSetting(componentName, newState, PackageManager.DONT_KILL_APP)
    }
    return
  }

  // ----------------

  private suspend fun handleQueryStorageItem(
    type: String,
  ): String {
    check(type == "user_home" || type == "application_shared" || type == "application_temporary")
    var target = null as String?
    if (type == "user_home") {
      target = "${Environment.getExternalStorageDirectory().absolutePath}"
    }
    if (type == "application_shared") {
      target = "${this.activity.getExternalFilesDir(null)!!.absolutePath}"
    }
    if (type == "application_temporary") {
      target = "${this.activity.getCacheDir().absolutePath}/temporary"
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
    val intent = Intent()
    intent.setComponent(ComponentName("com.android.documentsui", "com.android.documentsui.files.FilesActivity"))
    intent.setAction(Intent.ACTION_VIEW)
    intent.setData("content://com.android.externalstorage.documents/document/primary%3A${Uri.encode(targetSegment)}".toUri())
    intent.addFlags(Intent.FLAG_ACTIVITY_SINGLE_TOP)
    this.activity.startActivity(intent)
    return
  }

  private suspend fun handlePickStorageItem(
    type: String,
    multiply: Boolean,
    location: String,
    name: String,
  ): List<String> {
    check(type == "load_file" || type == "load_directory" || type == "save_file")
    val intent = Intent()
    intent.setComponent(ComponentName("com.android.documentsui", "com.android.documentsui.picker.PickActivity"))
    if (type == "load_file") {
      intent.setAction(Intent.ACTION_OPEN_DOCUMENT)
      intent.addCategory(Intent.CATEGORY_OPENABLE)
      if (multiply) {
        intent.putExtra(Intent.EXTRA_ALLOW_MULTIPLE, true)
      }
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
    this.activity.startActivityForResult(intent, this.requestCodeForPickStorageItem)
    @Suppress("UNCHECKED_CAST")
    val targetUri = this.continuation.receive() as List<Uri>
    if (type == "save_file" && !targetUri.isEmpty()) {
      if (this.queryDatabaseOfContentUri<Long>(targetUri.first(), DocumentsContract.Document.COLUMN_LAST_MODIFIED)!! > timeBeforePick) {
        check(DocumentsContract.deleteDocument(this.activity.contentResolver, targetUri.first()))
      }
    }
    val target = targetUri.map({ item -> this.resolveContentUri(item) })
    return target
  }

  // ----------------

  private suspend fun handlePushSystemNotification(
    title: String,
    description: String,
  ): Unit {
    val intent = PendingIntent.getActivity(
      this.activity.context,
      0,
      Intent(this.activity.context, MainActivity::class.java).also {
        it.flags = Intent.FLAG_ACTIVITY_NEW_TASK or Intent.FLAG_ACTIVITY_CLEAR_TOP
      },
      PendingIntent.FLAG_IMMUTABLE,
    )
    val builder = Notification.Builder(this.activity.context, "main")
      .setSmallIcon(R.mipmap.ic_launcher_foreground)
      .setContentTitle(title)
      .setContentText(description)
      .setContentIntent(intent)
      .setAutoCancel(true)
    val manager = this.activity.getSystemService(Context.NOTIFICATION_SERVICE) as NotificationManager
    manager.notify((System.currentTimeMillis() % Int.MAX_VALUE).toInt(), builder.build())
    return
  }

  // endregion

  // region invoke

  private suspend fun invoke(
    method: String,
    argument: Map<String, Any?>,
  ): Unit {
    val result = this.channel.invokeMethod(method, argument)
    return result
  }

  // ----------------

  private suspend fun invokeReceiveApplicationLink(
    target: String,
  ): Unit {
    return this.invoke("receive_application_link", mapOf(
      "target" to this.encodeFlutterValue(target),
    ))
  }

  // endregion

  // region utility

  private val requestCodeForPickStorageItem: Int = 1001

  private val requestForToggleStoragePermission: Int = 1002

  private val requestForToggleNotificationPermission: Int = 1003

  // ----------------

  private inline fun <reified TValue> encodeFlutterValue(
    ripe: TValue,
  ): Any? {
    return ripe
  }

  private inline fun <reified TValue> decodeFlutterValue(
    raw: Any?,
  ): TValue {
    return raw as TValue
  }

  private fun extractFlutterValueMap(
    map: MutableMap<Any?, Any?>,
    name: String,
  ): Any? {
    return map.getValue(name)
  }

  private fun infuseFlutterValueMap(
    map: MutableMap<Any?, Any?>,
    name: String,
    value: Any?,
  ): Unit {
    map[name] = value
    return
  }

  // ----------------

  private fun queryApplicationIdentifier(
  ): String {
    return this.activity.packageName
  }

  // ----------------

  private inline fun <reified TValue> queryDatabaseOfContentUri(
    uri: Uri,
    name: String,
  ): TValue? {
    var value = null as TValue?
    this.activity.contentResolver.query(uri, arrayOf(name), null, null, null, null).use { cursor ->
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

  // endregion

}
