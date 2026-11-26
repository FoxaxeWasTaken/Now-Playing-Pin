plugins {
    id("com.android.application")
}

// The protocol test vectors, shared with the C++ tests (docs/testing.md, level 2).
val protocolVectors = rootProject.file("../tools/vectors")

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

    testOptions {
        unitTests.all {
            it.systemProperty("vectors.dir", protocolVectors.absolutePath)
            it.inputs.dir(protocolVectors)
        }
    }
}

dependencies {
    testImplementation("junit:junit:4.13.2")
}
