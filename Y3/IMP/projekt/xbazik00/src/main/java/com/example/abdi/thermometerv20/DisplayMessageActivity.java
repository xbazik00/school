package com.example.abdi.thermometerv20;

/**
 * Activity that takes care of getting data from WiFi AP and displaying it on the device.
 * Author: Martin Bazik, xbazik00, 80%
 * Date: 9.12.2017
 * Tutorial: https://www.tutorialspoint.com/android/android_json_parser.htm
 */

import android.os.AsyncTask;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.ListAdapter;
import android.widget.ListView;
import android.widget.SimpleAdapter;
import android.widget.TextView;
import android.widget.Toast;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;
import java.util.Calendar;
import java.util.HashMap;


/**
 * This class represents activity for reading and displaying temperature in given format.
 */
public class DisplayMessageActivity extends AppCompatActivity {

    // Debug tag
    private String TAG = DisplayMessageActivity.class.getSimpleName();

    // Views for displaying temperatures
    private ListView lv;
    private TextView tv;

    // Hash map for temperatures
    private ArrayList<HashMap<String, String>> tempList;

    // Current temperature
    private String current_temp;

    // Adapter for displaying list
    private ListAdapter adapter;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_display_message);

        // Current temperature
        tv = findViewById(R.id.textView);
        current_temp = new String();

        // History of temperature
        lv = findViewById(R.id.list);
        tempList = new ArrayList<>();

        adapter = new SimpleAdapter(DisplayMessageActivity.this, tempList,
                R.layout.list_item, new String[]{ "id","value"},
                new int[]{R.id.id, R.id.value});

        // Start
        new GetTemps().execute();
    }

    /**
     * Method that starts reading of temperatures by clicking a button
     * @param view current view
     */
    public void readTemp(View view){
        new GetTemps().execute();
    }

    /**
     * Class for actual reading and displaying temperatures
     */
    private class GetTemps extends AsyncTask<Void, Void, Void> {
        /**
         * Method that gives info that json is being downloaded.
         */
        @Override
        protected void onPreExecute() {
            super.onPreExecute();
            Toast.makeText(DisplayMessageActivity.this,"Json Data is downloading",
                    Toast.LENGTH_LONG).show();

        }

        /**
         * Method downloads and parses data.
         */
        @Override
        protected Void doInBackground(Void... arg0) {
            tempList.clear();

            // http handler
            HttpHandler sh = new HttpHandler();

            // AP IP address
            String jsonStr = sh.makeServiceCall("http://192.168.4.1:80");

            // debug log
            Log.e(TAG, "Response from url: " + jsonStr);

            // if json was received
            if (jsonStr != null) {
                try {
                    // Whole json
                    JSONObject jsonObj = new JSONObject(jsonStr);

                    // Parses current temperature
                    current_temp = jsonObj.getString("temperature");
                    Log.e(TAG, "Temperature: " + current_temp);

                    // Parses array of temperatures
                    JSONArray history = jsonObj.getJSONArray("old");

                    // Number of temperatures
                    int length = history.length() - 1;

                    int counter = 0;

                    // Parses history of temperatures
                    for (int i = length ; i >= 0 ; i--) {
                        // No temperature
                        if(!history.getString(i).equals("nan")){

                            // current time
                            Calendar now = Calendar.getInstance();

                            // Temperatures are saved in 1 minute interval
                            now.add(Calendar.MINUTE, -(counter++));
                            String tempTime = now.get(Calendar.HOUR_OF_DAY) + ":" + now.get(Calendar.MINUTE);

                            HashMap<String, String> temp_map = new HashMap<>();

                            // Hash mapping
                            temp_map.put("id", "Time: " + tempTime);
                            temp_map.put("value", history.getString(i) + "°C");

                            // Added to list for list view
                            tempList.add(temp_map);
                        }
                    }

                } catch (final JSONException e) {
                    Log.e(TAG, "Json parsing error: " + e.getMessage());
                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            Toast.makeText(getApplicationContext(),
                                    "Json parsing error: " + e.getMessage(),
                                    Toast.LENGTH_LONG).show();
                        }
                    });

                }

            } else {
                Log.e(TAG, "Couldn't get json from server.");
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        Toast.makeText(getApplicationContext(),
                                "Couldn't get json from server. Check LogCat for possible errors!",
                                Toast.LENGTH_LONG).show();
                    }
                });
            }

            return null;
        }

        @Override
        protected void onPostExecute(Void result) {
            super.onPostExecute(result);

            // ListView adapter is set to current history
            lv.setAdapter(adapter);

            // Text is set to current temperature
            String result_temp = current_temp + "°C";
            tv.setText(result_temp);
        }
    }
}
