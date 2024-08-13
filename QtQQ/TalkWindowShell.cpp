#include "TalkWindowShell.h"
#include "CommonUtils.h"


#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QListWidget>
#include <QMessageBox>
#include <QFile>
#include "WindowManager.h"
#include "ReceiveFile.h"

QString gFileName;	//�ļ�����
QString gFileData;	//�ļ�����

#define TCP_PORT 8888
#define UDP_PORT 6666

const int gTcpPort = 8888;
const int gUdpPort = 6666;

extern QString gLoginEmployeeID;

TalkWindowShell::TalkWindowShell(QWidget *parent)
	: BasicWindow(parent)
{
	ui.setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose);
	initControl();
	initTcpSocket();
	initUdpSocket();
	
    QFile file(":/Resources/MainWindow/MsgHtml/msgtmpl.js");

	if (!file.size())
	{
		QStringList employeesIDList;
		getEmployeesID(employeesIDList);

		if (!createJSFile(employeesIDList))
		{
			QMessageBox::information(this, QString("ERROR"), QString(u8"����js�ļ�����ʧ��"));
		}
	}
	file.close();




}

TalkWindowShell::~TalkWindowShell()
{
	/*
	delete m_emotionWindow;
	m_emotionWindow = nullptr;
	*/
}

void TalkWindowShell::addTalkWindow(TalkWindow* talkWindow, TalkWindowItem* talkWindowItem, const QString& uid/*, GroupType grouptype*/)
{
	ui.rightStackedWidget->addWidget(talkWindow);
	connect(m_emotionWindow, &EmotionWindow::signalEmotionWindowHide,
		talkWindow, &TalkWindow::onSetEmotionBtnStatus);

	QListWidgetItem* aItem = new QListWidgetItem(ui.listWidget);
	m_talkWindowItemMap.insert(aItem, talkWindow);
	
	aItem->setSelected(true);

	//�ж�Ⱥ�Ļ��ǵ���
	QSqlQueryModel sqlDepModel;
	QString strQuery = QString("SELECT picture FROM tab_department WHERE departmentID = %1").arg(uid.toInt());
	sqlDepModel.setQuery(strQuery);

	if (sqlDepModel.rowCount() == 0)
	{
		strQuery = QString("SELECT picture FROM tab_employees WHERE employeeID = %1").arg(uid.toInt());
		sqlDepModel.setQuery(strQuery);
	}
	QModelIndex index;
	index = sqlDepModel.index(0, 0);	//0��0��
	QImage img;
	img.load(sqlDepModel.data(index).toString());



	talkWindowItem->setHeadPixmap(QPixmap::fromImage(img));	//����ͷ��
	ui.listWidget->addItem(aItem);
	ui.listWidget->setItemWidget(aItem, talkWindowItem);

	onTalkWindowItemClicked(aItem);

	connect(talkWindowItem, &TalkWindowItem::signalCloseClicked,
		[talkWindowItem, talkWindow, aItem, this] {
			m_talkWindowItemMap.remove(aItem);
			talkWindow->close();
			ui.listWidget->takeItem(ui.listWidget->row(aItem));
			delete talkWindowItem;
			ui.rightStackedWidget->removeWidget(talkWindow);
			if (ui.rightStackedWidget->count() < 1)
				close();
		});

}

void TalkWindowShell::setCurrentWidget(QWidget* widget)
{
	ui.rightStackedWidget->setCurrentWidget(widget);

}


QMap <QListWidgetItem*, QWidget*> TalkWindowShell::getTalkWindowItemMap() const
{
	return m_talkWindowItemMap;
}

void TalkWindowShell::initControl()
{
	loadStyleSheet("TalkWindow");
	setWindowTitle(QString(u8"ţţ-���촰��"));

	m_emotionWindow = new EmotionWindow;
	m_emotionWindow->hide();

	QList <int> leftWidgetSize;
	leftWidgetSize << 154 << width() - 154;
	ui.splitter->setSizes(leftWidgetSize);		//���������óߴ�

	ui.listWidget->setStyle(new CustomProxyStyle(this));

	connect(ui.listWidget, &QListWidget::itemClicked, this, &TalkWindowShell::onTalkWindowItemClicked);
	connect(m_emotionWindow, &EmotionWindow::signalEmotionItemClicked, this, &TalkWindowShell::onEmotionItemClicked);

}



