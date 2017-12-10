package org.zero.demo;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {


    private TextView mTextView1;
    private TextView mTextView2;
    private TextView mTextView3;
    private TextView mTextView4;
    private TextView mTextView5;
    private TextView mTextView6;

    public void init() {
        mTextView1 = findViewById(R.id.textView1);
        mTextView2 = findViewById(R.id.textView2);
        mTextView3 = findViewById(R.id.textView3);
        mTextView4 = findViewById(R.id.textView4);
        mTextView5 = findViewById(R.id.textView5);
        mTextView6 = findViewById(R.id.textView6);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_demo);

        init();

        /* 类成员函数 */
        Button button1 = findViewById(R.id.button1);
        button1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Candy candy = new Candy("default");
                String result = candy.Caramel("caramel");

                mTextView1.setText("[result]:" + result);
            }
        });

        /* 私有成员函数 */
        Button button2 = findViewById(R.id.button2);
        button2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Candy candy = new Candy("default");
                String result = candy.callPoppingCandy("popping candy");

                mTextView2.setText("[result]:" + result);
            }
        });

        /* 静态成员函数 */
        Button button3 = findViewById(R.id.button3);
        button3.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                String result = Candy.ChocolateCandy("Chocolate");

                mTextView3.setText("[result]:" + result);
            }
        });

        /* Native函数 */
        Button button4 = findViewById(R.id.button4);
        button4.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                String result = Candy.CottonCandy(5, 6);

                mTextView4.setText("[result]:" + result);
            }
        });

        /* 匿名类成员函数 */
        Button button5 = findViewById(R.id.button5);
        button5.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                CandyAbstract candy = new CandyAbstract() {
                    @Override
                    public String Candy(String incantation) {
                        return incantation;
                    }
                };

                String result = candy.Candy("sesame candy");
                mTextView5.setText("[result]:" + result);
            }
        });

        /* 内部类成员函数 */
        Button button6 = findViewById(R.id.button6);
        button6.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Candy candy = new Candy("default");
                Candy.InteriorCandy innerCandy = candy.new InteriorCandy();

                String result = innerCandy.FruitCandy("fruit candy");
                mTextView6.setText(result);
            }
        });

    }


}
