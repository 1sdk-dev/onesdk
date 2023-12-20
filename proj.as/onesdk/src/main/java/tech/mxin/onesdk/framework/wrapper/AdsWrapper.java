package tech.mxin.onesdk.framework.wrapper;


import android.view.Gravity;
import android.view.View;
import android.view.WindowManager;

import tech.mxin.onesdk.framework.NativeInvoker;
import tech.mxin.onesdk.framework.protocol.InterfaceAds;

@SuppressWarnings("WeakerAccess")
public class AdsWrapper {

	public static final int RESULT_CODE_ADS_RECEIVED = 0;                 // The ad is received
	public static final int RESULT_CODE_ADS_SHOWN = 1;                    // The advertisement shown
	public static final int RESULT_CODE_ADS_DISMISSED = 2;                // The advertisement dismissed
	public static final int RESULT_CODE_POINTS_SPEND_SUCCEED = 3;         // The points spend succeed
	public static final int RESULT_CODE_POINTS_SPEND_FAILED = 4;          // The points spend failed
	public static final int RESULT_CODE_NETWORK_ERROR = 5;                // Network error
	public static final int RESULT_CODE_UNKNOWN_ERROR = 6;                // Unknown error
	public static final int RESULT_CODE_OFFER_WALL_ON_POINTS_CHANGED = 7; // Change the point of offer wall
	public static final int RESULT_CODE_REWARDED_VIDEO_WITH_REWARD = 8;   // The callback of receiving the reward of rewarded video
	public static final int RESULT_CODE_FEE_FINISHED= 9;                  // The callback of finishing Fee ad
	public static final int RESULT_CODE_ADS_CLICKED = 10;                 // The callback of the advertisement clicked
	public static final int RESULT_CODE_ADS_FAILED = 11;                  // Ads info empty. *
	public static final int RESULT_CODE_ADS_PRELOAD_FAILED = 12;          // Ads preload failed. *
	public static final int RESULT_CODE_ADS_SHOWN_FAILED = 13;            // Ads shown failed. *
	public static final int RESULT_CODE_ADS_RETRY_PRELOAD = 14;           // Ads should retry preload function by user. *
	public static final int RESULT_CODE_ADS_ON_LEAVE = 15;                // Ads on leave. *
	public static final int RESULT_CODE_ADS_ON_AD_IMPRESSION = 16;        // Ads on impression. *
	public static final int RESULT_CODE_ADS_EXTENSION = 40000;            // extension code

	public static final int POS_BOTTOM = 0;
	public static final int POS_CENTER = 1;
	public static final int POS_TOP = 2;

	public static void addAdView(WindowManager mWm, View adView, int pos) {
		WindowManager.LayoutParams mLayoutParams = new WindowManager.LayoutParams();
		mLayoutParams.type = WindowManager.LayoutParams.TYPE_APPLICATION_PANEL;
		mLayoutParams.width = WindowManager.LayoutParams.WRAP_CONTENT;
		mLayoutParams.height = WindowManager.LayoutParams.WRAP_CONTENT;
		mLayoutParams.flags |= WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE;

		switch (pos) {
			case POS_BOTTOM:
				mLayoutParams.gravity = Gravity.BOTTOM;
				break;
			case POS_CENTER:
				mLayoutParams.gravity = Gravity.CENTER;
				break;
			case POS_TOP:
				mLayoutParams.gravity = Gravity.TOP;
				break;
			default:
				break;
		}
		mWm.addView(adView, mLayoutParams);
	}

	public static void onAdsResult(InterfaceAds adapter, int code, String msg) {
		NativeInvoker.onAdsResult(adapter, code, msg);
	}

}
