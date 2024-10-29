package com.infotronikblog.dfplayerminibt;

import android.content.Intent;
import android.os.AsyncTask;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothSocket;
import android.bluetooth.BluetoothDevice;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.SeekBar;
import android.widget.Toast;
import android.app.ProgressDialog;
import java.io.IOException;
import java.util.UUID;

public class Reproductor extends AppCompatActivity {

    Button btAnterior, btPlay, btStop, btSiguiente, btConectar, btDesconectar;
    SeekBar vol;
    TextView volumen;
    String direccion = null;
    BluetoothAdapter myBluetooth = null;
    BluetoothSocket btSocket = null;
    private boolean isBtConected = false;
    private ProgressDialog progress;
    static final UUID myUUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Intent newint = getIntent();
        direccion = newint.getStringExtra(lista_dispositivos.EXTRA_ADDRESS);

        //Vista del reproductor
        setContentView(R.layout.activity_reproductor);
        //Llamada a los Widgets
        btAnterior =   (Button) findViewById(R.id.btnAnterior);
        btPlay =   (Button) findViewById(R.id.btnPlay);
        btStop =   (Button) findViewById(R.id.btnStop);
        btSiguiente    =   (Button) findViewById(R.id.btnSiguiente);
        btDesconectar  =   (Button) findViewById(R.id.btnDesconectar);
        volumen   =   (TextView) findViewById(R.id.idVolumen);
        vol =   (SeekBar)   findViewById(R.id.seekBar);


        //Botones
        btAnterior.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                cancion_anterior();
            }
        });
        btPlay.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                reproducir();
            }
        });
        btStop.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                parar();
            }
        });
        btSiguiente.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                cancion_siguiente();
            }
        });
        btDesconectar.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                desconectar();
            }
        });
        vol.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                if (fromUser==true)
                {
                    volumen.setText(String.valueOf(progress));
                    try
                    {
                        btSocket.getOutputStream().write(String.valueOf(progress).getBytes());
                    }
                    catch (IOException e)
                    {

                    }
                }
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });
        new ConnectBT().execute(); //LLama al class para conectar
    }

    private void desconectar() {
        if (btSocket != null) {
            try {
                btSocket.close();
            } catch (IOException e) {
                msg("Error");
            }
        }
        finish();
    }

    private void cancion_anterior() {
        if (btSocket != null) {
            try {
                btSocket.getOutputStream().write("F".toString().getBytes());
            } catch (IOException e) {
                msg("Error");
            }
        }
    }

    private void reproducir() {
        if (btSocket != null) {
            try {
                btSocket.getOutputStream().write("P".toString().getBytes());
            } catch (IOException e) {
                msg("Error");
            }
        }
    }

    private void parar() {
        if (btSocket != null) {
            try {
                btSocket.getOutputStream().write("S".toString().getBytes());
            } catch (IOException e) {
                msg("Error");
            }
        }
    }

    private void cancion_siguiente() {
        if (btSocket != null) {
            try {
                btSocket.getOutputStream().write("N".toString().getBytes());

            }   catch (IOException e){
                msg("Error");
            }
        }
    }


    private void msg(String s) {
        Toast.makeText(getApplicationContext(), s, Toast.LENGTH_LONG).show();
    }


    private class ConnectBT extends AsyncTask<Void, Void, Void> {
        private boolean ConnectSucces = true;

        @Override
        protected void onPreExecute() {
            progress = ProgressDialog.show(Reproductor.this,"Conectando...","Estara listo en un momento");
        }
        @Override
        protected Void doInBackground(Void... devices){
            try {
                if (btSocket == null || !isBtConected){
                    myBluetooth = BluetoothAdapter.getDefaultAdapter();
                    BluetoothDevice dispositivo = myBluetooth.getRemoteDevice(direccion);
                    btSocket =  dispositivo.createInsecureRfcommSocketToServiceRecord(myUUID);
                    BluetoothAdapter.getDefaultAdapter().cancelDiscovery();
                    btSocket.connect();
                }
            }   catch (IOException e){
                ConnectSucces = false;
            }
            return  null;
        }
        @Override
        protected void onPostExecute (Void result){
            super.onPostExecute(result);
            if (!ConnectSucces){
                msg("Conexion Fallida, intentelo de nuevo");
                finish();
            }   else    {
                msg("Conectado.");
                isBtConected = true;
            }
            progress.dismiss();
        }
    }
    //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
}