void TalkWindowShell::initTcpSocket()
{
	m_tcpClinetSocket = new QTcpSocket(this);
	m_tcpClinetSocket->connectToHost("127.0.0.1", gTcpPort);
}

void TalkWindowShell::initUdpSocket()
{
	m_udpReceiver = new QUdpSocket(this);
	
	for (quint16 port = gUdpPort; port < gUdpPort + 200; port++)
	{
		bool isBind = m_udpReceiver->bind(port, QUdpSocket::ShareAddress);
		if (isBind)
			break;
	}
	connect(m_udpReceiver, &QUdpSocket::readyRead, this, &TalkWindowShell::processPendingData);
}

bool TalkWindowShell::createJSFile(QStringList& employeesList)
{
	//��ȡtxt�ļ�����
	QString strFileTxtPath = "Resources/MainWindow/MsgHtml/msgtmpl.txt";
	QFile fileRead(strFileTxtPath);
	QString strFile;

	if (fileRead.open(QIODevice::ReadOnly))
	{
		strFile = fileRead.readAll();
		fileRead.close();
	}
	else
	{
		QMessageBox::information(this, QString(u8"��ʾ"), QString(u8"��ȡ msgtmpl.txt ʧ��!"));
		return false;
	}

	//�滻    ��external0��appendHtml0�����Լ�����Ϣʹ�ã�
	QFile fileWrite("Resources/MainWindow/MsgHtml/msgtmpl.js");
	if (fileWrite.open(QIODevice::WriteOnly | QIODevice::Truncate))
	{
		//���¿�ֵ
		QString strSourceInitNull = "var external = null;";

		//���³�ʼ��
		QString strSourceInit = "external = channel.objects.external;";

		//����newWebChannel
		QString strSourceNew =
			"new QWebChannel(qt.webChannelTransport,\
			function(channel) {\
			external = channel.objects.external;\
			}\
			);";

		//����׷��recvHtml,�ű�����˫�����޷�ֱ�ӽ��и�ֵ�����ö��ļ���ʽ
		QString strSourceRecvHtml;
		QFile fileRecvHtml("Resources/MainWindow/MsgHtml/recvHtml.txt");
		if (fileRecvHtml.open(QIODevice::ReadOnly))
		{
			strSourceRecvHtml = fileRecvHtml.readAll();
			fileRecvHtml.close();
		}
		else
		{
			QMessageBox::information(this, QString(u8"��ʾ"), QString(u8"��ȡ recvHtml.txt ʧ��!"));
			return false;
		}

		//�����滻��Ľű�
		QString strReplaceInitNull;
		QString strReplaceInit;
		QString strReplaceNew;
		QString strReplaceRecvHtml;

		for (int i = 0; i < employeesList.length(); i++)
		{
			//�༭�滻��Ŀ�ֵ
			QString strInitNull = strSourceInitNull;
			strInitNull.replace("external", QString("external_%1").arg(employeesList.at(i)));
			strReplaceInitNull += strInitNull;
			strReplaceInitNull += "\n";

			//�༭�滻��ĳ�ʼֵ
			QString strInit = strSourceInit;
			strInit.replace("external", QString("external_%1").arg(employeesList.at(i)));
			strReplaceInit += strInit;
			strReplaceInit += "\n";

			//�༭�滻���newWebChannel
			QString strNew = strSourceNew;
			strNew.replace("external", QString("external_%1").arg(employeesList.at(i)));
			strReplaceNew += strNew;
			strReplaceNew += "\n";

			//�༭�滻���recvHtml
			QString strRecvHtml = strSourceRecvHtml;
			strRecvHtml.replace("external", QString("external_%1").arg(employeesList.at(i)));
			strRecvHtml.replace("recvHtml", QString("recvHtml_%1").arg(employeesList.at(i)));
			strReplaceRecvHtml += strRecvHtml;
			strReplaceRecvHtml += "\n";
		}

		strFile.replace(strSourceInitNull, strReplaceInitNull);
		strFile.replace(strSourceInit, strReplaceInit);
		strFile.replace(strSourceNew, strReplaceNew);
		strFile.replace(strSourceRecvHtml, strReplaceRecvHtml);
		
		QTextStream stream(&fileWrite);
		stream << strFile;
		fileWrite.close();
		return true;

	}
	else
	{
		QMessageBox::information(this, QString(u8"��ʾ"), QString(u8"д msgtmpl.js ʧ��!"));
		return false;
	}
}

