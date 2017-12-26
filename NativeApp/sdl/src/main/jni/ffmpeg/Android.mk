LOCAL_PATH := $(call my-dir)

###########################
#
# SDL shared library
#
###########################

#LOCAL_C_INCLUDES := $(LOCAL_PATH)/ffmpeg/include
#LOCAL_EXPORT_C_INCLUDES := $(LOCAL_C_INCLUDES)

include $(CLEAR_VARS)
LOCAL_MODULE := ffmpeg
LOCAL_SRC_FILES := libffmpeg.so
include $(PREBUILT_SHARED_LIBRARY)




