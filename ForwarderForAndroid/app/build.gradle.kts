import java.util.Properties
import java.io.FileInputStream

plugins {
	alias(libs.plugins.com.android.application)
	alias(libs.plugins.org.jetbrains.kotlin.android)
}

val keystoreProperties = Properties()
val keystorePropertiesFile = rootProject.file("keystore.properties")
assert(keystorePropertiesFile.exists())
keystoreProperties.load(FileInputStream(keystorePropertiesFile))

android {
	namespace = "com.twinstar.toolkit.forwarder_for_android"
	compileSdk = 33
	buildToolsVersion = "33.0.1"

	compileOptions {
		sourceCompatibility = JavaVersion.VERSION_1_8
		targetCompatibility = JavaVersion.VERSION_1_8
	}

	kotlinOptions {
		jvmTarget = "1.8"
	}

	defaultConfig {
		applicationId = "com.twinstar.toolkit.forwarder_for_android"
		minSdk = 28
		targetSdk = 33
		versionCode = 2
		versionName = "2.0.0"
		
		testInstrumentationRunner = "androidx.test.runner.AndroidJUnitRunner"
	}

	signingConfigs {
		create("release") {
			keyAlias = keystoreProperties["keyAlias"] as String
			keyPassword = keystoreProperties["keyPassword"] as String
			storeFile = file(keystoreProperties["storeFile"] as String)
			storePassword = keystoreProperties["storePassword"] as String
		}
	}

	buildTypes {
		release {
			isMinifyEnabled = true
			isShrinkResources = true
			signingConfig = signingConfigs.getByName("release")
		}
	}
}

dependencies {
}