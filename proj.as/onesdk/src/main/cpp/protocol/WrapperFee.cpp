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

#include "WrapperFee.h"
#include "PluginUtils.h"

namespace onesdk {

    JNI_METHOD(void, nativeOnFeeResult)(JNIEnv *env, jclass clazz, jstring className, jint ret, jstring msg) {
        std::string strMsg = JniHelper::jstring2string(msg, env);
        std::string strClassName = JniHelper::jstring2string(className, env);
        IPlugin *pPlugin = PluginUtils::getPluginPtr(strClassName);
        if (pPlugin != nullptr) {
            WrapperFee *pFee = static_cast<WrapperFee *>(pPlugin);
            FeeListener *listener = pFee->getListener();
            FeeActionResult res;
            res.code = (FeeResultCode) ret;
            res.msg = strMsg;
            res.className = strClassName;
            if (nullptr != listener) {
                listener->onFeeResult((FeeResultCode) ret, strMsg.c_str());
            } else {
                WrapperFee::pushActionResult(res);
                PluginUtils::outputLog(ANDROID_LOG_WARN, "WrapperFee", "Listener of plugin %s not set correctly", pFee->getPluginName().c_str());
            }
        } else {
            FeeActionResult res;
            res.code = (FeeResultCode) ret;
            res.msg = strMsg;
            res.className = strClassName;
            WrapperFee::pushActionResult(res);
            PluginUtils::outputLog(ANDROID_LOG_WARN, "WrapperFee", "plugin is null");
        }
    }

    std::vector<FeeActionResult> WrapperFee::_listenerResult;

    WrapperFee::WrapperFee() : _listener(nullptr) {}

    WrapperFee::~WrapperFee() { _listenerResult.clear(); }

    void WrapperFee::feeForProduct(TProductInfo info) {
        if (info.empty()) {
            if (nullptr != _listener) _listener->onFeeResult(kFeeFailed, "Product info error");
            PluginUtils::outputLog(ANDROID_LOG_ERROR, "WrapperFee", "The product info is empty!");
            return;
        }
        callVoidFunc("feeForProduct", info);
    }

    void WrapperFee::setListener(FeeListener *pListener) {
        _listener = pListener;
        popActionResult();
    }

    FeeListener *WrapperFee::getListener() {
        return _listener;
    }

    void WrapperFee::setPluginName(const char *name) {
        _pluginName = name;
    }

    std::string WrapperFee::getPluginName() {
        return _pluginName;
    }

    void WrapperFee::pushActionResult(FeeActionResult res) {
        _listenerResult.push_back(res);
    }

    void WrapperFee::popActionResult() {
        std::vector<FeeActionResult>::iterator iter = _listenerResult.begin();
        while (iter != _listenerResult.end()) {
            FeeActionResult res = *iter;
            WrapperFee *pFee = static_cast<WrapperFee *>(PluginUtils::getPluginPtr(res.className));
            if (pFee) {
                FeeListener *listener = pFee->getListener();
                if (nullptr != listener) {
                    listener->onFeeResult(res.code, res.msg.c_str());
                    iter = _listenerResult.erase(iter);
                    continue;
                }
            }
            iter++;
        }

    }

} // onesdk