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

#ifndef ONESDK_PLUGIN_DEFINES_H
#define ONESDK_PLUGIN_DEFINES_H

#include <map>
#include <string>

namespace onesdk {

    typedef std::map<std::string, std::string> StringMap;

    typedef enum {
        kPluginNull = 0,    /**< enum value is the type of Empty. */
        kPluginCustom = 1,  /**< enum value is the type of Custom. */
        kPluginUser = 2,    /**< enum value is the type of User. */
        kPluginFee = 4,     /**< enum value is the type of Fee. */
        kPluginAds = 8,     /**< enum value is the type of Ads. */
    } PluginType;

} // onesdk

#endif //ONESDK_PLUGIN_DEFINES_H
