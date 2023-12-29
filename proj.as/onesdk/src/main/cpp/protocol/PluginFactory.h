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

#ifndef ONESDK_PLUGIN_FACTORY_H
#define ONESDK_PLUGIN_FACTORY_H

#include "../include/IPlugin.h"
#include "../include/PluginDefines.h"

namespace onesdk {

    class PluginFactory {
    public:

        /**
         @brief Get singleton of PluginFactory
         */
        static PluginFactory *shared();

        /**
         @brief Destroy the instance of PluginFactory
         */
        static void purge();

    private:
        friend class PluginManager;

        PluginFactory();

        ~PluginFactory();

        /**
         @brief create the plugin by name and type
         @param the name of plugin
         @param the type of plugin
         */
        IPlugin *createPlugin(const char *name, PluginType pluginType);
    };

} // onesdk

#endif //ONESDK_PLUGIN_FACTORY_H
