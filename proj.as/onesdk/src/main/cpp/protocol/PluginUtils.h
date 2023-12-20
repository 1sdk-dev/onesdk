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

#ifndef ONESDK_PLUGIN_UTILS_H
#define ONESDK_PLUGIN_UTILS_H

#include <map>
#include <list>
#include <vector>
#include <android/log.h>

#include "../include/JniHelper.h"
#include "../include/IPlugin.h"
#include "../include/PluginDefines.h"
#include "PluginMacros.h"

namespace onesdk {

    class PluginUtils {
    public:

        static void initJavaPlugin(IPlugin *pPlugin, jobject jObj, const char *className, int type);

        static PluginJavaData *getPluginJavaData(IPlugin *pKeyObj);

        static void setPluginJavaData(IPlugin *pKeyObj, PluginJavaData *pData, int type);

        static void erasePluginJavaData(IPlugin *pKeyObj, int type);

        static IPlugin *getPluginPtr(std::string className);

        static void outputLog(int type, const char *logTag, const char *pFormat, ...);

        static void output(int type, const char *logTag, const char *contents);

        static void resetLogLevel();

    private:
        static std::string _logLevel;
    };

} // onesdk

#endif //ONESDK_PLUGIN_UTILS_H
