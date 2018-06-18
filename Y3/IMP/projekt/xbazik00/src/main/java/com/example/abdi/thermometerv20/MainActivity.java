package com.example.abdi.thermometerv20;

/**
 * Main activity that starts display activity.
 * Author: Martin Bazik, xbazik00
 * Date: 9.12.2017
 */

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;

/**
 * Class of main activity
 */
public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }

    /**
     * Called when the user taps the Read temperature button.
     */
    public void readTemp(View view) {
        Intent intent = new Intent(this, DisplayMessageActivity.class);
        startActivity(intent);
    }

}
