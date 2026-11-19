plugins {
    id("com.android.application")
}

android {
    namespace = "io.github.foxaxewastaken.nowplayingpin"
    compileSdk = 37

    defaultConfig {
        applicationId = "io.github.foxaxewastaken.nowplayingpin"
        minSdk = 33 // Android 13 (docs/android-app.md)
        targetSdk = 37
        versionCode = 1
        versionName = "0.1.0"
    }
}

dependencies {
    testImplementation("junit:junit:4.13.2")
}
