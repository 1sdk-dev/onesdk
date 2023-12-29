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

#ifndef ONESDK_I_PLUGIN_CUSTOM_H
#define ONESDK_I_PLUGIN_CUSTOM_H

#include "IPlugin.h"

namespace onesdk {

    typedef enum {
        kCustomExtension = 10000   /**< enum value is  extension code . */
    } CustomResultCode;

    class CustomListener {
    public:
        virtual void onCustomResult(CustomResultCode code, const char *msg) = 0;
    };

    class IPluginCustom : public IPlugin {
    public:
        virtual void setListener(CustomListener *listener) = 0;

        virtual CustomListener *getListener() = 0;
    };

} // onesdk

#endif //ONESDK_I_PLUGIN_CUSTOM_H
