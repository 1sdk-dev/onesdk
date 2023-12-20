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

#ifndef ONESDK_SDK_MANAGER_H
#define ONESDK_SDK_MANAGER_H

#include <map>
#include <string>

#include "IPlugin.h"
#include "IPluginAds.h"
#include "IPluginCustom.h"
#include "IPluginFee.h"
#include "IPluginUser.h"

namespace onesdk {

    class SDKManager {
    public:

        /**
         @brief Get singleton of SDKManager
         */
        static SDKManager *shared();

        /**
         @brief Destroy the instance of SDKManager
         */
        static void end();

        /**
        @breif the init of SDKManager
        */
        void init(const std::string appId);

        /**
         @brief Get User plugin
         @return  if User plugin exist, return value is User plugin, else return value is null pointer.
         */
        std::map<std::string, IPluginUser *> *getUserPlugins();

        IPluginUser *getUserPlugin(const std::string pluginId = "");

        /**
         @brief Get Fee plugin
         @return  if Fee plugin exist, return value is Fee plugin, else return value is null pointer.
         */
        std::map<std::string, IPluginFee *> *getFeePlugins();

        IPluginFee *getFeePlugin(const std::string pluginId = "");

        /**
         @brief Get Fee plugin
         @return  if Fee plugin exist, return value is Fee plugin, else return value is null pointer.
         */
        std::map<std::string, IPluginCustom *> *getCustomPlugins();

        IPluginCustom *getCustomPlugin(const std::string pluginId = "");

        /**
         @brief Get Ads plugin
         @return  if Ads plugin exist, return value is Ads plugin, else return value is null pointer.
         */
        IPluginAds *getAdsPlugin();

        /**
         @brief Get channel ID
         @return  return value is channel ID.
         */
        const std::string getChannelId();

        /**
         @brief Get framework version
         @return  return value is the version of Cocos SDKHub.
         */
        const std::string getFrameworkVersion();

        /**
         @brief Get plugin support ids
         @return  plugin support ids
         */
        const std::string getSupportPluginIds();

    private:
        SDKManager();

        ~SDKManager();

        void loadAllPlugins();

        void unloadAllPlugins();

        const std::string getSupportPlugin();

        void loadPlugin(const char *name, int type);

        void setDebugMode();

        void release();

        std::map<std::string, IPluginCustom *> _pluginsCustomMap;
        std::map<std::string, IPluginUser *> _pluginsUserMap;
        std::map<std::string, IPluginFee *> _pluginsFeeMap;
        IPluginAds *_pAds;
        bool bIsPluginLoaded;
        std::string supportPluginIds;
    };

} // onesdk

#endif //ONESDK_SDK_MANAGER_H
