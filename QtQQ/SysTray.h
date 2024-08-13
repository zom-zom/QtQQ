#pragma once

#include <QSystemTrayIcon>
#include <QWidget>

class SysTray  : public QSystemTrayIcon
{
	Q_OBJECT

public:
	SysTray(QWidget *parent);
	~SysTray();
	
private:
	QWidget* m_parent;

public slots:
	void onIconActiveted(QSystemTrayIcon::ActivationReason reason);

private:
	void initSystemTray();
	void addSysTrayMenu();
};
