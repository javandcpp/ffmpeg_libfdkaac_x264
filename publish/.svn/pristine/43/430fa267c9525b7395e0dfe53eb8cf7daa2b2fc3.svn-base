
#ifndef __DEF_FLV_H__
#define __DEF_FLV_H__

#include <stdio.h>
#include <stdlib.h>

typedef __int64_t int64_t;

#ifdef __cplusplus
extern "C" {
#endif

	// $_FUNCTION_BEGIN *****************************************
	// 函数名称: FlvCreat
	// 函数说明: 创建上下文需要使用到的p_handle
	// $_FUNCTION_END *******************************************
	void * FlvCreate(unsigned uiFlowId);


	// $_FUNCTION_BEGIN *****************************************
	// 函数名称: FLVVideoInfo
	//	@para：p_handle:  FlvCreat()函数创建的。
	//	@para: width  && height && fps, 分别为视频的宽高和帧率
	//  @para:返回值: 返回零表示成功，返回负值表示输入参数有误
	// 函数说明: 有视频时调用该函数，否则不调用；且参数上不必考虑颜色空间
	// $_FUNCTION_END *******************************************
	int FLVVideoInfo(void* p_handle, int width, int height, int fps);


	// $_FUNCTION_BEGIN *****************************************
	// 函数名称: FLVAudioInfo
	//	@para：p_handle:  FlvCreat()函数创建的。
	//	@para: sampleRate && channel && bitRate && sampleWidth，分别为音频的采样率、声道数、码率和位宽。我们只支持sampleWidth=16的
	//  @para:返回值: 返回零表示成功，返回负值表示输入参数有误
	// 函数说明: 有音频时调用该函数，否则不调用
	//					sampleRate，一般是48000；channel，一般是2；bitRate单位是bits，AAC一般是24000
	// $_FUNCTION_END *******************************************
	int FLVAudioInfo(void* p_handle, int sampleRate, int channel, int bitRate, int ampleWidth);


	// $_FUNCTION_BEGIN *****************************************
	// 函数名称: FLVWriteHeader
	//	@para：outputBuf  : 输出信息内存，外部使用者进行malloc和 free. outputBuf暂时定为1KB大小
	//	@para: bufLen：   outputBuf  的长度。
	//	@para: inputBuf  && inputlen ： 输入视频帧的信息；
	//	@para: pts        ： 初始化时间戳
	//	@para: 返回值: 返回正值为输出信息中的有效字节数，返回负值，表示失败；
	// 函数说明:对flv头信息的打包，实时数据流传输前，需要flv头信息进行初始化下;
	//  该函数必须在输入为视频并且为I帧时，方可调用；
	//  z只有该函数调用成功后，方可调用视频、音频帧写入函数。
	// $_FUNCTION_END *******************************************
	int FLVWriteHeader(void* p_handle, unsigned char *outputBuf, unsigned int bufLen,unsigned char *inputBuf, unsigned int inputlen,int64_t pts,int64_t  ai64UserId);

// $_FUNCTION_BEGIN *****************************************
// 函数名称: FLVWriteVideoFrame
//  @para：uiIndex    ：/* 计数器 */
//	@para：outputBuf  ：输出的信息。  outputBuf大小应该比视频编码数据大1KB
//	@para:  bufLen：    outputBuf  的长度。
//	@para: pts : 时间戳信息。
//	@para: videoType: 输入视频帧的类型。0 ： I帧； 1： P帧。
//	@para: inputBuf && inputlen  : 输入视频帧的信息；inputBuf初始化大小为视频宽*高（字节数）即可
//	@para:返回值: 返回正值为输出信息中的有效字节数，返回负值，表示失败；
// 函数说明:对视频帧进行打包
// $_FUNCTION_END *******************************************

	//int FLVWriteVideoFrame(void* p_handle,unsigned uiIndex, char *outputBuf, unsigned int bufLen,int64_t pts,int videoType,unsigned char *inputBuf, unsigned int inputlen);
	int FLVWriteVideoFrame(void* p_handle,unsigned uiIndex, char *outputBuf, unsigned int bufLen,int64_t pts,int videoType,unsigned char *inputBuf, unsigned int inputlen,int64_t  ai64UserID);


	// $_FUNCTION_BEGIN *****************************************
	// 函数名称: FLVWriteAudioFrame
	//  @para：uiIndex    ：/* 计数器 */
	//	@para：outputBuf  ：输出的信息。 outputBuf大小应该比音频编码数据大1KB
	//	@para: bufLen：    outputBuf  的长度。
	//	@para: pts: 时间戳信息
	//	@para: inputBuf && inputlen  : 输入音频帧的信息；inputBuf初始化大小为1KB即可
	//	@para:返回值: 返回正值为输出信息中的有效字节数，返回负值，表示失败；
	// 函数说明:对音频帧进行打包
	// $_FUNCTION_END *******************************************

	int FLVWriteAudioFrame(void* p_handle,unsigned uiIndex, char *outputBuf,unsigned int bufLen,int64_t pts, unsigned char *inputBuf, unsigned int inputlen);


	// $_FUNCTION_BEGIN *****************************************
	// 函数名称: FLVClose
	//	@para:返回值: 返回负值，表示失败；
	// 函数说明:关闭handle
	// $_FUNCTION_END *******************************************
	int FLVClose (void* p_handle);
#ifdef __cplusplus
}
#endif

#endif  // __DEF_FLV_H__
