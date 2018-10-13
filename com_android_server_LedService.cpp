#define LOG_TAG "LedService"

#include "jni.h"
#include "JNIHelp.h"
#include "android_runtime/AndroidRuntime.h"

#include <utils/misc.h>
#include <utils/Log.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>


namespace android
{
    static jint fd;

    jint ledOpen(JNIEnv *env, jobject cls)
    {
        fd = open("/dev/zf_led", O_RDWR);//打开设备LED节点
        ALOGI("native ledOpen : %d", fd);
        if (fd >= 0)
            return 0;
        else
            return -1;
    }


    void ledClose(JNIEnv *env, jobject cls)
    {
      ALOGI("native ledClose ...");
      close(fd);
    }

    jint ledCtrl(JNIEnv *env, jobject cls, jint which, jint status)
    {
        int ret = ioctl(fd, status, which);//给设备节点传输控制信息
        ALOGI("native ledCtrl : %d, %d, %d", which, status, ret);
        return ret;
    }

    /* 定义JNI字段描述符 */
    static const JNINativeMethod methods[] = {//定义一个映射数组
        {"native_ledOpen", "()I", (void *)ledOpen},//对应c语言的ledOpen,这个函数没有参数,返回值是int类型
        {"native_ledClose", "()V", (void *)ledClose},//对应c语言的ledClose,这个函数没有参数,返回值是void类型
        {"native_ledCtrl", "(II)I", (void *)ledCtrl},//对应c语言的ledCtrl,这个函数有两个int参数,返回值是int类型
    };

    int register_android_server_LedService(JNIEnv *env)
    {
        //注册方法
        return jniRegisterNativeMethods(env, "com/android/server/LedService",
                methods, NELEM(methods));
    }
}


