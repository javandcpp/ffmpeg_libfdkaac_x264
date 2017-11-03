### YUV420P给AVFrame赋值

```
//队列中获取I420数据
 uint8_t *picture_buf = *encoderH264->frame_queue.wait_and_pop().get();
 
 //赋值AVFrame
 memcpy(encoderH264->outputYUV->data[0], picture_buf, in_y_size);//Y
 memcpy(encoderH264->outputYUV->data[1], picture_buf + in_y_size, in_y_size / 4);//U
 memcpy(encoderH264->outputYUV->data[2], picture_buf + in_y_size * 5 / 4, in_y_size / 4);//V

 encoderH264->outputYUV->linesize[0] = encoderH264->arguments.in_width;
 encoderH264->outputYUV->linesize[1] = (encoderH264->arguments.in_width + 1) / 2;
 encoderH264->outputYUV->linesize[2] = (encoderH264->arguments.in_width + 1) / 2;


```