void TalkWindowShell::getEmployeesID(QStringList& employeesIDList)
{
	QSqlQueryModel queryModel;
	queryModel.setQuery("SELECT employeeID FROM tab_employees WHERE status = 1");

	
	//�Ż�ģ�͵���������Ա��������
	int employeesNum = queryModel.rowCount();
	QModelIndex index;
	for (int i = 0; i < employeesNum; i++)
	{
		index = queryModel.index(i, 0);//����
		employeesIDList << queryModel.data(index).toString();
	}
}



void TalkWindowShell::handleReceivedMsg(const int& senderEmployeeID, const int& msgType, const QString& strMsg)
{
	QMsgTextEdit msgTextEdit;
	msgTextEdit.setText(strMsg);

	if (msgType == 1)	//�ı���Ϣ
	{
		msgTextEdit.document()->toHtml();
	}
	else if (msgType==0)	//������Ϣ
	{
		const int emotionWidth = 3;
		int emotionNum = strMsg.length() / emotionWidth;
		
		for (int i = 0; i < emotionNum; i++)
		{
			msgTextEdit.addEmotionUrl(strMsg.mid(i * emotionWidth, emotionWidth).toInt());
		}
	}

	QString htmlText = msgTextEdit.document()->toHtml();

	if (!htmlText.contains(".png") && !htmlText.contains("</span>"))
	{
		QString text = strMsg;
		QString fontHtml = QString("<span style = \"font-size:10pt;\">%1</span>").arg(text);
		if (!htmlText.contains(fontHtml))
		{
			htmlText.replace(text, fontHtml);
		}
		//QString fontHtml;
		//QFile file(":/Resources/MainWindow/MsgHtml/msgFont.txt");
// 		if (file.open(QIODevice::ReadOnly))
// 		{
// 			fontHtml = file.readAll();
// 			fontHtml.replace("%1", strMsg);
// 			file.close();
// 			qDebug() << fontHtml;
// 		}
// 		else
// 		{
// 			QMessageBox::information(this, QString(u8"��ʾ"), QString("δ�ҵ������ļ� msgFont.txt!"));
// 			return;
// 		}
// 		if (!htmlText.contains(fontHtml))
// 		{
// 			htmlText.replace(strMsg, fontHtml);
// 		}
	}

	TalkWindow* talkWindow = dynamic_cast<TalkWindow*>(ui.rightStackedWidget->currentWidget());
	talkWindow->ui.msgWidget->appendMsg(htmlText, QString::number(senderEmployeeID));
}

void TalkWindowShell::onEmotionBtnClicked(bool)
{
	m_emotionWindow->setVisible(!m_emotionWindow->isVisible());
	//����ǰ�ؼ������λ��ת��Ϊ��Ļ�ľ���λ��
	QPoint emotionPoint = this->mapToGlobal(QPoint(0, 0));

	emotionPoint.setX(emotionPoint.x() + 170);
	emotionPoint.setY(emotionPoint.y() + 220);
	m_emotionWindow->move(emotionPoint);

}

