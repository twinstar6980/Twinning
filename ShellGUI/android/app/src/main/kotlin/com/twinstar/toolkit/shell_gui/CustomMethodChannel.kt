package com.twinstar.toolkit.shell_gui

import android.Manifest
import android.content.ContentResolver
import android.content.Intent
import android.content.pm.PackageManager
import android.net.Uri
import android.os.Build
import android.os.Environment
import android.provider.OpenableColumns
import android.provider.Settings
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

class CustomMethodChannel {

	// region variable

	private val host: FlutterActivity

	private var continuation: Channel<Any?>

	private var command: MutableList<String>

	// endregion

	// region construct

	public constructor(
		host: FlutterActivity,
	) {
		this.host = host
		this.continuation = Channel()
		this.command = mutableListOf()
	}

	// endregion

	// region register

	public fun register_onConfigureFlutterEngine(
		flutterEngine: FlutterEngine,
	): Unit {
		MethodChannel(
			flutterEngine.dartExecutor.binaryMessenger,
			"com.twinstar.toolkit.shell_gui.CustomMethodChannel",
		).setMethodCallHandler { call, result ->
			CoroutineScope(Dispatchers.Main).launch { this@CustomMethodChannel.handle(call, result) }
			return@setMethodCallHandler
		}
		val link = this.host.intent.data
		if (link != null && link.scheme == "twinstar.toolkit.shell-gui" && link.host == null && link.port == -1 && link.path == "/run") {
			this.command.addAll(link.getQueryParameters("command"))
		}
		return
	}

