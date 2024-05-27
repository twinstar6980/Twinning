import java.util.Properties

plugins {
	alias(libs.plugins.android.application)
	alias(libs.plugins.jetbrains.kotlin.android)
}

val keystoreProperties = Properties().also {
	it.load(rootProject.file("keystore.properties").inputStream())
}

android {
	namespace = "com.twinstar.toolkit.forwarder"
	compileSdk = 34

	defaultConfig {
		applicationId = "com.twinstar.toolkit.forwarder"
		minSdk = 28
		targetSdk = 34
		versionCode = 9
		versionName = "9"
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
			proguardFiles(getDefaultProguardFile("proguard-android-optimize.txt"))

			signingConfig = signingConfigs.getByName("release")
		}
	}

	compileOptions {
		sourceCompatibility = JavaVersion.VERSION_17
		targetCompatibility = JavaVersion.VERSION_17
	}

	kotlinOptions {
		jvmTarget = "17"
	}

	buildFeatures {
		compose = true
	}

	composeOptions {
		kotlinCompilerExtensionVersion = "1.5.14"
	}

	packaging {
		resources {
			excludes += "/META-INF/{AL2.0,LGPL2.1}"
		}
	}
}

dependencies {
	implementation(libs.androidx.activity.compose)
	implementation(libs.androidx.material3)
}