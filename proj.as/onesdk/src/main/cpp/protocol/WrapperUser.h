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

#ifndef ONESDK_WRAPPER_USER_H
#define ONESDK_WRAPPER_USER_H

#include <string>
#include <vector>

#include "../include/IPluginUser.h"

namespace onesdk {

    typedef struct _UserActionResult_ {
        UserResultCode code;
        std::string msg;
        std::string className;
    } UserActionResult;

    class WrapperUser : public IPluginUser {
    public:
        WrapperUser();

        virtual ~WrapperUser();

        virtual void login();

        virtual void logout();

        virtual void submitScore(TLeaderBoardInfo info);

        virtual void showLeaderBoard(TLeaderBoardInfo info);

        virtual void unlockAchievement(TAchievementInfo achInfo);

        virtual void showAchievements(TAchievementInfo achInfo);

        virtual void showToolBar(ToolBarPlace place = kToolBarTopLeft);

        virtual void hideToolBar();

        virtual void setUserInfo(StringMap userInfo);

        virtual StringMap getUserInfo();

        virtual void setListener(UserListener *listener);

        virtual UserListener *getListener();

        virtual void setPluginName(const char *name);

        virtual std::string getPluginName();

        static void pushActionResult(UserActionResult res);

        static void popActionResult();

    protected:
        UserListener *_listener;
        std::string _pluginName;
        static std::vector<UserActionResult> _listenerResult;
    };

} // onesdk

#endif //ONESDK_WRAPPER_USER_H
