/*****************************************************************************
 * flv.c: flv muxer
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include  <arpa/inet.h>
//#include <Mmsystem.h>
#include "flv.h"
#include "BaseThread.h"
#include "DebugTrace.h"

int64_t  g_i64Time = 0;

//定义是否是测试flv流（腾讯视频），如未定义此宏是保存flv文件
//#define DEF_TEST_FLV_STREAM	

#ifdef DEF_TEST_FLV_STREAM
	//#include <WinSock2.h>
#endif

typedef signed char int8_t;
typedef unsigned char   uint8_t;
typedef short  int16_t;
typedef unsigned short  uint16_t;
typedef int  int32_t;
typedef unsigned   uint32_t;
typedef long unsigned int  uint64_t;
#pragma pack(1)
typedef struct FLVStream {
	int version;
	int length;
	int pos;
	unsigned char *data;
}FLVStream;

typedef struct
{
	uint32_t  uiFlowId;            /* 直播流ID*/
	int flag;  //表明 音视频存在情况
	// video
	int hasVideo;
	int  width;
	int  height;
	int  fps;
	// audio
	int hasAudio;
	int  sampleRate;
	int  channel;
	int  bitRate;
	int  sampleWidth;
// 
	FLVStream flv;
	int     videoFirst;
	int64_t initTime;
//  
}STRU_FLV_CONTEXT;

#pragma pack()

/* offsets for packed values */
#define FLV_AUDIO_SAMPLESSIZE_OFFSET 1
#define FLV_AUDIO_SAMPLERATE_OFFSET  2
#define FLV_AUDIO_CODECID_OFFSET     4

#define FLV_VIDEO_FRAMETYPE_OFFSET   4
/* bitmasks to isolate specific values */
#define FLV_AUDIO_CHANNEL_MASK    0x01
#define FLV_AUDIO_SAMPLESIZE_MASK 0x02
#define FLV_AUDIO_SAMPLERATE_MASK 0x0c
#define FLV_AUDIO_CODECID_MASK    0xf0

#define FLV_VIDEO_CODECID_MASK    0x0f
#define FLV_VIDEO_FRAMETYPE_MASK  0xf0

#define AMF_END_OF_OBJECT         0x09

#define KEYFRAMES_TAG            "keyframes"
#define KEYFRAMES_TIMESTAMP_TAG  "times"
#define KEYFRAMES_BYTEOFFSET_TAG "filepositions"


enum {
	FLV_HEADER_FLAG_HASVIDEO = 1,
	FLV_HEADER_FLAG_HASAUDIO = 4,
};
typedef enum {
	AMF_DATA_TYPE_NUMBER      = 0x00,
	AMF_DATA_TYPE_BOOL        = 0x01,
	AMF_DATA_TYPE_STRING      = 0x02,
	AMF_DATA_TYPE_OBJECT      = 0x03,
	AMF_DATA_TYPE_NULL        = 0x05,
	AMF_DATA_TYPE_UNDEFINED   = 0x06,
	AMF_DATA_TYPE_REFERENCE   = 0x07,
	AMF_DATA_TYPE_MIXEDARRAY  = 0x08,
	AMF_DATA_TYPE_OBJECT_END  = 0x09,
	AMF_DATA_TYPE_ARRAY       = 0x0a,
	AMF_DATA_TYPE_DATE        = 0x0b,
	AMF_DATA_TYPE_LONG_STRING = 0x0c,
	AMF_DATA_TYPE_UNSUPPORTED = 0x0d,
} AMFDataType;

enum {
	FLV_CODECID_H263    = 2,
	FLV_CODECID_SCREEN  = 3,
	FLV_CODECID_VP6     = 4,
	FLV_CODECID_VP6A    = 5,
	FLV_CODECID_SCREEN2 = 6,
	FLV_CODECID_H264    = 7,
	FLV_CODECID_REALH263= 8,
	FLV_CODECID_MPEG4   = 9,
};

enum {
	NOT_I_FRAME =0,
	I_FRAME =1,
};


