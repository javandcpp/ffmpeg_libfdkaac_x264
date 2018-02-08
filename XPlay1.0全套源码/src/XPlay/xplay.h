/*******************************************************************************
**                                                                            **
**                    Jiedi(China nanjing)Ltd.                                **
**	      ��Ȩ���Ĳܿ����У��˴���ֻ����Ϊѧϰ�ο�����������                  **                                                                            **
*******************************************************************************/

/*****************************FILE INFOMATION***********************************
**
** Project       : XPlay
** Description   : ��������
** Contact       : xiacaojun@qq.com
**        ����   : http://blog.csdn.net/jiedichina
**        QQ	 : 99981441
**		  QQȺ	 ��462249121
**   ΢�Ź��ں�  : jiedi2007
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
