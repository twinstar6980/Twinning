package com.twinstar.twinning.assistant

import android.Manifest
import android.content.ContentResolver
import android.content.Intent
import android.content.pm.PackageManager
import android.net.Uri
import android.os.Build
import android.os.Environment
import android.provider.DocumentsContract
import android.provider.OpenableColumns
import android.provider.Settings
import androidx.core.database.getFloatOrNull
import androidx.core.database.getLongOrNull
import androidx.core.database.getStringOrNull
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
	}

	// endregion

	// region register

	public fun register_onConfigureFlutterEngine(
		flutterEngine: FlutterEngine,
	): Unit {
		MethodChannel(
			flutterEngine.dartExecutor.binaryMessenger,
			"com.twinstar.twinning.assistant.CustomMethodChannel",
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
			REQUEST_PICK_STORAGE_ITEM -> {
				runBlocking {
					this@CustomMethodChannel.continuation.send(data?.data)
				}
			}
			REQUEST_REQUEST_EXTERNAL_STORAGE_PERMISSION -> {
				runBlocking {
					this@CustomMethodChannel.continuation.send(null)
				}
			}
		}
		return
	}

	public fun register_onRequestPermissionsResult(
		requestCode: Int,
		permissions: Array<out String>,
		grantResults: IntArray,
	) {
		when (requestCode) {
			REQUEST_REQUEST_EXTERNAL_STORAGE_PERMISSION -> {
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
					val detailType = call.argument<String>("type")!!
					val detailInitialDirectory = call.argument<String>("initial_directory")!!
					val detailTarget = this.handlePickStorageItem(
						detailType,
						detailInitialDirectory,
					)
					result.success(detailTarget)
				}
				"copy_storage_file" -> {
					val detailSource = call.argument<String>("source")!!
					val detailPlacement = call.argument<String>("placement")!!
					val detailDestination = this.handleCopyStorageFile(
						detailSource,
						detailPlacement,
					)
					result.success(detailDestination)
				}
				"check_external_storage_permission" -> {
					val detailMode = call.argument<String>("mode")!!
					val detailState = this.handleCheckExternalStoragePermission(
						detailMode,
					)
					result.success(detailState)
				}
				"query_external_storage_path" -> {
					val detailResult = this.handleQueryExternalStoragePath(
					)
					result.success(detailResult)
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
		initialDirectory: String,
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
		var initialDirectorySafe = ""
		val primaryDirectory = Environment.getExternalStorageDirectory().absolutePath + "/"
		if (initialDirectory.startsWith(primaryDirectory)) {
			initialDirectorySafe = initialDirectory.substring(primaryDirectory.length)
		}
		intent.putExtra(DocumentsContract.EXTRA_INITIAL_URI, Uri.parse("content://com.android.externalstorage.documents/document/primary%3A${Uri.encode(initialDirectorySafe)}"))
		val timeBeforePick = Date().time
		this.host.startActivityForResult(intent, REQUEST_PICK_STORAGE_ITEM)
		val targetUri = this.continuation.receive() as Uri?
		if (type == "save_file") {
			if (targetUri != null && queryDatabaseOfContentUri<Long>(targetUri, DocumentsContract.Document.COLUMN_LAST_MODIFIED, this.host.contentResolver)!! > timeBeforePick) {
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
		val sourceUri = Uri.parse(source)
		check(sourceUri.scheme == "content")
		val destinationName = queryDatabaseOfContentUri<String>(sourceUri, OpenableColumns.DISPLAY_NAME, this.host.contentResolver)
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
			if (Build.VERSION.SDK_INT < Build.VERSION_CODES.R) {
				this.host.requestPermissions(arrayOf(Manifest.permission.READ_EXTERNAL_STORAGE, Manifest.permission.WRITE_EXTERNAL_STORAGE), REQUEST_REQUEST_EXTERNAL_STORAGE_PERMISSION)
			}
			else {
				this.host.startActivityForResult(Intent(Settings.ACTION_MANAGE_APP_ALL_FILES_ACCESS_PERMISSION, Uri.parse("package:${this.host.packageName}")), REQUEST_REQUEST_EXTERNAL_STORAGE_PERMISSION)
			}
			this.continuation.receive()
		}
		val state: Boolean
		if (Build.VERSION.SDK_INT < Build.VERSION_CODES.R) {
			state = this.host.checkSelfPermission(Manifest.permission.READ_EXTERNAL_STORAGE) == PackageManager.PERMISSION_GRANTED &&
					this.host.checkSelfPermission(Manifest.permission.WRITE_EXTERNAL_STORAGE) == PackageManager.PERMISSION_GRANTED
		}
		else {
			state = Environment.isExternalStorageManager()
		}
		return state
	}

	private suspend fun handleQueryExternalStoragePath(
	): String {
		return Environment.getExternalStorageDirectory().absolutePath
	}

	// endregion

	// region static

	companion object {

		// region request code

		private const val REQUEST_PICK_STORAGE_ITEM: Int = 1001

		private const val REQUEST_REQUEST_EXTERNAL_STORAGE_PERMISSION: Int = 1002

		// endregion

		// region content uri

		private inline fun <reified T> queryDatabaseOfContentUri(
			uri: Uri,
			name: String,
			contentResolver: ContentResolver,
		): T? {
			var value = null as T?
			contentResolver.query(uri, arrayOf(name), null, null, null, null).use { cursor ->
				check(cursor != null)
				check(cursor.columnCount == 1)
				check(cursor.moveToFirst())
				value = when (T::class.java) {
					java.lang.Long::class.java -> cursor.getLongOrNull(0)as T?
					java.lang.Float::class.java -> cursor.getFloatOrNull(0) as T?
					java.lang.String::class.java -> cursor.getStringOrNull(0) as T?
					else -> throw Exception()
				}
			}
			return value
		}

		// endregion

	}

	// endregion

}
