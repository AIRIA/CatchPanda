package org.cocos2dx.cpp;

import org.json.JSONException;
import org.json.JSONObject;

import a.b.c.AdManager;
import a.b.c.st.SpotManager;
import android.app.Activity;
import android.util.Log;
import android.widget.Toast;

public class JniBrige {

	public native void payHandler();
	
	private static JniBrige _instance;
	static String TAG = "Crazy Bomber";
	String base64EncodedPublicKey;
	static final String SKU_PREMIUM = "premium";
	static final int RC_REQUEST = 10001;
	boolean mIsPremium = false;
	Activity context;
	boolean enableAds = false;

	public static Object instance() {
		return getInstance();
	}

	public static JniBrige getInstance() {
		if (_instance == null) {
			_instance = new JniBrige();
			Log.v(TAG, "instance init success");
		}
		return _instance;
	}

	/**
	 * 初始化服务 判断是否可以连接google play
	 * 
	 * @param ctx
	 */
	public void init(final Activity ctx) {
		context = ctx;
		Log.d(TAG, "Starting setup.");
		
		context.runOnUiThread(new Runnable() {
			public void run() {
				String res = NetManager.sendHttpRequest("https://gist.githubusercontent.com/AIRIA/ba5c20580622cf7c7b92/raw");
				if(res!=null){
					/* 检查是否开启了广告 */
					try {
						JSONObject json = new JSONObject(res);
						enableAds = json.getBoolean("v1_xiaomi");
						Log.v(TAG, res);
						if(enableAds){
							AdManager.getInstance(ctx).init("2850e25d57de960d",
									"7f4a715d5137b3cf", false);
							SpotManager.getInstance(ctx).loadSpotAds();
							AdManager.getInstance(ctx).setUserDataCollect(true);
							AdManager.getInstance(ctx).setEnableDebugLog(false);
						}
					} catch (JSONException e) {
						e.printStackTrace();
					}
						
				}
			}
		});

	}



	public void doSdkPay(final String params) {
		Log.v(TAG, "invoke pay method");
		context.runOnUiThread(new Runnable() {

			@Override
			public void run() {
				doSdkShowOffersWall(params);
			}
		});
	}

	public void doSdkToast(final String params) {
		context.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				Toast.makeText(context, params, Toast.LENGTH_SHORT).show();
			}
		});
	}
	
//--------------------------------------------------------------------------------------
	/**
	 * 显示迷你广告条
	 * @param params
	 */
	public void doSdkShowMiniAds(String params){
		
	}
	
	/**
	 * 显示插屏广告
	 * @param params
	 */
	public void doSdkShowSpotAds(String params){
		if(enableAds==false)
			return;
		Log.v(TAG, "invoke sdk show spot ads");
		context.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				SpotManager.getInstance(context).showSpotAds(context);
			}
		});
	}
	
	/**
	 * 显示积分墙
	 * @param params
	 */
	public void doSdkShowOffersWall(String params){
		
	}

}