enum {
	FLV_CODECID_PCM                  = 0,
	FLV_CODECID_ADPCM                = 1 << FLV_AUDIO_CODECID_OFFSET,
	FLV_CODECID_MP3                  = 2 << FLV_AUDIO_CODECID_OFFSET,
	FLV_CODECID_PCM_LE               = 3 << FLV_AUDIO_CODECID_OFFSET,
	FLV_CODECID_NELLYMOSER_16KHZ_MONO = 4 << FLV_AUDIO_CODECID_OFFSET,
	FLV_CODECID_NELLYMOSER_8KHZ_MONO = 5 << FLV_AUDIO_CODECID_OFFSET,
	FLV_CODECID_NELLYMOSER           = 6 << FLV_AUDIO_CODECID_OFFSET,
	FLV_CODECID_PCM_ALAW             = 7 << FLV_AUDIO_CODECID_OFFSET,
	FLV_CODECID_PCM_MULAW            = 8 << FLV_AUDIO_CODECID_OFFSET,
	FLV_CODECID_AAC                  = 10<< FLV_AUDIO_CODECID_OFFSET,
	FLV_CODECID_SPEEX                = 11<< FLV_AUDIO_CODECID_OFFSET,
};

enum {
	FLV_TAG_TYPE_AUDIO = 0x08,
	FLV_TAG_TYPE_VIDEO = 0x09,
	FLV_TAG_TYPE_META  = 0x12,
};


enum {
	FLV_MONO   = 0,
	FLV_STEREO = 1,
};

enum {
	FLV_SAMPLESSIZE_8BIT  = 0,
	FLV_SAMPLESSIZE_16BIT = 1 << FLV_AUDIO_SAMPLESSIZE_OFFSET,
};

enum {
	FLV_SAMPLERATE_SPECIAL = 0, /**< signifies 5512Hz and 8000Hz in the case of NELLYMOSER */
	FLV_SAMPLERATE_11025HZ = 1 << FLV_AUDIO_SAMPLERATE_OFFSET,
	FLV_SAMPLERATE_22050HZ = 2 << FLV_AUDIO_SAMPLERATE_OFFSET,
	FLV_SAMPLERATE_44100HZ = 3 << FLV_AUDIO_SAMPLERATE_OFFSET,
};

//uint8_t  uiType;          		/* 1：flv头 2：flv数据 */
enum {
	FLV_STREAM_HEAD = 1,	
	FLV_STREAM_DATA = 2,
};

void * FlvCreate(unsigned uiFlowId)
{
	void * p_handle=NULL;
	STRU_FLV_CONTEXT *p;

	p_handle =malloc(sizeof(STRU_FLV_CONTEXT));
	memset(p_handle,0,sizeof(STRU_FLV_CONTEXT));

	p = (STRU_FLV_CONTEXT *)p_handle;
	p->uiFlowId = uiFlowId;
	p->videoFirst=0;
	return  p_handle;
}

int FLVVideoInfo(void* p_handle, int width, int height, int fps)
{
	int liHeight = 0;
	STRU_FLV_CONTEXT *p;
	p= (STRU_FLV_CONTEXT *)p_handle;

	if(p_handle==NULL)
		return -1;
	if(!(width && height && fps))
		return -2;

	p->flag  += FLV_HEADER_FLAG_HASVIDEO;
	p->hasVideo =1;
	p->width     = width;
	p->height    = height;

	liHeight = p->height % 16;
	if (0 != liHeight)
	{
		p->height += 16 - liHeight;
	}
	p->fps       =fps;
	return 0;
}


int FLVAudioInfo(void* p_handle, int sampleRate, int channel, int bitRate, int ampleWidth)
{
	STRU_FLV_CONTEXT *p;
	p= (STRU_FLV_CONTEXT *)p_handle;

	if(p_handle==NULL)
		return -1;
	if(!(sampleRate && channel && bitRate && ampleWidth))
		return -2;
	if((1!=channel) && (2!=channel))
		return -3;
	if(16!=ampleWidth)
		return -4;

	p->flag    += FLV_HEADER_FLAG_HASAUDIO;
	p->hasAudio =1;
	p->sampleRate  = sampleRate;
	p->channel     = channel;
	p->sampleWidth = ampleWidth;
	p->bitRate     = bitRate;
	return 0;
}


