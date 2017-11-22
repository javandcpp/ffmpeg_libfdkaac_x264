LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)     
LOCAL_MODULE    := Libavcodec     
LOCAL_SRC_FILES := libavcodec.a     
include $(PREBUILT_STATIC_LIBRARY)  
  
include $(CLEAR_VARS)     
LOCAL_MODULE    := Libavdevice  
LOCAL_SRC_FILES :=libavdevice.a     
include $(PREBUILT_STATIC_LIBRARY)  

include $(CLEAR_VARS)     
LOCAL_MODULE    := Libavfilter  
LOCAL_SRC_FILES := libavfilter.a
include $(PREBUILT_STATIC_LIBRARY)  

include $(CLEAR_VARS)     
LOCAL_MODULE    := Libavformat 
LOCAL_SRC_FILES := libavformat.a
include $(PREBUILT_STATIC_LIBRARY)  

include $(CLEAR_VARS)     
LOCAL_MODULE    := Libavutil 
LOCAL_SRC_FILES := libavutil.a
include $(PREBUILT_STATIC_LIBRARY)  

include $(CLEAR_VARS)     
LOCAL_MODULE    := Libfdkaac 
LOCAL_SRC_FILES := libfdk-aac.a
include $(PREBUILT_STATIC_LIBRARY)  

include $(CLEAR_VARS)     
LOCAL_MODULE    := Libpostproc
LOCAL_SRC_FILES := libpostproc.a
include $(PREBUILT_STATIC_LIBRARY) 
 
include $(CLEAR_VARS)     
LOCAL_MODULE    := Libswresample
LOCAL_SRC_FILES := libswresample.a
include $(PREBUILT_STATIC_LIBRARY)  

include $(CLEAR_VARS)     
LOCAL_MODULE    := Libswscale
LOCAL_SRC_FILES := libswscale.a
include $(PREBUILT_STATIC_LIBRARY)  

include $(CLEAR_VARS)     
LOCAL_MODULE    := Libx264
LOCAL_SRC_FILES := libx264.a
include $(PREBUILT_STATIC_LIBRARY) 



include $(CLEAR_VARS)
LOCAL_MODULE    := Media
#LOCAL_SRC_FILES := test.c
LOCAL_STATIC_LIBRARIES := Libavcodec Libavdevice Libavfilter Libavformat Libavutil Libfdkaac Libpostproc Libswresample Libswscale Libx264
include $(BUILD_SHARED_LIBRARY)