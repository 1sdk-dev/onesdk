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

#include "WrapperUser.h"
#include "PluginUtils.h"

namespace onesdk {

    JNI_METHOD(void, nativeOnUserResult)(JNIEnv *env, jclass clazz, jstring className, jint ret, jstring msg) {
        std::string strMsg = JniHelper::jstring2string(msg, env);
        std::string strClassName = JniHelper::jstring2string(className, env);
        IPlugin *pPlugin = PluginUtils::getPluginPtr(strClassName);
        if (pPlugin != nullptr) {
            WrapperUser *pUser = static_cast<WrapperUser *>(pPlugin);
            UserListener *listener = pUser->getListener();
            UserActionResult res;
            res.code = (UserResultCode) ret;
            res.msg = strMsg;
            res.className = strClassName;
            if (nullptr != listener) {
                listener->onUserResult((UserResultCode) ret, strMsg.c_str());
            } else {
                WrapperUser::pushActionResult(res);
                PluginUtils::outputLog(ANDROID_LOG_WARN, "WrapperUser", "callback of plugin %s not set correctly", pPlugin->getPluginName().c_str());
            }
        } else {
            UserActionResult res;
            res.code = (UserResultCode) ret;
            res.msg = strMsg;
            res.className = strClassName;
            WrapperUser::pushActionResult(res);
            PluginUtils::outputLog(ANDROID_LOG_WARN, "WrapperUser", "plugin is null");
        }
    }

    std::vector<UserActionResult> WrapperUser::_listenerResult;

    WrapperUser::WrapperUser() : _listener(nullptr) {}

    WrapperUser::~WrapperUser() { _listenerResult.clear(); }

    void WrapperUser::login() {
        callVoidFunc("login");
    }

    void WrapperUser::logout() {
        callVoidFunc("logout");
    }

    void WrapperUser::submitScore(TLeaderBoardInfo info) {
        if (info.empty()) {
            PluginUtils::outputLog(ANDROID_LOG_DEBUG, "WrapperUser", "The leaderBoard info is empty!");
            return;
        }
        callVoidFunc("submitScore", info);
    }

    void WrapperUser::showLeaderBoard(TLeaderBoardInfo info) {
        if (info.empty()) {
            PluginUtils::outputLog(ANDROID_LOG_DEBUG, "WrapperUser", "The leaderBoard info is empty!");
            return;
        }
        callVoidFunc("showLeaderBoard", info);
    }

    void WrapperUser::unlockAchievement(TAchievementInfo achInfo) {
        if (achInfo.empty()) {
            PluginUtils::outputLog(ANDROID_LOG_DEBUG, "WrapperUser", "The achievement info is empty!");
            return;
        }
        callVoidFunc("showLeaderBoard", achInfo);
    }

    void WrapperUser::showAchievements(TAchievementInfo achInfo) {
        if (achInfo.empty()) {
            PluginUtils::outputLog(ANDROID_LOG_DEBUG, "WrapperUser", "The achievement info is empty!");
            return;
        }
        callVoidFunc("showAchievements", achInfo);
    }

    void WrapperUser::showToolBar(ToolBarPlace place) {
        callVoidFunc("showToolBar", (int) place);
    }

    void WrapperUser::hideToolBar() {
        callVoidFunc("hideToolBar");
    }

    void WrapperUser::setUserInfo(StringMap userInfo) {
        if (userInfo.empty()) {
            PluginUtils::outputLog(ANDROID_LOG_DEBUG, "WrapperUser", "The user info is empty!");
            return;
        }
        callVoidFunc("setUserInfo", userInfo);
    }

    StringMap WrapperUser::getUserInfo() {
        PluginJavaData *pData = PluginUtils::getPluginJavaData(this);
        if (pData == nullptr) return StringMap();
        jobject jMap = JniHelper::callObjectFunc(pData->jobj, pData->jclassName.c_str(), "getUserInfo", "Ljava/util/Hashtable;");
        StringMap userInfo = JniHelper::convertJObjectToStringMap(jMap);
        return userInfo;
    }

    void WrapperUser::setListener(UserListener *listener) {
        _listener = listener;
        popActionResult();
    }

    UserListener *WrapperUser::getListener() {
        return _listener;
    }

    void WrapperUser::setPluginName(const char *name) {
        _pluginName = name;
    }

    std::string WrapperUser::getPluginName() {
        return _pluginName;
    }

    void WrapperUser::pushActionResult(UserActionResult res) {
        _listenerResult.push_back(res);
    }

    void WrapperUser::popActionResult() {
        std::vector<UserActionResult>::iterator iter = _listenerResult.begin();
        while (iter != _listenerResult.end()) {
            UserActionResult res = *iter;
            WrapperUser *pUser = static_cast<WrapperUser *>(PluginUtils::getPluginPtr(res.className));
            if (pUser != nullptr) {
                UserListener *listener = pUser->getListener();
                if (nullptr != listener) {
                    listener->onUserResult(res.code, res.msg.c_str());
                    iter = _listenerResult.erase(iter);
                    continue;
                }
            }
            iter++;
        }
    }
} // onesdk