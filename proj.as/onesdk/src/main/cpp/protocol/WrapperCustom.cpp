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

#include "WrapperCustom.h"
#include "PluginUtils.h"

namespace onesdk {

    JNI_METHOD(void, nativeOnCustomResult)(JNIEnv *env, jclass clazz, jstring className, jint ret, jstring msg) {
        std::string strMsg = JniHelper::jstring2string(msg, env);
        std::string strClassName = JniHelper::jstring2string(className, env);
        IPlugin *pPlugin = PluginUtils::getPluginPtr(strClassName);
        if (pPlugin != nullptr) {
            WrapperCustom *pCustom = static_cast<WrapperCustom *>(pPlugin);
            CustomListener *pListener = pCustom->getListener();
            CustomActionResult res;
            res.code = (CustomResultCode) ret;
            res.msg = strMsg;
            res.className = strClassName;
            if (nullptr != pListener) {
                pListener->onCustomResult((CustomResultCode) ret, strMsg.c_str());
            } else {
                WrapperCustom::pushActionResult(res);
                PluginUtils::outputLog(ANDROID_LOG_WARN, "WrapperCustom", "Listener of plugin %s not set correctly",
                                       pPlugin->getPluginName().c_str());
            }
        } else {
            CustomActionResult res;
            res.code = (CustomResultCode) ret;
            res.msg = strMsg;
            res.className = strClassName;
            WrapperCustom::pushActionResult(res);
            PluginUtils::outputLog(ANDROID_LOG_WARN, "WrapperCustom", "plugin is null");
        }
    }

    std::vector<CustomActionResult> WrapperCustom::_listenerResult;

    WrapperCustom::WrapperCustom() : _listener(nullptr) {}

    WrapperCustom::~WrapperCustom() { _listenerResult.clear(); }

    void WrapperCustom::setListener(CustomListener *pListener) {
        _listener = pListener;
        popActionResult();
    }

    CustomListener *WrapperCustom::getListener() { return _listener; }

    void WrapperCustom::setPluginName(const char *name) {
        _pluginName = name;
    }

    std::string WrapperCustom::getPluginName() {
        return _pluginName;
    }

    void WrapperCustom::pushActionResult(CustomActionResult res) {
        _listenerResult.push_back(res);
    }

    void WrapperCustom::popActionResult() {
        std::vector<CustomActionResult>::iterator iter = _listenerResult.begin();
        while (iter != _listenerResult.end()) {
            CustomActionResult res = *iter;
            WrapperCustom *pCustom = static_cast<WrapperCustom *>(PluginUtils::getPluginPtr(res.className));
            if (pCustom != nullptr) {
                CustomListener *listener = pCustom->getListener();
                if (nullptr != listener) {
                    listener->onCustomResult(res.code, res.msg.c_str());
                    iter = _listenerResult.erase(iter);
                    continue;
                }
            }
            iter++;
        }
    }
} // onesdk