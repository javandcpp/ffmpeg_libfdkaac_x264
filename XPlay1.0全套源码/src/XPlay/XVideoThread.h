/*******************************************************************************
**                                                                            **
**                    Jiedi(China nanjing)Ltd.                                **
**	      版权归夏曹俊所有，此代码只能作为学习参考，不得商用                  **                                                                            **
*******************************************************************************/

/*****************************FILE INFOMATION***********************************
**
** Project       : XVideoThread
** Description   : 音视频播放处理线程
** Contact       : xiacaojun@qq.com
**        博客   : http://blog.csdn.net/jiedichina
**        QQ	 : 99981441
**		  QQ群	 : 462249121
**   微信公众号  : jiedi2007
** Creation date : 2016-12-20
**
*******************************************************************************/

#pragma once
#include <QThread>
class XVideoThread:public QThread
{
public:
	static XVideoThread *Get()
	{
		static XVideoThread vt;
		return &vt;
	}
	void run();
	XVideoThread();
	virtual ~XVideoThread();
};

