/*******************************************************************************
**                                                                            **
**                    Jiedi(China nanjing)Ltd.                                **
**	      ��Ȩ���Ĳܿ����У��˴���ֻ����Ϊѧϰ�ο�����������                  **                                                                            **
*******************************************************************************/

/*****************************FILE INFOMATION***********************************
**
** Project       : XSlider
** Description   : ������QT�Ļ�������
** Contact       : xiacaojun@qq.com
**        ����   : http://blog.csdn.net/jiedichina
**        QQ	 : 99981441
**		  QQȺ	 ��462249121
**   ΢�Ź��ں�  : jiedi2007
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

