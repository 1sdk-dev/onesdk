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

#include "PluginFactory.h"
#include "PluginUtils.h"
#include "WrapperAds.h"
#include "WrapperFee.h"
#include "WrapperUser.h"
#include "WrapperCustom.h"

#define ANDROID_PLUGIN_PACKAGE_PREFIX "tech/mxin/onesdk/framework/"

namespace onesdk {

    static PluginFactory *s_pPluginFactory = nullptr;

    PluginFactory::PluginFactory() {}

    PluginFactory::~PluginFactory() {}

    PluginFactory *PluginFactory::shared() {
        if (nullptr == s_pPluginFactory) s_pPluginFactory = new PluginFactory();
        return s_pPluginFactory;
    }

    void PluginFactory::purge() {
        if (nullptr != s_pPluginFactory) {
            delete s_pPluginFactory;
            s_pPluginFactory = nullptr;
        }
    }

    IPlugin *PluginFactory::createPlugin(const char *name, PluginType pluginType) {
        IPlugin *pRet = nullptr;
        do {
            if (name == nullptr || strlen(name) == 0) break;
            std::string jClassName = ANDROID_PLUGIN_PACKAGE_PREFIX;
            jClassName.append(name);
            PluginUtils::outputLog(ANDROID_LOG_DEBUG, "PluginFactory", "Java class name of plugin %s is : %s", name, jClassName.c_str());
            jobject jObj = JniHelper::callObjectFunc("tech/mxin/onesdk/framework/SDKHelper", "initPlugin","Ljava/lang/Object;", jClassName.c_str());
            if (jObj == nullptr) {
                PluginUtils::outputLog(ANDROID_LOG_ERROR, "PluginFactory", "Can't find java class %s", jClassName.c_str());
                break;
            }

            switch (pluginType) {
                case kPluginCustom:
                    pRet = new WrapperCustom();
                    break;
                case kPluginUser:
                    pRet = new WrapperUser();
                    break;
                case kPluginFee:
                    pRet = new WrapperFee();
                    break;
                case kPluginAds:
                    pRet = new WrapperAds();
                    break;
                default:
                    break;
            }
            if (pRet != nullptr) {
                pRet->setPluginName(name);
                PluginUtils::outputLog(ANDROID_LOG_DEBUG, "PluginFactory", "java class %s:%d", jClassName.c_str(), pluginType);
                PluginUtils::initJavaPlugin(pRet, jObj, jClassName.c_str(), pluginType);
            }
        } while (false);

        return pRet;
    }

} // onesdk