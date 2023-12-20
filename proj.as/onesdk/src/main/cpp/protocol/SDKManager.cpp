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

#include <sstream>
#include <vector>

#include "../include/JniHelper.h"
#include "../include/SDKManager.h"
#include "PluginFactory.h"
#include "PluginManager.h"
#include "PluginUtils.h"

#define ONESDK_WRAPPER "tech/mxin/onesdk/framework/SDKHelper"

namespace onesdk {

    JNI_METHOD(void, nativeInit)(JNIEnv *env, jclass clazz) {
        JavaVM *vm;
        env->GetJavaVM(&vm);
        JniHelper::setJavaVM(vm);
    }

    JNI_METHOD(void, nativePurge)(JNIEnv *env, jclass clazz) {
        SDKManager::end();
    }

    static SDKManager *s_pAgentManager = nullptr;

    SDKManager::SDKManager() : bIsPluginLoaded(false) {
        _pluginsUserMap.empty();
        _pluginsFeeMap.empty();
        _pluginsCustomMap.empty();
        supportPluginIds = "[]";
    }

    SDKManager::~SDKManager() {
        unloadAllPlugins();
    }

    SDKManager *SDKManager::shared() {
        if (s_pAgentManager == nullptr) s_pAgentManager = new SDKManager();
        return s_pAgentManager;
    }

    void SDKManager::end() {
        if (s_pAgentManager != nullptr) {
            s_pAgentManager->release();
            delete s_pAgentManager;
            s_pAgentManager = nullptr;
        }
    }

    void SDKManager::init(const std::string appId) {
        if (appId.size() == 0) exit(-1);
        JniHelper::callVoidFunc(ONESDK_WRAPPER, "setAppParam", appId);
        loadAllPlugins();
    }


    std::map<std::string, IPluginUser *> *SDKManager::getUserPlugins() {
        return &_pluginsUserMap;
    }

    IPluginUser *SDKManager::getUserPlugin(const std::string pluginId) {
        if (_pluginsUserMap.size() == 0) return nullptr;
        return pluginId.size() == 0 ? _pluginsUserMap.begin()->second : _pluginsUserMap[pluginId];
    }

    std::map<std::string, IPluginFee *> *SDKManager::getFeePlugins() {
        return &_pluginsFeeMap;
    }

    IPluginFee *SDKManager::getFeePlugin(const std::string pluginId) {
        if (_pluginsFeeMap.size() == 0) return nullptr;
        return pluginId.size() == 0 ? _pluginsFeeMap.begin()->second : _pluginsFeeMap[pluginId];
    }

    std::map<std::string, IPluginCustom *> *SDKManager::getCustomPlugins() {
        return &_pluginsCustomMap;
    }

    IPluginCustom *SDKManager::getCustomPlugin(const std::string pluginId) {
        if (_pluginsCustomMap.size() == 0) return nullptr;
        return pluginId.size() == 0 ? _pluginsCustomMap.begin()->second : _pluginsCustomMap[pluginId];
    }

    IPluginAds *SDKManager::getAdsPlugin() {
        return _pAds;
    }

    const std::string SDKManager::getChannelId() {
        return JniHelper::callStringFunc(ONESDK_WRAPPER, "getChannelId");
    }

    const std::string SDKManager::getFrameworkVersion() {
        return JniHelper::callStringFunc(ONESDK_WRAPPER, "getFrameworkVersion");
    }

    const std::string SDKManager::getSupportPluginIds() {
        if (supportPluginIds != "[]") return supportPluginIds;
        return JniHelper::callStringFunc(ONESDK_WRAPPER, "getSupportPluginIds");
    }

    void SDKManager::loadAllPlugins() {
        if (bIsPluginLoaded) return;
        bIsPluginLoaded = true;
        std::string fileData = getSupportPlugin();
        if (fileData == "[]") {
            PluginUtils::outputLog(ANDROID_LOG_DEBUG, "SDKManager", "loadALLPlugin(), Get the supported plugins: nullptr");
            setDebugMode();
            return;
        }
        PluginUtils::outputLog(ANDROID_LOG_DEBUG, "SDKManager", "loadALLPlugin(), Get the supported plugins : %s", fileData.c_str());
        fileData = fileData.substr(1, fileData.size() - 2);
        std::stringstream ss(fileData);
        std::string text;
        while (getline(ss, text, ',')) { //以,为间隔分割fileData的内容
            while (text.substr(0, 1) == " ") text = text.substr(1, text.size() - 1);
            if (text.find("User") != -1) {
                loadPlugin(text.c_str(), kPluginUser);
            } else if (text.find("Fee") != -1) {
                loadPlugin(text.c_str(), kPluginFee);
            } else if (text.find("Ads") != -1) {
                loadPlugin(text.c_str(), kPluginAds);
            } else if (text.find("Custom") != -1) {
                loadPlugin(text.c_str(), kPluginCustom);
            }
        }
    }

