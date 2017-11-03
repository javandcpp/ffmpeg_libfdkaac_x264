package app.mobile.anative.com.openglesforandroid.shape;

import android.content.Context;
import android.opengl.GLES20;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

import app.mobile.anative.com.openglesforandroid.R;
import app.mobile.anative.com.openglesforandroid.helper.ShaderHelper;
import app.mobile.anative.com.openglesforandroid.utils.TextResourceReader;

/**
 * Created by android on 11/2/17.
 */

public class Squre {

    private static final String A_POSITION = "a_Position";
    private static final String U_COLOR = "u_Color";
    private static final int POSITION_COMPONENT_COUNT = 3;
    private final int uColorLocation;
    private final int aPositionLocation;

    private Context context;

    private static final int BYTES_PER_FLOAT = 4;
    private FloatBuffer vertexBuffer;

    // 数组中每个顶点的坐标数
    static final int COORDS_PER_VERTEX = 2;
    // 每个顶点的坐标数     X ,  Y
    static float triangleCoords[] = {
            0f, 0.5f,    // top
            0.5f, -0.5f,   // bottom left
            0f, -0.5f,
        };   // bottom right
    private int program;

    public Squre(Context context) {
        this.context = context;

        /*我们在该类中定义的float类型的数据并不能直接被opengl使用，float[ ]是属于虚拟机环境的，而Opengl作为本地系统库直接运行
        在硬件上，所以我们需要将float[ ] 转化为FloatBuffer以使数据可以被opengl使用
        */
        vertexBuffer = ByteBuffer
                .allocateDirect(triangleCoords.length * BYTES_PER_FLOAT)
                .order(ByteOrder.nativeOrder())
                .asFloatBuffer();
        // 把坐标们加入FloatBuffer中
        vertexBuffer.put(triangleCoords);
        // 设置buffer，从第一个坐标开始读
        vertexBuffer.position(0);
        getProgram();

        uColorLocation = GLES20.glGetUniformLocation(program, U_COLOR);
        aPositionLocation = GLES20.glGetAttribLocation(program, A_POSITION);

        //---------第五步: 传入数据
        GLES20.glVertexAttribPointer(aPositionLocation, COORDS_PER_VERTEX,
                GLES20.GL_FLOAT, false, 0, vertexBuffer);
        GLES20.glEnableVertexAttribArray(aPositionLocation);
    }

    //获取program的id
    private void getProgram() {
        //获取顶点着色器文本
        String vertexShaderSource = TextResourceReader
                .readTextFileFromResource(context, R.raw.simple_vertex_shader);
        //获取片段着色器文本
        String fragmentShaderSource = TextResourceReader
                .readTextFileFromResource(context, R.raw.simple_fragment_shader);
        //获取program的id
        program = ShaderHelper.buildProgram(vertexShaderSource, fragmentShaderSource);
        GLES20.glUseProgram(program);
    }

    //----------第七步:绘制
    public void draw() {
        GLES20.glUniform4f(uColorLocation, 0.0f, 1.0f, 0.0f, 1.0f);
        //参数说明:顶点组合方式
        /**
         *
         * GL_TRIANGGLES ：将传入的顶点按照没3个一组组成一个三角形进行绘制

         GL_TRIANGLE_TRIP：将传入的顶点按照顺序三个一组组成三角形进行，前面三个顶点的后两个顶点做为下一个三角形的前两个顶点，
         比如 有v0 v1 v2 v3 四个顶点顺序排列，则v0 v1 v2组成一个三角形，v1,v2,v3组成一个三角形。

         GL_TRIANGLE_FAN：三角形扇的形式，将传入的顶点数据的第一个顶点做为中心点，其他点做为边缘点绘制一系列组成扇形的相邻三角形。
         */
        GLES20.glDrawArrays(GLES20.GL_TRIANGLES, 0, POSITION_COMPONENT_COUNT);
    }


}
