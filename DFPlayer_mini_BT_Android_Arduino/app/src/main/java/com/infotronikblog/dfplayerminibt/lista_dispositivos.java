package com.infotronikblog.dfplayerminibt;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;

import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import java.util.ArrayList;
import java.util.Set;

public class lista_dispositivos extends AppCompatActivity {

    Button      btnVinculados;
    ListView    listadoDispositivos;
    private BluetoothAdapter myBluetooth = null;
    private Set<BluetoothDevice>dispositivosVinculados;
    public static String EXTRA_ADDRESS = "device_address";
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_lista_dispositivos);

        btnVinculados = (Button)findViewById(R.id.btnVinculados);
        listadoDispositivos = (ListView)findViewById(R.id.idLista);

        myBluetooth = BluetoothAdapter.getDefaultAdapter();

        if (myBluetooth == null){
            Toast.makeText(getApplicationContext(),"Dispositivo Bluetooth no Disponible", Toast.LENGTH_LONG).show();
            finish();
        }   else if (!myBluetooth.isEnabled()){
            Intent turnBTon = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(turnBTon,1);
        }
        btnVinculados.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                pairedDeviceList();
            }
        });
    }
    private void pairedDeviceList(){
        dispositivosVinculados = myBluetooth.getBondedDevices();
        ArrayList  lista =   new ArrayList();

        if (dispositivosVinculados.size()>0){
            for (BluetoothDevice bt : dispositivosVinculados){
                lista.add(bt.getName()+"\n"+bt.getAddress());

            }
        }   else {
            Toast.makeText(getApplicationContext(),"No se encontraron Dispositivos", Toast.LENGTH_LONG).show();
        }

        final ArrayAdapter adapter = new ArrayAdapter(this,android.R.layout.simple_list_item_1,lista);
        listadoDispositivos.setAdapter (adapter);
        listadoDispositivos.setOnItemClickListener(myListClickListener); //Method called when the device from the list is clicked
    }

    private AdapterView.OnItemClickListener myListClickListener = new AdapterView.OnItemClickListener()
    {
        public void onItemClick (AdapterView<?> av, View v, int arg2, long arg3)
        {
            // Get the device MAC address, the last 17 chars in the View

            String info = ((TextView) v).getText().toString();
            String address = info.substring(info.length() - 17);

            // Make an intent to start next activity.
            Intent i = new Intent(lista_dispositivos.this, Reproductor.class);



            //Change the activity.
            i.putExtra(EXTRA_ADDRESS, address); //this will be received at ledControl (class) Activity
            startActivity(i);
        }
    };



    //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
}
