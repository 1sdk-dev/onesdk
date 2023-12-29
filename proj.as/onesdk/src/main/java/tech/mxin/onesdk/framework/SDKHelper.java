/**
 * Copyright 2023 xin.yang
 * <p/>
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * <p/>
 *     http://www.apache.org/licenses/LICENSE-2.0
 * <p/>
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package tech.mxin.onesdk.framework;

import android.content.Context;

import java.util.Hashtable;
import java.util.List;
import java.util.Map;
import java.util.Vector;

@SuppressWarnings("WeakerAccess")
public class SDKHelper {
    public final static String VERSION = "0.1";
    private static final String TAG = "SDKHelper";
    private static Hashtable<String, String> mChannelInfo = new Hashtable<>();
    private static Hashtable<String, Hashtable<String, String>> mPluginInfo = new Hashtable<>();
    private static Hashtable<String, String> mPluginParams = new Hashtable<>();
    private static Vector<String> mSupportForPlugins = new Vector<>();
    private static Vector<String> mSupportForPluginIDs = new Vector<>();

    private static Object initPlugin(String classFullName) {
        SDKLogger.logI(TAG, "class name : ----" + classFullName + "----");
        Class<?> c;
        try {
            c = Class.forName(classFullName.replace('/', '.'));
            Context ctx = SDKWrapper.getContext();
            if (ctx != null) return c.getDeclaredConstructor(Context.class).newInstance(ctx);
            else SDKLogger.logE(TAG, "Plugin " + classFullName + " wasn't initialized.");
        } catch (ClassNotFoundException e) {
            SDKLogger.logE(TAG, "Class " + classFullName + " not found.");
            e.printStackTrace();
            return null;
        } catch (Exception e) {
            e.printStackTrace();
            return null;
        }
        return null;
    }

    private static boolean bAnalysisSDKHubConfig = false;

    @SuppressWarnings({"unchecked", "MismatchedQueryAndUpdateOfCollection"})
    static void analyticsSDKHubConfig() {
        try {
            if (bAnalysisSDKHubConfig) return;
            bAnalysisSDKHubConfig = true;
            mChannelInfo = new Hashtable<>();
            mPluginInfo = new Hashtable<>();
            mPluginParams = new Hashtable<>();
            mSupportForPlugins = new Vector<>();
            mSupportForPluginIDs = new Vector<>();
            String json = getConfigContent();
//            if (!(json.startsWith("{") && json.endsWith("}"))) json = NativeInvoker.decode(json);
            //noinspection unchecked
            Map<String, Object> sdkHubConfig = (Map<String, Object>) SDKUtils.jsonToMap(json);
            Map<String, Object> channel = (Map<String, Object>) sdkHubConfig.get("channel");
            if (channel != null) {
                for (String channelKey : channel.keySet()) {
                    Object channelValue = channel.get(channelKey);
                    if ("pluginLs".equals(channelKey)) {
                        List<Object> pluginLs = (List<Object>) channelValue;
                        for (Object plugin : pluginLs) {
                            Hashtable<String, String> pluginTable = new Hashtable<>();
                            String pluginKey = "";
                            Map<String, Object> pluginItem = (Map<String, Object>) plugin;
                            for (String _pluginKey : pluginItem.keySet()) {
                                Object pluginVale = pluginItem.get(_pluginKey);
                                pluginTable.put(_pluginKey, pluginVale.toString());
                                if ("className".equals(_pluginKey)) pluginKey = pluginVale.toString();
                                if ("idPlugin".equals(_pluginKey)) mSupportForPluginIDs.add(pluginVale.toString());
                            }
                            mSupportForPlugins.add(pluginKey);
                            mPluginInfo.put(pluginKey, pluginTable);
                        }
                    } else if ("param".equals(channelKey)) {
                        Map<String, Object> param = (Map<String, Object>) channelValue;
                        for (String paramKey : param.keySet()) mPluginParams.put(paramKey, param.get(paramKey).toString());
                    } else {
                        mChannelInfo.put(channelKey, channelValue.toString());
                    }
                }
            }
        } catch (Exception ignore) {
        }
    }

    public static String getSupportPlugin() {
        return mSupportForPlugins == null ? "[]" : mSupportForPlugins.toString();
    }

    public static String getSupportPluginIds() {
        return mSupportForPluginIDs == null ? "[]" : mSupportForPluginIDs.toString();
    }

    public static Hashtable<String, String> getPluginParams() {
        return mPluginParams == null ? new Hashtable<>() : mPluginParams;
    }

    public static Hashtable<String, String> getChannelInfo() {
        return mChannelInfo == null ? new Hashtable<>() : mChannelInfo;
    }

    public static Hashtable<String, Hashtable<String, String>> getPluginInfo() {
        return mPluginInfo == null ? new Hashtable<>() : mPluginInfo;
    }

    public static String getChannelId() {
        return getHashTableValue(mChannelInfo, "idChannel", "999999");
    }

    public static String getFrameworkVersion() {
        return SDKHelper.VERSION;
    }

    public static void setAppParam(String appId) {
        if (mChannelInfo != null) mChannelInfo.put("gameId", appId);
    }

    public static String logLevel() {
        return getHashTableValue(mChannelInfo, "logMode", "verbose");
    }

    private static String getConfigContent() {
        String[] names = NativeInvoker.getConfigFiles().split(";");
        String fileContent = "";
        for (String name : names) {
            if (!"".equals(fileContent)) break;
            fileContent = SDKUtils.getFileContentWithName(name);
        }
        if ("".equals(fileContent)) return "{}";
        return fileContent;
    }

    private static String getHashTableValue(Hashtable<String, String> table, String key, String defaultValue) {
        return (table != null && table.containsKey(key)) ? table.get(key) : defaultValue;
    }

    public static void release() {
        mChannelInfo = null;
        mPluginInfo = null;
        mPluginParams = null;
        mSupportForPlugins = null;
        bAnalysisSDKHubConfig = false;
    }
}
