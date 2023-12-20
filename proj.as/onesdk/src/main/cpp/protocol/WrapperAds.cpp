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

#include "WrapperAds.h"
#include "PluginUtils.h"

namespace onesdk {

    JNI_METHOD(void, nativeOnAdsResult)(JNIEnv *env, jclass clazz, jstring className, jint ret, jstring msg) {
        std::string strMsg = JniHelper::jstring2string(msg, env);
        std::string strClassName = JniHelper::jstring2string(className, env);
        IPlugin *pPlugin = PluginUtils::getPluginPtr(strClassName);
        if (pPlugin != nullptr) {
            WrapperAds *pAds = static_cast<WrapperAds *>(pPlugin);
            AdsListener *listener = pAds->getListener();
            AdsActionResult res;
            res.code = (AdsResultCode) ret;
            res.msg = strMsg;
            res.className = strClassName;
            if (nullptr != listener) {
                listener->onAdsResult((AdsResultCode) ret, strMsg.c_str());
            } else {
                WrapperAds::pushActionResult(res);
                PluginUtils::outputLog(ANDROID_LOG_WARN, "WrapperAds", "Listener of plugin %s not set correctly", pPlugin->getPluginName().c_str());
            }
        } else {
            AdsActionResult res;
            res.code = (AdsResultCode) ret;
            res.msg = strMsg;
            res.className = strClassName;
            WrapperAds::pushActionResult(res);
            PluginUtils::outputLog(ANDROID_LOG_WARN, "WrapperAds", "plugin is null");
        }
    }

    std::vector<AdsActionResult> WrapperAds::_listenerResult;

    WrapperAds::WrapperAds() : _listener(nullptr) {}

    WrapperAds::~WrapperAds() { _listenerResult.clear(); }

    void WrapperAds::showAds(TAdsInfo adsInfo) {
        if (adsInfo.empty()) {
            if (nullptr != _listener) _listener->onAdsResult(kAdsFailed, "Ads info error");
            PluginUtils::outputLog(ANDROID_LOG_ERROR, "WrapperAds", "The ads info is empty");
            return;
        }
        callVoidFunc("showAds", adsInfo);
    }

    void WrapperAds::hideAds(TAdsInfo adsInfo) {
        if (adsInfo.empty()) {
            if (nullptr != _listener) _listener->onAdsResult(kAdsFailed, "Ads info error");
            PluginUtils::outputLog(ANDROID_LOG_ERROR, "WrapperAds", "The ads info is empty");
            return;
        }
        callVoidFunc("hideAds", adsInfo);
    }

    void WrapperAds::preloadAds(TAdsInfo adsInfo) {
        if (adsInfo.empty()) {
            if (nullptr != _listener) _listener->onAdsResult(kAdsFailed, "Ads info error");
            PluginUtils::outputLog(ANDROID_LOG_ERROR, "WrapperAds", "The ads info is empty");
            return;
        }
        callVoidFunc("preloadAds", adsInfo);
    }

    void WrapperAds::setListener(AdsListener *listener) {
        _listener = listener;
        popActionResult();
    }

    AdsListener *WrapperAds::getListener() {
        return _listener;
    }

    void WrapperAds::setPluginName(const char *name) {
        _pluginName = name;
    }

    std::string WrapperAds::getPluginName() {
        return _pluginName;
    }

    void WrapperAds::pushActionResult(AdsActionResult res) {
        _listenerResult.push_back(res);
    }

    void WrapperAds::popActionResult() {
        std::vector<AdsActionResult>::iterator iter = _listenerResult.begin();
        while (iter != _listenerResult.end()) {
            AdsActionResult res = *iter;
            WrapperAds *pAds = static_cast<WrapperAds *>(PluginUtils::getPluginPtr(res.className));
            if (pAds != nullptr) {
                AdsListener *listener = pAds->getListener();
                if (nullptr != listener) {
                    listener->onAdsResult(res.code, res.msg.c_str());
                    iter = _listenerResult.erase(iter);
                    continue;
                }
            }
            iter++;
        }
    }

} // onesdk