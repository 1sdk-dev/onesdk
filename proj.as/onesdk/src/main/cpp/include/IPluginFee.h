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

#ifndef ONESDK_I_PLUGIN_FEE_H
#define ONESDK_I_PLUGIN_FEE_H

#include "PluginDefines.h"
#include "IPlugin.h"

namespace onesdk {
    /// typedef TProductInfo.
    typedef StringMap TProductInfo;
    /** @brief FeeResultCode enum, with inline docs */
    typedef enum {
        kFeeSucceed = 0,               /**< enum value is callback of succeeding in feeding . */
        kFeeFailed = 1,                /**< enum value is callback of failing to fee . */
        kFeeCancel = 2,                /**< enum value is callback of canceling to fee . */
        kFeeNetworkError = 3,          /**< enum value is callback of network error . */
        kFeeProductionIncomplete = 4,  /**< enum value is callback of incomplete info . */
        kFeeInitSucceed = 6,           /**< enum value is callback of succeeding in init sdk . */
        kFeeInitFailed = 6,            /**< enum value is callback of failing to init sdk . */
        kFeeNowFeeding = 7,            /**< enum value is callback of feeding now . */
        kFeeRechargeSucceed = 8,       /**< enum value is callback of succeeding in recharging. */
        kFeeExtension = 30000          /**< enum value is extension code . */
    } FeeResultCode;

    /**
     *@class FeeListener
     *@brief the interface of fee callback
     */
    class FeeListener {
    public:
        /**   
         *@brief the interface of fee callback 
         *@param the id of callback
         *@param the information of callback
         *@param the info of fee
         */
        virtual void onFeeResult(FeeResultCode code, const char *msg) = 0;
    };

    /**   
     *@class  IPluginFee
     *@brief the interface of fee   
     */

    class IPluginFee : public IPlugin {
    public:

        /**
        @brief fee for product
        @param info The info of product, must contains key:
                productName         The name of product
                productPrice        The price of product(must can be parse to float)
                productDesc         The description of product
        @warning For different plugin, the parameter should have other keys to fee.
                 Look at the manual of plugins.
        */
        virtual void feeForProduct(TProductInfo info) = 0;

        /**
        @breif set the result listener
        @param pListener The callback object for fee result
        @wraning Must invoke this interface before feeForProduct.
        */
        virtual void setListener(FeeListener *pListener) = 0;

        virtual FeeListener *getListener() = 0;
    };

} // onesdk

#endif //ONESDK_I_PLUGIN_FEE_H
