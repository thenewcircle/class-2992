#include <jni.h>
#include <android/log.h>

namespace com_marakana_android_lognative {

void log(JNIEnv *env, jclass clazz, jint priority, jstring tag,
		jstring message) {
	// Convert to C string
	const char* cTag = env->GetStringUTFChars(tag, 0);
	const char* cMessage = env->GetStringUTFChars(message, 0);

	// Use log lib to log
	__android_log_write(priority, cTag, cMessage);

	// Release C string
	env->ReleaseStringUTFChars(tag, cTag);
	env->ReleaseStringUTFChars(message, cMessage);
}

/* Method table mapping Java calls to JNI */
static JNINativeMethod method_table[] = {
		{ "logN", "(ILjava/lang/String;Ljava/lang/String;)V", (void *) log }
};

}

using namespace com_marakana_android_lognative;

/* Executes when System.loadLibrary() loads this library */
extern "C" jint JNI_OnLoad(JavaVM* vm, void* reserved) {
	JNIEnv* env;
	if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
		return JNI_ERR;
	} else {
		jclass clazz = env->FindClass(
				"com/marakana/android/lognative/LogLib");
		if (clazz) {
			jint ret = env->RegisterNatives(clazz, method_table,
					sizeof(method_table) / sizeof(method_table[0]));
			env->DeleteLocalRef(clazz);
			return ret == 0 ? JNI_VERSION_1_6 : JNI_ERR;
		} else {
			return JNI_ERR;
		}
	}
}

