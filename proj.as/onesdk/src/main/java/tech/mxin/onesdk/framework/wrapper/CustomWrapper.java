package tech.mxin.onesdk.framework.wrapper;


import tech.mxin.onesdk.framework.NativeInvoker;
import tech.mxin.onesdk.framework.protocol.InterfaceCustom;

public class CustomWrapper {

	public static final int CUSTOM_RESULT_CUSTOM_EXTENSION = 80000;

	public static void onCustomResult(InterfaceCustom obj, int ret, String msg) {
		NativeInvoker.onCustomResult(obj, ret, msg);
	}
}
