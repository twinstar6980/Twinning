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
		versionCode = 5
		versionName = "5.0.0"
		
		testInstrumentationRunner = "androidx.test.runner.AndroidJUnitRunner"
	}

	signingConfigs {
		create("release") {
			keyAlias = keystoreProperties.getProperty("keyAlias")!!
			keyPassword = keystoreProperties.getProperty("keyPassword")!!
			storeFile = keystoreProperties.getProperty("storeFile")!!.let { file(it) }
			storePassword = keystoreProperties.getProperty("storePassword")!!
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