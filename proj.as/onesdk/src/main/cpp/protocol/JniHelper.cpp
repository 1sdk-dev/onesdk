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

#include <android/log.h>
#include <pthread.h>

#include "../include/JniHelper.h"
#include "PluginMacros.h"

#define LOG_TAG "JniHelper"

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

static pthread_key_t g_key;

namespace onesdk {


    static JNIEnv *_cache_jni_env(JavaVM *jvm) {
        JNIEnv *_env = nullptr;
        // get jni environment
        jint ret = jvm->GetEnv((void **) &_env, JNI_VERSION_1_4);

        switch (ret) {
            case JNI_OK :
                // Success!
                pthread_setspecific(g_key, _env);
                return _env;
            case JNI_EDETACHED :
                // Thread not attached
                if (jvm->AttachCurrentThread(&_env, nullptr) < 0) {
                    LOGE("Failed to get the environment using AttachCurrentThread()");
                } else {
                    // Success : Attached and obtained JNIEnv!
                    pthread_setspecific(g_key, _env);
                    return _env;
                }
            case JNI_EVERSION :
                // Cannot recover from this error
                LOGE("JNI interface version 1.4 not supported");
            default :
                LOGE("Failed to get the environment using GetEnv()");
        }
        return nullptr;
    }

    jclass JniHelper::getClassID(const char *className, JNIEnv *env) {
        jclass jClass = env->FindClass(className);
        if (jClass == nullptr) {
            LOGE("Failed to find class of %s", className);
            return nullptr;
        }
        return jClass;
    }

    JavaVM *JniHelper::_psJavaVM = nullptr;
    std::unordered_map<JNIEnv *, std::vector<jobject>> JniHelper::localRefs;

    void JniHelper::setJavaVM(JavaVM *javaVM) {
        _psJavaVM = javaVM;
        pthread_key_create(&g_key, JniHelper::_DetachThread);
    }

    JNIEnv *JniHelper::getEnv() {
        JNIEnv *_env = (JNIEnv *) pthread_getspecific(g_key);
        return _env ? _env : _cache_jni_env(_psJavaVM);
    }

    bool JniHelper::getStaticMethodInfo(JniMethodInfo &methodInfo, const char *className, const char *methodName, const char *sig) {
        if ((nullptr == className) || (nullptr == methodName) || (nullptr == sig)) return false;
        JNIEnv *pEnv = methodInfo.env ? methodInfo.env : JniHelper::getEnv();
        if (pEnv == nullptr) return false;

        jclass classID = getClassID(className, pEnv);
        if (classID != nullptr) {
            jmethodID methodID = pEnv->GetStaticMethodID(classID, methodName, sig);
            if (methodID != nullptr) {
                methodInfo.env = pEnv;
                methodInfo.classID = classID;
                methodInfo.methodID = methodID;
                return true;
            } else {
                pEnv->ExceptionClear();
                reportNotFoundStaticError(className, methodName, sig);
            }
        }
        return false;
    }

    bool JniHelper::getMethodInfo(JniMethodInfo &methodInfo, const char *className, const char *methodName, const char *sig) {
        if ((nullptr == className) || (nullptr == methodName) || (nullptr == sig)) return false;
        JNIEnv *pEnv = methodInfo.env ? methodInfo.env : JniHelper::getEnv();
        if (pEnv == nullptr) return false;

        jclass classID = getClassID(className, pEnv);
        if (classID != nullptr) {
            jmethodID methodID = pEnv->GetMethodID(classID, methodName, sig);
            if (methodID != nullptr) {
                methodInfo.env = pEnv;
                methodInfo.classID = classID;
                methodInfo.methodID = methodID;
                return true;
            } else {
                pEnv->ExceptionClear();
                reportNotFoundError(className, methodName, sig);
            }
        }
        return false;
    }

    std::string JniHelper::jstring2string(jstring jstr) {
        return jstr != nullptr ? jstring2string(jstr, getEnv()) : "";
    }

