package com.example.wuhui.contral;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.pm.LabeledIntent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.text.TextUtils;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;


public class MainActivity extends Activity {

    private Button leftBtn, rightBtn, upBtn, downBtn, aBtn, bBtn, cBtn, dBtn, rBtn, sBtn;
    private TextView connTextView;
    private String ip = "10.15.51.11";
    private String port = "4444";
    private boolean connFlag;
    private SocketThread thread;
    Handler mainHandler;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        connFlag = false;

        leftBtn = (Button) findViewById(R.id.leftBtn);
        rightBtn = (Button) findViewById(R.id.rightBtn);
        upBtn = (Button) findViewById(R.id.upBtn);
        downBtn = (Button) findViewById(R.id.downBtn);
        aBtn = (Button) findViewById(R.id.aBtn);
        bBtn = (Button) findViewById(R.id.bBtn);
        cBtn = (Button) findViewById(R.id.cBtn);
        dBtn = (Button) findViewById(R.id.dBtn);
        rBtn = (Button) findViewById(R.id.rBtn);
        sBtn = (Button) findViewById(R.id.sBtn);

        connTextView = (TextView) findViewById(R.id.connTextView);

        leftBtn.setOnClickListener(new MyButtonClickListener());
        rightBtn.setOnClickListener(new MyButtonClickListener());
        upBtn.setOnClickListener(new MyButtonClickListener());
        downBtn.setOnClickListener(new MyButtonClickListener());
        aBtn.setOnClickListener(new MyButtonClickListener());
        bBtn.setOnClickListener(new MyButtonClickListener());
        cBtn.setOnClickListener(new MyButtonClickListener());
        dBtn.setOnClickListener(new MyButtonClickListener());
        rBtn.setOnClickListener(new MyButtonClickListener());
        sBtn.setOnClickListener(new MyButtonClickListener());

        connTextView.setOnClickListener(new MyTextEditClickListener());

        mainHandler = new Handler(){
            @Override
            public void handleMessage(Message msg) {
                Log.v("MainActivity", msg.obj.toString());
                if(msg.obj.toString() == "connected")
                {
                    connFlag = true;
                }
                else if(msg.obj.toString() == "disconnected")
                {
                    connFlag = false;
                }
                connTextView.setText(msg.obj.toString());
            }
        };
        thread = new SocketThread(ip, port, mainHandler);
        thread.start();

        connTextView.callOnClick();
    }

    class MyButtonClickListener implements View.OnClickListener {
        public void onClick(View v) {
            String str = ((Button) v).getText().toString();
            String sendStr;
            switch (str) {
                case "UP":
                    sendStr = "1\n";
                    break;
                case "DOWN":
                    sendStr = "2\n";
                    break;
                case "LEFT":
                    sendStr = "3\n";
                    break;
                case "RIGHT":
                    sendStr = "4\n";
                    break;
                case "A":
                    sendStr = "5\n";
                    break;
                case "B":
                    sendStr = "6\n";
                    break;
                case "C":
                    sendStr = "7\n";
                    break;
                case "D":
                    sendStr = "8\n";
                    break;
                case "R":
                    sendStr = "9\n";
                    break;
                case "S":
                    sendStr = "10\n";
                    break;
                default:
                    sendStr = "0\n";
                    break;
            }
            if(!connFlag)
            {
                Toast.makeText(v.getContext(), "服务器连接已断开", Toast.LENGTH_SHORT).show();
                return;
            }
            Message msg = new Message();
            msg.what = 0x02;
            msg.obj = sendStr;
            thread.handler.sendMessage(msg);
            Log.v("MainActivity", "toSocketThread: " + sendStr);
        }
    }

    class MyTextEditClickListener implements View.OnClickListener {
        public void onClick(View v) {
            LayoutInflater factor = LayoutInflater.from(v.getContext());
            final  View textEntryView = factor.inflate(R.layout.ip_port_dialog, null);
            final EditText ipEditText = (EditText)textEntryView.findViewById(R.id.ipEditText);
            final EditText portEditText = (EditText)textEntryView.findViewById(R.id.portEditText);

            ipEditText.setText(ip);
            portEditText.setText(String.valueOf(port));
            ipEditText.setSingleLine(true);
            portEditText.setSingleLine(true);

            new AlertDialog.Builder(v.getContext()).setTitle("请输入服务器IP地址和端口号").setIcon(
                    android.R.drawable.ic_dialog_info).setView(textEntryView).setPositiveButton("确定", new DialogInterface.OnClickListener() {
                        @Override
                        public void onClick(DialogInterface dialog, int which) {
                            if (TextUtils.isEmpty(ipEditText.getText().toString()) || !TextUtils.isDigitsOnly(portEditText.getText().toString())){
                                Toast.makeText(ipEditText.getContext(), "IP地址或端口号错误", Toast.LENGTH_SHORT).show();
                                return;
                            }
                            else        // 发送连接信息
                            {
                                ip = ipEditText.getText().toString();
                                port = portEditText.getText().toString();
                                Message msg = new Message();
                                msg.what = 0x01;
                                msg.obj = ip + ":" + port;
                                thread.handler.sendMessage(msg);
                                Log.v("MainActivity", "address: " + ip + ":" + port);
                            }
                        }
            }).setNegativeButton("取消", null).show();
        }
    }
}