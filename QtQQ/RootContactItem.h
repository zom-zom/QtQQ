#pragma once

#include <QLabel>
#include <QPropertyAnimation>

class RootContactItem  : public QLabel
{
	Q_OBJECT

	//箭头角度
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
	QString m_titleText;	//显示的文本
	int m_rotation;			//箭头的角度
	bool m_hasArrow;		//是否有箭头

};
