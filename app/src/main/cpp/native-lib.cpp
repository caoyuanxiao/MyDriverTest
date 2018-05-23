#include <jni.h>
#include <string>
#include <stdio.h>
#include "string.h"
#include "android/log.h"
#include "stdlib.h"
#include "sys/ioctl.h"
#include "sys/types.h"

#define TAG "myDemo-jni" // 这个是自定义的LOG的标识
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG ,__VA_ARGS__) // 定义LOGD类型
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG ,__VA_ARGS__) // 定义LOGI类型
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,TAG ,__VA_ARGS__) // 定义LOGW类型
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG ,__VA_ARGS__) // 定义LOGE类型
#define LOGF(...) __android_log_print(ANDROID_LOG_FATAL,TAG ,__VA_ARGS__) // 定义LOGF类型

int sum(int a ,int b){
    return a+b;
}

struct fields_t{
    jfieldID  g_field;
    jmethodID callbackMethodID;
    jmethodID callbackArrayMethodID;
    JavaVM *g_jvm;
    jobject g_obj;
    jclass  cls;
};

int (*Func)(int,int);
typedef int  (*Function) (int,int);
fields_t fields;

#define  TEST_CODE 100;
pthread_t  pthread;
jboolean destory= false;
static timer_t  enroll_timeout_id;
char gs_raw_data[256];


#define CHIPS_ENROLL_TIMEOUT_TIME    6000000000   //60s
#define CHIPS_ENROLL_RESTART_TIMEOUT_TIME    1000000	//0.1s

static void chips_enroll_timeout_func(union sigval s){
    LOGI("<%s> ",__FUNCTION__);
    _JNIEnv * env;
    fields.g_jvm->AttachCurrentThread(&env,NULL);
    env->CallVoidMethod(fields.g_obj,fields.callbackMethodID,env->NewStringUTF("success"),1,2);
}

/**
* chips_timer_create - 创建定时器
* @timerid: 定时器id号
* @sigev_notify_func: 函数指针，定时器超时处理函数
* @data: 定时器超时处理函数参数
*
* @return: 成功返回0，失败返回负数
*/
static int chips_timer_create(timer_t *timerid,void (*sigev_notify_func)(union sigval),void *data)
{
    //print_ids("main thread");

    struct sigevent sev;
    memset(&sev,0,sizeof(sev));

    //Create the timer
    sev.sigev_notify = SIGEV_THREAD;   //notify 方式
    sev.sigev_notify_function = sigev_notify_func;
    sev.sigev_value.sival_ptr = data;  //传参

    if(timer_create(CLOCK_REALTIME,&sev,timerid) == -1)
    {
        LOGI("<%s> timer_create error ",__FUNCTION__);
        return -1;
    }


    return 0;
}

/**
* chips_timer_settime - 设置定时器超时时间
* @timerid: 定时器id号
* @value_nanosecs: 设置定时器第一次超时触发时间
* @interval_nanosecs: 设置定时器循环超时触发时间
*
* @return: 成功返回0，失败返回负数
*/
static int chips_timer_settime(timer_t timerid,long long value_nanosecs,long interval_nanosecs)
{

    struct itimerspec its;
    memset(&its,0,sizeof(its));

    //Start the timer
    its.it_value.tv_sec = value_nanosecs /  1000000000;
    its.it_value.tv_nsec = value_nanosecs % 1000000000;

    its.it_interval.tv_sec = interval_nanosecs /  1000000000;
    its.it_interval.tv_nsec = interval_nanosecs % 1000000000;

    if(timer_settime(timerid,0,&its,NULL) == -1)
    {

        return -1;
    }

    return 0;
}

static void *pthread_run(void* arg){

    // pthread_t tid=pthread_self();

    pthread_detach(pthread);
    _JNIEnv* env;
    fields.g_jvm->AttachCurrentThread(&env,NULL);


    for (;;) {
    if (!destory){

    }else{
        LOGI("destory");
        env->CallVoidMethod(fields.g_obj,fields.callbackMethodID,env->NewStringUTF("yuanxiao"),100,101);
       // return NULL ;
        break;
    }
    }

}
extern "C"
JNIEXPORT jstring


JNICALL
Java_com_yuanxiao_mydrivertest_NativeTest_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";


    return env->NewStringUTF(hello.c_str());
}


