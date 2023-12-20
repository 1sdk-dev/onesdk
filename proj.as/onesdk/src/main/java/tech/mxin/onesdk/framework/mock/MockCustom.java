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
import android.os.Handler;

import java.lang.reflect.Method;

import tech.mxin.onesdk.framework.SDKLogger;
import tech.mxin.onesdk.framework.SDKHelper;
import tech.mxin.onesdk.framework.SDKWrapper;
import tech.mxin.onesdk.framework.protocol.InterfaceCustom;
import tech.mxin.onesdk.framework.wrapper.CustomWrapper;

public class MockCustom implements InterfaceCustom {

    private Context mContext = null;
    protected static String TAG = "MockCustom";
    private MockCustom mAdapter = null;

    protected static void LogE(String msg, Exception e) {
        try {
            SDKLogger.logE(TAG, msg, e);
        } catch (Exception e2) {
            SDKLogger.logE(TAG, msg, e2);
        }

    }

    protected static void LogD(String msg) {
        try {
            SDKLogger.logD(TAG, msg);
        } catch (Exception e) {
            SDKLogger.logE(TAG, msg, e);
        }

    }

    public MockCustom(Context context) {
        mContext = context;
        mAdapter = this;
        SDKWrapper.runOnMainThread(() -> new Handler().postDelayed(() -> CustomResult(CustomWrapper.CUSTOM_RESULT_CUSTOM_EXTENSION, "CUSTOMRESULT_CUSTOMREXTENSION"), 1000));
    }

    public void CustomResult(int ret, String msg) {
        CustomWrapper.onCustomResult(mAdapter, ret, msg);
        LogD("CustomResult result : " + ret + " msg : " + msg);
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
        return "MockCustom";
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
