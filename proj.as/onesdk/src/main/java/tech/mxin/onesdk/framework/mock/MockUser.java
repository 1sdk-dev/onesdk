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

package tech.mxin.onesdk.framework.mock;

import android.app.AlertDialog;
import android.content.Context;
import android.os.Handler;
import android.widget.EditText;
import android.widget.Toast;

import java.lang.reflect.Method;
import java.util.Hashtable;

import tech.mxin.onesdk.framework.SDKLogger;
import tech.mxin.onesdk.framework.SDKHelper;
import tech.mxin.onesdk.framework.SDKWrapper;
import tech.mxin.onesdk.framework.SDKUtils;
import tech.mxin.onesdk.framework.protocol.InterfaceUser;
import tech.mxin.onesdk.framework.wrapper.UserWrapper;

public class MockUser implements InterfaceUser {
    private static Context mContext = null;
    private static String TAG = "MockUser";
    private MockUser mAdapter = null;
    private boolean isInited = false;
    private static boolean mLogined = false;
    private static String mUserId = "";
    private static String mSessionId = "";
    private Hashtable<String, String> userInfo = null;

    private static void LogE(String msg, Exception e) {
        try {
            SDKLogger.logE(TAG, msg, e);
        } catch (Exception e2) {
            SDKLogger.logE(TAG, msg, e2);
        }
    }

