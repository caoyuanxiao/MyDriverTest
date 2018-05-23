package com.yuanxiao.mydrivertest;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {

    private static final String TAG = "MainActivity";
    NativeTest mNativeTest;
    TextView tv;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        findViewById(R.id.btn_destory_pthread).setOnClickListener(this);

        // Example of a call to a native method
        tv = (TextView) findViewById(R.id.sample_text);

        mNativeTest=NativeTest.getmNativeTest();
        new Thread(new Runnable() {
            @Override
            public void run() {


                mNativeTest.setJniEnv();
                mNativeTest.setCallback();

                mNativeTest.TestJniAdd(2,3);
            }
        }).start();


      //  tv.setText(mNativeTest.stringFromJNI());

    }


    @Override
    public void onClick(View view) {

        int name=mNativeTest.getName();
        Log.i(TAG, "onClick: name:"+name);
        byte[] datas=new byte[3];
        mNativeTest.getBitmap(datas);

        Log.i(TAG, "bytes[1]:"+datas[1] +"bytes.length:"+datas.length);
        for (int i=0;i<datas.length;i++){
            Log.i(TAG, "bytes:"+datas[i]);
        }



    }
}
