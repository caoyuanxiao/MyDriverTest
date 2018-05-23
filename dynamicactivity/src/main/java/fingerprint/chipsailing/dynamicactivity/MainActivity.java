package fingerprint.chipsailing.dynamicactivity;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;

public class MainActivity extends AppCompatActivity {

    private static final String TAG = "MainActivity";
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Native mNative=new Native();
       // mNative.init();
        int add =mNative.add(2,3);
        Log.i(TAG, "onCreate: add="+add);


    }
}