//�ı����ݰ���ʽ��Ⱥ�ı�־ + ����ϢԱ��QQ�� + ����ϢԱ��QQ�ţ�Ⱥqq�ţ� + ��Ϣ���� + ���ݳ��� + ���ݣ�hello��
//�������ݰ���ʽ��Ⱥ�ı�־ + ����ϢԱ��QQ�� + ����ϢԱ��QQ�ţ�Ⱥqq�ţ� + ��Ϣ���� + ������� + images + ��������
//msgType 0������Ϣ 1�ı���Ϣ 2�ļ���Ϣ
void TalkWindowShell::updateSendTcpMsg(QString& strData, int& msgType, QString fileName /*= ""*/)
{
	//��ȡ��ǰ����촰��
	TalkWindow* culTalkWindow = dynamic_cast<TalkWindow*>(ui.rightStackedWidget->currentWidget());
	QString talkId = culTalkWindow->getTalkId();

	QString strGroupFlag;		//1��Ⱥ�� 0�ǵ���
	QString strSend;

	if (talkId.length() == 4)	//ȺQQ�ĳ���
	{
		strGroupFlag = "1";
	}
	else
	{
		strGroupFlag = "0";
	}

	//��ȡ�ı���Ϣ����  ����λ�� �������м�λ��
	int dataLength = QString::number(strData.length()).length();
	const int sourceDataLength = strData.length();
	QString	strDataLength;

	if (msgType == 1)	//�����ı���Ϣ
	{
		//�ı���Ϣ����Լ��Ϊ5λ
		if (dataLength == 1)
		{
			strDataLength = "0000" + QString::number(sourceDataLength);
		}
		else if (dataLength == 2)
		{
			strDataLength = "000" + QString::number(sourceDataLength);
		}
		else if (dataLength == 3)
		{
			strDataLength = "00" + QString::number(sourceDataLength);
		}
		else if (dataLength == 4)
		{
			strDataLength = "0" + QString::number(sourceDataLength);
		}
		else if (dataLength == 5)
		{
			strDataLength = QString::number(sourceDataLength);
		}
		else
		{
			QMessageBox::information(this, QString(u8"��ʾ"), QString(u8"����������ݳ��ȣ�"));
		}
		
		//�ı����ݰ���ʽ��Ⱥ�ı�־ + ����ϢԱ��QQ�� + ����ϢԱ��QQ�ţ�Ⱥqq�ţ� + ��Ϣ����(1) + ���ݳ��� + ���ݣ�hello��
		strSend = strGroupFlag + gLoginEmployeeID + talkId + "1" + strDataLength + strData;
	}
	else if (msgType == 0)	//���ͱ�����Ϣ
	{
		//�������ݰ���ʽ��Ⱥ�ı�־ + ����ϢԱ��QQ�� + ����ϢԱ��QQ�ţ�Ⱥqq�ţ� + ��Ϣ����(0) + ������� + images + ��������
		strSend = strGroupFlag + gLoginEmployeeID + talkId + "0" + strData;

	}
	else if (msgType == 2)
	{
		/*�ļ����ݱ��ʽ��Ⱥ�ı�־ + ����ϢԱ��QQ�� + ����ϢԱ��QQ�ţ�Ⱥqq�ţ� + ��Ϣ����(3) + �ļ�����
		+ "bytes" + �ļ����� + "data_begin" + �ļ�����*/
		QByteArray bt = strData.toUtf8();
		QString strLength = QString::number(bt.length());
		
		strSend = strGroupFlag + gLoginEmployeeID + talkId + "2"
			+ strLength + "bytes" + fileName + "data_begin" + strData;

	}

	QByteArray dataBt;
	dataBt.resize(strSend.length());
	dataBt = strSend.toUtf8();

	m_tcpClinetSocket->write(dataBt);

}

void TalkWindowShell::onTalkWindowItemClicked(QListWidgetItem* item)
{
	QWidget* talkWindowWidget = m_talkWindowItemMap.find(item).value();
	ui.rightStackedWidget->setCurrentWidget(talkWindowWidget);
}

void TalkWindowShell::onEmotionItemClicked(int emotionNum)
{
	TalkWindow* curTalkWindow = dynamic_cast<TalkWindow*>(ui.rightStackedWidget->currentWidget());
	if (curTalkWindow)
	{
		curTalkWindow->addEmotionImage(emotionNum);
	}


}

