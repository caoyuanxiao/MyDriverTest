package com.yuanxiao.mydrivertest;

import android.util.Log;

/**
 * Created by root on 18-5-7.
 */

public class NativeTest {

    private static final String TAG = "NativeTest";

    private static  NativeTest mNativeTest;

    public int getName() {
        return Name;
    }

    public int Name=1;

    public static NativeTest getmNativeTest() {
        if (mNativeTest==null){
            mNativeTest=new NativeTest();
        }
        return mNativeTest;
    }

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    /**
     *
     * @param result
     */
    public void testCallBack(String name,int result,int result2){
        Log.i(TAG, "testCallBack: result:"+result +"name:"+name+"result2:"+result2);
    }

    public void testByteArray(byte[] data){
        Log.i(TAG, "testByteArray: data.length:"+data.length);
        Log.i(TAG, "testByteArray: byte[0]"+data[0]);
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    public native int TestJniAdd(int a,int b);

    public native  void setJniEnv();

    public native void setCallback();

    public native void destoryThread();

    public native void getBitmap(byte[] datas);

    public native void ParamGetByteArray(byte[] image);

    public static native void test();




}