    std::string JniHelper::jstring2string(jstring jstr, JNIEnv *env) {
        if (nullptr == env) return "";
        jclass classID = env->FindClass("java/lang/String");
        jstring strEncode = env->NewStringUTF("utf-8");
        jmethodID methodID = env->GetMethodID(classID, "getBytes", "(Ljava/lang/String;)[B");
        jbyteArray bArr = (jbyteArray) env->CallObjectMethod(jstr, methodID, strEncode);
        jsize arrLen = env->GetArrayLength(bArr);
        if (arrLen > 0) {
            jbyte *arrBuf = env->GetByteArrayElements(bArr, nullptr);
            std::string ret((const char *) arrBuf, (size_t) arrLen);
            env->ReleaseByteArrayElements(bArr, arrBuf, 0);

            env->DeleteLocalRef(classID);
            env->DeleteLocalRef(strEncode);
            env->DeleteLocalRef(bArr);
            return ret;
        }
        env->DeleteLocalRef(classID);
        env->DeleteLocalRef(strEncode);
        env->DeleteLocalRef(bArr);
        return "";
    }

    jstring JniHelper::newStringUTF(const std::string &utf8Str) {
        return newStringUTF(JniHelper::getEnv(), utf8Str);
    }

    jstring JniHelper::newStringUTF(JNIEnv *env, const std::string &utf8Str) {
        if (nullptr == env) return nullptr;

        jclass clazz = env->FindClass("java/lang/String");
        jmethodID id = env->GetMethodID(clazz, "<init>", "([BLjava/lang/String;)V");

        jstring encoding = env->NewStringUTF("utf-8");
        size_t utf8StrLen = utf8Str.length();
        jbyteArray bytes = env->NewByteArray(utf8StrLen);
        env->SetByteArrayRegion(bytes, 0, utf8StrLen, (jbyte *) utf8Str.c_str());

        jstring ret = (jstring) env->NewObject(clazz, id, bytes, encoding);

        env->DeleteLocalRef(bytes);
        env->DeleteLocalRef(clazz);
        env->DeleteLocalRef(encoding);

        return ret;
    }

