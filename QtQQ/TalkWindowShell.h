#pragma once

#include "basicwindow.h"
#include "ui_TalkWindowShell.h"

#include "TalkWindow.h"

#include "TalkWindowItem.h"
#include "EmotionWindow.h"

#include <QMap>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QListWidgetItem>

//class TalkWindow;
//class TalkWindowItem;
//class QListWidgetItem;
//class EmotionWindow;

//enum GroupType {
//	COMPANY = 0,		//公司群
//	PERSONELGROUP,		//人事部
//	DEVELOPMENTGROUP,	//研发部
//	MARKETGROUP,		//市场部
//	PTOP,				//同事之间单独聊天
//};
/*using namespace TalkWindow;*/

class TalkWindowShell : public BasicWindow
{
	Q_OBJECT

public:
	TalkWindowShell(QWidget *parent = nullptr);
	~TalkWindowShell();

public:

	//添加新的聊天窗口
	void addTalkWindow(TalkWindow* talkWindow, TalkWindowItem* talkWindowItem, const QString& uid/*, GroupType grouptype*/);
	
	//设置当前聊天窗口
	void setCurrentWidget(QWidget* widget);


	//获取打开的聊天窗口映射
	QMap <QListWidgetItem*, QWidget*> getTalkWindowItemMap() const;
	
private:
	//初始化控件
	void initControl();

	//初始化TCP
	void initTcpSocket();

	//初始化UDP
	void initUdpSocket();

	//创建所有的员工qq号
	bool createJSFile(QStringList& employeesList);

	//获取所有的员工qq号
	void getEmployeesID(QStringList& employeesIDList);

	//处理接受到的信息
	void handleReceivedMsg(const int& senderEmployeeID, const int& msgType, const QString& strMsg);


public slots:
	//表情按钮点击后执行的槽函数
	void onEmotionBtnClicked(bool);

	//客户端发送Tcp数据（数据，数据类型，文件）
	void updateSendTcpMsg(QString& strData, int& msgType, QString fileName = "");

private slots:
	//左侧列表点击后执行的槽函数
	void onTalkWindowItemClicked(QListWidgetItem* item);
	//表情被选中
	void onEmotionItemClicked(int emotionNum);

	//处理UDP广播收到的数据
	void processPendingData();


private:
	Ui::TalkWindowShellClass ui;
	//打开的聊天窗口映射
	QMap<QListWidgetItem*, QWidget*>m_talkWindowItemMap;
	
	//表情窗口
	EmotionWindow* m_emotionWindow;

private:
	QTcpSocket* m_tcpClinetSocket;	//Tcp客户端
	QUdpSocket* m_udpReceiver;		//Udp客户端



};
