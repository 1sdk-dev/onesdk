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

package tech.mxin.onesdk.framework;

import android.util.Log;

public class SDKLogger {

    public static String getLogLevel() {
        return SDKHelper.logLevel();
    }

    private static void outputLog(int type, String tag, String msg) {
        NativeInvoker.outputLog(type, tag, msg);
    }

    public static void logV(String tag, String msg) {
        outputLog(Log.VERBOSE, tag, msg);
    }

    public static void logV(String tag, String format, Object... args) {
        logV(tag, String.format(format, args));
    }

    public static void logE(String tag, String msg) {
        outputLog(Log.ERROR, tag, msg);
    }

    public static void logE(String tag, String format, Object... args) {
        logE(tag, String.format(format, args));
    }

    public static void logE(String tag, Exception e, String msg) {
        outputLog(Log.ERROR, tag, msg + "\n" + e.toString());
        e.printStackTrace();
    }

    public static void logE(String tag, Exception e, String format, Object... args) {
        logE(tag, e, String.format(format, args));
    }

    public static void logD(String tag, String msg) {
        outputLog(Log.DEBUG, tag, msg);
    }

    public static void logD(String tag, String format, Object... args) {
        logD(tag, String.format(format, args));
    }

    public static void logI(String tag, String msg) {
        outputLog(Log.INFO, tag, msg);
    }

    public static void logI(String tag, String format, Object... args) {
        logI(tag, String.format(format, args));
    }
}