	public fun register_onActivityResult(
		requestCode: Int,
		resultCode: Int,
		data: Intent?,
	): Unit {
		when (requestCode) {
			REQUEST_STORAGE_PERMISSION -> {
				runBlocking {
					this@CustomMethodChannel.continuation.send(null)
				}
			}
			REQUEST_PICK_PATH -> {
				runBlocking {
					this@CustomMethodChannel.continuation.send(data?.data)
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
			REQUEST_STORAGE_PERMISSION -> {
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
				"checkStoragePermission" -> {
					result.success(this.handle_checkStoragePermission(
					))
				}
				"requestStoragePermission" -> {
					result.success(this.handle_requestStoragePermission(
					))
				}
				"pickPath" -> {
					result.success(this.handle_pickPath(
						call.argument<Boolean>("isDirectory")!!,
						call.argument<String>("fallbackDirectory")!!,
					))
				}
				"getCommand" -> {
					result.success(this.handle_getCommand(
						call.argument<String>("fallbackDirectory")!!,
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

	private suspend fun handle_checkStoragePermission(
	): Boolean {
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

	private suspend fun handle_requestStoragePermission(
	): Boolean {
		if (Build.VERSION.SDK_INT < Build.VERSION_CODES.R) {
			this.host.requestPermissions(arrayOf(Manifest.permission.READ_EXTERNAL_STORAGE, Manifest.permission.WRITE_EXTERNAL_STORAGE), REQUEST_STORAGE_PERMISSION)
		}
		else {
			this.host.startActivityForResult(Intent(Settings.ACTION_MANAGE_APP_ALL_FILES_ACCESS_PERMISSION, Uri.parse("package:${this.host.packageName}")), REQUEST_STORAGE_PERMISSION)
		}
		this.continuation.receive()
		return this.handle_checkStoragePermission()
	}

	private suspend fun handle_pickPath(
		isDirectory: Boolean,
		fallbackDirectory: String,
	): String? {
		val intent = Intent(if (!isDirectory) (Intent.ACTION_GET_CONTENT) else (Intent.ACTION_OPEN_DOCUMENT_TREE)).also {
			it.putExtra(Intent.EXTRA_LOCAL_ONLY, true)
			if (!isDirectory) {
				it.addCategory(Intent.CATEGORY_OPENABLE)
				it.setType("*/*")
			}
		}
		this.host.startActivityForResult(intent, REQUEST_PICK_PATH)
		val uri = this.continuation.receive() as Uri?
		return uri?.let { parsePathOfContentProviderUri(it, fallbackDirectory, this.host.contentResolver) ?: it.toString() }
	}

	private suspend fun handle_getCommand(
		fallbackDirectory: String,
	): List<String> {
		return this.command.map { if (!it.startsWith("content://")) (it) else (parsePathOfContentProviderUri(Uri.parse(it), fallbackDirectory, this.host.contentResolver) ?: it) }
	}

	// endregion

	// region static

	companion object {

		// region request code

		private const val REQUEST_STORAGE_PERMISSION: Int = 1001

		private const val REQUEST_PICK_PATH: Int = 1002

		// endregion

		// region content uri

		private fun parsePathOfContentProviderUri(
			uri: Uri,
			fallbackDirectory: String,
			contentResolver: ContentResolver,
		): String? {
			check(uri.scheme == "content")
			var result = null as String?
			val provider = uri.authority!!
			var path = uri.path!!
			when (provider) {
				// AOSP DocumentsUI
				"com.android.externalstorage.documents" -> {
					// /tree/primary:<path-relative-external-storage>
					if (path.startsWith("/tree/primary:")) {
						result = "${Environment.getExternalStorageDirectory().path}${path.substring("/tree/primary:".length).let { if (it.isEmpty()) (it) else ("/${it}") }}"
					}
					// /document/primary:<path-relative-external-storage>
					if (path.startsWith("/document/primary:")) {
						result = "${Environment.getExternalStorageDirectory().path}${path.substring("/document/primary:".length).let { if (it.isEmpty()) (it) else ("/${it}") }}"
					}
				}
				"com.android.providers.media.documents" -> {
					// /document/document:<id>
					// /document/image:<id>
					// /document/audio:<id>
					// /document/video:<id>
				}
				"com.android.providers.downloads.documents" -> {
					// /document/raw:<path-absolute>
					if (path.startsWith("/document/raw:")) {
						result = "${path.substring("/document/raw:".length)}"
					}
					// /document/msf:<id>
				}
				// Material Files
				"me.zhanghai.android.files.file_provider" -> {
					path = Uri.decode(path)
					// /file://<path-absolute>
					if (path.startsWith("/file://")) {
						result = "${Uri.parse(path.substring("/".length)).path!!}"
					}
				}
				// Explorer
				"com.speedsoftware.explorer.fileprovider",
					// Root Explorer
				"com.speedsoftware.rootexplorer.fileprovider" -> {
					// /root/<path-relative-root>
					if (path.startsWith("/root/")) {
						result = "/${path.substring("/root/".length)}"
					}
				}
				// Solid Explorer
				"pl.solidexplorer2.files",
					// MT Manager
				"bin.mt.plus.fp",
					// NMM
				"in.mfile.files" -> {
					// <path-absolute>
					result = path
				}
				else -> {
					if (path.startsWith("/") && File(path).exists()) {
						result = path
					}
				}
			}
			if (result == null && fallbackDirectory.isNotEmpty()) {
				check(File(fallbackDirectory).isDirectory)
				val fallbackName = this.queryDatabaseOfContentProviderUri(uri, OpenableColumns.DISPLAY_NAME, contentResolver) ?: uri.lastPathSegment
				if (fallbackName != null) {
					val fallbackFile = "${fallbackDirectory}/${fallbackName}"
					if (File(fallbackFile).exists()) {
						check(File(fallbackFile).deleteRecursively())
					}
					try {
						contentResolver.openInputStream(uri)!!.use { input ->
							FileOutputStream(fallbackFile, false).use { output ->
								input.copyTo(output)
							}
						}
						result = fallbackFile
					}
					catch (e: Exception) {
					}
				}
			}
			return result
		}

		private fun queryDatabaseOfContentProviderUri(
			uri: Uri,
			name: String,
			contentResolver: ContentResolver,
		): String? {
			var value = null as String?
			try {
				val cursor = contentResolver.query(uri, arrayOf(name), null, null, null, null)
				if (cursor != null) {
					if (cursor.moveToFirst() && cursor.columnCount == 1) {
						value = cursor.getString(0)
					}
					cursor.close()
				}
			}
			catch (e: Exception) {
			}
			return value
		}

		// endregion

	}

	// endregion

}
