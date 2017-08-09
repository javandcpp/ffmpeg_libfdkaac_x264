package com.guagua.nativeapp;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;

import org.libsdl.app.SDLActivity;

public class EntranceActivity extends AppCompatActivity implements View.OnClickListener {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_entrance);
        findViewById(R.id.MainActivity).setOnClickListener(this);
        findViewById(R.id.SDLActivity).setOnClickListener(this);
    }

    @Override
    public void onClick(View view) {

        switch (view.getId()) {
            case R.id.MainActivity:
                startActivity(new Intent(this, MainActivity.class));
                break;

            case R.id.SDLActivity:
                startActivity(new Intent(this, SDLPlayerActivity.class));
                break;
        }
    }
}
