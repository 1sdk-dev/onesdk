/**
 * Copyright 2023 xin.yang
 * <p/>
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * <p/>
 *     http://www.apache.org/licenses/LICENSE-2.0
 * <p/>
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

#include "PluginUtils.h"
#include "PluginMacros.h"

#define MAX_LOG_LEN 16*1024
#define LOG_TAG "ONE_SDK"

namespace onesdk {

    JNI_METHOD(void, nativeOutputLog)(JNIEnv *env, jclass thiz, jint type, jstring tag, jstring msg) {
        std::string sTag = JniHelper::jstring2string(tag, env);
        std::string sMsg = JniHelper::jstring2string(msg, env);
        PluginUtils::output((int) type, sTag.c_str(), sMsg.c_str());
    }

    JNI_METHOD(jstring, nativeGetConfigFiles)(JNIEnv *env, jclass thiz) {
        return JniHelper::newStringUTF(env, "tmosc;onesdk.json");
    }

    void PluginUtils::initJavaPlugin(IPlugin *pPlugin, jobject jObj, const char *className, int type) {
        PluginJavaData *pUserData = new PluginJavaData();
        pUserData->jobj = JniHelper::getEnv()->NewGlobalRef(jObj);
        pUserData->jclassName = className;
        PluginUtils::setPluginJavaData(pPlugin, pUserData, type);
    }

    std::map<IPlugin *, PluginJavaData *> s_PluginObjMap;
    std::map<std::string, IPlugin *> s_JObjPluginMap;

    typedef std::map<IPlugin *, PluginJavaData *>::iterator ObjMapIter;
    typedef std::map<std::string, IPlugin *>::iterator JObjPluginMapIter;

    PluginJavaData *PluginUtils::getPluginJavaData(IPlugin *pKeyObj) {
        PluginJavaData *ret = nullptr;
        ObjMapIter it = s_PluginObjMap.find(pKeyObj);
        if (it != s_PluginObjMap.end()) ret = it->second;
        return ret;
    }

    void PluginUtils::setPluginJavaData(IPlugin *pKeyObj, PluginJavaData *pData, int type) {
        s_PluginObjMap.insert(std::pair<IPlugin *, PluginJavaData *>(pKeyObj, pData));
        char temp[256];
        sprintf(temp, "%s%d", pData->jclassName.c_str(), type);
        s_JObjPluginMap.insert(std::pair<std::string, IPlugin *>(temp, pKeyObj));
    }

    void PluginUtils::erasePluginJavaData(IPlugin *pKeyObj, int type) {
        ObjMapIter it = s_PluginObjMap.find(pKeyObj);
        if (it != s_PluginObjMap.end()) {
            PluginJavaData *pData = it->second;
            if (pData != nullptr) {
                jobject jobj = pData->jobj;
                char temp[256];
                sprintf(temp, "%s%d", pData->jclassName.c_str(), type);
                JObjPluginMapIter pluginIt = s_JObjPluginMap.find(temp);
                if (pluginIt != s_JObjPluginMap.end()) s_JObjPluginMap.erase(pluginIt);
                outputLog(ANDROID_LOG_DEBUG, "PluginUtils", "Delete global reference.");
                JniHelper::getEnv()->DeleteGlobalRef(jobj);
                delete pData;
            }
            s_PluginObjMap.erase(it);
        }
    }

    IPlugin *PluginUtils::getPluginPtr(std::string className) {
        IPlugin *ret = nullptr;
        JObjPluginMapIter it = s_JObjPluginMap.find(className);
        if (it != s_JObjPluginMap.end()) ret = it->second;
        return ret;
    }

    std::string PluginUtils::_logLevel = "";

    void PluginUtils::outputLog(int type, const char *logTag, const char *pFormat, ...) {
        va_list args;
        va_start(args, pFormat);
        int bufferSize = MAX_LOG_LEN;
        char *buf;
        do {
            buf = new(std::nothrow) char[bufferSize];
            if (buf == nullptr) return; // not enough memory
            int ret = vsnprintf(buf, static_cast<size_t>(bufferSize - 3), pFormat, args);
            if (ret < 0) {
                bufferSize *= 2;
                delete[] buf;
            } else break;
        } while (true);
        strcat(buf, "\n");
        va_end(args);
        PluginUtils::output(type, logTag, buf);
    }

    void PluginUtils::output(int type, const char *logTag, const char *contents) {
        if (_logLevel == "") {
            _logLevel = JniHelper::callStringFunc("tech/mxin/onesdk/framework/SDKHelper", "logLevel");
        }

        if (_logLevel.compare("verbose") == 0) {
            __android_log_print(type, LOG_TAG, "%s: %s", logTag, contents);
            return;
        }

        if (_logLevel.compare("debug") == 0 && type != ANDROID_LOG_VERBOSE) {
            __android_log_print(type, LOG_TAG, "%s: %s", logTag, contents);
            return;
        }

        if (_logLevel.compare("info") == 0 && (type == ANDROID_LOG_INFO || type == ANDROID_LOG_ERROR || type == ANDROID_LOG_WARN)) {
            __android_log_print(type, LOG_TAG, "%s: %s", logTag, contents);
            return;
        }

        if (_logLevel.compare("warning") == 0 && (type == ANDROID_LOG_ERROR || type == ANDROID_LOG_WARN)) {
            __android_log_print(type, LOG_TAG, "%s: %s", logTag, contents);
            return;
        }

        if (_logLevel.compare("error") == 0 && type == ANDROID_LOG_ERROR) {
            __android_log_print(type, LOG_TAG, "%s: %s", logTag, contents);
            return;
        }
    }

    void PluginUtils::resetLogLevel() {
        _logLevel = "";
    }


} // onesdk