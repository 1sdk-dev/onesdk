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

#ifndef ONESDK_I_PLUGIN_H
#define ONESDK_I_PLUGIN_H

#include <vector>
#include <string>

namespace onesdk {

    class IPlugin {
    public:
        virtual ~IPlugin();

        /**
         *@brief set plugin name
         *@param the name of plugin
        */
        virtual void setPluginName(const char *name) = 0;

        /**
         *@brief get plugin name
         *@return the name of plugin
        */
        virtual std::string getPluginName() = 0;

        /**
         @brief get plugin id
         @return the plugin id
         */
        virtual std::string getPluginId();

        /**
         *@brief get the version of plugin
         *@return the version of plugin
        */
        virtual std::string getPluginVersion();

        /**
         *@brief get the version of sdk
         *@return the version of sdk
        */
        virtual std::string getSDKVersion();

        virtual bool isFuncSupported(const char *funcName);

        virtual std::string getPluginMethods();

        template<typename... Ts>
        void callVoidFunc(const char *funcName, Ts... ts);

        template<typename... Ts>
        bool callBoolFunc(const char *funcName, Ts... ts);

        template<typename... Ts>
        std::unique_ptr<bool[]> callBoolArrayFunc(const char *funcName, Ts... ts);

        template<typename... Ts>
        int callIntFunc(const char *funcName, Ts... ts);

        template<typename... Ts>
        std::unique_ptr<int[]> callIntArrayFunc(const char *funcName, Ts... ts);

        template<typename... Ts>
        long callLongFunc(const char *funcName, Ts... ts);

        template<typename... Ts>
        std::unique_ptr<long[]> callLongArrayFunc(const char *funcName, Ts... ts);

        template<typename... Ts>
        float callFloatFunc(const char *funcName, Ts... ts);

        template<typename... Ts>
        std::unique_ptr<float[]> callFloatArrayFunc(const char *funcName, Ts... ts);

        template<typename... Ts>
        std::string callStringFunc(const char *funcName, Ts... ts);
    };

} // onesdk

#endif //ONESDK_I_PLUGIN_H
