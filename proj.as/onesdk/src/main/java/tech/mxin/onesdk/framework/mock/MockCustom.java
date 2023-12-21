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
import android.os.Handler;

import tech.mxin.onesdk.framework.SDKHelper;
import tech.mxin.onesdk.framework.SDKLogger;
import tech.mxin.onesdk.framework.SDKWrapper;
import tech.mxin.onesdk.framework.protocol.InterfaceCustom;
import tech.mxin.onesdk.framework.wrapper.CustomWrapper;

public class MockCustom implements InterfaceCustom {
    private static final String TAG = "MockCustom";

    private static final String PLUGIN_ID = "MockCustom";
    private final Context mContext;
    private final MockCustom mAdapter;

    protected static void LogD(String format, Object... args) {
        SDKLogger.logD(TAG, format, args);
    }

    public MockCustom(Context context) {
        mContext = context;
        mAdapter = this;
        SDKWrapper.runOnMainThread(() -> new Handler().postDelayed(() -> CustomResult(CustomWrapper.CUSTOM_RESULT_CUSTOM_EXTENSION,
                "CUSTOMRESULT_CUSTOMREXTENSION"), 1000));
    }

    public void CustomResult(int ret, String msg) {
        CustomWrapper.onCustomResult(mAdapter, ret, msg);
        LogD("CustomResult result : %d msg : %s", ret, msg);
        MockUtils.makeText(mContext, "CustomResult result : %d msg : %s", ret, msg);
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

}
