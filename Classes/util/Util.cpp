//
//  Util.cpp
//  CreazyBomber
//
//  Created by AIRIA on 14-6-23.
//
//

#include "Util.h"

void Util::toast(std::string msg) {
#if (CC_PLATFORM_ANDROID == CC_TARGET_PLATFORM)
	PluginUtil::invoke(kPPdoSdkToast,msg);
#else

#endif
}

void Util::charge() {
#if (CC_PLATFORM_ANDROID == CC_TARGET_PLATFORM)
	PluginUtil::invoke(kPPdoSdkPay,"");
#else

#endif
}
void Util::chargeConfirm() {
#if (CC_PLATFORM_ANDROID == CC_TARGET_PLATFORM)
	PluginUtil::invoke(kPPdoSdkPayConfirm,"");
#else

#endif
}

void Util::gameAnalyze(AnalyzeType type) {
#if (CC_PLATFORM_ANDROID == CC_TARGET_PLATFORM)
	
#else

#endif
}

void Util::share() {
//	auto winSize = Director::getInstance()->getWinSize();
//	auto render = RenderTexture::create(winSize.width, winSize.height);
//	auto runningScene = Director::getInstance()->getRunningScene();
//	render->begin();
//	runningScene->visit();
//	render->end();
//	render->saveToFile("record.jpg", Image::Format::JPG);
//	auto shareImgPath = FileUtils::getInstance()->getWritablePath()
//			+ "record.jpg";
#if (CC_PLATFORM_ANDROID == CC_TARGET_PLATFORM)
//	PluginUtil::invoke(kPPdoSdkShare,shareImgPath);
#else

#endif
}

void Util::showSpotAds() {
#if (CC_PLATFORM_ANDROID == CC_TARGET_PLATFORM)
	PluginUtil::invoke(kPPdoSdkShowSpotAds,"");
#else

#endif
}

void Util::showOffersWall() {
#if (CC_PLATFORM_ANDROID == CC_TARGET_PLATFORM)
	PluginUtil::invoke(kPPdoSdkShowOffersWall,"");
#else

#endif
}

void Util::spendPoint(int point) {

}

void Util::rewardPoint(int point) {

}

int Util::getPoint() {
#if (CC_PLATFORM_ANDROID == CC_TARGET_PLATFORM)
	return PluginUtil::getPoint();
#else
    return 0;
#endif
}

#pragma mark----------------------plugin util-------------------------------

#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)

#define PP_SDK_CLASS "org/cocos2dx/cpp/JniBrige"
#define PP_SDK_INSTANCE_METHOD "instance"

JniMethodInfo PluginUtil::minfo;
jobject PluginUtil::obj;

void PluginUtil::init() {
	bool isHave = JniHelper::getStaticMethodInfo(minfo, PP_SDK_CLASS,
			PP_SDK_INSTANCE_METHOD, "()Ljava/lang/Object;");
	if (isHave) {
		obj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
	} else {
		obj = NULL;
	}
}

int PluginUtil::getPoint() {
    std::string methodName = "doSdkGetPoint";
	bool isExist = JniHelper::getMethodInfo(minfo, PP_SDK_CLASS, methodName.c_str(),
			"()I");
    
	if (isExist == false) {
		CCLog("%s not found", methodName.c_str());
		return -1;
	}
	jint result = minfo.env->CallIntMethod(obj, minfo.methodID);
	return result;
}

void PluginUtil::invoke(MethodType key, std::string param) {
	if (obj == NULL) {
		CCLog("Plugin Object is NULL");
		return;
	}
	std::string methodName;
	switch (key) {
		case kPPdoSdkShowAds:
		methodName = "doSdkShowAds";
		break;
		case kPPdoSdkHideAds:
		methodName = "doSdkHideAds";
		break;
		case kPPdoSdkShowScoreWall:
		methodName = "doSdkShowScoreWall";
		break;
		case kPPdoSdkPay:
		methodName = "doSdkPay";
		break;
		case kPPdoSdkLogin:
		methodName = "doSdkLogin";
		break;
		case kPPdoSdkToast:
		methodName = "doSdkToast";
		break;
		case kPPdoSdkPayConfirm:
		methodName = "doSdkPayConfirm";
		break;
		case kPPdoSdkAnalyze:
		methodName = "doSdkAnalyze";
		break;
		case kPPdoSdkShare:
		methodName = "doSdkShare";
		break;
		case kPPdoSdkShowSpotAds:
		methodName = "doSdkShowSpotAds";
		break;
		case kPPdoSdkShowOffersWall:
		methodName = "doSdkShowOffersWall";
		break;
		default:
		break;
	}
	bool isExist = JniHelper::getMethodInfo(minfo, PP_SDK_CLASS, methodName.c_str(),
			"(Ljava/lang/String;)V");
	if (isExist == false) {
		CCLog("%s not found", methodName.c_str());
		return;
	}
	jstring paramStr = minfo.env->NewStringUTF(param.c_str());
	minfo.env->CallVoidMethod(obj, minfo.methodID, paramStr);
}

extern "C"
{
	JNIEXPORT void JNICALL Java_com_giant_crazy_jni_JniBrige_payHandler(JNIEnv *env,jobject thiz)
	{

	}

}

#endif

