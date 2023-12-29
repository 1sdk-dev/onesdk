/**
 * Copyright 2023 xin.yang
 * <p/>
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * <p/>
 * http://www.apache.org/licenses/LICENSE-2.0
 * <p/>
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package tech.mxin.onesdk.framework;

import android.app.Activity;
import android.app.Application;
import android.content.Context;
import android.content.Intent;
import android.content.res.Configuration;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;

import java.lang.ref.WeakReference;
import java.util.Vector;

public class SDKWrapper {
    private static WeakReference<Context> contextRef = null;
    private static WeakReference<Application> applicationRef = null;
    private static Handler mainHandler = null;

    public static void init(Context context) {
        contextRef = new WeakReference<>(context);
        if (null == mainHandler) {
            mainHandler = new Handler(Looper.getMainLooper());
        }
        if (null == applicationRef && context instanceof Activity) {
            applicationRef = new WeakReference<>(((Activity) context).getApplication());
        }
        NativeInvoker.onInit();
        SDKHelper.analyticsSDKHubConfig();
    }

    private static final Vector<IActivityLifeCycle> mActivityCallback = new Vector<>();

    private static final Vector<IApplicationLifeCycle> mApplicationCallback = new Vector<>();

    public static void onActivityResult(int requestCode, int resultCode, Intent data) {
        for (IActivityLifeCycle callback : mActivityCallback) {
            callback.onActivityResult(requestCode, resultCode, data);
        }
    }

    public static void onPause() {
        for (IActivityLifeCycle callback : mActivityCallback) {
            callback.onPause();
        }
    }

    public static void onResume() {
        for (IActivityLifeCycle callback : mActivityCallback) {
            callback.onResume();
        }
    }

    public static void onNewIntent(Intent intent) {
        for (IActivityLifeCycle callback : mActivityCallback) {
            callback.onNewIntent(intent);
        }
    }

    public static void onStop() {
        for (IActivityLifeCycle callback : mActivityCallback) {
            callback.onStop();
        }
    }

    public static void onDestroy() {
        for (IActivityLifeCycle callback : mActivityCallback) {
            callback.onDestroy();
        }
        NativeInvoker.onPurge();
    }

    public static void onRestart() {
        for (IActivityLifeCycle callback : mActivityCallback) {
            callback.onRestart();
        }
    }

    public static void onBackPressed() {
        for (IActivityLifeCycle callback : mActivityCallback) {
            callback.onBackPressed();
        }
    }

    public static void onConfigurationChanged(Configuration newConfig) {
        for (IActivityLifeCycle callback : mActivityCallback) {
            callback.onConfigurationChanged(newConfig);
        }
    }

    public static void onRestoreInstanceState(Bundle savedInstanceState) {
        for (IActivityLifeCycle callback : mActivityCallback) {
            callback.onRestoreInstanceState(savedInstanceState);
        }
    }

    public static void onSaveInstanceState(Bundle outState) {
        for (IActivityLifeCycle callback : mActivityCallback) {
            callback.onSaveInstanceState(outState);
        }
    }

    public static void onStart() {
        for (IActivityLifeCycle callback : mActivityCallback) {
            callback.onStart();
        }
    }

    public static void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
        for (IActivityLifeCycle callback : mActivityCallback) {
            callback.onRequestPermissionsResult(requestCode, permissions, grantResults);
        }
    }

    public static void setActivityCallback(IActivityLifeCycle activityCallback) {
        mActivityCallback.add(activityCallback);
    }

    public static void setApplicationCallback(IApplicationLifeCycle applicationCallback) {
        mApplicationCallback.add(applicationCallback);
    }

    public static void onAppCreate(Application application) {
        applicationRef = new WeakReference<>(application);
        for (IApplicationLifeCycle applicationLifeCycle : mApplicationCallback) {
            applicationLifeCycle.onCreate(application);
        }
    }

    public static void onAppLowMemory() {
        for (IApplicationLifeCycle applicationLifeCycle : mApplicationCallback) {
            applicationLifeCycle.onLowMemory();
        }
    }

    public static void onAppTrimMemory(final int level) {
        for (IApplicationLifeCycle applicationLifeCycle : mApplicationCallback) {
            applicationLifeCycle.onTrimMemory(level);
        }
    }

    public static void onAppTerminate() {
        for (IApplicationLifeCycle applicationLifeCycle : mApplicationCallback) {
            applicationLifeCycle.onTerminate();
        }
    }

    public static Context getContext() {
        return contextRef.get();
    }

    public static Application getApplication() {
        return applicationRef.get();
    }

    public static void runOnMainThread(Runnable r) {
        if (Looper.myLooper() == Looper.getMainLooper()) {
            ((Activity) contextRef.get()).runOnUiThread(r);
        } else if (null != mainHandler) {
            mainHandler.post(r);
        }
    }
}
