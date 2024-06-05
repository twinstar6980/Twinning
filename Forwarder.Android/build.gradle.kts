plugins {
	alias(libs.plugins.android.application) apply false
	alias(libs.plugins.jetbrains.kotlin.android) apply false
}

layout.buildDirectory.set(rootProject.file(".build"))
subprojects {
	layout.buildDirectory.set(rootProject.file(".build/${name}"))
}