#define FLVGROWBY 4096
 void grow(FLVStream *flv) {
	flv->length += FLVGROWBY;
	flv->data =(unsigned char*) realloc(flv->data, flv->length);
}

static  void __putChar(unsigned char *data, uint32_t val) {
	*data = (val & 0x000000ff);
}

void writeData(FLVStream *flv, unsigned char *data, int size) {
	while(flv->pos + size >= flv->length)
		grow(flv);
	memcpy(flv->data + flv->pos, data, size);
	flv->pos += size;
}

static  void putChar(FLVStream *flv, uint32_t val) {
	if( flv->pos + 1 >= flv->length ) 
		grow(flv);

	__putChar(flv->data + flv->pos++, val);
}

static void putBool(FLVStream *flv, int b)
{
	putChar(flv, AMF_DATA_TYPE_BOOL);
	putChar(flv, !!b);
}

static  void putUI16(FLVStream *flv, uint32_t val) {
	putChar(flv, (val >> 8));
	putChar(flv, val);
}

static  void putUI24(FLVStream *flv, uint32_t val) {
	putChar(flv, (val >> 16));
	putUI16(flv, val);
}


static  void putUI32(FLVStream *flv, uint32_t val) {
	putChar(flv, (val >> 24));
	putUI24(flv, val);
}

static  void putString(FLVStream *flv, const char *str) {
	int len = strlen(str);
	putUI16(flv,len);
	memcpy(&flv->data[flv->pos],str,len);
	flv->pos +=len;	
}

union av_intfloat64 {
	uint64_t i;
	double   f;
};

static  uint64_t av_double2int(double f)
{
	union av_intfloat64 v;
	v.f = f;
	return v.i;
}
static void putDouble(FLVStream *flv,double  d) 
{
	uint64_t val=0;
	putChar( flv, AMF_DATA_TYPE_NUMBER);
	val= av_double2int(d);
	putUI32(flv, (uint32_t)( val>>32) );
	putUI32(flv, (uint32_t)(val & 0xffffffff));
}

static unsigned char* FindNoSPS_PPSNal(unsigned char *buf,int len,int headLen)
{

	int jj=0;
	unsigned char *headAddr = NULL;
	for (jj=0; jj<len-4; jj++)
	{
		if (headLen==4)
		{
			if (buf[jj]==0x0 && 
				buf[jj+1]==0x0 && 
				buf[jj+2]==0x0 && 
				buf[jj+3]==0x1)
			{

				if((buf[jj+4] & 0x1F) != 7 && (buf[jj+4] & 0x1F) != 8)
				{
					headAddr = &buf[jj];
					break;
				}

			}
		}
		else
		{
			if (buf[jj]==0x0 && 
				buf[jj+1]==0x0 && 
				buf[jj+2]==0x1)
			{
				if((buf[jj+3] & 0x1F) !=7 && (buf[jj+3] & 0x1F) != 8)
				{
					headAddr = &buf[jj];
					break;
				};

			}
		}
	

	

	}
	return headAddr;
}
static int FindNextNAL(unsigned char *buf,int len)
{
	int jj=0;
	bool bFind = false;
	for (jj=0; jj<len-4; jj++)
	{
		if (buf[jj]==0x0 && 
                buf[jj+1]==0x0 && 
                buf[jj+2]==0x0 && 
                buf[jj+3]==0x1)
		{
			bFind = true;
			break;
		}

		if (buf[jj]==0x0 && 
                buf[jj+1]==0x0 && 
                buf[jj+2]==0x1)
		{
			bFind = true;
			break;
		}
	}
	if (bFind)
	{
		return jj;
	}
	else
	{
		return len;
	}
}

