package tech.mxin.onesdk.framework;

import android.app.Application;

public class OneApplication extends Application {

    @Override
    public void onCreate() {
        super.onCreate();
        SDKWrapper.onAppCreate(this);
    }

    @Override
    public void onLowMemory() {
        super.onLowMemory();
        SDKWrapper.onAppLowMemory();
    }

    @Override
    public void onTrimMemory(final int level) {
        super.onTrimMemory(level);
        SDKWrapper.onAppTrimMemory(level);
    }

    @Override
    public void onTerminate() {
        super.onTerminate();
        SDKWrapper.onAppTerminate();
    }

}
