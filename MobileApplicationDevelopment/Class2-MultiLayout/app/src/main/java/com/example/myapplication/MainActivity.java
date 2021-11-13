package com.example.myapplication;

import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;

import android.app.Dialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.graphics.Color;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;
import android.widget.Toast;

import java.util.ArrayList;
import java.util.List;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Intent intent2 = new Intent(MainActivity.this,SecondActivity.class);
        startActivity(intent2);
    }

    public void click(View v) {
        TextView textView = (TextView) findViewById(R.id.textView);
        textView.setTextColor(Color.RED);
    }

    public void TestToast(View v){
        Toast.makeText(this, "Hello ", Toast.LENGTH_SHORT).show();
    }

    public void TestAlertDialog(View v)
    {
        String[] items = new String[]{"战斗到底", "单挑","群殴","伏地魔","苟都可以苟"};
        boolean[] result = new boolean[]{false,false,false,false,false};
        List<Integer> choice = new ArrayList<>();
        AlertDialog.Builder testAlertDialog = new AlertDialog.Builder(MainActivity.this);
        testAlertDialog.setIcon(R.mipmap.ic_launcher);
        testAlertDialog.setTitle("你过来呀！");
//        testAlertDialog.setMessage("^_^");
        testAlertDialog.setMultiChoiceItems(items, result, new DialogInterface.OnMultiChoiceClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which, boolean isChecked) {
                if(isChecked){
                    choice.add(which);
                }else
                {
                    choice.remove(choice.indexOf(which));
                }
            }
        });
        testAlertDialog.setPositiveButton("Right", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                StringBuilder result  = new StringBuilder();
                for (int j=0;j<choice.size();j++)
                {
                    result.append(items[choice.get(j)]);
                }
                Toast.makeText(MainActivity.this, "Successful\n" + result, Toast.LENGTH_SHORT).show();
            }
        });
        testAlertDialog.create();
        testAlertDialog.show();
    }
}