extern "C"
JNIEXPORT jint JNICALL
Java_com_yuanxiao_mydrivertest_NativeTest_TestJniAdd(JNIEnv *env, jobject instance, jint a, jint b) {

    jint result=a+b;
    LOGI("TestJniAdd a=%d,b=%d result=%d \n",a,b, result);
    env->CallVoidMethod(instance,fields.callbackMethodID,env->NewStringUTF("yuanxiao"),result,b-a);

    return  result;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_yuanxiao_mydrivertest_NativeTest_setCallback(JNIEnv *env, jobject instance) {
    fields.cls=env->GetObjectClass(instance);

    //fields.cls=env->FindClass("com/yuanxiao/mydrivertest/NativeTest");
    fields.g_field=env->GetFieldID(fields.cls,"Name","I");

    jint  name=env->GetIntField(instance,fields.g_field);
    LOGI("%s %d",__FUNCTION__,name);

    env->SetIntField(instance,fields.g_field,10);

    name=env->GetIntField(instance,fields.g_field);
    LOGI("%s %d",__FUNCTION__,name);

    //string Ljava/lang/String;  int : I
    fields.callbackMethodID=env->GetMethodID(fields.cls,"testCallBack","(Ljava/lang/String;II)V");
    fields.callbackArrayMethodID=env->GetMethodID(fields.cls,"testByteArray","([B)V");
    if (fields.callbackArrayMethodID==NULL){
        LOGI("callbackMethodID is null");
    }

    if (fields.callbackMethodID==NULL){
        LOGI("byteArrayMethodID is NULL");
    }

   //jint result=Java_com_yuanxiao_mydrivertest_NativeTest_TestJniAdd(env,instance,1,2);

   // env->CallVoidMethod(instance,callbackMethodID,result);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_yuanxiao_mydrivertest_NativeTest_setJniEnv(JNIEnv *env, jobject instance) {

    malloc(sizeof(fields));

    env->GetJavaVM(&fields.g_jvm);
    fields.g_obj=env->NewGlobalRef(instance);

    chips_timer_create(&enroll_timeout_id,chips_enroll_timeout_func,NULL);

    chips_timer_settime(enroll_timeout_id,CHIPS_ENROLL_TIMEOUT_TIME,0);

    jint ret=pthread_create(&pthread, NULL, pthread_run,NULL);
    if (ret!=0){
       LOGI("%s creat thread failed ",__FUNCTION__);
     }else{
        LOGI("%s creat thread success ",__FUNCTION__);
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_yuanxiao_mydrivertest_NativeTest_destoryThread(JNIEnv *env, jobject instance) {
    // TODO

    if(enroll_timeout_id !=0)
    {
        timer_delete(enroll_timeout_id);
    }
    destory= true;

    jint ret=pthread_join(pthread,NULL);
    LOGI("%s join end ret=%d ",__FUNCTION__,ret);




    // pthread_exit(0);
    //LOGI("ret = %d",ret);
   // jint  ret=pthread_detach(pthread);

}

void swap(int * p1,int *p2){
    int temp=*p1;
    *p1=*p2;
    *p2=temp;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_yuanxiao_mydrivertest_NativeTest_getBitmap(JNIEnv *env, jobject instance,jbyteArray gs_raw_data) {

    LOGI("%s",__FUNCTION__);
    jint len=(*env).GetArrayLength(gs_raw_data);
    char * images=NULL;
    images= (char *) malloc(len * sizeof(char));
    if (images==NULL){
        LOGI("malloc failed");
    }
    memset(images,0,len);

    int data=200000;

    char mToken[3]={0};

    mToken[0]=(char)data&0xff;
    mToken[1]=(char)(data>>8)&0xff;
    mToken[2]=(char)(data>>16)&0xff;

    images=mToken;

    env->SetByteArrayRegion(gs_raw_data, 0, len, (const jbyte *) images);

    //char* -> jbytearray

    // TODO
   // memset(&gs_raw_data,0, sizeof(gs_raw_data));
    //jint  len=sizeof(gs_raw_data);
    LOGI("len =%d ",len);
    //for (int i = 0; i < sizeof(gs_raw_data) ; i++) {

   ///     gs_raw_data[i]=0x1;
     //   LOGI("i=%d,gs_raw_data[i]=%d",i,gs_raw_data[i]);
   // }

    //初始化byte数组
    //jbyteArray  jbyteArray1=env->NewByteArray(sizeof(gs_raw_data));

    //    java数据拷贝到本地数据中
    //env->SetByteArrayRegion(jbyteArray1,0, len,gs_raw_data);


    int i=10;
    int j=20;
    LOGI("i=%d",i);
    LOGI("j=%d",j);
    swap(&i,&j);
    LOGI("i=%d",i);
    LOGI("j=%d",j);

    Func=&sum;
    int add=(*Func)(2,3);
    LOGI("add = %d",add);

    Function function=&sum;
    add=(*function)(3,3);
    LOGI("add = %d",add);

   // env->CallVoidMethod(instance,fields.callbackArrayMethodID,jbyteArray1);
    //return jbyteArray1;

    if (images!=NULL){
        free(images);
        images=NULL;
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_yuanxiao_mydrivertest_NativeTest_ParamGetByteArray(JNIEnv *env, jobject instance,
                                                            jbyteArray image_) {
    jbyte *image = env->GetByteArrayElements(image_, NULL);

    memset(&gs_raw_data,0, sizeof(gs_raw_data));
    jint  len=sizeof(gs_raw_data);
    LOGI("len =%d ",len);
    for (int i = 0; i < sizeof(gs_raw_data) ; i++) {

        gs_raw_data[i]=0x1;
        LOGI("i=%d,gs_raw_data[i]=%d",i,gs_raw_data[i]);
    }

    image_=env->NewByteArray(sizeof(gs_raw_data));

    malloc(sizeof(jbyte)*len);

   // env->ReleaseByteArrayElements(image_, image, 0);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_yuanxiao_mydrivertest_NativeTest_test(JNIEnv *env, jclass type) {

}