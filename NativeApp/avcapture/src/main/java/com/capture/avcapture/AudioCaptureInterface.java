package com.capture.avcapture;

public interface AudioCaptureInterface {

	// 打开音频采集设备结果
	enum OpenAudioDeviceReturn {
		OPEN_DEVICE_SUCCESS,		// 成功打开
		OPEN_ERROR_OCCUPIED,		// 已经打开
		OPEN_ERROR_PARAM,			// 不支持的参数
		OPEN_ERROR_BUFFER			// 内存分配失败
	}
	
	// 获取音频采集数据结果
	enum GetAudioDataReturn {
		RET_SUCCESS,				// 成功采集
		RET_CAPTURE_NO_START,		// 采集未开始
		RET_ERROR_PARAM,			// 参数错误
		RET_NO_AUDIO_DATA			// 无音频数据
	}
	
	/**
	 * 打开音频采集设备，并设置采集参数
	 * @param aiSamplesPerSec [in] int aiSamplesPerSec，采集音频频率，需要设置为 44100 
	 * @param aiChannels [in] int aiChannels，采集音频声道数，可选1或2
	 * @param aiBitsPerSample [in] int aiBitsPerSample，采集音频编码格式，需要设置为 16
	 * @param aiBufferLen [in/out] int[] aiBufferLen，采集音频需要分配的缓存长度，即 aiBufferLen[0]
	 * @return 打开音频采集设备结果，参考 enum OpenAudioDeviceReturn
	 */
	public OpenAudioDeviceReturn OpenAudioDevice(int aiSamplesPerSec, int aiChannels, int aiBitsPerSample, int[] aiBufferLen);
	
	/**
	 * 确认是否打开设备
	 * @return 当前设备打开结果，boolean 类型
	 */
	public boolean IsOpenAudioDevice();
	
	/**
	 * 关闭音频采集设备
	 */
	public void CloseAudioDevice();
	
	/**
	 * 开始音频采集
	 */
	public void StartAudioCapture();
	
	/**
	 * 音频采集是否正在进行
	 * @return 音频采集是否正在进行，boolean 类型
	 */
	public boolean IsStartAudioCapture();
	
	/**
	 * 停止音频采集
	 */
	public void StopAudioCapture();
	
	/**
	 * 获取采集数据
	 * @param abyBuffer [in/out] byte[] abyBuffer，获取采集数据分配的缓冲区
	 * @param aiBufferLength [in] int aiBufferLength，分配缓冲区的实际长度
	 * @param aiDataLength [in/out] int[] aiDataLength，实际采集到的音频数据长度 aiDataLength[0]
	 * @return 获取音频采集数据结果，参考 enum GetAudioDataReturn
	 */
	public GetAudioDataReturn GetAudioData(byte[] abyBuffer, int aiBufferLength, int[] aiDataLength);
}
