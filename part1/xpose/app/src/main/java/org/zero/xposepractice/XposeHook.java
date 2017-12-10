package org.zero.xposepractice;

import android.util.Log;

import de.robv.android.xposed.IXposedHookLoadPackage;
import de.robv.android.xposed.XC_MethodHook;
import de.robv.android.xposed.XposedBridge;
import de.robv.android.xposed.XposedHelpers;
import de.robv.android.xposed.callbacks.XC_LoadPackage;

/**
 * Created by bingghost on 2017/12/7.
 *      xpose hook demo工程
 */
public class XposeHook implements IXposedHookLoadPackage {
    public static final String TAG_HOOK = "__BING_HOOK";

    /* 配置HOOK的包名 */
    private static final String TARGET_PACKAGE_NAME = "org.zero.demo";

    private boolean isTargetPackage(String currentPackage, String targetPackage) {
        return currentPackage.equals(targetPackage);
    }

    @Override
    public void handleLoadPackage(XC_LoadPackage.LoadPackageParam loadPackageParam) throws Throwable {
        /* 如果载入的不是指定包名就退出 */
        if (!isTargetPackage(loadPackageParam.packageName, TARGET_PACKAGE_NAME)) {
            return;
        }
        XposedBridge.log("Loaded app: " + loadPackageParam.packageName);
        Log.v(TAG_HOOK, "Hook Demo Load success!!!");

        /* hook 成员函数*/
        demoHookMemberFunction(loadPackageParam);

        /* hook 私有成员函数*/
        demoHookPrivateMemberFunction(loadPackageParam);

        /* hook 静态函数*/
        demoHookStaticFunction(loadPackageParam);

        /* hook native函数 */
        demoHookNativeFunction(loadPackageParam);

        /* hook 匿名函数 */
        demoHookAnonymousFunction(loadPackageParam);

        /* Hook 内部类成员函数 */
        demoHookInnerClassMemberFunction(loadPackageParam);
    }

    /* Hook 内部类成员函数 */
    private void demoHookInnerClassMemberFunction(XC_LoadPackage.LoadPackageParam loadPackageParam) {
        XposedHelpers.findAndHookMethod("org.zero.demo.Candy$InteriorCandy",
                loadPackageParam.classLoader,
                "FruitCandy",
                String.class,
                new XC_MethodHook() {
                    protected void afterHookedMethod(MethodHookParam param) {

                    }

                    protected void beforeHookedMethod(MethodHookParam param) {
                        /* 修改参数 */
                        param.args[0] = "(Hook FruitCandy)";
                    }
                });
    }

    /* hook 匿名函数 */
    private void demoHookAnonymousFunction(XC_LoadPackage.LoadPackageParam loadPackageParam) {
        XposedHelpers.findAndHookMethod("org.zero.demo.MainActivity$5$1",
                loadPackageParam.classLoader,
                "Candy",
                String.class,
                new XC_MethodHook() {
                    protected void afterHookedMethod(MethodHookParam param) {

                    }

                    protected void beforeHookedMethod(MethodHookParam param) {
                        /* 修改参数 */
                        param.args[0] = "(Hook Annotation Candy)";
                    }
                });
    }

    /* hook native函数 */
    private void demoHookNativeFunction(XC_LoadPackage.LoadPackageParam loadPackageParam) {
        XposedHelpers.findAndHookMethod("org.zero.demo.Candy",
                loadPackageParam.classLoader,
                "CottonCandy",
                int.class,
                int.class,
                new XC_MethodHook() {
                    protected void afterHookedMethod(MethodHookParam param) {
                        Integer  para1 =  (Integer) param.args[0];   // 获取参数1
                        Integer para2 = (Integer) param.args[1];     // 获取参数2
                        String s1 = Integer.toString(para1);
                        String s2 = Integer.toString(para2);
                        Log.v(TAG_HOOK,"hook param1:" + s1);
                        Log.v(TAG_HOOK,"hook param2:" + s2);
                    }

                    protected void beforeHookedMethod(MethodHookParam param) {
                        param.args[0] = 10;
                        param.args[1] = 14;
                    }
                });
    }

    /* hook 静态函数*/
    private void demoHookStaticFunction(XC_LoadPackage.LoadPackageParam loadPackageParam) {
        XposedHelpers.findAndHookMethod("org.zero.demo.Candy",
                loadPackageParam.classLoader,
                "ChocolateCandy",
                String.class,
                new XC_MethodHook() {
                    protected void afterHookedMethod(MethodHookParam param) {

                    }

                    protected void beforeHookedMethod(MethodHookParam param) {
                        /* 修改参数 */
                        param.args[0] = "(Hook ChocolateCandy)";
                    }
                });
    }
    /* hook 私有成员函数*/
    private void demoHookPrivateMemberFunction(XC_LoadPackage.LoadPackageParam loadPackageParam) {
        XposedHelpers.findAndHookMethod("org.zero.demo.Candy",
                loadPackageParam.classLoader,
                "PoppingCandy",
                String.class,
                new XC_MethodHook() {
                    protected void afterHookedMethod(MethodHookParam param) {

                    }

                    protected void beforeHookedMethod(MethodHookParam param) {
                        /* 修改参数 */
                        param.args[0] = "(Hook PoppingCandy)";
                    }
                });
    }

    /* hook 成员函数*/
    private void demoHookMemberFunction(XC_LoadPackage.LoadPackageParam loadPackageParam) {
        XposedHelpers.findAndHookMethod("org.zero.demo.Candy",
                loadPackageParam.classLoader,
                "Caramel",
                String.class,
                new XC_MethodHook() {


                    protected void afterHookedMethod(MethodHookParam param) {
                        /* 修改结果 */
                        String str = (String) param.getResult();
                        Log.v(TAG_HOOK,"hook after result :" + str);

                        String  arg1 =  (String) param.args[0];      // 获取参数1
                        param.setResult("(Hook Caramel)");           // 设置返回值

                        Log.v("get Caramel param1:", arg1);
                    }

                    protected void beforeHookedMethod(MethodHookParam param) {

                    }
                });
    }
}
