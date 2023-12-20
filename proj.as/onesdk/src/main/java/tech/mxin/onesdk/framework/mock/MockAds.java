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

import android.content.Context;
import android.widget.Toast;

import java.lang.reflect.Method;
import java.util.Hashtable;

import tech.mxin.onesdk.framework.SDKLogger;
import tech.mxin.onesdk.framework.SDKHelper;
import tech.mxin.onesdk.framework.SDKWrapper;
import tech.mxin.onesdk.framework.protocol.InterfaceAds;

public class MockAds implements InterfaceAds {

    private Context mContext = null;
    private static final String TAG = "MockAds";

    private static void LogE(String msg, Exception e) {
        try {
            SDKLogger.logE(TAG, msg, e);
        } catch (Exception e2) {
            e.printStackTrace();
        }

    }

    private static void LogD(String msg) {
        try {
            SDKLogger.logD(TAG, msg);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public MockAds(Context context) {
        mContext = context;
    }

    @Override
    public String getSDKVersion() {
        LogD("getSDKVersion() invoked!");
        SDKWrapper.runOnMainThread(() -> Toast.makeText(mContext, "getSDKVersion() invoked!", Toast.LENGTH_SHORT).show());
        return SDKHelper.VERSION;
    }

    @Override
    public String getPluginVersion() {
        LogD("getPluginVersion() invoked!");
        SDKWrapper.runOnMainThread(() -> Toast.makeText(mContext, "getPluginVersion() invoked!", Toast.LENGTH_SHORT).show());
        return SDKHelper.VERSION;
    }

    @Override
    public void showAds(Hashtable<String, String> info) {
        LogD("showAds(" + info.toString() + ")invoked!");
        SDKWrapper.runOnMainThread(() -> Toast.makeText(mContext, "showAds(" + info.toString() + ")invoked!", Toast.LENGTH_SHORT).show());
    }

    @Override
    public void hideAds(Hashtable<String, String> info) {
        LogD("hideAds(" + info.toString() + ")invoked!");
        SDKWrapper.runOnMainThread(() -> Toast.makeText(mContext, "hideAds(" + info.toString() + ")invoked!", Toast.LENGTH_SHORT).show());
    }

    @Override
    public void preloadAds(Hashtable<String, String> info) {
        LogD("preloadAds(" + info.toString() + ")invoked!");
        LogD("debug not support preloadAds");
        SDKWrapper.runOnMainThread(() -> Toast.makeText(mContext, "preloadAds(" + info.toString() + ")invoked!", Toast.LENGTH_SHORT).show());
    }

    @Override
    public Hashtable<String, String> getAdsStateInfo() {
        Hashtable<String, String> adsInfo = new Hashtable<>();
        adsInfo.put("id", "11111111");
        adsInfo.put("type", "banner");
        adsInfo.put("profit", "1");
        return adsInfo;
    }

    @Override
    public String getPluginId() {
        LogD("getPluginId() invoked!");
        SDKWrapper.runOnMainThread(() -> Toast.makeText(mContext, "getPluginId() invoked!", Toast.LENGTH_SHORT).show());
        return "MockAds";
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

}
