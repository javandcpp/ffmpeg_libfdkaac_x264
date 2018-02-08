
#ifndef __DEF_FLV_H__
#define __DEF_FLV_H__

#include <stdio.h>
#include <stdlib.h>

typedef __int64_t int64_t;

#ifdef __cplusplus
extern "C" {
#endif

	// $_FUNCTION_BEGIN *****************************************
	// ��������: FlvCreat
	// ����˵��: ������������Ҫʹ�õ���p_handle
	// $_FUNCTION_END *******************************************
	void * FlvCreate(unsigned uiFlowId);


	// $_FUNCTION_BEGIN *****************************************
	// ��������: FLVVideoInfo
	//	@para��p_handle:  FlvCreat()���������ġ�
	//	@para: width  && height && fps, �ֱ�Ϊ��Ƶ�Ŀ�ߺ�֡��
	//  @para:����ֵ: �������ʾ�ɹ������ظ�ֵ��ʾ�����������
	// ����˵��: ����Ƶʱ���øú��������򲻵��ã��Ҳ����ϲ��ؿ�����ɫ�ռ�
	// $_FUNCTION_END *******************************************
	int FLVVideoInfo(void* p_handle, int width, int height, int fps);


	// $_FUNCTION_BEGIN *****************************************
	// ��������: FLVAudioInfo
	//	@para��p_handle:  FlvCreat()���������ġ�
	//	@para: sampleRate && channel && bitRate && sampleWidth���ֱ�Ϊ��Ƶ�Ĳ����ʡ������������ʺ�λ������ֻ֧��sampleWidth=16��
	//  @para:����ֵ: �������ʾ�ɹ������ظ�ֵ��ʾ�����������
	// ����˵��: ����Ƶʱ���øú��������򲻵���
	//					sampleRate��һ����48000��channel��һ����2��bitRate��λ��bits��AACһ����24000
	// $_FUNCTION_END *******************************************
	int FLVAudioInfo(void* p_handle, int sampleRate, int channel, int bitRate, int ampleWidth);


	// $_FUNCTION_BEGIN *****************************************
	// ��������: FLVWriteHeader
	//	@para��outputBuf  : �����Ϣ�ڴ棬�ⲿʹ���߽���malloc�� free. outputBuf��ʱ��Ϊ1KB��С
	//	@para: bufLen��   outputBuf  �ĳ��ȡ�
	//	@para: inputBuf  && inputlen �� ������Ƶ֡����Ϣ��
	//	@para: pts        �� ��ʼ��ʱ���
	//	@para: ����ֵ: ������ֵΪ�����Ϣ�е���Ч�ֽ��������ظ�ֵ����ʾʧ�ܣ�
	// ����˵��:��flvͷ��Ϣ�Ĵ����ʵʱ����������ǰ����Ҫflvͷ��Ϣ���г�ʼ����;
	//  �ú�������������Ϊ��Ƶ����ΪI֡ʱ�����ɵ��ã�
	//  zֻ�иú������óɹ��󣬷��ɵ�����Ƶ����Ƶ֡д�뺯����
	// $_FUNCTION_END *******************************************
	int FLVWriteHeader(void* p_handle, unsigned char *outputBuf, unsigned int bufLen,unsigned char *inputBuf, unsigned int inputlen,int64_t pts,int64_t  ai64UserId);

// $_FUNCTION_BEGIN *****************************************
// ��������: FLVWriteVideoFrame
//  @para��uiIndex    ��/* ������ */
//	@para��outputBuf  ���������Ϣ��  outputBuf��СӦ�ñ���Ƶ�������ݴ�1KB
//	@para:  bufLen��    outputBuf  �ĳ��ȡ�
//	@para: pts : ʱ�����Ϣ��
//	@para: videoType: ������Ƶ֡�����͡�0 �� I֡�� 1�� P֡��
//	@para: inputBuf && inputlen  : ������Ƶ֡����Ϣ��inputBuf��ʼ����СΪ��Ƶ��*�ߣ��ֽ���������
//	@para:����ֵ: ������ֵΪ�����Ϣ�е���Ч�ֽ��������ظ�ֵ����ʾʧ�ܣ�
// ����˵��:����Ƶ֡���д��
// $_FUNCTION_END *******************************************

	//int FLVWriteVideoFrame(void* p_handle,unsigned uiIndex, char *outputBuf, unsigned int bufLen,int64_t pts,int videoType,unsigned char *inputBuf, unsigned int inputlen);
	int FLVWriteVideoFrame(void* p_handle,unsigned uiIndex, char *outputBuf, unsigned int bufLen,int64_t pts,int videoType,unsigned char *inputBuf, unsigned int inputlen,int64_t  ai64UserID);


	// $_FUNCTION_BEGIN *****************************************
	// ��������: FLVWriteAudioFrame
	//  @para��uiIndex    ��/* ������ */
	//	@para��outputBuf  ���������Ϣ�� outputBuf��СӦ�ñ���Ƶ�������ݴ�1KB
	//	@para: bufLen��    outputBuf  �ĳ��ȡ�
	//	@para: pts: ʱ�����Ϣ
	//	@para: inputBuf && inputlen  : ������Ƶ֡����Ϣ��inputBuf��ʼ����СΪ1KB����
	//	@para:����ֵ: ������ֵΪ�����Ϣ�е���Ч�ֽ��������ظ�ֵ����ʾʧ�ܣ�
	// ����˵��:����Ƶ֡���д��
	// $_FUNCTION_END *******************************************

	int FLVWriteAudioFrame(void* p_handle,unsigned uiIndex, char *outputBuf,unsigned int bufLen,int64_t pts, unsigned char *inputBuf, unsigned int inputlen);


	// $_FUNCTION_BEGIN *****************************************
	// ��������: FLVClose
	//	@para:����ֵ: ���ظ�ֵ����ʾʧ�ܣ�
	// ����˵��:�ر�handle
	// $_FUNCTION_END *******************************************
	int FLVClose (void* p_handle);
#ifdef __cplusplus
}
#endif

#endif  // __DEF_FLV_H__
