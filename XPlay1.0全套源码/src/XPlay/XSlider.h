/*******************************************************************************
**                                                                            **
**                    Jiedi(China nanjing)Ltd.                                **
**	      版权归夏曹俊所有，此代码只能作为学习参考，不得商用                  **                                                                            **
*******************************************************************************/

/*****************************FILE INFOMATION***********************************
**
** Project       : XSlider
** Description   : 重载了QT的滑动条类
** Contact       : xiacaojun@qq.com
**        博客   : http://blog.csdn.net/jiedichina
**        QQ	 : 99981441
**		  QQ群	 ：462249121
**   微信公众号  : jiedi2007
** Creation date : 2016-12-20
**
*******************************************************************************/

#pragma once
#include <qslider.h>
class XSlider:public QSlider
{
	Q_OBJECT
public:
	XSlider(QWidget*p=NULL);
	~XSlider();
	void mousePressEvent(QMouseEvent *e);
};

