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
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.lang.reflect.Method;
import java.nio.charset.StandardCharsets;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;


public class SDKUtils {

    private final static String TAG = "SDKUtils";

    public static Method[] getAllMethods(final Class<?> clazz) {
        if (clazz == null) return null;
        final List<Method> methodList = new ArrayList<>();
        Class<?> currentClass = clazz;
        while (currentClass != null) {
            if ("Object".equals(currentClass.getSimpleName())) break;
            final Method[] methods = currentClass.getDeclaredMethods();
            methodList.addAll(Arrays.asList(methods));
            currentClass = currentClass.getSuperclass();
        }
        return methodList.toArray(new Method[0]);
    }

    public static String getFileContentWithName(String fileName) {
        StringBuilder Result = new StringBuilder();
        try {
            InputStreamReader inputReader = new InputStreamReader(SDKWrapper.getContext().getAssets().open(fileName));
            BufferedReader bufReader = new BufferedReader(inputReader);
            String line;
            while ((line = bufReader.readLine()) != null) Result.append(line);
        } catch (Exception ignored) {
        }
        return Result.toString();
    }

    public static Object jsonToMap(String content) {
        content = content.trim();
        Map<String, Object> map = new HashMap<>();
        List<Object> list = new LinkedList<>();
        try {
            if (content.charAt(0) == '{') {
                JSONObject jsonObject = new JSONObject(content);
                //noinspection rawtypes
                Iterator iterator = jsonObject.keys();
                while (iterator.hasNext()) {
                    String key = (String) iterator.next();
                    Object value = jsonObject.get(key);
                    map.put(key, (value instanceof JSONArray || value instanceof JSONObject)
                            ? SDKUtils.jsonToMap(value.toString().trim()) : value.toString().trim());
                }
                return map;
            } else if (content.charAt(0) == '[') {
                JSONArray jsonArray = new JSONArray(content);
                for (int i = 0; i < jsonArray.length(); i++) {
                    Object value = jsonArray.get(i);
                    list.add((value instanceof JSONArray || value instanceof JSONObject)
                            ? SDKUtils.jsonToMap(value.toString().trim()) : value.toString().trim());
                }
                return list;
            }
        } catch (JSONException ignore) {
        }
        return map;
    }

    public static String getSHA1HexString(String str) {
        try {
            MessageDigest md = MessageDigest.getInstance("SHA-1");
            md.update(str.getBytes(StandardCharsets.UTF_8));
            byte[] digest = md.digest();
            StringBuilder builder = new StringBuilder();
            String shaHex;
            for (byte b : digest) {
                shaHex = Integer.toHexString(b & 0xFF);
                if (shaHex.length() < 2) builder.append(0);
                builder.append(shaHex);
            }
            return builder.toString();
        } catch (NoSuchAlgorithmException e) {
            return "";
        }
    }

    public static String getMd5(String str) {
        try {
            MessageDigest md = MessageDigest.getInstance("MD5");
            byte[] bytes = str.getBytes(StandardCharsets.UTF_8);
            byte[] digest = md.digest(bytes);
            StringBuilder hexValue = new StringBuilder();
            for (byte b : digest) {
                int val = ((int) b) & 0xff;
                if (val < 16) hexValue.append("0");
                hexValue.append(Integer.toHexString(val));
            }
            return hexValue.toString();
        } catch (Exception e) {
            SDKLogger.logE(TAG, "", e);
        }
        return str;
    }

    public static String getApplicationName() {
        PackageManager packageManager = null;
        ApplicationInfo applicationInfo = null;
        try {
            packageManager = SDKWrapper.getContext().getApplicationContext().getPackageManager();
            applicationInfo = packageManager.getApplicationInfo(SDKWrapper.getContext().getPackageName(), 0);
        } catch (PackageManager.NameNotFoundException ignored) {
        }
        assert applicationInfo != null;
        return (String) packageManager.getApplicationLabel(applicationInfo);
    }

    public static String getApplicationVersion() {
        try {
            PackageManager packageManager = SDKWrapper.getContext().getApplicationContext().getPackageManager();
            PackageInfo packInfo = packageManager.getPackageInfo(SDKWrapper.getContext().getApplicationContext().getPackageName(), 0);
            return packInfo.versionName;
        } catch (Exception ignored) {
        }
        return "";
    }

    public static String getApkPath() {
        return SDKWrapper.getContext().getApplicationInfo().sourceDir;
    }

    @SuppressWarnings("BooleanMethodIsAlwaysInverted")
    public static boolean networkReachable(Context ctx) {
        try {
            ConnectivityManager conn = (ConnectivityManager) ctx.getSystemService(Context.CONNECTIVITY_SERVICE);
            NetworkInfo netInfo = conn != null ? conn.getActiveNetworkInfo() : null;
            return null != netInfo && netInfo.isAvailable();
        } catch (Exception e) {
            e.printStackTrace();
        }
        return false;
    }
}
