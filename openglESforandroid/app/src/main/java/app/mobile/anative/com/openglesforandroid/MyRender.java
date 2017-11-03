package app.mobile.anative.com.openglesforandroid;

import android.content.Context;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import app.mobile.anative.com.openglesforandroid.shape.Circle;
import app.mobile.anative.com.openglesforandroid.shape.Squre;
import app.mobile.anative.com.openglesforandroid.shape.Triangle;

import static android.opengl.GLES20.GL_COLOR_BUFFER_BIT;
/**
 * Created by android on 11/2/17.
 */
public class MyRender implements GLSurfaceView.Renderer {
    private final Context context;
    private Triangle triangle;
    private Squre square;
    private Circle circle;

    public MyRender(Context context){
        this.context = context;
    }

    @Override
    public void onSurfaceCreated(GL10 gl10, EGLConfig eglConfig) {
        GLES20.glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
//        triangle = new Triangle(context);
//        square=new Squre(context);
        circle = new Circle(context);
    }

    @Override
    public void onSurfaceChanged(GL10 gl10, int width, int height) {
        GLES20.glViewport(0,0,width,height);
        circle.projectionMatrix(width, height);
    }

    @Override
    public void onDrawFrame(GL10 gl10) {
        GLES20.glClear(GL_COLOR_BUFFER_BIT);
//        triangle.draw();
        circle.draw();
//        square.draw();
    }
}
