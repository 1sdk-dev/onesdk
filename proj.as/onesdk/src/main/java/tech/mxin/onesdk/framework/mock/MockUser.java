/**
 * Copyright 2023 xin.yang
 * <p>
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * <p>
 * http://www.apache.org/licenses/LICENSE-2.0
 * <p>
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package tech.mxin.onesdk.framework.mock;

import android.app.AlertDialog;
import android.content.Context;
import android.os.Handler;

import java.util.Hashtable;

import tech.mxin.onesdk.framework.SDKHelper;
import tech.mxin.onesdk.framework.SDKLogger;
import tech.mxin.onesdk.framework.SDKUtils;
import tech.mxin.onesdk.framework.SDKWrapper;
import tech.mxin.onesdk.framework.protocol.InterfaceUser;
import tech.mxin.onesdk.framework.wrapper.UserWrapper;

public class MockUser implements InterfaceUser {
    private static final String TAG = "MockUser";
    private static final String PLUGIN_ID = "MockUser";
    private final Context mContext;
    private final MockUser mAdapter;
    private boolean isInited = false;

    private Hashtable<String, String> userInfo = null;

    private static void LogD(String format, Object... args) {
        SDKLogger.logD(TAG, format, args);
    }

    public MockUser(Context context) {
        mContext = context;
        mAdapter = this;
        configPluginParams(SDKHelper.getPluginParams());
    }

    private void configPluginParams(Hashtable<String, String> cpInfo) {
        SDKWrapper.runOnMainThread(() -> {
            if (isInited) return;
            new Handler().postDelayed(() -> {
                isInited = true;
                userResult(UserWrapper.ACTION_RET_INIT_SUCCESS, "init success");
            }, 1000);
        });
    }

    @Override
    public void login() {
        LogD("login() invoked!");
        SDKWrapper.runOnMainThread(() -> {
            if (!isInited) {
                userResult(UserWrapper.ACTION_RET_LOGIN_FAIL, "not init");
                return;
            }
            if (!SDKUtils.networkReachable(mContext)) {
                userResult(UserWrapper.ACTION_RET_LOGIN_TIMEOUT, "network is unreachable");
                return;
            }
            MockUtils.alertLogin(mContext, new ILoginCallback() {
                @Override
                public void success(String username) {
                    MockUtils.setLoginState(true);
                    Hashtable<String, String> user = new Hashtable<>();
                    user.put("userID", "userID11111111");
                    user.put("userName", username);
                    user.put("userLevel", "userLevel-1");
                    setUserInfo(user);
                    userResult(UserWrapper.ACTION_RET_LOGIN_SUCCESS, "success");
                }

                @Override
                public void failed(String reason) {
                    MockUtils.setLoginState(false);
                    MockUtils.makeText(mContext, reason);
                    userResult(UserWrapper.ACTION_RET_LOGIN_FAIL, reason);
                }

                @Override
                public void cancel() {
                    MockUtils.setLoginState(false);
                    MockUtils.makeText(mContext, "the login has been canceled");
                    userResult(UserWrapper.ACTION_RET_LOGIN_CANCEL, "the login has been canceled");
                }
            });
        });
    }

    @Override
    public void logout() {
        LogD("logout() invoked!");
        MockUtils.makeText(mContext, "logout() invoked!");
        SDKWrapper.runOnMainThread(() -> {
            if (!MockUtils.getLoginState()) {
                userResult(UserWrapper.ACTION_RET_LOGOUT_FAIL, "not need logout");
                LogD("User not logined!");
                MockUtils.makeText(mContext, "User not logined!");
                return;
            }
            userResult(UserWrapper.ACTION_RET_LOGOUT_SUCCESS, "");
        });

    }

    public void pause() {
        LogD("pause() invoked!");
        MockUtils.makeText(mContext, "pause() invoked!");
    }

    public void exit() {
        LogD("exit() invoked!");
        SDKWrapper.runOnMainThread(() -> {
            AlertDialog.Builder dialog02 = new AlertDialog.Builder(mContext);
            dialog02.setTitle("Exit");
            dialog02.setMessage("Exit");
            dialog02.setPositiveButton("Confirm", (dialogInterface, i) -> userResult(UserWrapper.ACTION_RET_EXIT_PAGE, "exit"));
            dialog02.setNegativeButton("Cancel", null).create();
            dialog02.show();
        });
    }

    public void enterPlatform() {
        LogD("enterPlatform() invoked!");
        MockUtils.makeText(mContext, "enterPlatform() invoked!");

    }

    @Override
    public void showToolBar(int place) {
        LogD("showToolBar(" + place + ") invoked!");
        MockUtils.makeText(mContext, "showToolBar(" + place + ") invoked!");
    }

    @Override
    public void hideToolBar() {
        LogD("hideToolBar() invoked!");
        MockUtils.makeText(mContext, "hideToolBar() invoked!");
    }

    @Override
    public void setUserInfo(Hashtable<String, String> userInfo) {
        this.userInfo = userInfo;
        LogD("setUserInfo(" + userInfo.toString() + ") invoked!");
        MockUtils.makeText(mContext, "setUserInfo(" + userInfo + ") invoked!");
    }

    @Override
    public Hashtable<String, String> getUserInfo() {
        LogD("getUserInfo() invoked!");
        MockUtils.makeText(mContext, "getUserInfo() invoked!");
        return this.userInfo;
    }

    @Override
    public Hashtable<String, String> getUserStateInfo() {
        return this.userInfo;
    }

    public void antiAddictionQuery() {
        LogD("antiAddictionQuery() invoked!");
        MockUtils.makeText(mContext, "antiAddictionQuery() invoked!");
    }

    public void realNameRegister() {
        LogD("realNameRegister() invoked!");
        MockUtils.makeText(mContext, "realNameRegister() invoked!");
    }

    public void submitLoginGameRole(Hashtable<String, String> info) {
        LogD("submitLoginGameRole(" + info.toString() + ")invoked");
        MockUtils.makeText(mContext, "submitLoginGameRole(" + info + ")invoked");
    }

    public void accountSwitch() {
        LogD("accountSwitch() invoked!");
        MockUtils.makeText(mContext, "accountSwitch() invoked!");
    }

    @Override
    public void submitScore(Hashtable<String, String> info) {
        LogD("submitScore(" + info.toString() + ")invoked!");
        MockUtils.makeText(mContext, "submitScore(" + info + ")invoked!");
    }

    @Override
    public void showLeaderBoard(Hashtable<String, String> info) {
        LogD("showLeaderBoard(" + info.toString() + ")invoked!");
        MockUtils.makeText(mContext, "showLeaderBoard(" + info + ")invoked!");
    }

    @Override
    public void unlockAchievement(Hashtable<String, String> achInfo) {
        LogD("unlockAchievement(" + achInfo.toString() + ")invoked!");
        MockUtils.makeText(mContext, "unlockAchievement(" + achInfo + ")invoked!");
    }

    @Override
    public void showAchievements(Hashtable<String, String> achInfo) {
        LogD("showAchievements(" + achInfo.toString() + ") invoked!");
        MockUtils.makeText(mContext, "showAchievements(" + achInfo + ") invoked!");
    }

    private void userResult(int ret, String msg) {
        UserWrapper.onUserResult(mAdapter, ret, msg);
        LogD("userResult : %d msg : %s", ret, msg);
    }

    @Override
    public String getSDKVersion() {
        LogD("getSDKVersion() invoked!");
        return SDKHelper.VERSION;
    }

    @Override
    public String getPluginVersion() {
        LogD("getPluginVersion() invoked!");
        return SDKHelper.VERSION;
    }

    @Override
    public String getPluginId() {
        LogD("getPluginId() invoked!");
        return PLUGIN_ID;
    }


}
