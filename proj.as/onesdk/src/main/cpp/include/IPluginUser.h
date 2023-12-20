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

#ifndef ONESDK_I_PLUGIN_USER_H
#define ONESDK_I_PLUGIN_USER_H

#include "PluginDefines.h"
#include "IPlugin.h"

namespace onesdk {

    /// typedef TAchievementInfo.
    typedef StringMap TAchievementInfo;
    /// typedef TLeaderBoardInfo.
    typedef StringMap TLeaderBoardInfo;
    /** @brief Plugin_type enum, with inline docs */
    typedef enum {
        kInitSucceed = 0,                       /**< enum value is callback of succeeding in initing sdk. */
        kInitFailed = 1,                        /**< enum value is callback of failing to init sdk. */
        kLoginSucceed = 2,                      /**< enum value is callback of succeeding in login.*/
        kLoginNetworkError = 3,                 /**< enum value is callback of network error*/
        kLoginNoNeed = 4,                       /**< enum value is callback of no need login.*/
        kLoginFailed = 5,                       /**< enum value is callback of failing to login. */
        kLoginCancel = 6,                       /**< enum value is callback of canceling to login. */
        kLogoutSucceed = 7,                     /**< enum value is callback of succeeding in logout. */
        kLogoutFailed = 8,                      /**< enum value is callback of failing to logout. */
        kPlatformEnter = 9,                     /**< enum value is callback after enter platform. */
        kPlatformBack = 10,                     /**< enum value is callback after exit antiAddiction. */
        kPausePage = 11,                        /**< enum value is callback after exit pause page. */
        kExitPage = 12,                         /**< enum value is callback after exit exit page. */
        kAntiAddictionQuery = 13,               /**< enum value is callback after querying antiAddiction. */
        kRealNameRegister = 14,                 /**< enum value is callback after registering real name. */
        kAccountSwitchSucceed = 15,             /**< enum value is callback of succeeding in switching account. */
        kAccountSwitchFailed = 16,              /**< enum value is callback of failing to switch account. */
        kOpenShop = 17,                         /**< enum value is callback of open the shop. */
        kAccountSwitchCancel = 18,              /**< enum value is callback of canceling to switch account. */
        kGameExitPage = 19,                     /**< enum value is callback of no channel exit page. */
        kScoreSubmitSucceed = 20,               /**< enum value is callback of succeeding in submit. */
        kScoreSubmitFailed = 21,                /**< enum value is callback of failing to submit . */
        kAchUnlockSucceed = 22,                 /**< enum value is callback of succeeding in unlocking. */
        kAchUnlockFailed = 23,                  /**< enum value is callback of failing to  unlock. */
        kShowLeaderBoardSucceed = 24,           /**< enum value is callback of show loaderBoard success.*/
        kShowLeaderBoardFailed = 25,            /**< enum value is callback of show loaderBoard fail.*/
        kShowAchievementSucceed = 26,           /**< enum value is callback of show achievement success.*/
        kShowAchievementFailed = 27,            /**< enum value is callback of show achievement fail.*/
        kServerVerify = 28,                     /**< enum value is callback of server verify.*/
        kUserExtension = 20000                  /**< enum value is  extension code . */
    } UserResultCode;

    /** @brief ToolBarPlace enum, with inline docs */
    typedef enum {
        kToolBarTopLeft = 1,                    /**< enum the toolbar is at topLeft. */
        kToolBarTopRight = 2,                   /**< enum the toolbar is at topRight. */
        kToolBarMidLeft = 3,                    /**< enum the toolbar is at midLeft. */
        kToolBarMidRight = 4,                   /**< enum the toolbar is at midRight. */
        kToolBarBottomLeft = 5,                 /**< enum the toolbar is at bottomLeft. */
        kToolBarBottomRight = 6,                /**< enum the toolbar is at bottomRight. */
    } ToolBarPlace;

    class UserListener {
    public:
        virtual void onUserResult(UserResultCode code, const char *msg) = 0;
    };

    class IPluginUser : public IPlugin {
    public:
        /**
         @brief User login
         */
        virtual void login() = 0;

        /**
         @brief User logout
         */
        virtual void logout() = 0;

        /**
         @brief submit the score
         @param the info of leaderBoard
         */
        virtual void submitScore(TLeaderBoardInfo info) = 0;

        /**
         @brief show the id of LeaderBoard page
         @param the info of achievement
         */
        virtual void showLeaderBoard(TLeaderBoardInfo info) = 0;

        /**
         @brief methods of achievement feature
         @param the info of achievement
         */
        virtual void unlockAchievement(TAchievementInfo achInfo) = 0;

        /**
         @brief show the page of achievements
         */
        virtual void showAchievements(TAchievementInfo achInfo) = 0;

        /**
         @brief show the tool bar
         */
        virtual void showToolBar(ToolBarPlace place = kToolBarTopLeft) = 0;

        /**
         @brief hide the tool bar
         */
        virtual void hideToolBar() = 0;

        /**
         @brief set user login info
         */
        virtual void setUserInfo(StringMap userInfo) = 0;

        /**
         @brief get user login info
         */
        virtual StringMap getUserInfo() = 0;

        /**
         @brief set the result listener
         @param pListener The callback object for user result
         */
        virtual void setListener(UserListener *listener) = 0;

        /**
         @brief get pListener The callback object for user result
         @return the listener
         */
        virtual UserListener *getListener() = 0;
    };

} // onesdk

#endif //ONESDK_I_PLUGIN_USER_H
