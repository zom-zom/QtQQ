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
	void initGroupTalkStatus();		//��ʼ��Ⱥ��״̬���ж��Ƿ�Ϊ���Ļ�Ⱥ��
	int getCompDepID();			//��ȡ��˾����ID��


	//void initCompanyTalk();		//��˾Ⱥ��ʼ��
	//void initPersonelTalk();	//���²���ʼ��
	//void initDevelopmentTalk();	//�з�����ʼ��
	//void initMarketTalk();		//�г�����ʼ��
	
	void initGroupTalk();		//��ʼ��Ⱥ��
	void initPtoPTalk();		//���ĳ�ʼ��
	void addPeopInfo(QTreeWidgetItem* pRootGroupItem, int employeeID);



private:

	bool m_isGroupTalk;	//�Ƿ�Ϊ����Ⱥ��

	Ui::TalkWindowClass ui;
	QString  m_talkId;
//	GroupType m_groupType;
	QMap <QTreeWidgetItem*, QString> m_groupPeopleMap;	//���з�����ϵ������

	friend class TalkWindowShell;
};

