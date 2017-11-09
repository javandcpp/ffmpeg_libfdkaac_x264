package com.capture.avcapture;

import java.util.List;

import android.hardware.Camera.Size;
import android.view.SurfaceHolder;

public interface VideoCaptureInterface {

	// 摄像头参数信息
	class CameraDeviceInfo {
		public int m_nID;					// 摄像头 ID
		public int m_nType;					// 摄像头类别（用于区分前后摄像头）
		public int m_nRotation;				// 旋转角度（0/90/180/270）
		public List<Size> m_lsPreviewSize;	// 采集视频分辨率列表
		public int[] m_liFPSRange;			// 采集视频帧率范围（最小，最大）
		public int m_nMaxZoom;				// 最大缩放数值，0 表示不支持缩放
		public boolean m_bFlash;			// 是否支持闪光灯
	};
	
	// 摄像头选择（为了接口兼容，这里不使用枚举）
	class CameraDeviceType {
		public static final int CAMERA_FACING_BACK = 0;		// 后置摄像头
		public static final int CAMERA_FACING_FRONT = 1;	// 前置摄像头
	};
	
	// 打开视频采集设备结果
	enum OpenVideoDeviceReturn {
		OPEN_DEVICE_SUCCESS,		// 成功打开
		OPEN_ERROR_OCCUPIED,		// 已经打开
		OPEN_ERROR_TYPE,			// 不支持的摄像头
		OPEN_ERROR_SIZE,			// 不支持的尺寸
		OPEN_ERROR_BUFFER,			// 分配内存失败
		OPEN_ERROR_UNKNOWN			// 未知错误
	}
	
	// 开始采集结果
	enum StartVideoCaptureReturn {
		STATUS_SUCCESS,				// 开启采集成功
		STATUS_NO_DEVICE			// 采集设备无效
	}
	
	// 获取视频采集数据结果
	enum GetFrameDataReturn {
		RET_SUCCESS,				// 成功采集
		RET_CAPTURE_NO_START,		// 采集未开始
		RET_ERROR_PARAM,			// 参数错误
		RET_NO_VIDEO_DATA			// 无视频数据
	}
	
	// 设置对焦、缩放，开启/关闭闪光灯的状态返回
	enum StatusReturn {
		RET_SUCCESS,				// 成功
		RET_NO_CAMERA,				// 摄像头没打开
		RET_NOT_SUPPORTED		// 不支持该操作
	}
	
	/**
	 * 获取 avcapture.jar 版本号
	 * @return 返回日期，如：20151127
	 */
	public int GetAVCaptureVersion();
	
	/**
	 * 枚举所有的摄像头
	 * @return 摄像头列表信息，List 中的每个元素请参考 class CameraDeviceInfo
	 */
	public List<CameraDeviceInfo> EnumerateCameraDevice();
	
	/**
	 * 打开指定 ID 的视频采集设备，并设置采集参数
	 * @param aiID [in] int aiID，采集设备 ID
	 * @param aiWidth [in] int aiWidth，设置的采集视频宽度
	 * @param aiHeight [in] int aiHeight，设置的采集视频高度
	 * @param aiFPS [in] int aiFPS，设置采集视频的帧率
	 * @param aiBufferLen [out] int[] aiBufferLen，采集视频需要分配的缓存长度 aiBufferLen[0]
	 * @return 打开视频采集设备结果，参考 enum OpenVideoDeviceReturn
	 */
	public OpenVideoDeviceReturn OpenVideoDevice(int aiID, int aiWidth, int aiHeight, int aiFPS, int[] aiBufferLen);
	
	/**
	 * 确认是否打开设备
	 * @return 当前设备打开结果，boolean 类型
	 */
	public boolean IsOpenVideoDevice();
	
	/**
	 * 关闭视频采集设备
	 */
	public void CloseVideoDevice();
	
	/**
	 * 设置 SurfaceHolder 用于以下情景：
	 * 1.对于 Android 各个版本的手机，如需在采集数据的基础上还要求支持视频的预览显示，那么调用这个函数
	 * 2.对于 Android 2.3.3 的手机，仅采集视频数据不预览，那么请传递长宽均为 0 的 SurfaceHolder 以节省 CPU
	 * @param aoHolder [in] SurfaceHolder aoHolder，预览介质
	 */
	public void SetSurfaceHolder(SurfaceHolder aoHolder);
	
	/**
	 * 开始视频采集，如需预览则在此之前需要调用 SetSurfaceHolder
	 * @return 开始采集结果，参考 enum StartVideoCaptureReturn
	 */
	public StartVideoCaptureReturn StartVideoCapture();
	
	/**
	 * 视频采集是否正在进行
	 * @return 视频采集是否正在进行，boolean 类型
	 */
	public boolean IsStartVideoCapture();
	
	/**
	 * 停止视频采集
	 */
	public void StopVideoCapture();
	
	/**
	 * 获取采集数据
	 * @param abyBuffer [out] byte[] abyBuffer，获取采集数据的缓冲区
	 * @param aiLength [in] int aiLength，缓冲区长度
	 * @return 获取视频采集数据结果，参考 enum GetFrameDataReturn
	 */
	public GetFrameDataReturn GetFrameData(byte[] abyBuffer, int aiLength);
	
	/**
	 * 根据坐标进行对焦
	 * @param aix [in] int aiPointX，横坐标
	 * @param aiy [in] int aiPointY，纵坐标
	 * @return 对焦执行结果，参考 enum StatusReturn
	 */
	public StatusReturn FocusByCameraPoint(int aiPointX, int aiPointY);
	
	/**
	 * 设置缩放级别，级别为 0~m_nMaxZoom，级别越高缩放越大
	 * @param aiZoomLevel [in] int aiZoomLevel，缩放级别
	 * @return 缩放执行结果，参考 enum StatusReturn
	 */
	public StatusReturn SetZoomLevel(int aiZoomLevel);
	
	/**
	 * 开启/关闭闪光灯操作
	 * @param abStatus [in] boolean abStatus，true 表示开启，false 表示关闭
	 * @return 开启/关闭闪光灯结果，参考 enum StatusReturn
	 */
	public StatusReturn TurnFlash(boolean abStatus);
}
