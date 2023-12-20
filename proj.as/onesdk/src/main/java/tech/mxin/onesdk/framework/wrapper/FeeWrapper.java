package tech.mxin.onesdk.framework.wrapper;

import tech.mxin.onesdk.framework.NativeInvoker;
import tech.mxin.onesdk.framework.protocol.InterfaceFee;

public class FeeWrapper {
	public static final int FEE_RESULT_SUCCESS = 0;
	public static final int FEE_RESULT_FAIL = 1;
	public static final int FEE_RESULT_CANCEL = 2;
	public static final int FEE_RESULT_NETWORK_ERROR = 3;
	public static final int FEE_RESULT_PRODUCTION_INCOMPLETE = 4;
	public static final int FEE_RESULT_INIT_SUCCESS = 5;
	public static final int FEE_RESULT_INIT_FAIL = 6;
	public static final int FEE_RESULT_NOW_FEEDING = 7;
	public static final int FEE_RESULT_RECHARGE_SUCCESS = 8;
	public static final int FEE_RESULT_EXTENSION = 30000;

	public static void onFeeResult(InterfaceFee obj, int ret, String msg) {
		NativeInvoker.onFeeResult(obj, ret, msg);
	}
}