/* NAL unit types */
enum {
	enum_NAL_SLICE = 1,
	enum_NAL_DPA,
	enum_NAL_DPB,
	enum_NAL_DPC,
	enum_NAL_IDR_SLICE,
	enum_NAL_SEI,
	enum_NAL_SPS,
	enum_NAL_PPS,
	enum_NAL_AUD,
	enum_NAL_END_SEQUENCE,
	enum_NAL_END_STREAM,
	enum_NAL_FILLER_DATA,
	enum_NAL_SPS_EXT,
	enum_NAL_AUXILIARY_SLICE = 19
};
enum {
	FLV_FRAME_KEY            = 1 << FLV_VIDEO_FRAMETYPE_OFFSET, ///< key frame (for AVC, a seekable frame)
	FLV_FRAME_INTER          = 2 << FLV_VIDEO_FRAMETYPE_OFFSET, ///< inter frame (for AVC, a non-seekable frame)
	FLV_FRAME_DISP_INTER     = 3 << FLV_VIDEO_FRAMETYPE_OFFSET, ///< disposable inter frame (H.263 only)
	FLV_FRAME_GENERATED_KEY  = 4 << FLV_VIDEO_FRAMETYPE_OFFSET, ///< generated key frame (reserved for server use only)
	FLV_FRAME_VIDEO_INFO_CMD = 5 << FLV_VIDEO_FRAMETYPE_OFFSET, ///< video info/command frame
};



