LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE    := ffmpeg
LOCAL_LDLIBS    := -llog -lm -landroid

LOCAL_CFLAGS := -Wall -Wno-deprecated -DANDROID=1 #-DHAVE_LOG=5 #-DMODULE_DEBUG
LOCAL_LDLIBS := -llog -landroid

LOCAL_STATIC_LIBRARIES:=$(LOCAL_PATH)/libavcodec.a \
												$(LOCAL_PATH)/libavfilter.a \
												$(LOCAL_PATH)/libavformat.a \
												$(LOCAL_PATH)/libavutil.a \
												$(LOCAL_PATH)/libswresample.a \
												$(LOCAL_PATH)/libswscale.a \
												$(LOCAL_PATH)/libpostproc.a \
												$(LOCAL_PATH)/libavdevice.a \

LOCAL_EXPORT_C_INCLUDES:=$(LOCAL_PATH)/include

include $(BUILD_SHARED_LIBRARY)