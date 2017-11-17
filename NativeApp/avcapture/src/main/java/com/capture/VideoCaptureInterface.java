package com.capture;

import android.hardware.Camera.Size;
import android.view.SurfaceHolder;

import java.util.List;

public interface VideoCaptureInterface {

    class CameraDeviceInfo {
        public int m_nID;                    // camera ID
        public int m_nType;                    // camera Type
        public int m_nRotation;
        public List<Size> m_lsPreviewSize;
        public int[] m_liFPSRange;
        public int m_nMaxZoom;
        public boolean m_bFlash;
    }

    ;

    class CameraDeviceType {
        public static final int CAMERA_FACING_BACK = 0;        // 后置摄像头
        public static final int CAMERA_FACING_FRONT = 1;    // 前置摄像头
    }


    enum OpenVideoDeviceReturn {
        OPEN_DEVICE_SUCCESS,        // 成功打开
        OPEN_ERROR_OCCUPIED,        // 已经打开
        OPEN_ERROR_TYPE,            // 不支持的摄像头
        OPEN_ERROR_SIZE,            // 不支持的尺寸
        OPEN_ERROR_BUFFER,            // 分配内存失败
        OPEN_ERROR_UNKNOWN            // 未知错误
    }

    enum StartVideoCaptureReturn {
        STATUS_SUCCESS,                // 开启采集成功
        STATUS_NO_DEVICE            // 采集设备无效
    }

    enum GetFrameDataReturn {
        RET_SUCCESS,                // 成功采集
        RET_CAPTURE_NO_START,        // 采集未开始
        RET_ERROR_PARAM,            // 参数错误
        RET_NO_VIDEO_DATA            // 无视频数据
    }

    enum StatusReturn {
        RET_SUCCESS,                // 成功
        RET_NO_CAMERA,                // 摄像头没打开
        RET_NOT_SUPPORTED        // 不支持该操作
    }

    public int GetAVCaptureVersion();

    public List<CameraDeviceInfo> EnumerateCameraDevice();

    public OpenVideoDeviceReturn OpenVideoDevice(int aiID, int aiWidth, int aiHeight, int aiFPS, int[] aiBufferLen);

    public boolean IsOpenVideoDevice();

    public void CloseVideoDevice();

    public void SetSurfaceHolder(SurfaceHolder aoHolder);

    public StartVideoCaptureReturn StartVideoCapture();

    public boolean IsStartVideoCapture();

    public void StopVideoCapture();

    public GetFrameDataReturn GetFrameData(byte[] abyBuffer, int aiLength);

    public StatusReturn FocusByCameraPoint(int aiPointX, int aiPointY);

    public StatusReturn SetZoomLevel(int aiZoomLevel);

    public StatusReturn TurnFlash(boolean abStatus);
}
