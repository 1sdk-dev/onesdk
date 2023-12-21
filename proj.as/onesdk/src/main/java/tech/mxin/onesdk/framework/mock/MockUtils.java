package tech.mxin.onesdk.framework.mock;

import android.app.AlertDialog;
import android.content.Context;
import android.widget.EditText;
import android.widget.Toast;

import tech.mxin.onesdk.framework.SDKWrapper;

public class MockUtils {

    private static boolean isLogin = false;
    private static String userId = "";
    private static String sessionId = "";

    public static void makeText(Context context, String msg) {
        SDKWrapper.runOnMainThread(() -> Toast.makeText(context, msg, Toast.LENGTH_SHORT).show());
    }

    public static void makeText(Context context, String format, Object... args) {
        makeText(context, String.format(format, args));
    }

    public static void alertLogin(Context context, ILoginCallback callback) {
        EditText editText = new EditText(context);
        editText.setHint("Please input username");
        AlertDialog.Builder dialog = new AlertDialog.Builder(context);
        dialog.setTitle("Login").setView(editText).setCancelable(false);
        dialog.setPositiveButton("Login", (dialogInterface, i) -> {
            if (editText.getText().toString().isEmpty()) {
                callback.failed("username or password is empty");
            } else {
                callback.success(editText.getText().toString());
            }
        });
        dialog.setNegativeButton("Cancel", (dialogInterface, i) -> callback.cancel());
        dialog.create().show();
    }

    /** @noinspection BooleanMethodIsAlwaysInverted*/
    public static boolean getLoginState() {
        return isLogin;
    }

    public static void setLoginState(boolean flag) {
        isLogin = flag;
    }

    public static String getSimUserId() {
        return userId;
    }

    public static String getSimSessionId() {
        return sessionId;
    }

    public static void setSimUserInfo(String userId, String sessionId) {
        MockUtils.userId = userId;
        MockUtils.sessionId = sessionId;
    }
}
