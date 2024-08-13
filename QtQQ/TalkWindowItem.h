#pragma once

#include <QWidget>
#include "ui_TalkWindowItem.h"

class TalkWindowItem : public QWidget
{
	Q_OBJECT

public:
	TalkWindowItem(QWidget *parent = nullptr);
	~TalkWindowItem();

	void setHeadPixmap(const QPixmap& headPath);
	void setMsgLabelContent(const QString& msg);
	QString getMsgLabelText();
	 
private:
	void initControl();

signals:
	void signalCloseClicked();

private:
	void enterEvent(QEvent* event) override;
	void leaveEvent(QEvent* event) override;
	void resizeEvent(QResizeEvent* event) override;


private:
	Ui::TalkWindowItemClass ui;
};