int FLVWriteHeader(void* p_handle, unsigned char *outputBuf, 
        unsigned int bufLen,unsigned char *inputBuf, 
        unsigned int inputlen,int64_t pts,int64_t ai64UserId)
{
	int64_t metadata_size_pos, data_size, metadata_count_pos;
    int64_t data_size_pos = 0;
	int64_t lastPos;
	int len=0;
	int metadata_count;
	STRU_FLV_CONTEXT *p;
	int liSizePos;		//记录数据大小写入的位置
	int getPPS=-1;      // 防止获得头信息时，传入的不是I帧；

	p= (STRU_FLV_CONTEXT *)p_handle;
	if(p_handle==NULL)
		return -1;

	p->flv.pos = 0;

	p->flv.data= outputBuf;
	p->flv.length = bufLen;

#ifdef DEF_TEST_FLV_STREAM
	putUI32(&p->flv, htonl(p->uiFlowId));		/* 直播流ID*/
	putChar(&p->flv, FLV_STREAM_HEAD);	/* 1：flv头 2：flv数据 */
	putUI32(&p->flv, htonl(1));				/* 计数器 */
	liSizePos = p->flv.pos;				// 记录数据大小写入的位置
	putUI32(&p->flv, 0);				/* 数据部分大小 */
#endif
	
	putChar(&p->flv,'F');
	putChar(&p->flv, 'L');
	putChar(&p->flv, 'V');

	putChar(&p->flv, 1);
	putChar(&p->flv, p->flag);
	putUI32(&p->flv, 9);
	putUI32(&p->flv, 0);

	/* write meta_tag */
	putChar(&p->flv, 18); // tag type META
	metadata_size_pos =p->flv.pos;
	putUI24(&p->flv, 0);         // 需要后面更改！！！！！！！
	putUI24(&p->flv, 0);           
	putUI32(&p->flv, 0);           

	/* now data of data_size size */

	/* first event name as a string */
	putChar(&p->flv, AMF_DATA_TYPE_STRING);
	putString(&p->flv, "onMetaData"); // 12 bytes
	putChar(&p->flv, AMF_DATA_TYPE_MIXEDARRAY);

	metadata_count_pos = p->flv.pos;
	metadata_count = 4 +5 +	2;   //    需要注意！！！！！

	putUI32(&p->flv, 8);
	putString(&p->flv, "duration");
	// fill in the guessed duration, it'll be corrected later if incorrect
	putDouble(&p->flv,  0  );   // 需要注意！！！ 


	if(p->hasAudio)
	{
		putString(&p->flv, "audiodatarate");
		putDouble(&p->flv, p->bitRate / 1024.0);

		putString(&p->flv, "audiosamplerate");
		putDouble(&p->flv, p->sampleRate);

		putString(&p->flv, "audiosamplesize");
		putDouble(&p->flv, p->sampleWidth);

		putString(&p->flv, "stereo");
		putBool(&p->flv, p->channel == 2);

		putString(&p->flv, "audiocodecid");
		putDouble(&p->flv, (FLV_CODECID_AAC>>FLV_AUDIO_CODECID_OFFSET));

	}

	if(p->hasVideo)
	{
		putString(&p->flv, "width"); 
		putDouble(&p->flv, p->width);
		putString(&p->flv, "height");
		putDouble(&p->flv, p->height);

		putString(&p->flv, "videodatarate");
		putDouble(&p->flv, 0 / 1024.0);

		putString(&p->flv, "framerate");
		putDouble(&p->flv, p->fps);
		metadata_count++;

		putString(&p->flv, "videocodecid");
		putDouble(&p->flv, FLV_CODECID_H264);

	}

	putString(&p->flv, "filesize");
	//flv->filesize_offset = avio_tell(pb);
	putDouble(&p->flv, 0); // delayed write  ///!!!!


	putString(&p->flv, "");
	putChar(&p->flv, AMF_END_OF_OBJECT);

	/* write total size of tag */
	lastPos =p->flv.pos;
	data_size = lastPos - metadata_size_pos - 10 ;

	metadata_count = 12;          //    需要注意！！！！！
	//avio_seek(pb, metadata_count_pos, SEEK_SET);
	p->flv.pos = metadata_count_pos;
	putUI32(&p->flv, metadata_count);
	p->flv.pos = metadata_size_pos;
	putUI24(&p->flv, data_size);
	//avio_skip(pb, data_size + 10 - 3);
	p->flv.pos = lastPos;
	putUI32(&p->flv, data_size + 11);

	if(p->hasAudio)
	{
		int data_size_pos=0;
		int data_size;
		int lastpos=0;

		//int lnBytesUsed = inputlen;
		//putChar(&p->flv, FLV_TAG_TYPE_AUDIO );
		data_size_pos = p->flv.pos;
        //putUI24(&p->flv, 0); // size patched later
        //putUI24(&p->flv, 0); // ts
        //putChar(&p->flv, 0);   // ts ext
        //putUI24(&p->flv, 0); // streamid

		putChar(&p->flv, 0xAF);
		putChar(&p->flv, 0); // AAC sequence header

		if(p->sampleRate == 48000)
		{
			unsigned char esbuf[2]={0x13,0x08};
			memcpy(&p->flv.data[p->flv.pos],esbuf,2);
			p->flv.pos +=2;
		}
		else
		{
			unsigned char esbuf[7]={0x13,0x8,0x56,0xe5,0xa5,0x48,0x80};
			memcpy(&p->flv.data[p->flv.pos],esbuf,7);
			p->flv.pos +=7;

		}
		
		lastpos =p->flv.pos;
		data_size = lastpos - data_size_pos;
		//avio_seek(pb, -data_size - 10, SEEK_CUR);
		//avio_wb24(pb, data_size);
		//p->flv.pos = data_size_pos;
		//putUI24(&p->flv, data_size-10);
		//p->flv.pos = lastpos;
		//putUI32(&p->flv, data_size + 1); // previous tag size
	}


	if(p->hasVideo)
	{
		int i;
		int data_size_pos=0;
		int data_size;
		int lastpos=0;

		int nal_unit_type ;
		int nal_len=0;
		int lnBytesUsed =inputlen;

		p->videoFirst=0;

        //int lnBytesUsed = inputlen;
		//putChar(&p->flv, FLV_TAG_TYPE_VIDEO );
		//data_size_pos = p->flv.pos;
		//putUI24(&p->flv, 0); // size patched later
		//putUI24(&p->flv, 0); // ts
		//putChar(&p->flv, 0);   // ts ext
		//putUI24(&p->flv, 0); // streamid

		putChar(&p->flv, 0x17); // flags

		putChar(&p->flv,  0); // AVC sequence header
		putUI24(&p->flv,  0); // composition time

		for(i=0;i< lnBytesUsed;i++)
		{
            int len = 0;
            if( (inputBuf[i]==0x0 && inputBuf[i+1]==0x0 && 
                 inputBuf[i+2]==0x1 ))
            {
				nal_unit_type = inputBuf[i+3] & 0x1F ;
                len = 3;
            }
            
			if( inputBuf[i]==0x0 && inputBuf[i+1]==0x0 && 
                     inputBuf[i+2]==0x0 && inputBuf[i+3]==0x1)
            {
				nal_unit_type = inputBuf[i+4] & 0x1F ;
                len = 4;
            }

            if (len == 3 || len == 4)
            {
                switch(nal_unit_type)
                {
                    case enum_NAL_SPS:
                        nal_len=FindNextNAL(&inputBuf[i+len],lnBytesUsed-(i+len));

                        //if (nal_len == 0)
                        //{
                            //int a = 10;
                        //}

                        //TraceLog0("SPS ret len:%d\n",nal_len);

                        putChar(&p->flv, 1); /* version */
                        putChar(&p->flv, inputBuf[i+len+1]); /* profile */
                        putChar(&p->flv, inputBuf[i+len+2]); /* profile compat */
                        putChar(&p->flv, inputBuf[i+len+3]); /* level */
                        putChar(&p->flv, 0xff); /* 6 bits reserved (111111) + 2 bits nal size length - 1 (11) */
                        putChar(&p->flv, 0xe1); /* 3 bits reserved (111) + 5 bits number of sps (00001) */

                        putUI16(&p->flv, nal_len);
                        memcpy(&p->flv.data[p->flv.pos],
                                &inputBuf[i+len],nal_len);
                        p->flv.pos +=nal_len;

                        i=i+nal_len+(len-1);
                        break;
                    case enum_NAL_PPS:
                        nal_len=FindNextNAL(&inputBuf[i+len],lnBytesUsed-(i+len));
						if (nal_len==0)
						{
							nal_len = lnBytesUsed-(i+len);
						}
                        //TraceLog0("pps ret len:%d\n",nal_len);
                        putChar(&p->flv, 1); /* number of pps */
                        putUI16(&p->flv, nal_len);
                        memcpy(&p->flv.data[p->flv.pos],
                                &inputBuf[i+len],nal_len);
                        p->flv.pos +=nal_len;
                        i=lnBytesUsed;  // 快速跳出for循环
                        getPPS=0;
                        break;
                }
            }
		}	
		lastpos =p->flv.pos;
		data_size = lastpos - data_size_pos;
		//avio_seek(pb, -data_size - 10, SEEK_CUR);
		//avio_wb24(pb, data_size);
		//p->flv.pos = data_size_pos;
		//putUI24(&p->flv, data_size-10);
		//p->flv.pos = lastpos;
		//putUI32(&p->flv, data_size + 1); // previous tag size

		if(getPPS<0)
			return -3;
		p->videoFirst =1;
	}

	len = p->flv.pos;
#ifdef DEF_TEST_FLV_STREAM
	//重新更新数据大小部分；13个字节是 _zb_old_flv_protocol_t 结构体的大小，需要减除
	p->flv.pos = liSizePos;
	putUI32(&p->flv, htonl(len-13));				/* 数据部分大小 */ 

	
	if (0 == p->initTime)
	{
		//if (ai64UserId == 363928)
		//{
            g_i64Time = CBaseThread::GetSystemTime();
            char tempFileName[100];
            sprintf(tempFileName,"%I64d.flv",g_i64Time);
            FILE *fp=fopen(tempFileName,"ab+");
            fwrite(&outputBuf[13],1,len-13,fp);
            fflush(fp);
            fclose(fp);
		//}
	}
#endif
#if 0
	p->initTime=timeGetTime();  
#else
	p->initTime=pts;
#endif

	return len;
}