    void SDKManager::unloadAllPlugins() {
        std::map<std::string, IPluginUser *>::iterator _itUser = _pluginsUserMap.begin();
        for (; _itUser != _pluginsUserMap.end();) {
            PluginManager::shared()->unloadPlugin((_itUser->second)->getPluginName().c_str(), kPluginUser);
            _pluginsUserMap.erase(_itUser++);
        }
        _pluginsUserMap.clear();

        std::map<std::string, IPluginFee *>::iterator _itFee = _pluginsFeeMap.begin();
        for (; _itFee != _pluginsFeeMap.end();) {
            PluginManager::shared()->unloadPlugin((_itFee->second)->getPluginName().c_str(), kPluginFee);
            _pluginsFeeMap.erase(_itFee++);
        }
        _pluginsFeeMap.clear();

        if (_pAds) {
            PluginManager::shared()->unloadPlugin(_pAds->getPluginName().c_str(), kPluginAds);
            _pAds = nullptr;
        }

        std::map<std::string, IPluginCustom *>::iterator _itCustom = _pluginsCustomMap.begin();
        for (; _itCustom != _pluginsCustomMap.end();) {
            PluginManager::shared()->unloadPlugin((_itCustom->second)->getPluginName().c_str(), kPluginCustom);
            _pluginsCustomMap.erase(_itCustom++);
        }
        _pluginsCustomMap.clear();

        release();
        PluginFactory::purge();
    }

    const std::string SDKManager::getSupportPlugin() {
        return JniHelper::callStringFunc(ONESDK_WRAPPER, "getSupportPlugin");
    }

    void SDKManager::loadPlugin(const char *name, int type) {
        switch (type) {
            case kPluginAds: {
                _pAds = static_cast<IPluginAds *>(PluginManager::shared()->loadPlugin(name, kPluginAds));
                break;
            }
            case kPluginFee: {
                IPluginFee *_pFee = static_cast<IPluginFee *>(PluginManager::shared()->loadPlugin(name, kPluginFee));
                if (_pFee) _pluginsFeeMap[_pFee->getPluginId()] = _pFee;
                break;
            }
            case kPluginUser: {
                IPluginUser *_pUser = static_cast<IPluginUser *>(PluginManager::shared()->loadPlugin(name, kPluginUser));
                if (_pUser) _pluginsUserMap[_pUser->getPluginId()] = _pUser;
                break;
            }
            case kPluginCustom: {
                IPluginCustom *_pCustom = static_cast<IPluginCustom *>(PluginManager::shared()->loadPlugin(name, kPluginCustom));
                if (_pCustom) _pluginsCustomMap[_pCustom->getPluginId()] = _pCustom;
                break;
            }
        }
    }

    void SDKManager::setDebugMode() {
        std::string className = "";
        supportPluginIds = "[";
        className = "mock/MockCustom";
        IPluginCustom *_pCustom = static_cast<IPluginCustom *>(PluginManager::shared()->loadPlugin(className.c_str(), kPluginCustom));
        if (_pCustom) {
            _pluginsCustomMap[_pCustom->getPluginId()] = _pCustom;
            supportPluginIds += _pCustom->getPluginId() + ",";
        }
        className = "mock/MockUser";
        IPluginUser *_pUser = static_cast<IPluginUser *>(PluginManager::shared()->loadPlugin(className.c_str(), kPluginUser));
        if (_pUser) {
            _pluginsUserMap[_pUser->getPluginId()] = _pUser;
            supportPluginIds += _pUser->getPluginId() + ",";
        }
        className = "mock/MockFee";
        IPluginFee *_pFee = static_cast<IPluginFee *>(PluginManager::shared()->loadPlugin(className.c_str(), kPluginFee));
        if (_pFee) {
            _pluginsFeeMap[_pFee->getPluginId()] = _pFee;
            supportPluginIds += _pFee->getPluginId() + ",";
        }
        className = "mock/MockAds";
        _pAds = static_cast<IPluginAds *> (PluginManager::shared()->loadPlugin(className.c_str(), kPluginAds));
        if (_pAds) supportPluginIds += _pAds->getPluginId() + ",";
        supportPluginIds = supportPluginIds.size() == 1 ? "[]" : supportPluginIds.substr(0, supportPluginIds.size() - 1) + "]";
    }

    void SDKManager::release() {
        PluginUtils::resetLogLevel(); //重置日志模式
        JniHelper::callVoidFunc(ONESDK_WRAPPER, "release");
    }

} // onesdk