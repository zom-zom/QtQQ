#pragma once

#include "TalkWindowShell.h"
#include <QObject>




class WindowManager  : public QObject
{
	Q_OBJECT

public:
	WindowManager();
	~WindowManager();

public:
	QWidget* findWindowName(const QString& qsWindowName);
	void deleteWindowName(const QString& qsWindowName);
	void addWindowName(const QString& qsWindowName, QWidget* qWidget);

	static WindowManager* getInstance();
	void addNewTalkWindow(const QString& uid/*, GroupType groupType = COMPANY, const QString& strPepole = ""*/);

	TalkWindowShell* getTalkWindowShell();

	QString getCreatingTalkId();

private:
	TalkWindowShell* m_talkwindowshell;
	QMap <QString, QWidget*> m_windowMap;
	QString m_strCreatingTalkId = "";	//正在创建的聊天窗口（QQ号）
};