int FLVWriteVideoFrame(void* p_handle,unsigned uiIndex, 
        char *outputBuf, 
        unsigned int bufLen,int64_t pts,
        int videoType,unsigned char *inputBuf, 
        unsigned int inputlen,
	int64_t  ai64UserId)
{
	STRU_FLV_CONTEXT *p;
	unsigned ts =pts;
	int i;
	int nal_unit_type ;
	int nal_len=0;
	int flags = -1, flags_size;
	int lnBytesUsed = inputlen;
	int liSizePos;		//记录数据大小写入的位置

	p= (STRU_FLV_CONTEXT *)p_handle;
	if(p_handle==NULL)
		return -1;

	//ts = pts-p->initTime;   // 获取时间差值
	ts = pts;   // 获取时间差值

	p->flv.data=(unsigned char*) outputBuf;
	p->flv.length = bufLen;
	p->flv.pos = 0;

	if(p->videoFirst)
	{
		int PayloadSize = inputlen;
		int len = 0;
		if(inputBuf[0]==0x0 && inputBuf[1]==0x0 && 
			inputBuf[2]==0x0 && inputBuf[3]==0x1)
		{
			len = 4;
			nal_unit_type = inputBuf[4] & 0x1F ;
		}

		if(inputBuf[0]==0x0 && inputBuf[1]==0x0 && 
			inputBuf[2]==0x1 )
		{
			len = 3;
			nal_unit_type = inputBuf[3] & 0x1F ;
		}
		if(nal_unit_type==enum_NAL_SPS)
		{
			unsigned char* FrameHead = NULL;
			FrameHead = FindNoSPS_PPSNal(inputBuf,inputlen,len);
			PayloadSize = inputlen + inputBuf - FrameHead;
			inputBuf = FrameHead;
		}
		int flag=-1;
		flags_size = 5;
		
#ifdef DEF_TEST_FLV_STREAM
		putUI32(&p->flv, htonl(p->uiFlowId));		/* 直播流ID*/
		putChar(&p->flv, FLV_STREAM_DATA);	/* 1：flv头 2：flv数据 */
		putUI32(&p->flv, htonl(uiIndex));			/* 计数器 */
		liSizePos = p->flv.pos;				// 记录数据大小写入的位置
		putUI32(&p->flv, 0);				/* 数据部分大小 */
#endif

		putChar(&p->flv, FLV_TAG_TYPE_VIDEO);
		//flags = enc->codec_tag; //   xuyaozhuyi
		putUI24(&p->flv, PayloadSize + flags_size);
		putUI24(&p->flv, ts & 0xFFFFFF);
		putChar(&p->flv, (ts >> 24) & 0x7F); // timestamps are 32 bits _signed_
		putUI24(&p->flv, 0);

		flags = videoType==I_FRAME ? FLV_FRAME_KEY+7 : FLV_FRAME_INTER+7;
		putChar(&p->flv,flags);
		putChar(&p->flv, 1); // AVC NALU
		putUI24(&p->flv, 0);

//		lnBytesUsed = inputlen;
		lnBytesUsed = PayloadSize;
		for(i=0;i< lnBytesUsed;i++)
		{
            int len = 0;
			if(inputBuf[i]==0x0 && inputBuf[i+1]==0x0 && 
                    inputBuf[i+2]==0x0 && inputBuf[i+3]==0x1)
            {
                len = 4;
				nal_unit_type = inputBuf[i+4] & 0x1F ;
            }

			if(inputBuf[i]==0x0 && inputBuf[i+1]==0x0 && 
                    inputBuf[i+2]==0x1 )
            {
                len = 3;
				nal_unit_type = inputBuf[i+3] & 0x1F ;
            }

            if (len == 3 || len == 4)
            {
                nal_len=FindNextNAL(&inputBuf[i+len],lnBytesUsed-(i+len));
				if (nal_len==0)
				{
					nal_len = lnBytesUsed-(i+len);
				}
                putUI32(&p->flv, nal_len);
                memcpy(&p->flv.data[p->flv.pos],&inputBuf[i+len],nal_len);
                p->flv.pos +=nal_len;
                i=i+nal_len + (len -1);
            }
                //switch(nal_unit_type)
                //{
                    //case enum_NAL_SPS:
                        //nal_len=FindNextNAL(&inputBuf[i+len],lnBytesUsed-(i+len-1));
                        //putUI32(&p->flv, nal_len);
                        //memcpy(&p->flv.data[p->flv.pos],&inputBuf[i+len],nal_len);
                        //p->flv.pos +=nal_len;
                        //i=i+nal_len+len;
                        //break;

                    //case enum_NAL_PPS:
                        //nal_len=FindNextNAL(&inputBuf[i+len],lnBytesUsed-(i+len-1));
                        //putUI32(&p->flv, nal_len);
                        //memcpy(&p->flv.data[p->flv.pos],&inputBuf[i+len],nal_len);
                        //p->flv.pos +=nal_len;
                        //i=i+nal_len+len;
                        //break;
                    //case enum_NAL_SLICE:
                    //case enum_NAL_IDR_SLICE:
                        //nal_len=FindNextNAL(&inputBuf[i+len],lnBytesUsed-(i+len-1));
                        //putUI32(&p->flv, nal_len-(len-1));
                        //memcpy(&p->flv.data[p->flv.pos],&inputBuf[i+len],nal_len-(len-1));
                        //p->flv.pos +=nal_len-(len-1);
                        //i=i+nal_len;

                        //break;
                    //default:
                        //nal_len=FindNextNAL(&inputBuf[i+len],lnBytesUsed-i);
                        //i=i+nal_len;
                        //break;
                //}
            //}
		}
		putUI32(&p->flv, PayloadSize + flags_size + 11); // previous tag size

		lnBytesUsed = p->flv.pos;
#ifdef DEF_TEST_FLV_STREAM
		//重新更新数据大小部分；13个字节是 _zb_old_flv_protocol_t 结构体的大小，需要减除
		p->flv.pos = liSizePos;
		putUI32(&p->flv, htonl(lnBytesUsed-13));				/* 数据部分大小 */ 

		if (ai64UserId == 363928)
		{
			//char tempFileName[100];
			//sprintf(tempFileName,"%I64d.flv",g_i64Time);
			//FILE *fp=fopen(tempFileName,"ab+");
			//fwrite(&outputBuf[13],1,lnBytesUsed-13,fp);
			//fflush(fp);
			//fclose(fp);

		}
#endif
		return lnBytesUsed;
	}
	else
	    return 0;
}

