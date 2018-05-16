package com.example.kuadam.weatherrover;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.os.Bundle;
import android.os.Handler;
import android.support.v7.app.AppCompatActivity;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Set;
import java.util.UUID;

public class MainActivity extends AppCompatActivity {

    TextView textViewH, textViewT;
    String Bluetooth_MAC = "00:18:E4:00:19:B9";
    BluetoothAdapter bluetoothAdapter = null;
    BluetoothDevice bluetoothDevice = null;
    BluetoothSocket bluetoothSocket = null;
    OutputStream outputStream;
    InputStream inputStream;
    Thread workerThread;
    byte[] readBuffer;
    int readBufferPosition;
    volatile boolean stopWorker;
    UUID uuid = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
    boolean connected = false;
    boolean upDown = false;
    char direction = 's';
    boolean[] dir = {false,false,false,false};
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Button buttonL = (Button) findViewById(R.id.buttonl);
        Button buttonR = (Button) findViewById(R.id.buttonr);
        Button buttonU = (Button) findViewById(R.id.buttonu);
        Button buttonD = (Button) findViewById(R.id.buttond);
        Button buttonS = (Button) findViewById(R.id.buttons);

        textViewH = (TextView) findViewById(R.id.textHum);
        textViewT = (TextView) findViewById(R.id.textTemp);

        buttonL.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View view, MotionEvent motionEvent) {
                if (connected) {
                    switch (motionEvent.getAction()) {
                        case MotionEvent.ACTION_DOWN:
                            dir[0]=true;
                            break;
                        case MotionEvent.ACTION_UP:
                            dir[0]=false;
                            break;
                    }
                    Send();
                } else {
                    Toast.makeText(getApplicationContext(), "Connect bluetooth", Toast.LENGTH_SHORT).show();
                }
                return false;
            }
        });

        buttonR.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View view, MotionEvent motionEvent) {
                if (connected) {
                    switch (motionEvent.getAction()) {
                        case MotionEvent.ACTION_DOWN:
                            dir[1]=true;
                            break;
                        case MotionEvent.ACTION_UP:
                            dir[1]=false;
                            break;
                    }
                    Send();
                } else {
                    Toast.makeText(getApplicationContext(), "Connect bluetooth", Toast.LENGTH_SHORT).show();
                }
                return false;
            }
        });

        buttonU.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View view, MotionEvent motionEvent) {
                if (connected) {
                    switch (motionEvent.getAction()) {
                        case MotionEvent.ACTION_DOWN:
                            dir[2]=true;
                            break;
                        case MotionEvent.ACTION_UP:
                            dir[2]=false;
                            break;
                    }
                    Send();
                } else {
                    Toast.makeText(getApplicationContext(), "Connect bluetooth", Toast.LENGTH_SHORT).show();
                }
                return false;
            }
        });

        buttonD.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View view, MotionEvent motionEvent) {
                if (connected) {
                    switch (motionEvent.getAction()) {
                        case MotionEvent.ACTION_DOWN:
                            dir[3]=true;
                            break;
                        case MotionEvent.ACTION_UP:
                            dir[3]=false;
                            break;
                    }
                    Send();
                } else {
                    Toast.makeText(getApplicationContext(), "Connect bluetooth", Toast.LENGTH_SHORT).show();
                }
                return false;
            }
        });

        buttonS.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (initBT()) {
                    connectBT();
                }
            }
        });


    }

    public void Send() {
        try {
            if(dir[0]){
                outputStream.write('l');
                outputStream.flush();
            }
            else if(dir[1]){
                outputStream.write('r');
                outputStream.flush();
            }
            else if(dir[2]){
                outputStream.write('u');
                outputStream.flush();
            }
            else if(dir[3]){
                outputStream.write('d');
                outputStream.flush();
            }
            else{
                outputStream.write('s');
                outputStream.flush();
            }

        } catch (IOException e) {
            Toast.makeText(getApplicationContext(), "Connection error", Toast.LENGTH_SHORT).show();
        }
    }

    public boolean initBT() {
        bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        if (bluetoothAdapter == null) {
            Toast.makeText(getApplicationContext(), "Device does not support Bluetooth", Toast.LENGTH_SHORT).show();
        } else {
            if (!bluetoothAdapter.isEnabled()) {
                Toast.makeText(getApplicationContext(), "Bluetooth is not enable", Toast.LENGTH_SHORT).show();
            }
        }
        Set<BluetoothDevice> pairedDevices = bluetoothAdapter.getBondedDevices();
        boolean good = false;
        if (pairedDevices.isEmpty()) {
            Toast.makeText(getApplicationContext(), "No paired devices", Toast.LENGTH_SHORT).show();
        } else {
            for (BluetoothDevice it : pairedDevices) {
                if (it.getAddress().equals(Bluetooth_MAC)) {
                    bluetoothDevice = it;
                    good = true;
                    break;
                }
            }
            Toast.makeText(getApplicationContext(), "Can't find device", Toast.LENGTH_SHORT).show();
        }
        return good;
    }

    public void connectBT() {
        boolean good = false;
        try {
            bluetoothSocket = bluetoothDevice.createRfcommSocketToServiceRecord(uuid);
            BluetoothAdapter.getDefaultAdapter().cancelDiscovery();
            bluetoothSocket.connect();
            Toast.makeText(getApplicationContext(), "connected", Toast.LENGTH_LONG).show();
            good = true;
            connected = true;
        } catch (IOException e) {
            Toast.makeText(getApplicationContext(), "Connection error 1", Toast.LENGTH_SHORT).show();
        }
        if (good) {
            try {
                outputStream = bluetoothSocket.getOutputStream();
                inputStream = bluetoothSocket.getInputStream();
                readData();
                textViewH.setText("BT ready");
            } catch (IOException e) {
                Toast.makeText(getApplicationContext(), "Connection error 2", Toast.LENGTH_SHORT).show();
            }
        }
    }

    void readData() {
        final Handler handler = new Handler();
        final byte end = 10;

        stopWorker = false;
        readBufferPosition = 0;
        readBuffer = new byte[1024];
        workerThread = new Thread(new Runnable() {
            public void run() {
                while (!Thread.currentThread().isInterrupted() && !stopWorker) {
                    try {
                        int bytesAvailable = inputStream.available();
                        if (bytesAvailable > 0) {
                            byte[] packetBytes = new byte[bytesAvailable];
                            inputStream.read(packetBytes);
                            for (int i = 0; i < bytesAvailable; i++) {
                                byte b = packetBytes[i];
                                if (end == b) {
                                    byte[] encodedBytes = new byte[readBufferPosition];
                                    System.arraycopy(readBuffer, 0, encodedBytes, 0, encodedBytes.length);
                                    final String data = new String(encodedBytes, "US-ASCII");
                                    readBufferPosition = 0;

                                    handler.post(new Runnable() {
                                        public void run() {
                                            String tempH = data.substring(0, 2);
                                            String tempT = data.substring(2, 5);
                                            textViewH.setText(tempH + "%");
                                            textViewT.setText(tempT + "\u2103");
                                        }
                                    });
                                } else {
                                    readBuffer[readBufferPosition++] = b;
                                }
                            }
                        }
                    } catch (IOException ex) {
                        stopWorker = true;
                    }
                }
            }
        });
        workerThread.start();
    }
}
