package fingerprint.chipsailing.dynamicactivity;

import android.util.Log;

/**
 * Created by root on 18-5-22.
 */

public class Native {

    private static final String TAG = "MainActivity";

    static {
        Log.i(TAG, "static initializer: dynamic_jni");
      //  loadLibrary("dynamic");
        System.loadLibrary("dynamic_jni");
        NativeInit();
    }

    public  void init(){
        NativeInit();
    }

    public  int  add(int a,int b){
       return  NativeAdd(a,b);
    }

    public static native void NativeInit();

    public static native int NativeAdd(int a,int b);
}
