/**
 * Copyright 2023 xin.yang
 * <p>
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * <p>
 *     http://www.apache.org/licenses/LICENSE-2.0
 * <p>
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

#ifndef ONESDK_JNI_HELPER_H
#define ONESDK_JNI_HELPER_H

#include <jni.h>
#include <string>
#include <vector>
#include <unordered_map>
#include "PluginDefines.h"


#define CALL_OBJECT_FUNC(retCode, retSig)                                               \
JniMethodInfo t;                                                                        \
std::string signature = "(" + std::string(getJNISignature(ts...)) + ")" + retSig;       \
if (JniHelper::getMethodInfo(t, className, methodName, signature.c_str())) {            \
    ret = t.env->Call##retCode##Method(object, t.methodID, convert(t, ts)...);          \
    t.env->DeleteLocalRef(t.classID);                                                   \
    deleteLocalRefs(t.env);                                                             \
} else { reportNotFoundError(className, methodName, signature.c_str()); }               \

#define CALL_STATIC_FUNC(retCode, retSig)                                               \
JniMethodInfo t;                                                                        \
std::string signature = "(" + std::string(getJNISignature(ts...)) + ")" + retSig;       \
if (JniHelper::getStaticMethodInfo(t, className, methodName, signature.c_str())) {      \
    ret = t.env->CallStatic##retCode##Method(t.classID, t.methodID, convert(t, ts)...); \
    t.env->DeleteLocalRef(t.classID);                                                   \
    deleteLocalRefs(t.env);                                                             \
} else { reportNotFoundStaticError(className, methodName, signature.c_str()); }         \


namespace onesdk {

    struct JniMethodInfo {
        JNIEnv *env{nullptr};
        jclass classID{nullptr};
        jmethodID methodID{nullptr};
    };

    class JniHelper {
    public:
        static void setJavaVM(JavaVM *javaVM);

        static JNIEnv *getEnv();

        static jclass getClassID(const char *className, JNIEnv *env);

        static bool getStaticMethodInfo(JniMethodInfo &methodInfo, const char *className, const char *methodName, const char *sig);

        static bool getMethodInfo(JniMethodInfo &methodInfo, const char *className, const char *methodName, const char *sig);

        static std::string jstring2string(jstring jstr);

        static std::string jstring2string(jstring jstr, JNIEnv *env);

        static jstring newStringUTF(const std::string &utf8Str);

        static jstring newStringUTF(JNIEnv *env, const std::string &utf8Str);

        static jobject createJavaMapObject(StringMap *paramMap);

        static jobject createJavaListObject(std::vector<const char *> *paramList);

        static StringMap convertJObjectToStringMap(jobject &jObj);

        template<typename... Ts>
        static jobject newObject(const char *className, Ts... ts) {
            jobject ret = nullptr;
            static const char *methodName = "<init>";
            JniMethodInfo t;
            std::string signature = "(" + std::string(getJNISignature(ts...)) + ")V";
            if (JniHelper::getMethodInfo(t, className, methodName, signature.c_str())) {
                ret = t.env->NewObject(t.classID, t.methodID, convert(t, ts)...);
                t.env->DeleteLocalRef(t.classID);
                deleteLocalRefs(t.env);
            } else {
                reportNotFoundError(className, methodName, signature.c_str());
            }
            return ret;
        }

        template<typename... Ts>
        static void callVoidFunc(jobject object, const char *className, const char *methodName, Ts... ts) {
            JniMethodInfo t;
            std::string signature = "(" + std::string(getJNISignature(ts...)) + ")V";
            if (JniHelper::getMethodInfo(t, className, methodName, signature.c_str())) {
                t.env->CallVoidMethod(object, t.methodID, convert(t, ts)...);
                t.env->DeleteLocalRef(t.classID);
                deleteLocalRefs(t.env);
            } else {
                reportNotFoundError(className, methodName, signature.c_str());
            }
        }

        template<typename... Ts>
        static bool callBoolFunc(jobject object, const char *className, const char *methodName, Ts... ts) {
            jboolean ret = JNI_FALSE;
            CALL_OBJECT_FUNC(Boolean, "Z")
            return (ret == JNI_TRUE);
        }

        template<typename... Ts>
        static std::unique_ptr<bool[]> callBoolArrayFunc(jobject object, const char *className, const char *methodName, Ts... ts) {
            jobject ret = nullptr;
            CALL_OBJECT_FUNC(Object, "[Z")
            return convert(t, (jbooleanArray &) ret);
        }

        template<typename... Ts>
        static int callIntFunc(jobject object, const char *className, const char *methodName, Ts... ts) {
            int ret = 0;
            CALL_OBJECT_FUNC(Int, "I")
            return ret;
        }

        template<typename... Ts>
        static std::unique_ptr<int[]> callIntArrayFunc(jobject object, const char *className, const char *methodName, Ts... ts) {
            jobject ret = nullptr;
            CALL_OBJECT_FUNC(Object, "[I")
            return convert(t, (jintArray &) ret);
        }

        template<typename... Ts>
        static long callLongFunc(jobject object, const char *className, const char *methodName, Ts... ts) {
            long ret = 0;
            CALL_OBJECT_FUNC(Long, "J")
            return ret;
        }

        template<typename... Ts>
        static std::unique_ptr<long[]> callLongArrayFunc(jobject object, const char *className, const char *methodName, Ts... ts) {
            jobject ret = nullptr;
            CALL_OBJECT_FUNC(Object, "[J")
            return convert(t, (jlongArray &) ret);
        }

        template<typename... Ts>
        static float callFloatFunc(jobject object, const char *className, const char *methodName, Ts... ts) {
            float ret = 0.0f;
            CALL_OBJECT_FUNC(Float, "F")
            return ret;
        }

        template<typename... Ts>
        static std::unique_ptr<float[]> callFloatArrayFunc(jobject object, const char *className, const char *methodName, Ts... ts) {
            jobject ret = nullptr;
            CALL_OBJECT_FUNC(Object, "[F")
            return convert(t, (jfloatArray &) ret);
        }

        template<typename... Ts>
        static double callDoubleFunc(jobject object, const char *className, const char *methodName, Ts... ts) {
            double ret = 0.0f;
            CALL_OBJECT_FUNC(Double, "D")
            return ret;
        }

        template<typename... Ts>
        static std::unique_ptr<double[]> callDoubleArrayFunc(jobject object, const char *className, const char *methodName, Ts... ts) {
            jobject ret = nullptr;
            CALL_OBJECT_FUNC(Object, "[D")
            return convert(t, (jdoubleArray &) ret);
        }

        template<typename... Ts>
        static std::string callStringFunc(jobject object, const char *className, const char *methodName, Ts... ts) {
            jobject ret = nullptr;
            CALL_OBJECT_FUNC(Object, "Ljava/lang/String;")
            return convert(t, (jstring &) ret);
        }

        template<typename... Ts>
        static jobject callObjectFunc(jobject object, const char *className, const char *methodName, const char *retSig, Ts... ts) {
            jobject ret = nullptr;
            CALL_OBJECT_FUNC(Object, retSig)
            return ret;
        }

        template<typename... Ts>
        static void callVoidFunc(const char *className, const char *methodName, Ts... ts) {
            JniMethodInfo t;
            std::string signature = "(" + std::string(getJNISignature(ts...)) + ")V";
            if (JniHelper::getStaticMethodInfo(t, className, methodName, signature.c_str())) {
                t.env->CallStaticVoidMethod(t.classID, t.methodID, convert(t, ts)...);
                t.env->DeleteLocalRef(t.classID);
                deleteLocalRefs(t.env);
            } else {
                reportNotFoundStaticError(className, methodName, signature.c_str());
            }
        }

        template<typename... Ts>
        static bool callBoolFunc(const char *className, const char *methodName, Ts... ts) {
            jboolean ret = JNI_FALSE;
            CALL_STATIC_FUNC(Boolean, "Z")
            return (ret == JNI_TRUE);
        }

        template<typename... Ts>
        static std::unique_ptr<bool[]> callBoolArrayFunc(const char *className, const char *methodName, Ts... ts) {
            jobject ret = nullptr;
            CALL_STATIC_FUNC(Object, "[Z")
            return convert(t, (jbooleanArray &) ret);
        }

        template<typename... Ts>
        static int callIntFunc(const char *className, const char *methodName, Ts... ts) {
            jint ret = 0;
            CALL_STATIC_FUNC(Int, "I")
            return ret;
        }

        template<typename... Ts>
        static std::unique_ptr<int[]> callIntArrayFunc(const char *className, const char *methodName, Ts... ts) {
            jobject ret = nullptr;
            CALL_STATIC_FUNC(Object, "[I")
            return convert(t, (jintArray &) ret);
        }

        template<typename... Ts>
        static long callLongFunc(const char *className, const char *methodName, Ts... ts) {
            jlong ret = 0;
            CALL_STATIC_FUNC(Long, "J")
            return ret;
        }

        template<typename... Ts>
        static std::unique_ptr<long[]> callLongArrayFunc(const char *className, const char *methodName, Ts... ts) {
            jobject ret = nullptr;
            CALL_STATIC_FUNC(Object, "[J")
            return convert(t, (jlongArray &) ret);
        }

        template<typename... Ts>
        static float callFloatFunc(const char *className, const char *methodName, Ts... ts) {
            jfloat ret = 0.0;
            CALL_STATIC_FUNC(Float, "F")
            return ret;
        }

        template<typename... Ts>
        static std::unique_ptr<float[]> callFloatArrayFunc(const char *className, const char *methodName, Ts... ts) {
            jobject ret = nullptr;
            CALL_STATIC_FUNC(Object, "[F")
            return convert(t, (jfloatArray &) ret);
        }

        template<typename... Ts>
        static double callDoubleFunc(const char *className, const char *methodName, Ts... ts) {
            jdouble ret = 0.0;
            CALL_STATIC_FUNC(Float, "D")
            return ret;
        }

        template<typename... Ts>
        static std::unique_ptr<double[]> callDoubleArrayFunc(const char *className, const char *methodName, Ts... ts) {
            jobject ret = nullptr;
            CALL_STATIC_FUNC(Object, "[D")
            return convert(t, (jdoubleArray &) ret);
        }

        template<typename... Ts>
        static std::string callStringFunc(const char *className, const char *methodName, Ts... ts) {
            jobject ret;
            CALL_STATIC_FUNC(Object, "Ljava/lang/String;")
            return convert(t, (jstring &) ret);
        }

        template<typename... Ts>
        static jobject callObjectFunc(const char *className, const char *methodName, const char *retSig, Ts... ts) {
            jobject ret = nullptr;
            CALL_STATIC_FUNC(Object, retSig)
            return ret;
        }

    private:
        static JavaVM *_psJavaVM;
        static std::unordered_map<JNIEnv *, std::vector<jobject>> localRefs;

        static jstring convert(JniMethodInfo &t, const char *str);

        static jstring convert(JniMethodInfo &t, const std::string &str);

        static jobject convert(JniMethodInfo &t, std::map<std::string, std::string> map);

        static std::string convert(JniMethodInfo &t, jstring &jstr);

        static std::unique_ptr<bool[]> convert(JniMethodInfo &t, jbooleanArray &array);

        static std::unique_ptr<int[]> convert(JniMethodInfo &t, jintArray &array);

        static std::unique_ptr<long[]> convert(JniMethodInfo &t, jlongArray &array);

        static std::unique_ptr<float[]> convert(JniMethodInfo &t, jfloatArray &array);

        static std::unique_ptr<double[]> convert(JniMethodInfo &t, jdoubleArray &array);

        template<typename T>
        static T convert(JniMethodInfo &, T x) {
            return x;
        }

        static void deleteLocalRefs(JNIEnv *env);

        static std::string getJNISignature() { return ""; }

        static std::string getJNISignature(bool) { return "Z"; }

        static std::string getJNISignature(bool[]) { return "[Z"; }

        static std::string getJNISignature(char) { return "C"; }

        static std::string getJNISignature(char[]) { return "[C"; }

        static std::string getJNISignature(short) { return "S"; }

        static std::string getJNISignature(short[]) { return "[S"; }

        static std::string getJNISignature(int) { return "I"; }

        static std::string getJNISignature(int[]) { return "[I"; }

        static std::string getJNISignature(long) { return "J"; }

        static std::string getJNISignature(long[]) { return "[J"; }

        static std::string getJNISignature(float) { return "F"; }

        static std::string getJNISignature(float[]) { return "[F"; }

        static std::string getJNISignature(double) { return "D"; }

        static std::string getJNISignature(double[]) { return "[D"; }

        static std::string getJNISignature(const char *) { return "Ljava/lang/String;"; }

        static std::string getJNISignature(const char *[]) { return "[Ljava/lang/String;"; }

        static std::string getJNISignature(const std::string) { return "Ljava/lang/String;"; }

        static std::string getJNISignature(const std::string[]) { return "[Ljava/lang/String;"; }

        static std::string getJNISignature(const std::map<std::string, std::string>) { return "Ljava/util/Hashtable;"; }

        static std::string getJNISignature(const jobject jObj) {
            jclass cls = getEnv()->GetObjectClass(jObj);
            if (cls == nullptr) return "";
            jstring jClassName = (jstring) callObjectFunc(cls, "java/lang/Class", "getName", "Ljava/lang/String;");
            std::string name = callStringFunc(jClassName, "java/lang/String", "replace", '.', '/');
            return "L" + name + ";";
        }

        template<typename T>
        static std::string getJNISignature(T t) {
            // This template should never be instantiated
            static_assert(sizeof(t) == 0, "Unsupported argument type");
            return "";
        }

        template<typename T, typename... Ts>
        static std::string getJNISignature(T t, Ts... ts) {

            return getJNISignature(t) + getJNISignature(ts...);
        }

        static void reportNotFoundError(const char *className, const char *methodName, const char *signature);

        static void reportNotFoundStaticError(const char *className, const char *methodName, const char *signature);

        static void _DetachThread(void *);
    };

} // onesdk

#endif //ONESDK_JNI_HELPER_H
