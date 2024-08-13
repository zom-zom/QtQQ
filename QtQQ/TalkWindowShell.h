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
//	COMPANY = 0,		//��˾Ⱥ
//	PERSONELGROUP,		//���²�
//	DEVELOPMENTGROUP,	//�з���
//	MARKETGROUP,		//�г���
//	PTOP,				//ͬ��֮�䵥������
//};
/*using namespace TalkWindow;*/

class TalkWindowShell : public BasicWindow
{
	Q_OBJECT

public:
	TalkWindowShell(QWidget *parent = nullptr);
	~TalkWindowShell();

public:

	//����µ����촰��
	void addTalkWindow(TalkWindow* talkWindow, TalkWindowItem* talkWindowItem, const QString& uid/*, GroupType grouptype*/);
	
	//���õ�ǰ���촰��
	void setCurrentWidget(QWidget* widget);


	//��ȡ�򿪵����촰��ӳ��
	QMap <QListWidgetItem*, QWidget*> getTalkWindowItemMap() const;
	
private:
	//��ʼ���ؼ�
	void initControl();

	//��ʼ��TCP
	void initTcpSocket();

	//��ʼ��UDP
	void initUdpSocket();

	//�������е�Ա��qq��
	bool createJSFile(QStringList& employeesList);

	//��ȡ���е�Ա��qq��
	void getEmployeesID(QStringList& employeesIDList);

	//������ܵ�����Ϣ
	void handleReceivedMsg(const int& senderEmployeeID, const int& msgType, const QString& strMsg);


public slots:
	//���鰴ť�����ִ�еĲۺ���
	void onEmotionBtnClicked(bool);

	//�ͻ��˷���Tcp���ݣ����ݣ��������ͣ��ļ���
	void updateSendTcpMsg(QString& strData, int& msgType, QString fileName = "");

private slots:
	//����б�����ִ�еĲۺ���
	void onTalkWindowItemClicked(QListWidgetItem* item);
	//���鱻ѡ��
	void onEmotionItemClicked(int emotionNum);

	//����UDP�㲥�յ�������
	void processPendingData();


private:
	Ui::TalkWindowShellClass ui;
	//�򿪵����촰��ӳ��
	QMap<QListWidgetItem*, QWidget*>m_talkWindowItemMap;
	
	//���鴰��
	EmotionWindow* m_emotionWindow;

private:
	QTcpSocket* m_tcpClinetSocket;	//Tcp�ͻ���
	QUdpSocket* m_udpReceiver;		//Udp�ͻ���



};
