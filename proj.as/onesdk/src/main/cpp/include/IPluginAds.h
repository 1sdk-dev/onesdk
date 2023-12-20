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

#ifndef ONESDK_I_PLUGIN_ADS_H
#define ONESDK_I_PLUGIN_ADS_H

#include "PluginDefines.h"
#include "IPlugin.h"

namespace onesdk {

    /// typedef TAdsInfo.
    typedef StringMap TAdsInfo;
    /** @brief AdsResultCode enum, with inline docs */
    typedef enum {
        kAdsReceived = 0,               /**< enum the callback: the ad is received is at center. */
        kAdsShown = 1,                  /**< enum the callback: the advertisement dismissed. */
        kAdsDismissed = 2,              /**< enum the callback: the advertisement dismissed. */
        kPointsSpendSucceed = 3,        /**< enum the callback: the points spend succeed. */
        kPointsSpendFailed = 4,         /**< enum the callback: the points spend failed. */
        kNetworkError = 5,              /**< enum the callback of Network error at center. */
        kUnknownError = 6,              /**< enum the callback of Unknown error. */
        kOfferWallOnPointsChanged = 7,  /**< enum the callback of Changing the point of offer wall. */
        kRewardedVideoWithReward = 8,   /**< enum the callback of receiving the reward of rewarded video. */
        kFeeFinished = 9,               /**< enum the callback of finishing Fee ad. */
        kAdsClicked = 10,               /**< enum the callback of the advertisement clicked. */
        kAdsFailed = 11,                /**< enum the callback of the ads info empty. */
        kAdsPreloadFailed = 12,         /**< enum the callback of the ads preload failed. */
        kAdsShownFailed = 13,           /**< enum the callback of the ads shown failed. */
        kAdsRetryPreload = 14,          /**< enum the callback of the ads should retry preload function by user. */
        kAdsOnLeave = 15,               /**< enum the callback of the ads on leave. */
        kAdsOnAdImpression = 16,        /**< enum the callback of the ads on impression. */
        kAdsExtension = 40000           /**< enum value is extension code . */
    } AdsResultCode;
    /** @brief AdsPos enum, with inline docs */
    typedef enum {
        kPosBottom = 0,                 /**< enum the toolbar is at bottom. */
        kPosCenter = 1,                 /**< enum the toolbar is at center. */
        kPosTop = 2,                    /**< enum the toolbar is at top. */
    } AdsPos;

    class AdsListener {
    public:
        /**
         *@brief The advertisement request result
         *@param the id of callback
         *@param the information of callback
         */
        virtual void onAdsResult(AdsResultCode code, const char *msg) = 0;
    };

    class IPluginAds : public IPlugin {
    public:

        /**
        @brief show ad view
        @param the info of ads
        */
        virtual void showAds(TAdsInfo adsInfo) = 0;

        /**
        @brief Hide the ad view
        @param the info of ads
        */
        virtual void hideAds(TAdsInfo adsInfo) = 0;

        /**
        @brief preload the ad view
        @param the info of ads
        */
        virtual void preloadAds(TAdsInfo adsInfo) = 0;

        /**
         @brief set the Ads listener
         @param pListener The callback object for Ads result
         */
        virtual void setListener(AdsListener *listener) = 0;

        /**
         @brief get pListener The callback object for Ads result
         @return the listener
         */
        virtual AdsListener *getListener() = 0;
    };

} // onesdk

#endif //ONESDK_I_PLUGIN_ADS_H