/**********************************************************************************************************************************
	���ݰ���ʽ��
	�ı����ݰ���ʽ��Ⱥ�ı�־ + ����ϢԱ��QQ�� + ����ϢԱ��QQ�ţ�Ⱥqq�ţ� + ��Ϣ����(1) + ���ݳ��� + ���ݣ�hello��
	�������ݰ���ʽ��Ⱥ�ı�־ + ����ϢԱ��QQ�� + ����ϢԱ��QQ�ţ�Ⱥqq�ţ� + ��Ϣ����(0) + ������� + images + ��������
	�ļ����ݱ��ʽ��Ⱥ�ı�־ + ����ϢԱ��QQ�� + ����ϢԱ��QQ�ţ�Ⱥqq�ţ� + ��Ϣ����(3) + �ļ����� + "bytes" + �ļ����� + "data_begin" + �ļ�����
	
	Ⱥ�ı�־ռ1λ��0��ʾ���ģ�1��ʾȺ��
	��Ϣ����ռ1λ��0��ʾ������Ϣ��1��ʾ�ı���Ϣ��2��ʾ�ļ���Ϣ
	QQ��ռ5λ��ȺQQ��ռ4λ�����ݰ�ռ5λ�����������ռ3λ
	��ע�⣺��Ⱥ�ı�־Ϊ1ʱ�������ݰ�û������ϢԱ��QQ�ţ���������ϢԱ��QQ��
			��Ⱥ�ı�־Ϊ1ʱ�������ݰ�û������ϢȺQQ�ţ���������ϢȺQQ��

	Ⱥ���ı���Ϣ���� 1 10001 2001 1 00005 Hello
	����ͼƬ��Ϣ���� 0 10001 10002 0 1 images 060
	Ⱥ���ļ���Ϣ���� 1 10005 2000 2 10 bytes test.txt data_begin helloworld

***********************************************************************************************************************************/

