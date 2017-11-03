### 处理NV21转I420（YUV420P）及旋转和镜像


* 处理NV21转I420，并对I420数据进行镜像和旋转处理.
* 注意:如果旋转角度为90°/270°,旋转后Y U V行数据量,应该使用height计算.


```

JNIEXPORT jint JNICALL
Java_com_guagua_nativeapp_MediaProcess_encodeH264(JNIEnv *env, jclass type, jbyteArray data_,jint length, jint w, jint h, jint orientation) {
  jbyte *src = env->GetByteArrayElements(data_, NULL);
    //I420
    uint8_t *dstI420 = (uint8_t *) malloc(w * h * 3 / 2);
    //I420/mirror
    uint8_t *mirror = (uint8_t *) malloc(w * h * 3 / 2);
    //I420/mirror/rotate
    uint8_t *rotate = (uint8_t *) malloc(w * h * 3 / 2);
    
    //libyuv NV21 convert I420
    NV21ToI420((const uint8_t *) src, w, (uint8_t *) src + (w * h), w, dstI420, w, dstI420 + (w * h),
               w / 2, dstI420 + (w * h * 5 / 4), w / 2, w, h);
    
    uint8_t *src_y = dstI420;
    uint8_t *src_u = dstI420 + w * h;
    uint8_t *src_v = dstI420 + (w * h * 5 / 4);

    uint8_t *dst_y = mirror;
    uint8_t *dst_u = mirror + w * h;
    uint8_t *dst_v = mirror + (w * h * 5 / 4);

    RotationMode rotationMode = kRotate0;
    if (&encoderH264 != NULL) {
        if (orientation == 1) {
            //libyuv 前置镜像
            I420Mirror(src_y, w, src_u, w / 2, src_v, w / 2, dst_y,
                       w, dst_u, w / 2, dst_v, w / 2, w, h);
            //前置  注意:如果旋转90/270,旋转后,Y U V 行数据量,应该使用height计算
            rotationMode = kRotate90;
            I420Rotate(mirror, w, mirror + (w * h), w / 2, mirror + (w * h * 5 / 4), w / 2, rotate,
                       h, rotate + (w * h), h / 2, rotate + (w * h * 5 / 4), h / 2, w, h,
                       rotationMode);
        } else if (orientation == 0) {
            //后置  旋转90
            rotationMode = kRotate90;
            I420Rotate(src_y, w, src_u, w / 2, src_v, w / 2, rotate, h,
                       rotate + (w * h), h / 2, rotate + (w * h * 5 / 4), h / 2, w, h,
                       rotationMode);
        }
		//写入本地
        fwrite(rotate, 1, w * h * 3 / 2, encoderH264->iVideoFile);
        fflush(encoderH264->iVideoFile);

        encoderH264->frame_queue.push(rotate);
       
    return 0;
```
}