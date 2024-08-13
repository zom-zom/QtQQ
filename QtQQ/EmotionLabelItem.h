#pragma once

#include "QClickLabel.h"

class QMovie;

class EmotionLabelItem  : public QClickLabel
{
	Q_OBJECT

public:
	EmotionLabelItem(QWidget *parent);
	~EmotionLabelItem();

	//���ñ�������
	void setEmotionName(int emotionName);

private:
	//��ʼ���ؼ�
	void initControl();

signals:
	//���鱻���
	void emotionClicked(int emotionNum);
	
private:
	int m_emotionName;
	QMovie* m_apngMovie;

};
