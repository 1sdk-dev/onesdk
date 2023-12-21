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

import android.app.AlertDialog;
import android.content.Context;

import java.util.Hashtable;

import tech.mxin.onesdk.framework.SDKHelper;
import tech.mxin.onesdk.framework.SDKLogger;
import tech.mxin.onesdk.framework.SDKUtils;
import tech.mxin.onesdk.framework.SDKWrapper;
import tech.mxin.onesdk.framework.protocol.InterfaceFee;
import tech.mxin.onesdk.framework.wrapper.FeeWrapper;

public class MockFee implements InterfaceFee {
    private static final String TAG = "MockFee";
    private static final String PLUGIN_ID = "MockFee";
    private final Context mContext;
    private final MockFee mAdapter;
    private boolean isInited = false;
    private Hashtable<String, String> mGoodsInfo = null;

    private static void LogE(Exception e, String format, Object... args) {
        SDKLogger.logE(TAG, e, format, args);
    }

    private static void LogD(String format, Object... args) {
        SDKLogger.logD(TAG, format, args);
    }

    public MockFee(Context context) {
        mContext = context;
        mAdapter = this;
        configDeveloperInfo(SDKHelper.getPluginParams());
    }

    private void configDeveloperInfo(Hashtable<String, String> cpInfo) {
        SDKWrapper.runOnMainThread(() -> {
            if (isInited) return;
            isInited = true;
            feeResult(FeeWrapper.FEE_RESULT_INIT_SUCCESS, "init success");
        });
    }

    @Override
    public void feeForProduct(Hashtable<String, String> info) {
        LogD("feeForProduct(" + info.toString() + ") invoked ");
        mGoodsInfo = info;
        SDKWrapper.runOnMainThread(() -> {
            if (!SDKUtils.networkReachable(mContext)) {
                feeResult(FeeWrapper.FEE_RESULT_NETWORK_ERROR, "Network error!");
                return;
            }

            if (mGoodsInfo == null) {
                feeResult(FeeWrapper.FEE_RESULT_FAIL, "info is null");
                return;
            }
            if (!MockUtils.getLoginState()) feeLogin();
            else startFee();
        });

    }

    @Override
    public Hashtable<String, String> getFeeStateInfo() {
        Hashtable<String, String> feeInfo = new Hashtable<>();
        feeInfo.put("amount", "1");
        feeInfo.put("orderID", "111111111111111");
        feeInfo.put("payType", "AliPay");
        feeInfo.put("subjectID", "1111111111");
        feeInfo.put("currencyType", "cny");
        return feeInfo;
    }

    private Hashtable<String, String> getOrderInfo(Hashtable<String, String> productInfo, String userID) {
        Hashtable<String, String> orderInfo;
        try {
            String id = productInfo.get("Product_Id");
            String strName = productInfo.get("Product_Name");
            String strPrice = productInfo.get("Product_Price");
            String strCount = productInfo.get("Product_Count");
            String strRole_Id = productInfo.get("Role_Id");
            String strRole_Name = productInfo.get("Role_Name");
            String strServer_id = productInfo.get("Server_Id");
            if (id == null || strName == null || strPrice == null || strCount == null ||
                    strRole_Id == null || strRole_Name == null || strServer_id == null)
                return null;
            String strExt = productInfo.get("EXT");

            int count = Math.max(Integer.parseInt(strCount), 1);
            float money = Float.parseFloat(strPrice) * count;

            orderInfo = new Hashtable<>();
            orderInfo.put("money", String.valueOf(money));
            orderInfo.put("coin_num", strCount);
            orderInfo.put("game_user_id", strRole_Id);
            orderInfo.put("game_server_id", strServer_id);
            orderInfo.put("product_id", id);
            orderInfo.put("product_name", strName);
            orderInfo.put("user_id", userID);
            if (strExt != null) orderInfo.put("private_data", strExt);

        } catch (Exception e) {
            e.printStackTrace();
            return null;
        }

        return orderInfo;
    }

    private void getOrderId(Hashtable<String, String> productInfo) {
        Hashtable<String, String> orderInfo = getOrderInfo(productInfo, MockUtils.getSimUserId());
        if (orderInfo == null) {
            feeResult(FeeWrapper.FEE_RESULT_PRODUCTION_INCOMPLETE, "something is null");
            return;
        }
        orderInfo.put("order_type", "999");
        LogD("orderInfo: %s", orderInfo.toString());
        final float money = Float.parseFloat(orderInfo.get("money"));
        String mOrderId = "order_id";
        addFee(mOrderId, money);
    }

    private void addFee(final String orderId, final float money) {
        final Context curContext = mContext;
        SDKWrapper.runOnMainThread(() -> {
            try {
                AlertDialog.Builder dialog02 = new AlertDialog.Builder(curContext);
                dialog02.setTitle("支付").setMessage("Do you confirm the fee?");
                dialog02.setPositiveButton("Pay", (dialogInterface, i) -> fee(orderId, money));
                dialog02.setNegativeButton("Cancel", (dialogInterface, i) ->
                        feeResult(FeeWrapper.FEE_RESULT_CANCEL, "the fee has been canceled"));
                dialog02.create().show();

            } catch (Exception e) {
                LogE(e, "Error during fee");
                feeResult(FeeWrapper.FEE_RESULT_FAIL, "fee failed");
            }
        });

    }

    private void feeLogin() {
        MockUtils.alertLogin(mContext, new ILoginCallback() {
            @Override
            public void success(String username) {
                MockUtils.setLoginState(true);
                startFee();
            }

            @Override
            public void failed(String reason) {
                MockUtils.setLoginState(false);
                MockUtils.makeText(mContext, reason);
                feeResult(FeeWrapper.FEE_RESULT_FAIL, reason);
            }

            @Override
            public void cancel() {
                MockUtils.setLoginState(false);
                MockUtils.makeText(mContext, "the login has been canceled");
                feeResult(FeeWrapper.FEE_RESULT_CANCEL, "the login has been canceled");
            }
        });
    }

    private void fee(String orderId, float money) {
        feeResult(FeeWrapper.FEE_RESULT_FAIL, "");
    }

    private void feeResult(int ret, String msg) {
        FeeWrapper.onFeeResult(mAdapter, ret, msg);
        LogD("feeResult : %d msg : %s", ret, msg);
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

    @Override
    public String getPluginId() {
        LogD("getPluginId() invoked!");
        return PLUGIN_ID;
    }

    private void startFee() {
        getOrderId(mGoodsInfo);
    }
}
