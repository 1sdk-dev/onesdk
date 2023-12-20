package tech.mxin.onesdk.framework.wrapper;

import tech.mxin.onesdk.framework.NativeInvoker;
import tech.mxin.onesdk.framework.protocol.InterfaceUser;

public class UserWrapper {
	public static final int ACTION_RET_INIT_SUCCESS = 0; 
	public static final int ACTION_RET_INIT_FAIL = 1;
	public static final int ACTION_RET_LOGIN_SUCCESS = 2;
	public static final int ACTION_RET_LOGIN_TIMEOUT = 3;
	public static final int ACTION_RET_LOGIN_NO_NEED = 4;
	public static final int ACTION_RET_LOGIN_FAIL = 5;
	public static final int ACTION_RET_LOGIN_CANCEL = 6;
	public static final int ACTION_RET_LOGOUT_SUCCESS = 7;
	public static final int ACTION_RET_LOGOUT_FAIL = 8;
	public static final int ACTION_RET_PLATFORM_ENTER = 9;
	public static final int ACTION_RET_PLATFORM_BACK = 10;
	public static final int ACTION_RET_PAUSE_PAGE = 11;
	public static final int ACTION_RET_EXIT_PAGE = 12;
	public static final int ACTION_RET_ANTI_ADDICTION_QUERY = 13;
	public static final int ACTION_RET_REAL_NAME_REGISTER = 14;
	public static final int ACTION_RET_ACCOUNT_SWITCH_SUCCESS = 15;
	public static final int ACTION_RET_ACCOUNT_SWITCH_FAIL = 16;
	public static final int ACTION_RET_OPEN_SHOP = 17;
	public static final int ACTION_RET_ACCOUNT_SWITCH_CANCEL = 18;
	public static final int ACTION_RET_GAME_EXIT_PAGE = 19;
	public static final int ACTION_RET_USER_SCORE_SUBMIT_SUCCEED = 20;
	public static final int ACTION_RET_USER_SCORE_SUBMIT_FAIL = 21;
	public static final int ACTION_RET_USER_ACH_UNLOCK_SUCCEED = 22;
	public static final int ACTION_RET_USER_ACH_UNLOCK_FAIL = 23;
	public static final int ACTION_RET_SHOW_LEADER_BOARD_SUCCESS = 24;
	public static final int ACTION_RET_SHOW_LEADER_BOARD_FAIL = 25;
	public static final int ACTION_RET_SHOW_ACHIEVEMENT_SUCCESS = 26;
	public static final int ACTION_RET_SHOW_ACHIEVEMENT_FAIL = 27;
	public static final int ACTION_RET_SERVER_VERIFY = 28;
	public static final int ACTION_RET_USER_EXTENSION = 20000;

	public static void onUserResult(InterfaceUser obj, int ret, String msg) {
		NativeInvoker.onUserResult(obj, ret, msg);
	}

}
