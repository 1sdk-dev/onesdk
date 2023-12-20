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

#include "../include/IPlugin.h"
#include "PluginUtils.h"

#define _LOG_TAG "IPlugin"

#define LOGE(...) PluginUtils::outputLog(ANDROID_LOG_ERROR, _LOG_TAG, __VA_ARGS__)

#define EXPLICIT_TEMPLATE(retType, funcName) \
template retType funcName <>(const char *methodName);                                                                       \
template retType funcName <bool>(const char *methodName, bool);                                                             \
template retType funcName <bool[]>(const char *methodName, bool[]);                                                         \
template retType funcName <char>(const char *methodName, char);                                                             \
template retType funcName <char[]>(const char *methodName, char[]);                                                         \
template retType funcName <short>(const char *methodName, short);                                                           \
template retType funcName <short[]>(const char *methodName, short[]);                                                       \
template retType funcName <int>(const char *methodName, int);                                                               \
template retType funcName <int[]>(const char *methodName, int[]);                                                           \
template retType funcName <long>(const char *methodName, long);                                                             \
template retType funcName <long[]>(const char *methodName, long[]);                                                         \
template retType funcName <float>(const char *methodName, float);                                                           \
template retType funcName <float[]>(const char *methodName, float[]);                                                       \
template retType funcName <double>(const char *methodName, double);                                                         \
template retType funcName <double[]>(const char *methodName, double[]);                                                     \
template retType funcName <const char *>(const char *methodName, const char *);                                             \
template retType funcName <const char *[]>(const char *methodName, const char *[]);                                         \
template retType funcName <std::map<std::string, std::string>>(const char *methodName, std::map<std::string, std::string>); \


#define CALL_JAVA_FUNC(retType, retVal)                                                                                     \
PluginJavaData *pData = PluginUtils::getPluginJavaData(this);                                                               \
if (pData == nullptr) {                                                                                                     \
    LOGE("Can't find java data for plugin : %s", this->getPluginName().c_str());                                            \
    return retVal;                                                                                                          \
}                                                                                                                           \
return JniHelper::call##retType##Func(pData->jobj, pData->jclassName.c_str(), funcName, ts...);                             \


namespace onesdk {
    IPlugin::~IPlugin() {}

    std::string IPlugin::getPluginId() {
        return callStringFunc("getPluginId");
    }

    std::string IPlugin::getPluginVersion() {
        return callStringFunc("getPluginVersion");
    }

    std::string IPlugin::getSDKVersion() {
        return callStringFunc("getSDKVersion");
    }

    bool IPlugin::isFuncSupported(const char *funcName) {
        return callBoolFunc("isFuncSupported", funcName);
    }

    std::string IPlugin::getPluginMethods() {
        return callStringFunc("getPluginMethods");
    }

    template<typename... Ts>
    void IPlugin::callVoidFunc(const char *funcName, Ts... ts) {
        PluginJavaData *pData = PluginUtils::getPluginJavaData(this);
        if (pData == nullptr) {
            LOGE("Can't find java data for plugin : %s", getPluginName().c_str());
            return;
        }
        JniHelper::callVoidFunc(pData->jobj, pData->jclassName.c_str(), funcName, ts...);
    }

    EXPLICIT_TEMPLATE(void, IPlugin::callVoidFunc)

    template<typename... Ts>
    bool IPlugin::callBoolFunc(const char *funcName, Ts... ts) {
        CALL_JAVA_FUNC(Bool, false)
    }

    EXPLICIT_TEMPLATE(bool, IPlugin::callBoolFunc)

    template<typename... Ts>
    std::unique_ptr<bool[]> IPlugin::callBoolArrayFunc(const char *funcName, Ts... ts) {
        CALL_JAVA_FUNC(BoolArray, nullptr)
    }

    EXPLICIT_TEMPLATE(std::unique_ptr<bool[]>, IPlugin::callBoolArrayFunc)

    template<typename... Ts>
    int IPlugin::callIntFunc(const char *funcName, Ts... ts) {
        CALL_JAVA_FUNC(Int, 0)
    }

    EXPLICIT_TEMPLATE(int, IPlugin::callIntFunc)

    template<typename... Ts>
    std::unique_ptr<int[]> IPlugin::callIntArrayFunc(const char *funcName, Ts... ts) {
        CALL_JAVA_FUNC(IntArray, nullptr)
    }

    EXPLICIT_TEMPLATE(std::unique_ptr<int[]>, IPlugin::callIntArrayFunc)

    template<typename... Ts>
    long IPlugin::callLongFunc(const char *funcName, Ts... ts) {
        CALL_JAVA_FUNC(Long, 0L)
    }

    EXPLICIT_TEMPLATE(long, IPlugin::callLongFunc)

    template<typename... Ts>
    std::unique_ptr<long[]> IPlugin::callLongArrayFunc(const char *funcName, Ts... ts) {
        CALL_JAVA_FUNC(LongArray, nullptr)
    }

    EXPLICIT_TEMPLATE(std::unique_ptr<long[]>, IPlugin::callLongArrayFunc)

    template<typename... Ts>
    float IPlugin::callFloatFunc(const char *funcName, Ts... ts) {
        CALL_JAVA_FUNC(Float, 0.0f)
    }

    EXPLICIT_TEMPLATE(float, IPlugin::callFloatFunc)

    template<typename... Ts>
    std::unique_ptr<float[]> IPlugin::callFloatArrayFunc(const char *funcName, Ts... ts) {
        CALL_JAVA_FUNC(FloatArray, nullptr)
    }

    EXPLICIT_TEMPLATE(std::unique_ptr<float[]>, IPlugin::callFloatArrayFunc)

    template<typename... Ts>
    std::string IPlugin::callStringFunc(const char *funcName, Ts... ts) {
        CALL_JAVA_FUNC(String, "")
    }

    EXPLICIT_TEMPLATE(std::string, IPlugin::callStringFunc)

} // onesdk