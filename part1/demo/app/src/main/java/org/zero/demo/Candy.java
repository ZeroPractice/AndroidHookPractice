package org.zero.demo;


/**
 * Created by bingghost on 2017/12/8.
 *
 *  每个语法糖都会用一条咒语开启
 */

public class Candy {
    public String mProperty = "default";

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    public Candy(String property) {
        /* 带参构造 */
        mProperty = property;
    }

    /* 成员函数 */
    public String Caramel(String incantation) {
        return incantation;
    }

    /* 私有成员函数 */
    private String PoppingCandy(String incantation) {
        return incantation;
    }
    public String callPoppingCandy(String incantation) {
        return PoppingCandy(incantation);
    }


    /* 静态成员函数 */
    public static String ChocolateCandy(String incantation) {
        return incantation;
    }

    /* Native函数 */
    public static native String CottonCandy(int a, int b);


    public class InteriorCandy {

        /* 内部类成员函数 */
        public String FruitCandy(String incantation) {
            return incantation;
        }
    }
}

