package com.example.myapplication;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.renderscript.Sampler;
import android.view.View;
import android.widget.CheckBox;
import android.widget.EditText;
import android.widget.HeaderViewListAdapter;
import android.widget.TextView;
import android.widget.Toast;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.charset.StandardCharsets;

public class MainActivity extends AppCompatActivity {
    private TextView tv;
    private int count =0;
    private String user_name="";
    private String password="";
    private boolean state =false;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        tv = (TextView) findViewById(R.id.textView);
        new Thread()
        {
            @Override
            public void run() {
                while (true)
                {
                    AutoIncrease();
                }
            }
        }.start();

        Reload("");

    }

    public void AutoIncrease()
    {
        try{Thread.sleep(500);}catch (InterruptedException e){};
        count ++;
        Message msg = Message.obtain();
        msg.arg1 = count;
        handler.sendMessage(msg);
    }

    Handler handler = new Handler()
    {
        public void handleMessage(Message msg)
        {
            super.handleMessage(msg);
            tv.setText(msg.arg1+" Seconds");
        }
    };

    public void Login(View view)
    {
        EditText et = (EditText) findViewById(R.id.editTextTextPersonName);
        user_name = et.getText().toString();

        et = (EditText) findViewById(R.id.editTextTextPassword);
        password = et.getText().toString();

        CheckBox is_check = (CheckBox) findViewById(R.id.checkBox) ;
        state = is_check.isChecked();

        if(is_check.isClickable())SaveToLocal("");

        Toast.makeText(this, user_name+'\n'+password+'\n'+state, Toast.LENGTH_SHORT).show();
    }

    private void SaveToLocal(String path)
    {
        try {

            String filename1 = "user.txt";
            String filename2 = "password.txt";
            String filename3 = "state.txt";
            FileOutputStream fileOutputStream1 = openFileOutput(filename1,MODE_PRIVATE);
            FileOutputStream fileOutputStream2 = openFileOutput(filename2,MODE_PRIVATE);
            FileOutputStream fileOutputStream3 = openFileOutput(filename3,MODE_PRIVATE);

            fileOutputStream1.write(user_name.getBytes(StandardCharsets.UTF_8));
            fileOutputStream2.write(password.getBytes(StandardCharsets.UTF_8));
            fileOutputStream3.write(String.valueOf(state).getBytes(StandardCharsets.UTF_8));

            fileOutputStream1.close();
            fileOutputStream2.close();
            fileOutputStream3.close();
        }
        catch (FileNotFoundException e){} catch (IOException e) {
            e.printStackTrace();
        }
    }

    private void Reload(String path)
    {
        try {
            String filename1 = "user.txt";
            String filename2 = "password.txt";
            String filename3 = "state.txt";
            FileInputStream fileInputStream1 = openFileInput(filename1);
            FileInputStream fileInputStream2 = openFileInput(filename2);
            FileInputStream fileInputStream3 = openFileInput(filename3);

            byte[] buffer = new byte[fileInputStream1.available()];
            fileInputStream1.read(buffer);
            user_name = new String(buffer);

            buffer = new byte[fileInputStream2.available()];
            fileInputStream2.read(buffer);
            password = new String(buffer);

            buffer = new  byte[fileInputStream3.available()];
            fileInputStream3.read(buffer);
            state = Boolean.valueOf(new String(buffer));

            fileInputStream1.close();
            fileInputStream2.close();
        } catch (FileNotFoundException e) {} catch (IOException e) {
            e.printStackTrace();
        }
        EditText v = (EditText) findViewById(R.id.editTextTextPersonName);
        v.setText(user_name);
        v = (EditText) findViewById(R.id.editTextTextPassword);
        v.setText(password);
        CheckBox vc = (CheckBox)findViewById(R.id.checkBox);
        vc.setChecked(state);
    }


}