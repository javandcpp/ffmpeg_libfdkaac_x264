/*******************************************************************************
**                                                                            **
**                    Jiedi(China nanjing)Ltd.                                **
**	      版权归夏曹俊所有，此代码只能作为学习参考，不得商用                  **                                                                            **
*******************************************************************************/

/*****************************FILE INFOMATION***********************************
**
** Project       : XPlay
** Description   : 主界面类
** Contact       : xiacaojun@qq.com
**        博客   : http://blog.csdn.net/jiedichina
**        QQ	 : 99981441
**		  QQ群	 ：462249121
**   微信公众号  : jiedi2007
** Creation date : 2016-12-20
**
*******************************************************************************/


#ifndef XPLAY_H
#define XPLAY_H

#include <QtWidgets/QWidget>
#include "ui_xplay.h"

class XPlay : public QWidget
{
	Q_OBJECT

public:
	XPlay(QWidget *parent = 0);
	~XPlay();
	void resizeEvent(QResizeEvent *e);
	void timerEvent(QTimerEvent *e);
	void openFile(QString name);
public slots:
	void open();
	void play();
	void sliderPress();
	void sliderRelease();
private:
	Ui::XPlayClass ui;
};

#endif // XPLAY_H
