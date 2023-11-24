import java.util.Properties

plugins {
	alias(libs.plugins.com.android.application)
	alias(libs.plugins.org.jetbrains.kotlin.android)
}

val keystoreProperties = Properties().also {
	it.load(rootProject.file("keystore.properties").inputStream())
}

android {
	namespace = "com.twinstar.toolkit.forwarder_for_android"
	compileSdk = 34
	buildToolsVersion = "34.0.0"

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
		targetSdk = 34
		versionCode = 4
		versionName = "4.0.0"
		
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