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

package tech.mxin.onesdk.framework.protocol;

import org.json.JSONArray;
import org.json.JSONObject;

import java.lang.reflect.Method;
import java.lang.reflect.Modifier;

import tech.mxin.onesdk.framework.SDKLogger;
import tech.mxin.onesdk.framework.SDKUtils;

public interface Interface {
    default boolean isFuncSupported(String funcName) {
        if (funcName == null || funcName.isEmpty()) return false;
        Class<?> clazz = this.getClass();
        SDKLogger.logD(clazz.getSimpleName(), "isFuncSupported(" + funcName + ")invoked!");
        Method[] methods = SDKUtils.getAllMethods(clazz);
        for (Method method : methods) {
            if (method.getName().equals(funcName)) return true;
        }
        return false;
    }

    default String getPluginMethods() {
        try {
            Method[] methods = SDKUtils.getAllMethods(getClass());
            JSONArray jsonArray = new JSONArray();
            for (Method method : methods) {
                if (!Modifier.isPublic(method.getModifiers())) continue;
                if (method.getName().startsWith("lambda$")) continue;
                JSONObject jsonObject = new JSONObject();
                jsonObject.put("name", method.getName());
                jsonObject.put("returnType", method.getReturnType().toString());
                Class<?>[] parameterTypes = method.getParameterTypes();
                if (parameterTypes.length != 0) {
                    JSONArray params = new JSONArray();
                    for (Class<?> type : parameterTypes) params.put(type.getName());
                    jsonObject.put("parameterType", params);
                }
                jsonArray.put(jsonObject);
            }
            SDKLogger.logD("PluginMethods(JSON)", jsonArray.toString());
            return jsonArray.toString();
        } catch (Exception ignore) {
        }
        return "{}";
    }

    String getPluginId();

    String getSDKVersion();

    String getPluginVersion();
}