int FLVWriteAudioFrame(void* p_handle,unsigned uiIndex,
        char *outputBuf,unsigned int bufLen,int64_t pts,
        unsigned char *inputBuf, unsigned int inputlen)
{
	STRU_FLV_CONTEXT *p;
	unsigned ts =pts;
	int  flags_size;
	int lnBytesUsed = inputlen;
	int liSizePos;		//记录数据大小写入的位置

	p= (STRU_FLV_CONTEXT *)p_handle;
	if(p_handle==NULL)
		return -1;
	//ts = pts-p->initTime;
	ts = pts;

	//如果设置有视频，但视频未到的情况下，语音也不进行处理，直接丢弃
	if(p->hasVideo ==0 || (p->hasVideo==1 && p->videoFirst==1))
	{
		p->flv.data= (unsigned char*)outputBuf;
		p->flv.length = bufLen;
		p->flv.pos = 0;
		flags_size = 2;

#ifdef DEF_TEST_FLV_STREAM
		putUI32(&p->flv, htonl(p->uiFlowId));		/* 直播流ID*/
		putChar(&p->flv, FLV_STREAM_DATA);	/* 1：flv头 2：flv数据 */
		putUI32(&p->flv, htonl(uiIndex));			/* 计数器 */
		liSizePos = p->flv.pos;				// 记录数据大小写入的位置
		putUI32(&p->flv, 0);				/* 数据部分大小 */
#endif

		putChar(&p->flv, FLV_TAG_TYPE_AUDIO);
		putUI24(&p->flv, inputlen + flags_size);
		putUI24(&p->flv, ts & 0xFFFFFF);
		putChar(&p->flv, (ts >> 24) & 0x7F); // timestamps are 32 bits _signed_
		putUI24(&p->flv, 0);

		putChar(&p->flv,0xAF);
		putChar(&p->flv, 1); // AAC raw
		memcpy(&p->flv.data[p->flv.pos],inputBuf,inputlen);
		p->flv.pos +=inputlen;
		putUI32(&p->flv, inputlen + flags_size + 11); // previous tag size

		lnBytesUsed = p->flv.pos;
#ifdef DEF_TEST_FLV_STREAM
		//重新更新数据大小部分；13个字节是 _zb_old_flv_protocol_t 结构体的大小，需要减除
		p->flv.pos = liSizePos;
		{
			//FILE *fp=fopen("test_0922.flv","ab+");
			//fwrite(&outputBuf[13],1,lnBytesUsed-13,fp);
			//fflush(fp);
			//fclose(fp);
		}	putUI32(&p->flv, htonl(lnBytesUsed-13));				/* 数据部分大小 */ 
#endif
		return lnBytesUsed;
	}
	else 
		return 0;
}

int FLVClose (void* p_handle)
{
	if(p_handle==NULL)
		return -1;
	free(p_handle);
	p_handle =NULL;
	return 0;
}














