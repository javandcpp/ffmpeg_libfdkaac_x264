/*******************************************************************************
**                                                                            **
**                    Jiedi(China nanjing)Ltd.                                **
**	      ��Ȩ���Ĳܿ����У��˴���ֻ����Ϊѧϰ�ο�����������                  **                                                                            **
*******************************************************************************/

/*****************************FILE INFOMATION***********************************
**
** Project       : VideoWidget
** Description   : 3d��Ƶ���ſؼ�
** Contact       : xiacaojun@qq.com
**        ����   : http://blog.csdn.net/jiedichina
**        QQ	 : 99981441
**		  QQȺ	 ��462249121
**   ΢�Ź��ں�  : jiedi2007
** Creation date : 2016-12-20
**
*******************************************************************************/

#pragma once
#include <QtWidgets/qwidget.h>
#include <QOpenGLWidget>
class VideoWidget:public QOpenGLWidget
{
public:
	VideoWidget(QWidget *p = NULL);
	void paintEvent(QPaintEvent *e);
	void timerEvent(QTimerEvent *e);
	virtual ~VideoWidget();
};

