package com.example.wuhui.contral;

import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.text.TextUtils;
import android.util.Log;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.net.InetSocketAddress;
import java.net.Socket;

/**
 * Created by wuhui on 21/09/16.
 */
public class SocketThread extends Thread{
    private String ip;
    private int port;
    private Socket socket;
    private BufferedWriter sWrite;
    Handler handler;
    Handler uiHandler;

    public SocketThread(String ip, String port, Handler handler)
    {
        this.ip = ip;
        this.port = Integer.valueOf(port);
        this.uiHandler = handler;
    }

    public void run() {
        if (!TextUtils.isEmpty(ip))  //  建立连接
        {
            try {
                socket = new Socket(ip, port);
                // 如果连接成功，发送success消息到UI线程,再进行下面的初始化操作
                if(socket != null && socket.isConnected())
                {
                    Message msg = new Message();
                    msg.obj = "connected";
                    uiHandler.sendMessage(msg);
                }
                sWrite = new BufferedWriter(new OutputStreamWriter(socket.getOutputStream(), "utf-8"));
            } catch (IOException e) {
                if(socket != null && socket.isConnected())
                {
                    Message msg = new Message();
                    msg.obj = "disconnected";
                    uiHandler.sendMessage(msg);
                    try {
                        socket.close();
                    } catch (IOException e1) {
                        e1.printStackTrace();
                    }
                }
                e.printStackTrace();
            }
        }

        Looper.prepare();
        handler = new Handler(){
            @Override
            public void handleMessage(Message msg) {
                if(msg.what == 0x01)        // 关闭并重新连接
                {
                    try {
                        if(socket != null)
                        {
                            socket.close();
                        }
                        Message tmsg = new Message();
                        tmsg.obj = "disconnected";
                        uiHandler.sendMessage(tmsg);
                        if(socket != null)
                        {
                            socket = null;
                        }
                        if(sWrite != null)
                        {
                            sWrite = null;
                        }
                    } catch (IOException e) {
                        e.printStackTrace();
                    }

                    String recvStr = msg.obj.toString();
                    String address[] = recvStr.split(":");
                    ip = address[0];
                    port = Integer.valueOf(address[1]);

                    Log.v("SocketThread", "ip: " + ip + " port: " + port);
                    try {
                        socket = new Socket(ip, port);
                        Message tmsg = new Message();
                        tmsg.obj = "connected";
                        uiHandler.sendMessage(tmsg);
                        sWrite = new BufferedWriter(new OutputStreamWriter(socket.getOutputStream(), "utf-8"));
                    } catch (IOException e) {
                        Message tmsg = new Message();
                        tmsg.obj = "disconnected";
                        uiHandler.sendMessage(tmsg);
                        if(socket != null)
                        {
                            socket = null;
                        }
                        if(sWrite != null)
                        {
                            sWrite = null;
                        }
                        e.printStackTrace();
                    }
                }
                else
                {
                    try {
                        Log.v("SocketThread", "abcd: " + msg.obj.toString());
                        sWrite.write(msg.obj.toString());
                        sWrite.flush();
                    } catch (IOException e) {
                        Message tmsg = new Message();
                        tmsg.obj = "disconnected";
                        uiHandler.sendMessage(tmsg);
                        if(socket != null)
                        {
                            socket = null;
                        }
                        if(sWrite != null)
                        {
                            sWrite = null;
                        }
                        e.printStackTrace();
                    }
                }
            }
        };
        Looper.loop();
    }
}
