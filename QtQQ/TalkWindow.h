#pragma once

#include "basicwindow.h"
//#include "TalkWindowShell.h"
#include "ui_TalkWindow.h"


class TalkWindow : public QWidget
{
	Q_OBJECT

public:
	TalkWindow(QWidget *parent, const QString&uid/*, GroupType groupType */);
	~TalkWindow();

public:
	void addEmotionImage(int emotionNum);
	void setWindowName(const QString& name);
	QString getTalkId();

public slots:
	void onSetEmotionBtnStatus();
	void onSendBtnClicked(bool);
	void onItemDoubleClicked(QTreeWidgetItem* item, int column);

	void onFileOpenBtnClicked(bool);

private:
	void initControl();
	void initGroupTalkStatus();		//初始化群聊状态，判断是否为单聊或群聊
	int getCompDepID();			//获取公司部门ID号


	//void initCompanyTalk();		//公司群初始化
	//void initPersonelTalk();	//人事部初始化
	//void initDevelopmentTalk();	//研发部初始化
	//void initMarketTalk();		//市场部初始化
	
	void initGroupTalk();		//初始化群聊
	void initPtoPTalk();		//单聊初始化
	void addPeopInfo(QTreeWidgetItem* pRootGroupItem, int employeeID);



private:

	bool m_isGroupTalk;	//是否为单独群聊

	Ui::TalkWindowClass ui;
	QString  m_talkId;
//	GroupType m_groupType;
	QMap <QTreeWidgetItem*, QString> m_groupPeopleMap;	//所有分组联系人姓名

	friend class TalkWindowShell;
};

