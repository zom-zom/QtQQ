#pragma once

#include <QLabel>
#include <QPropertyAnimation>

class RootContactItem  : public QLabel
{
	Q_OBJECT

	//��ͷ�Ƕ�
	Q_PROPERTY(int rotation READ rotation WRITE setRotation)


public:
	RootContactItem(bool hasArrow,  QWidget *parent);
	~RootContactItem();


public:
	void setText(const QString& title);
	void setExpanded(bool expand);
	

private:
	int rotation();
	void setRotation(int rotation);

protected:
	void paintEvent(QPaintEvent* event) override;

private:
	QPropertyAnimation* m_animation;
	QString m_titleText;	//��ʾ���ı�
	int m_rotation;			//��ͷ�ĽǶ�
	bool m_hasArrow;		//�Ƿ��м�ͷ

};
