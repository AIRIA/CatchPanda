/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
Copyright (c) 2013-2014 Chukong Technologies Inc.
 
http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
 ****************************************************************************/
package org.cocos2dx.cpp;

import org.cocos2dx.lib.Cocos2dxActivity;

import a.b.c.AdManager;
import a.b.c.st.SpotManager;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.DialogInterface.OnClickListener;
import android.os.Bundle;
import android.view.KeyEvent;

import com.umeng.analytics.MobclickAgent;
import com.umeng.message.PushAgent;

public class AppActivity extends Cocos2dxActivity {
	private AlertDialog exitDialog;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		PushAgent mPushAgent = PushAgent.getInstance(this);
		mPushAgent.enable();
		PushAgent.getInstance(this).onAppStart();
		JniBrige.getInstance().init(this);
	}
	

	public void onResume() {
		super.onResume();
		MobclickAgent.onResume(this);
	}

	@Override
	protected void onDestroy() {
		SpotManager.getInstance(this).unregisterSceenReceiver();
		super.onDestroy();
	}

	public void onPause() {
		super.onPause();
		MobclickAgent.onPause(this);
	}

	@Override
	public boolean onKeyUp(int keyCode, KeyEvent event) {
		if (keyCode == KeyEvent.KEYCODE_BACK) {
			exitDialog = new AlertDialog.Builder(this).setTitle("围住大熊猫")
					.setMessage("你真的要退出游戏吗?")
					.setPositiveButton("继续", new OnClickListener() {

						@Override
						public void onClick(DialogInterface dialog, int which) {
							exitDialog.dismiss();
						}
					}).setNegativeButton("退出", new OnClickListener() {

						@Override
						public void onClick(DialogInterface dialog, int which) {
							android.os.Process.killProcess(android.os.Process
									.myPid());
						}
					}).show();
		}
		return super.onKeyUp(keyCode, event);
	}
	@Override
	public void onBackPressed() {
	    // 如果有需要，可以点击后退关闭插屏广告（可选）。
	    if (!SpotManager.getInstance(this).disMiss(true)) {
	        super.onBackPressed();
	    }
	}

	@Override
	protected void onStop() {
	    //如果不调用此方法，则按home键的时候会出现图标无法显示的情况。
	    SpotManager.getInstance(this).disMiss(false);

	    super.onStop();
	}
}
