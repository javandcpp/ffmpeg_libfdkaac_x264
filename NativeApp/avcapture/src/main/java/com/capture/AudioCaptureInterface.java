package com.capture;

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
	
	public OpenAudioDeviceReturn OpenAudioDevice(int aiSamplesPerSec, int aiChannels, int aiBitsPerSample, int[] aiBufferLen);

	public boolean IsOpenAudioDevice();
	
	public void CloseAudioDevice();
	
	public void StartAudioCapture();
	
	public boolean IsStartAudioCapture();
	
	public void StopAudioCapture();
	
	public GetAudioDataReturn GetAudioData(byte[] abyBuffer, int aiBufferLength, int[] aiDataLength);
}