    jobject JniHelper::createJavaMapObject(StringMap *paramMap) {
        JNIEnv *env = getEnv();
        jclass class_Hashtable = env->FindClass("java/util/Hashtable");
        jmethodID construct_method = env->GetMethodID(class_Hashtable, "<init>", "()V");
        jobject obj_Map = env->NewObject(class_Hashtable, construct_method);
        if (paramMap != nullptr) {
            jmethodID add_method = env->GetMethodID(class_Hashtable, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
            for (std::map<std::string, std::string>::const_iterator it = paramMap->begin(); it != paramMap->end(); ++it) {
                jstring first = newStringUTF(env, it->first);
                jstring second = newStringUTF(env, it->second);
                env->CallObjectMethod(obj_Map, add_method, first, second);
                env->DeleteLocalRef(first);
                env->DeleteLocalRef(second);
            }
        }
        env->DeleteLocalRef(class_Hashtable);
        return obj_Map;
    }

    jobject JniHelper::createJavaListObject(std::vector<const char *> *paramList) {
        JNIEnv *env = getEnv();
        jclass class_List = env->FindClass("java/util/ArrayList");
        jmethodID construct_method = env->GetMethodID(class_List, "<init>", "()V");
        jobject obj_List = env->NewObject(class_List, construct_method);
        if (paramList != nullptr) {
            jmethodID add_method = env->GetMethodID(class_List, "add", "(Ljava/lang/Object;)Z");
            for (std::vector<const char *>::const_iterator it = paramList->begin(); it != paramList->end(); ++it) {
                jstring value = newStringUTF(env, (*it));
                env->CallBooleanMethod(obj_List, add_method, value);
                env->DeleteLocalRef(value);
            }
        }
        env->DeleteLocalRef(class_List);
        return obj_List;
    }

    StringMap JniHelper::convertJObjectToStringMap(jobject &jObj) {
        StringMap stringMap;
        if (jObj == nullptr) return stringMap;
        JNIEnv *env = getEnv();
        // 获取 Hashtable 类 entrySet() 方法 ID
        jclass hashtableClass = env->FindClass("java/util/Hashtable");
        jmethodID entrySetMID = env->GetMethodID(hashtableClass, "entrySet", "()Ljava/util/Set;");
        // 调用 Set 类中的 iterator() 方法获取 Set 对象
        jobject setObj = env->CallObjectMethod((jobject) jObj, entrySetMID);
        // 获取 Set 类中 iterator() 方法 ID
        jclass setClass = env->FindClass("java/util/Set");
        jmethodID iteratorMID = env->GetMethodID(setClass, "iterator", "()Ljava/util/Iterator;");
        // 调用 iterator() 方法获取 Iterator 对象
        jobject iteratorObj = env->CallObjectMethod(setObj, iteratorMID);
        // 获取 Iterator 类中 hasNext() 方法 ID
        // 用于 while 循环判断 Hashtable 中是否还有数据
        jclass iteratorClass = env->FindClass("java/util/Iterator");
        jmethodID hasNextMID = env->GetMethodID(iteratorClass, "hasNext", "()Z");
        // 获取Iterator类中next()方法ID
        // 用于读取HashMap中的每一条数据
        jmethodID nextMID = env->GetMethodID(iteratorClass, "next", "()Ljava/lang/Object;");
        // 获取 Map.Entry 类中 getKey() 和 getValue() 的方法 ID
        // 用于读取“K-V”键值对，注意：内部类使用$符号表示
        jclass entryClass = env->FindClass("java/util/Map$Entry");
        jmethodID getKeyMID = env->GetMethodID(entryClass, "getKey", "()Ljava/lang/Object;");
        jmethodID getValueMID = env->GetMethodID(entryClass, "getValue", "()Ljava/lang/Object;");
        // 循环检测HashMap中是否还有数据
        while (env->CallBooleanMethod(iteratorObj, hasNextMID)) {
            // 读取一条数据
            jobject entryObj = env->CallObjectMethod(iteratorObj, nextMID);
            // 提取数据中key值：String 类型
            jstring keyJS = (jstring) env->CallObjectMethod(entryObj, getKeyMID);
            // jstring 转 C 风格字符串
            const char *keyStr = env->GetStringUTFChars(keyJS, nullptr);
            // 提取数据中 value 值：String 类型
            jstring valueJS = static_cast<jstring>(env->CallObjectMethod(entryObj, getValueMID));
            // jstring转C风格字符串
            const char *valueStr = env->GetStringUTFChars(valueJS, nullptr);
            // 插入到 StringMap 中
            stringMap[std::string(keyStr)] = std::string(valueStr);
            // 释放UTF字符串资源
            env->ReleaseStringUTFChars(keyJS, keyStr);
            // 释放JNI局部引用资源
            env->DeleteLocalRef(entryObj);
            env->DeleteLocalRef(keyJS);
            env->DeleteLocalRef(valueJS);
        }

        // 释放JNI局部引用: jclass jobject
        env->DeleteLocalRef(hashtableClass);
        env->DeleteLocalRef(setObj);
        env->DeleteLocalRef(setClass);
        env->DeleteLocalRef(iteratorObj);
        env->DeleteLocalRef(iteratorClass);
        env->DeleteLocalRef(entryClass);
        env->DeleteLocalRef(jObj);
        return stringMap;
    }

    jstring JniHelper::convert(JniMethodInfo &t, const char *str) {
        if (str) {
            jstring ret = newStringUTF(str);
            localRefs[t.env].push_back(ret);
            return ret;
        } else {
            return nullptr;
        }
    }

    jstring JniHelper::convert(JniMethodInfo &t, const std::string &str) {
        jstring ret = newStringUTF(str);
        localRefs[t.env].push_back(ret);
        return ret;
    }

    jobject JniHelper::convert(JniMethodInfo &t, std::map<std::string, std::string> map) {
        jobject ret = createJavaMapObject(&map);
        localRefs[t.env].push_back(ret);
        return ret;
    }

    std::string JniHelper::convert(JniMethodInfo &t, jstring &jstr) {
        if (jstr == nullptr) return "";
        std::string str = JniHelper::jstring2string(jstr);
        t.env->DeleteLocalRef(jstr);
        return str;
    }

    std::unique_ptr<bool[]> JniHelper::convert(JniMethodInfo &t, jbooleanArray &array) {
        if (array == nullptr) return nullptr;
        jsize len = t.env->GetArrayLength(array);
        std::unique_ptr<bool[]> arr(new bool[len]);
        jboolean *jBoolArr = t.env->GetBooleanArrayElements(array, NULL);
        for (int i = 0; i < len; i++) {
            arr[i] = (jBoolArr[i] == JNI_TRUE);
        }
        t.env->ReleaseBooleanArrayElements(array, jBoolArr, 0);
        t.env->DeleteLocalRef(array);
        return arr;
    }

    std::unique_ptr<int[]> JniHelper::convert(JniMethodInfo &t, jintArray &array) {
        if (array == nullptr) return nullptr;
        jsize len = t.env->GetArrayLength(array);
        std::unique_ptr<int[]> arr(new int[len]);
        jint *jArr = t.env->GetIntArrayElements(array, NULL);
        memcpy(arr.get(), jArr, sizeof(int) * len);
        t.env->ReleaseIntArrayElements(array, jArr, JNI_ABORT);
        t.env->DeleteLocalRef(array);
        return arr;
    }

    std::unique_ptr<long[]> JniHelper::convert(JniMethodInfo &t, jlongArray &array) {
        if (array == nullptr) return nullptr;
        jsize len = t.env->GetArrayLength(array);
        std::unique_ptr<long[]> arr(new long[len]);
        jlong *jArr = t.env->GetLongArrayElements(array, NULL);
        memcpy(arr.get(), jArr, sizeof(long) * len);
        t.env->ReleaseLongArrayElements(array, jArr, JNI_ABORT);
        t.env->DeleteLocalRef(array);
        return arr;
    }

    std::unique_ptr<float[]> JniHelper::convert(JniMethodInfo &t, jfloatArray &array) {
        if (array == nullptr) return nullptr;
        jsize len = t.env->GetArrayLength(array);
        std::unique_ptr<float[]> arr(new float[len]);
        jfloat *jArr = t.env->GetFloatArrayElements(array, NULL);
        memcpy(arr.get(), jArr, sizeof(float) * len);
        t.env->ReleaseFloatArrayElements(array, jArr, JNI_ABORT);
        t.env->DeleteLocalRef(array);
        return arr;
    }

    std::unique_ptr<double[]> JniHelper::convert(JniMethodInfo &t, jdoubleArray &array) {
        if (array == nullptr) return nullptr;
        jsize len = t.env->GetArrayLength(array);
        std::unique_ptr<double[]> arr(new double[len]);
        jdouble *jArr = t.env->GetDoubleArrayElements(array, NULL);
        memcpy(arr.get(), jArr, sizeof(double) * len);
        t.env->ReleaseDoubleArrayElements(array, jArr, JNI_ABORT);
        t.env->DeleteLocalRef(array);
        return arr;
    }

    void JniHelper::deleteLocalRefs(JNIEnv *env) {
        for (const auto &ref: localRefs[env]) {
            env->DeleteLocalRef(ref);
        }
        localRefs[env].clear();
    }

    void JniHelper::reportNotFoundError(const char *className, const char *methodName, const char *signature) {
        LOGE("Failed to find java method. Class name: %s, method name: %s, signature: %s ", className, methodName, signature);
    }

    void JniHelper::reportNotFoundStaticError(const char *className, const char *methodName, const char *signature) {
        LOGE("Failed to find static java method. Class name: %s, method name: %s, signature: %s ", className, methodName, signature);
    }

    void JniHelper::_DetachThread(void *) {
        JNIEnv *_env = nullptr;
        _psJavaVM->GetEnv((void **) &_env, JNI_VERSION_1_4);
        if (_env) {
            auto it = localRefs.find(_env);
            if (it != localRefs.end()) {
                localRefs.erase(it);
            }
        }
        _psJavaVM->DetachCurrentThread();
    }


} // onesdk