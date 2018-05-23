package com.yuanxiao.mydrivertest;

import android.app.Activity;
import android.hardware.fingerprint.FingerprintManager;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.util.Log;

/**
 * Created by root on 18-5-18.
 */

public class DisplacementActivity extends Activity {

    public static byte[] mToken=new byte[]{0x01,0x02,0x03,0x04};

    private static final String TAG = "DisplacementActivity";

    FingerprintManager mFingerprintManager;
    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        mFingerprintManager= (FingerprintManager) getSystemService(FINGERPRINT_SERVICE);
        int data=255;
        byte[] bytes=new byte[60];
        bytes[0]=(byte)(data & 0xff);
        int i = bytes[0];

        i = bytes[0]&0xff;
        Log.i(TAG, "onCreate: i:"+i);


        Log.i(TAG, "onCreate: "+bytes[0]);

        int data2=255;
        bytes[0]=(byte)((data2>>4)&0xff);
        Log.i(TAG, "onCreate: "+bytes[0]);

        int data3=255;
        bytes[0]=(byte)((data3<<4)&0xff);
        Log.i(TAG, "onCreate: "+bytes[0]);
        Log.i(TAG, "onCreate: "+data3);


        Log.i(TAG, "mToken.lenght:"+mToken.length);
        mToken=new byte[69];
        Log.i(TAG, "mToken.lenght:"+mToken.length);
        for (int j=0;j<mToken.length;j++){
            Log.i(TAG, " j:"+j);
            Log.i(TAG, "onCreate: j:"+mToken[j]);
        }
    }
}
