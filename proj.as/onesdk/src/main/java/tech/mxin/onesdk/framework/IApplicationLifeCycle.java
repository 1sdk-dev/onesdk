package tech.mxin.onesdk.framework;

import android.app.Application;

public interface IApplicationLifeCycle {
    default void onApplicationCreate(Application application) {}
    default void onCreate() {}
    default void onTerminate() {}
    default void onConfigurationChanged() {}
    default void onLowMemory() {}
    default void onTrimMemory(final int level) {}
}
