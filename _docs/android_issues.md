1. Editar build.gradle.kts (Module :app)

android {
	externalNativeBuild {
        cmake {
            path = file("src/main/cpp/CMakeLists.txt")
        }
    }
}

2. Editar AndroidManifest.xml

<manifest>
	<uses-feature android:glEsVersion="0x00030002" android:required="true" />

	<application>
	<activity>
		<meta-data android:name="android.app.lib_name" android:value="tsugaru2" />
	</activity>
	</application>
</manifest>

Note: In order to load a library into your app from Java/Kotlin, you must call System.loadLibrary() and pass the name of the library defined here; for GameActivity/NativeActivity derived applications, the same library name must be used in the AndroidManifest.xml file.


3. src/main/java/MainActivity

	public class MainActivity extends NativeActivity {
	}


4. src/main/cpp

CMAKE

# As linhas abaixo faz com que o header "<android_native_app_glue.h>" fique visivel
include(AndroidNdkModules)
android_ndk_import_module_native_app_glue()

# Resolve o seguinte problema: ANativeActivity_onCreate not found: undefined symbol: ANativeActivity_onCreate
# Export ANativeActivity_onCreate(),
# Refer to: https://github.com/android-ndk/ndk/issues/381.
set_target_properties(${CMAKE_PROJECT_NAME}
        PROPERTIES  LINK_FLAGS "-u ANativeActivity_onCreate")

target_link_libraries(${CMAKE_PROJECT_NAME}
        native_app_glue)





https://developer.android.com/reference/games/game-activity/struct/android-app

https://github.com/android/ndk-samples/tree/main/teapots

ANDROID HANDLE CMD - APP CMD START
ANDROID HANDLE CMD - APP CMD START

APP_CMD_INPUT_CHANGED
APP_CMD_INIT_WINDOW

ANDROID HANDLE CMD - APP CMD INIT WINDOW
ANDROID HANDLE CMD - APP CMD WINDOW RESIZED
ANDROID HANDLE CMD - APP CMD CONTENT RECT CHANGED
ANDROID HANDLE CMD - APP CMD WINDOW REDRAW NEEDED
ANDROID HANDLE CMD - APP CMD GAINED FOCUS

NDROID HANDLE CMD - APP CMD PAUSE
ANDROID HANDLE CMD - APP CMD LOST FOCUS
APP_CMD_TERM_WINDOW
ANDROID HANDLE CMD - APP CMD TERM WINDOW
PP_CMD_TERM_WINDOW
ANDROID HANDLE CMD - APP CMD STOP
NDROID HANDLE CMD - APP CMD SAVE STATE
APP_CMD_SAVE_STATE

ANDROID HANDLE CMD - APP CMD START
ANDROID HANDLE CMD - APP CMD START
APP_CMD_INIT_WINDOW
ANDROID HANDLE CMD - APP CMD INIT WINDOW
ANDROID HANDLE CMD - APP CMD WINDOW RESIZED
ANDROID HANDLE CMD - APP CMD WINDOW REDRAW NEEDED
ANDROID HANDLE CMD - APP CMD GAINED FOCUS

ANDROID HANDLE CMD - APP CMD PAUSE
ANDROID HANDLE CMD - APP CMD LOST FOCUS
APP_CMD_TERM_WINDOW
ANDROID HANDLE CMD - APP CMD TERM WINDOW
APP_CMD_TERM_WINDOW
ANDROID HANDLE CMD - APP CMD STOP
ANDROID HANDLE CMD - APP CMD SAVE STATE
APP_CMD_SAVE_STATE
