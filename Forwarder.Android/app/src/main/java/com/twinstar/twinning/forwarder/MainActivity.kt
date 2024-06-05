package com.twinstar.twinning.forwarder

import android.content.ClipData
import android.content.Intent
import android.net.Uri
import android.os.Build
import android.os.Bundle
import android.widget.Toast
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import androidx.compose.foundation.isSystemInDarkTheme
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.Button
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Text
import androidx.compose.material3.darkColorScheme
import androidx.compose.material3.dynamicDarkColorScheme
import androidx.compose.material3.dynamicLightColorScheme
import androidx.compose.material3.lightColorScheme
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.text.style.TextOverflow

class MainActivity : ComponentActivity() {

	// region implement - Activity

	protected override fun onCreate(
		savedInstanceState: Bundle?,
	): Unit {
		super.onCreate(savedInstanceState)
		this.enableEdgeToEdge()
		this.setContent() {
			MaterialTheme(
				colorScheme = if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.S) {
					if (!isSystemInDarkTheme()) {
						dynamicLightColorScheme(this@MainActivity)
					}
					else {
						dynamicDarkColorScheme(this@MainActivity)
					}
				}
				else {
					if (!isSystemInDarkTheme()) {
						lightColorScheme()
					}
					else {
						darkColorScheme()
					}
				},
			) {
				Scaffold(
					modifier = Modifier
						.fillMaxSize(),
				) { innerPadding ->
					Column(
						modifier = Modifier
							.fillMaxSize()
							.padding(innerPadding),
						verticalArrangement = Arrangement.Center,
						horizontalAlignment = Alignment.CenterHorizontally,
					) {
						Button(
							onClick = {
								try {
									this@MainActivity.forwardResource(listOf())
								}
								catch (e: Exception) {
									this@MainActivity.showException(e)
								}
							},
						) {
							Text(
								text = "Forward",
								overflow = TextOverflow.Ellipsis,
							)
						}
					}
				}
			}
		}
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
		command.add("-insert_tab")
		command.add("Resource Forwarder")
		command.add("resource_forwarder")
		command.add("-input")
		command.addAll(resource.map() { item -> item.toString() })
		val link = Uri.parse("twinstar.twinning.assistant:/launch?${command.joinToString("&") { item -> "command=${this.encodePercentString(item)}" }}")
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