    private static void LogD(String msg) {
        try {
            SDKLogger.logD(TAG, msg);
        } catch (Exception e) {
            SDKLogger.logE(TAG, msg, e);
        }
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
                UserWrapper.onUserResult(mAdapter, UserWrapper.ACTION_RET_INIT_SUCCESS, "init success");
            }, 1000);
        });
    }

    @Override
    public void login() {
        LogD("login() invoked!");
        SDKWrapper.runOnMainThread(() -> {
            if (!isInited) {
                UserWrapper.onUserResult(mAdapter, UserWrapper.ACTION_RET_LOGIN_FAIL, "not init");
                return;
            }
            if (!SDKUtils.networkReachable(mContext)) {
                UserWrapper.onUserResult(mAdapter, UserWrapper.ACTION_RET_LOGIN_TIMEOUT, "network is unreachable");
                return;
            }
            EditText editText = new EditText(mContext);
            editText.setHint("Please input username");
            AlertDialog.Builder dialog = new AlertDialog.Builder(mContext);
            dialog.setTitle("Login").setView(editText).setCancelable(false);
            dialog.setPositiveButton("Login", (dialogInterface, i) -> {
                if (editText.getText().toString().isEmpty()) {
                    UserWrapper.onUserResult(mAdapter, UserWrapper.ACTION_RET_LOGIN_FAIL, "username  is empty");
                    return;
                }
                userLogin(editText.getText().toString(), "password", (code, msg) -> {
                    if (code == UserWrapper.ACTION_RET_LOGIN_SUCCESS) {
                        mLogined = true;
                        Hashtable<String, String> user = new Hashtable<>();
                        user.put("userID", "userID11111111");
                        user.put("userName", "userName");
                        user.put("userLevel",  "userLevel-1");
                        setUserInfo(user);
                        UserWrapper.onUserResult(mAdapter, UserWrapper.ACTION_RET_LOGIN_SUCCESS, msg);
                    } else {
                        mLogined = false;
                        UserWrapper.onUserResult(mAdapter, UserWrapper.ACTION_RET_LOGIN_FAIL, msg);
                    }
                });
            });
            dialog.setNegativeButton("Cancel", (dialogInterface, i) -> {
                SDKWrapper.runOnMainThread(() -> Toast.makeText(mContext, "the login has been canceled", Toast.LENGTH_SHORT).show());
                UserWrapper.onUserResult(mAdapter, UserWrapper.ACTION_RET_LOGIN_CANCEL, "the login has been canceled");
            });
            dialog.create().show();
        });
    }

    @Override
    public void login(Hashtable<String, String> extension) {
        login();
    }

    private void userLogin(String name, String password, final ILoginCallback callback) {
        callback.onResult(UserWrapper.ACTION_RET_LOGIN_SUCCESS, "success");
    }

    @Override
    public void logout() {
        LogD("logout() invoked!");
        SDKWrapper.runOnMainThread(() -> Toast.makeText(mContext, "logout() invoked!", Toast.LENGTH_SHORT).show());
        SDKWrapper.runOnMainThread(() -> {
            if (!mLogined) {
                UserWrapper.onUserResult(mAdapter, UserWrapper.ACTION_RET_LOGOUT_FAIL, "not need logout");
                LogD("User not logined!");
                SDKWrapper.runOnMainThread(() -> Toast.makeText(mContext, "User not logined!", Toast.LENGTH_SHORT).show());
                return;
            }
            userLogout();
        });

    }

    private void userLogout() {
        UserWrapper.onUserResult(mAdapter, UserWrapper.ACTION_RET_LOGOUT_SUCCESS, "");
    }

    public void pause() {
        LogD("pause() invoked!");
        SDKWrapper.runOnMainThread(() -> Toast.makeText(mContext, "pause() invoked!", Toast.LENGTH_SHORT).show());
    }

    public void exit() {
        LogD("exit() invoked!");
        SDKWrapper.runOnMainThread(() -> {
            AlertDialog.Builder dialog02 = new AlertDialog.Builder(mContext);
            dialog02.setTitle("Exit");
            dialog02.setMessage("Exit");
            dialog02.setPositiveButton("Confirm", (dialogInterface, i) -> UserWrapper.onUserResult(mAdapter, UserWrapper.ACTION_RET_EXIT_PAGE, "exit"));
            dialog02.setNegativeButton("Cancel", null).create();
            dialog02.show();
        });
    }

    public void enterPlatform() {
        LogD("enterPlatform() invoked!");
        SDKWrapper.runOnMainThread(() -> Toast.makeText(mContext, "enterPlatform() invoked!", Toast.LENGTH_SHORT).show());

    }

    @Override
    public void showToolBar(int place) {
        LogD("showToolBar(" + place + ") invoked!");
        SDKWrapper.runOnMainThread(() -> Toast.makeText(mContext, "showToolBar(" + place + ") invoked!", Toast.LENGTH_SHORT).show());
    }

    @Override
    public void hideToolBar() {
        LogD("hideToolBar() invoked!");
        SDKWrapper.runOnMainThread(() -> Toast.makeText(mContext, "hideToolBar() invoked!", Toast.LENGTH_SHORT).show());
    }

    @Override
    public void setUserInfo(Hashtable<String, String> userInfo) {
        this.userInfo = userInfo;
        LogD("setUserInfo(" + userInfo.toString() + ") invoked!");
        SDKWrapper.runOnMainThread(() -> Toast.makeText(mContext, "setUserInfo(" + userInfo + ") invoked!", Toast.LENGTH_SHORT).show());
    }

    @Override
    public Hashtable<String, String> getUserInfo() {
        LogD("getUserInfo() invoked!");
        SDKWrapper.runOnMainThread(() -> Toast.makeText(mContext, "getUserInfo() invoked!", Toast.LENGTH_SHORT).show());
        return this.userInfo;
    }

    @Override
    public Hashtable<String, String> getUserStateInfo() {
        return this.userInfo;
    }

    public void antiAddictionQuery() {
        LogD("antiAddictionQuery() invoked!");
        SDKWrapper.runOnMainThread(() -> Toast.makeText(mContext, "antiAddictionQuery() invoked!", Toast.LENGTH_SHORT).show());
    }

    public void realNameRegister() {
        LogD("realNameRegister() invoked!");
        SDKWrapper.runOnMainThread(() -> Toast.makeText(mContext, "realNameRegister() invoked!", Toast.LENGTH_SHORT).show());
    }

    public void submitLoginGameRole(Hashtable<String, String> info) {
        LogD("submitLoginGameRole(" + info.toString() + ")invoked");
        SDKWrapper.runOnMainThread(() -> Toast.makeText(mContext, "submitLoginGameRole(" + info + ")invoked", Toast.LENGTH_SHORT).show());
    }

    public void accountSwitch() {
        LogD("accountSwitch() invoked!");
        SDKWrapper.runOnMainThread(() -> Toast.makeText(mContext, "accountSwitch() invoked!", Toast.LENGTH_SHORT).show());
    }

    public static void multiParam(int a, int b, String c) {
        LogD("multiParam() invoked!" + a + "," + b + "," + c);
    }

    @Override
    public void submitScore(Hashtable<String, String> info) {
        LogD("submitScore(" + info.toString() + ")invoked!");
        SDKWrapper.runOnMainThread(() -> Toast.makeText(mContext, "submitScore(" + info + ")invoked!", Toast.LENGTH_SHORT).show());
    }

    @Override
    public void showLeaderBoard(Hashtable<String, String> info) {
        LogD("showLeaderBoard(" + info.toString() + ")invoked!");
        SDKWrapper.runOnMainThread(() -> Toast.makeText(mContext, "showLeaderBoard(" + info + ")invoked!", Toast.LENGTH_SHORT).show());
    }

    @Override
    public void unlockAchievement(Hashtable<String, String> achInfo) {
        LogD("unlockAchievement(" + achInfo.toString() + ")invoked!");
        SDKWrapper.runOnMainThread(() -> Toast.makeText(mContext, "unlockAchievement(" + achInfo + ")invoked!", Toast.LENGTH_SHORT).show());
    }

    @Override
    public void showAchievements(Hashtable<String, String> achInfo) {
        LogD("showAchievements(" + achInfo.toString() + ") invoked!");
        SDKWrapper.runOnMainThread(() -> Toast.makeText(mContext, "showAchievements(" + achInfo + ") invoked!", Toast.LENGTH_SHORT).show());
    }

    @Override
    public String getSDKVersion() {
        LogD("getSDKVersion() invoked!");
        SDKWrapper.runOnMainThread(() -> Toast.makeText(mContext, "showAchievements() invoked!", Toast.LENGTH_SHORT).show());
        return SDKHelper.VERSION;
    }

    @Override
    public String getPluginVersion() {
        LogD("getPluginVersion() invoked!");
        SDKWrapper.runOnMainThread(() -> Toast.makeText(mContext, "getPluginVersion() invoked!", Toast.LENGTH_SHORT).show());
        return SDKHelper.VERSION;
    }

    @Override
    public boolean isFuncSupported(String functionName) {
        LogD("isFunctionSupported(" + functionName + ")invoked!");
        Method[] methods = this.getClass().getMethods();
        for (Method method : methods) {
            if (method.getName().equals(functionName)) return true;
        }
        return false;
    }

    @Override
    public String getPluginId() {
        LogD("getPluginId() invoked!");
        SDKWrapper.runOnMainThread(() -> Toast.makeText(mContext, "getPluginId() invoked!", Toast.LENGTH_SHORT).show());
        return "MockUser";
    }

    public static boolean getLoginState() {
        LogD("getLoginState() invoked!");
        SDKWrapper.runOnMainThread(() -> Toast.makeText(mContext, "getLoginState() invoked!", Toast.LENGTH_SHORT).show());
        return mLogined;
    }

    public static void setLoginState(boolean flag) {
        LogD("setLoginState() invoked!");
        SDKWrapper.runOnMainThread(() -> Toast.makeText(mContext, "setLoginState() invoked!", Toast.LENGTH_SHORT).show());
        mLogined = flag;
    }

    public static String getSimUserId() {
        LogD("getSimUserId() invoked!");
        SDKWrapper.runOnMainThread(() -> Toast.makeText(mContext, "getSimUserId() invoked!", Toast.LENGTH_SHORT).show());
        return mUserId;
    }

    public static String getSimSessionId() {
        LogD("getSimSessionId() invoked!");
        SDKWrapper.runOnMainThread(() -> Toast.makeText(mContext, "getSimSessionId() invoked!", Toast.LENGTH_SHORT).show());
        return mSessionId;
    }

    public static void setSimUserInfo(String userId, String sessionId) {
        LogD("setSimUserInfo(" + userId + ", " + sessionId + ") invoked!");
        SDKWrapper.runOnMainThread(() -> Toast.makeText(mContext, "setSimUserInfo(" + userId + ", " + sessionId + ") invoked!", Toast.LENGTH_SHORT).show());
        mUserId = userId;
        mSessionId = sessionId;
    }

}
