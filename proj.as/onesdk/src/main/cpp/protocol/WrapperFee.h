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

#ifndef ONESDK_WRAPPER_FEE_H
#define ONESDK_WRAPPER_FEE_H

#include "../include/IPluginFee.h"

namespace onesdk {

    typedef struct {
        FeeResultCode code;
        std::string msg;
        std::string className;
    } FeeActionResult;

    class WrapperFee : public IPluginFee {
    public:
        WrapperFee();

        virtual ~WrapperFee();

        virtual void feeForProduct(TProductInfo info);

        virtual void setListener(FeeListener *pListener);

        virtual FeeListener *getListener();

        virtual void setPluginName(const char *name);

        virtual std::string getPluginName();

        static void pushActionResult(FeeActionResult res);

        static void popActionResult();

    protected:
        FeeListener *_listener;
        std::string _pluginName;
        static std::vector<FeeActionResult> _listenerResult;
    };

} // onesdk

#endif //ONESDK_WRAPPER_FEE_H
