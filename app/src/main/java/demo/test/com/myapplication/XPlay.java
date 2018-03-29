package demo.test.com.myapplication;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;
import android.view.SurfaceHolder;

/**
 * Created by xiezheng on 2018/3/23.
 */

public class XPlay extends GLSurfaceView implements Runnable,SurfaceHolder.Callback{

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        new Thread(this).start();
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int w, int h) {
        super.surfaceChanged(holder, format, w, h);
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        super.surfaceDestroyed(holder);
    }

    public XPlay(Context context,AttributeSet attrs) {
        super(context,attrs);
    }

    @Override
    public void run() {
        //在线程之中使用，为了不阻塞画面的刷新
        open("/sdcard/1080.mp4",getHolder().getSurface());
    }

    public native void open(String url,Object obj);
}
