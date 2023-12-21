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

import android.content.Context;

import java.util.Hashtable;

import tech.mxin.onesdk.framework.SDKHelper;
import tech.mxin.onesdk.framework.SDKLogger;
import tech.mxin.onesdk.framework.protocol.InterfaceAds;

public class MockAds implements InterfaceAds {
    private static final String TAG = "MockAds";

    private static final String PLUGIN_ID = "MockAds";

    private final Context mContext;

    private static void LogD(String format, Object... args) {
        SDKLogger.logD(TAG, format, args);
    }

    public MockAds(Context context) {
        mContext = context;
    }

    @Override
    public String getPluginId() {
        LogD("getPluginId() invoked!");
        return PLUGIN_ID;
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
    public void showAds(Hashtable<String, String> info) {
        LogD("showAds(%s)invoked!", info.toString());
        MockUtils.makeText(mContext, "showAds(%s)invoked!", info.toString());
    }

    @Override
    public void hideAds(Hashtable<String, String> info) {
        LogD("hideAds(%d)invoked!", info.toString());
        MockUtils.makeText(mContext, "hideAds(%d)invoked!", info.toString());
    }

    @Override
    public void preloadAds(Hashtable<String, String> info) {
        LogD("preloadAds(%s)invoked!", info.toString());
        LogD("debug not support preloadAds");
        MockUtils.makeText(mContext, "preloadAds(%s)invoked!", info.toString());
    }

}
