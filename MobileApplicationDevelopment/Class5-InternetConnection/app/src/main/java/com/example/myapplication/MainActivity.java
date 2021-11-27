package com.example.myapplication;
import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.provider.MediaStore;
import android.util.JsonReader;
import android.view.View;
import android.widget.ImageView;
import android.widget.TextView;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.HashMap;
import java.util.Map;

public class MainActivity extends AppCompatActivity {
    Handler textHandle;
    Handler imageHandle;
    Handler cardHandle;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        textHandle = new Handler() {
            public void handleMessage(Message msg) {
                super.handleMessage(msg);
                TextView tv = (TextView) findViewById(R.id.textView);
                tv.setText(msg.obj + " ");
            }

        };

        imageHandle = new Handler()
        {
            public void handleMessage(Message msg)
            {
                super.handleMessage(msg);
                Bitmap image = (Bitmap) msg.obj;
                ImageView image_view = (ImageView) findViewById(R.id.image1);
                image_view.setImageBitmap(image);
            }
        };
        cardHandle = new Handler()
        {
            public void handleMessage(Message msg)
            {
                super.handleMessage(msg);
                TextView name = findViewById(R.id.editTextTextPersonName2);
                TextView birth = findViewById(R.id.editTextTextPersonName3);
                TextView idcard = findViewById(R.id.editTextTextPersonName4);
                TextView address = findViewById(R.id.editTextTextPersonName5);
                String result = new String();
                try {
                    JSONObject jsonObject = new JSONObject((String) msg.obj);
                    result = jsonObject.getJSONObject("words_result").getJSONObject("姓名").getString("words");
                    name.setText("姓名： "+result);
                    result = jsonObject.getJSONObject("words_result").getJSONObject("出生").getString("words");
                    birth.setText("出生： "+result);
                    result = jsonObject.getJSONObject("words_result").getJSONObject("公民身份号码").getString("words");
                    idcard.setText("身份证: "+result);
                    result = jsonObject.getJSONObject("words_result").getJSONObject("住址").getString("words");
                    address.setText("地址： "+result);
                } catch (JSONException e) {
                    e.printStackTrace();
                }

            }
        };
    }

    private void getWebInfo()
    {
        try {
            TextView edit_text= findViewById(R.id.editTextTextPersonName);
            URL url = new URL("https://" + edit_text.getText().toString());
            HttpURLConnection connection = (HttpURLConnection) url.openConnection();

            connection.setRequestMethod("GET");
            connection.setConnectTimeout(5000);
            InputStream inputStream = connection.getInputStream();
            InputStreamReader reader = new InputStreamReader(inputStream,"UTF-8");
            BufferedReader bufferedReader = new BufferedReader(reader);

            StringBuffer buffer = new StringBuffer();
            String tmp = null;

            while ((tmp = bufferedReader.readLine())!=null)
            {
                buffer.append(tmp);
            }
            bufferedReader.close();

            Message msg = Message.obtain();
            msg.obj = buffer;
            textHandle.sendMessage(msg);

        } catch (MalformedURLException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void getWebImage()
    {
        try {
            URL url = new URL("http://news.upc.edu.cn/__local/E/AD/5D/F50EF116C456B3F812EC5F74C89_96EB48A1_131E2.jpg");
            HttpURLConnection connection = (HttpURLConnection) url.openConnection();

            connection.setRequestMethod("GET");
            connection.setConnectTimeout(5000);
            InputStream inputStream = connection.getInputStream();
            Bitmap image = BitmapFactory.decodeStream(inputStream);
            Message msg  = Message.obtain();
            msg.obj = image;
            imageHandle.sendMessage(msg);


        } catch (IOException e) {
            e.printStackTrace();
        }
    }
    public void selectImage(View view)
    {
        Intent intent = new Intent(
                Intent.ACTION_PICK,
                MediaStore.Images.Media.EXTERNAL_CONTENT_URI
        );
        startActivityForResult(intent,2);
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, @Nullable Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        Bitmap  image= null;
        try {
            if(requestCode==2 && resultCode==RESULT_OK)
            {
                Uri imageInputStream = data.getData();
                image = BitmapFactory.decodeStream(getContentResolver().openInputStream(imageInputStream));
                Message msg = Message.obtain();
                msg.obj = image;
                imageHandle.sendMessage(msg);
            }
            String image_base64 = MyTools.bitmapToBase64(image);
            final Map<String,String> parameter = new HashMap<String,String>();
            parameter.put("image",image_base64);
            parameter.put("id_card_side","front");
            new Thread(new Runnable() {
                @Override
                public void run() {
                    try {
                        String url_path = "https://aip.baidubce.com/rest/2.0/ocr/v1/idcard?access_token=24.2ee1a2da05d28b1173eeda2c719ee461.2592000.1639958466.282335-23815654";
                        String result = MyTools.submitPostData(url_path,parameter,"UTF-8");
                        Message msg = Message.obtain();
                        msg.obj = result;
                        cardHandle.sendMessage(msg);
                    }catch (Exception e){}
                }
            }).start();

        }catch (FileNotFoundException e) {
            e.printStackTrace();
        } ;
    }


    public void getInfo(View view)
    {
        new Thread((Runnable) () ->{
            getWebInfo();
    }).start();
    }

    public void getImage(View view)
    {
        new Thread((Runnable) () ->{
            getWebImage();
        }).start();
    }

}