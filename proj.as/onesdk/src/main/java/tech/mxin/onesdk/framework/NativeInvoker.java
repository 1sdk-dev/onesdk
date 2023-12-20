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

package tech.mxin.onesdk.framework;

import tech.mxin.onesdk.framework.protocol.InterfaceAds;
import tech.mxin.onesdk.framework.protocol.InterfaceCustom;
import tech.mxin.onesdk.framework.protocol.InterfaceFee;
import tech.mxin.onesdk.framework.protocol.InterfaceUser;

public class NativeInvoker {
    private static native void nativeInit();

    private static native void nativePurge();

    private static native void nativeOnCustomResult(String className, int ret, String msg);

    private static native void nativeOnUserResult(String className, int ret, String msg);

    private static native void nativeOnFeeResult(String className, int ret, String msg);

    private native static void nativeOnAdsResult(String className, int code, String msg);

    private static native void nativeOutputLog(int type, String tag, String msg);

//    private static native String nativeDecode(String str);

    private static native String nativeGetConfigFiles();

    public static void onInit() {
        nativeInit();
    }

    public static void onPurge() {
        nativePurge();
    }

    public static void onCustomResult(final InterfaceCustom adapter, final int ret, final String msg) {
        SDKWrapper.runOnMainThread(() -> {
            String name = adapter.getClass().getName() + adapter.PluginType;
            name = name.replace('.', '/');
            nativeOnCustomResult(name, ret, msg);
        });
    }

    public static void onUserResult(final InterfaceUser obj, final int ret, final String msg) {
        SDKWrapper.runOnMainThread(() -> {
            String name = obj.getClass().getName() + obj.PluginType;
            name = name.replace('.', '/');
            nativeOnUserResult(name, ret, msg);
        });
    }

    public static void onFeeResult(final InterfaceFee obj, final int ret, final String msg) {
        SDKWrapper.runOnMainThread(() -> {
            String name = obj.getClass().getName() + obj.PluginType;
            name = name.replace('.', '/');
            nativeOnFeeResult(name, ret, msg);
        });
    }

    public static void onAdsResult(final InterfaceAds obj, final int ret, final String msg) {
        SDKWrapper.runOnMainThread(() -> {
            String name = obj.getClass().getName() + obj.PluginType;
            name = name.replace('.', '/');
            nativeOnAdsResult(name, ret, msg);
        });
    }

    public static void outputLog(int type, String tag, String msg) {
        nativeOutputLog(type, tag, msg);
    }

    public static String getConfigFiles() {
        return nativeGetConfigFiles();
    }
}