void TalkWindowShell::processPendingData()
{
	while (m_udpReceiver->hasPendingDatagrams())
	{
		const static int groupFlagWidth = 1;		//Ⱥ�ı�־ռλ
		const static int groupWidth = 4;			//ȺQQ�ſ��
		const static int employeeWidth = 5;			//Ա��QQ�ſ��
		const static int msgTypeWidth = 1;			//��Ϣ���Ϳ��
		const static int msgLengthWidth = 5;			//�ı���Ϣ���ȵĿ��
		const static int pictureWidth = 3;			//����ͼƬ��Ϣ�Ŀ��

		QByteArray btData;
		btData.resize(m_udpReceiver->pendingDatagramSize());
		qint64 readSize = m_udpReceiver->readDatagram(btData.data(), btData.size());


		QString strData = btData.data();
		QString strWindowID;	//���촰��ID��Ⱥ������Ⱥ�ţ���������Ա����
		QString strSendEmployeeID;		//���Ͷ�qq��
		QString strReceiveEmployeeID;	//���ն�qq��
		QString strMsg;		//����


		int msgLen = 0;		//���ݳ���
		int msgType = 1;	//��������

		strSendEmployeeID = strData.mid(groupFlagWidth, employeeWidth);
		
		//�Լ�������Ϣ��������
		if (strSendEmployeeID == gLoginEmployeeID)
		{
			return;
		}
		if (btData[0] == '1')	//Ⱥ��
		{
			//Ⱥqq��
			strWindowID = strData.mid(groupFlagWidth + employeeWidth, groupWidth);

			QChar cMsgType = btData[groupFlagWidth + employeeWidth + groupWidth];
			if (cMsgType == '1')	//�ı���Ϣ
			{
				msgType = 1;
				msgLen = strData.mid(groupFlagWidth + employeeWidth + groupWidth + msgTypeWidth, msgLengthWidth).toInt();
				strMsg = strData.mid(groupFlagWidth + employeeWidth + groupWidth + msgTypeWidth + msgLengthWidth, msgLen);
			}
			else if (cMsgType == '0')	//������Ϣ
			{
				msgType = 0;
				int posImages = strData.indexOf("images");
				strMsg = strData.right(strData.length() - posImages - QString("images").length());

			}
			else if (cMsgType == '2')		//�ļ���Ϣ
			{
				msgType = 2;
				int bytesWidth = QString("bytes").length();
				int posBytes = strData.indexOf("bytes");
				int posData_begin = strData.indexOf("data_begin");



				//�ļ�����
				QString fileName = strData.mid(posBytes + bytesWidth, posData_begin - posBytes - bytesWidth);
				gFileName = fileName;


				//�ļ�����
				int dataLengthWidth;
				int posData = posData_begin + QString("data_begin").length();
				strMsg = strData.mid(posData);
				gFileData = strMsg;
				
				//����employeeID��ȡ����������
				QString sender;
				int employeeID = strSendEmployeeID.toInt();
				QSqlQuery querySenderName(QString("SELECT employee_name FROM tab_employees WHERE employeeID = %1")
					.arg(employeeID));
				
				querySenderName.exec();
				if (querySenderName.first())
				{
					sender = querySenderName.value(0).toString();
				}

				//�����ļ��ĺ�����������
				ReceiveFile* recvFile = new ReceiveFile(this);
				connect(recvFile, &ReceiveFile::refuseFile, [this]() {
					return;
					});
				QString msgLabel = QString::fromLocal8Bit("�յ�����") + sender +
					QString::fromLocal8Bit("�������ļ����Ƿ���գ�");
				recvFile->setMsg(msgLabel);
				recvFile->show();
			}

		}
		else	//����
		{
			strReceiveEmployeeID = strData.mid(groupFlagWidth + employeeWidth, employeeWidth);
			strWindowID = strSendEmployeeID;
			//���Ƿ����Լ�����Ϣ��������
			if (strReceiveEmployeeID != gLoginEmployeeID)
			{
				return;
			}
			//��ȡ��Ϣ������
			QChar cMsgType = btData[groupFlagWidth + employeeWidth + employeeWidth];
			if (cMsgType == '1')		//�ı���Ϣ
			{
				msgType = 1;

				//�ı���Ϣ����
				msgLen = strData.mid(groupFlagWidth + employeeWidth + employeeWidth + msgTypeWidth, msgLengthWidth).toInt();
				strMsg = strData.mid(groupFlagWidth + employeeWidth + employeeWidth + msgTypeWidth + msgLengthWidth, msgLen);
			}
			else if (cMsgType == '0')	//������Ϣ
			{
				msgType = 0;
				int posImages = strData.indexOf("images");
				int imagesWidth = QString("images").length();
				strMsg = strData.mid(posImages + imagesWidth);

			}
			else if (cMsgType == '2')	//�ļ���Ϣ
			{
				msgType = 2;
				
				int bytesWidth = QString("bytes").length();
				int posBytes = strData.indexOf("bytes");
				int data_beginWidth = QString("data_begin").length();
				int posData_begin = strData.indexOf("data_begin");

				//�ļ�����
				QString fileName = strData.mid(posBytes + bytesWidth, posData_begin - posBytes - bytesWidth);
				gFileName = fileName;

				//�ļ�����
				strMsg = strData.mid(posData_begin + data_beginWidth);
				gFileData = strMsg;

				//����employeeID��ȡ����������
				QString sender;
				int employeeID = strSendEmployeeID.toInt();
				QSqlQuery querySenderName(QString("SELECT employee_name FROM tab_employees WHERE employeeID = %1")
					.arg(employeeID));

				querySenderName.exec();
				if (querySenderName.first())
				{
					sender = querySenderName.value(0).toString();
				}

				ReceiveFile* recvFile = new ReceiveFile(this);
				connect(recvFile, &ReceiveFile::refuseFile, [this]() {
					return;
					});
				QString msgLabel = QString::fromLocal8Bit("�յ�����") + sender +
					QString::fromLocal8Bit("�������ļ����Ƿ���գ�");
				recvFile->setMsg(msgLabel);
				recvFile->show();
			}
		}

		//�����촰����Ϊ��Ĵ���
		QWidget* widget = WindowManager::getInstance()->findWindowName(strWindowID);
		if (widget)	//���촰�ڴ���
		{
			this->setCurrentWidget(widget);

			//ͬ������������촰��
			QListWidgetItem* item = m_talkWindowItemMap.key(widget);
			item->setSelected(true);

		}
		else	//���촰��δ��
		{
			return;
		}

		//�ļ���Ϣ��������
		if (msgType != 2)
		{
			int sendEmployeeID = strSendEmployeeID.toInt();
			handleReceivedMsg(sendEmployeeID, msgType, strMsg);
		}



	}
}
