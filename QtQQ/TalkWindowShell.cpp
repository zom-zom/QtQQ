#include "TalkWindowShell.h"
#include "CommonUtils.h"


#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QListWidget>
#include <QMessageBox>
#include <QFile>
#include "WindowManager.h"
#include "ReceiveFile.h"

QString gFileName;	//文件名称
QString gFileData;	//文件数据

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
			QMessageBox::information(this, QString("ERROR"), QString(u8"更新js文件数据失败"));
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

	//判断群聊还是单聊
	QSqlQueryModel sqlDepModel;
	QString strQuery = QString("SELECT picture FROM tab_department WHERE departmentID = %1").arg(uid.toInt());
	sqlDepModel.setQuery(strQuery);

	if (sqlDepModel.rowCount() == 0)
	{
		strQuery = QString("SELECT picture FROM tab_employees WHERE employeeID = %1").arg(uid.toInt());
		sqlDepModel.setQuery(strQuery);
	}
	QModelIndex index;
	index = sqlDepModel.index(0, 0);	//0行0列
	QImage img;
	img.load(sqlDepModel.data(index).toString());



	talkWindowItem->setHeadPixmap(QPixmap::fromImage(img));	//设置头像
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
	setWindowTitle(QString(u8"牛牛-聊天窗口"));

	m_emotionWindow = new EmotionWindow;
	m_emotionWindow->hide();

	QList <int> leftWidgetSize;
	leftWidgetSize << 154 << width() - 154;
	ui.splitter->setSizes(leftWidgetSize);		//分类器设置尺寸

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
	//读取txt文件数据
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
		QMessageBox::information(this, QString(u8"提示"), QString(u8"读取 msgtmpl.txt 失败!"));
		return false;
	}

	//替换    （external0，appendHtml0用作自己发信息使用）
	QFile fileWrite("Resources/MainWindow/MsgHtml/msgtmpl.js");
	if (fileWrite.open(QIODevice::WriteOnly | QIODevice::Truncate))
	{
		//更新空值
		QString strSourceInitNull = "var external = null;";

		//更新初始化
		QString strSourceInit = "external = channel.objects.external;";

		//更新newWebChannel
		QString strSourceNew =
			"new QWebChannel(qt.webChannelTransport,\
			function(channel) {\
			external = channel.objects.external;\
			}\
			);";

		//更新追加recvHtml,脚本中有双引号无法直接进行赋值，采用读文件方式
		QString strSourceRecvHtml;
		QFile fileRecvHtml("Resources/MainWindow/MsgHtml/recvHtml.txt");
		if (fileRecvHtml.open(QIODevice::ReadOnly))
		{
			strSourceRecvHtml = fileRecvHtml.readAll();
			fileRecvHtml.close();
		}
		else
		{
			QMessageBox::information(this, QString(u8"提示"), QString(u8"读取 recvHtml.txt 失败!"));
			return false;
		}

		//保存替换后的脚本
		QString strReplaceInitNull;
		QString strReplaceInit;
		QString strReplaceNew;
		QString strReplaceRecvHtml;

		for (int i = 0; i < employeesList.length(); i++)
		{
			//编辑替换后的空值
			QString strInitNull = strSourceInitNull;
			strInitNull.replace("external", QString("external_%1").arg(employeesList.at(i)));
			strReplaceInitNull += strInitNull;
			strReplaceInitNull += "\n";

			//编辑替换后的初始值
			QString strInit = strSourceInit;
			strInit.replace("external", QString("external_%1").arg(employeesList.at(i)));
			strReplaceInit += strInit;
			strReplaceInit += "\n";

			//编辑替换后的newWebChannel
			QString strNew = strSourceNew;
			strNew.replace("external", QString("external_%1").arg(employeesList.at(i)));
			strReplaceNew += strNew;
			strReplaceNew += "\n";

			//编辑替换后的recvHtml
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
		QMessageBox::information(this, QString(u8"提示"), QString(u8"写 msgtmpl.js 失败!"));
		return false;
	}
}

void TalkWindowShell::getEmployeesID(QStringList& employeesIDList)
{
	QSqlQueryModel queryModel;
	queryModel.setQuery("SELECT employeeID FROM tab_employees WHERE status = 1");

	
	//放回模型的总行数，员工的总数
	int employeesNum = queryModel.rowCount();
	QModelIndex index;
	for (int i = 0; i < employeesNum; i++)
	{
		index = queryModel.index(i, 0);//行列
		employeesIDList << queryModel.data(index).toString();
	}
}



void TalkWindowShell::handleReceivedMsg(const int& senderEmployeeID, const int& msgType, const QString& strMsg)
{
	QMsgTextEdit msgTextEdit;
	msgTextEdit.setText(strMsg);

	if (msgType == 1)	//文本信息
	{
		msgTextEdit.document()->toHtml();
	}
	else if (msgType==0)	//表情信息
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
// 			QMessageBox::information(this, QString(u8"提示"), QString("未找到字体文件 msgFont.txt!"));
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
	//将当前控件的相对位置转换为屏幕的绝对位置
	QPoint emotionPoint = this->mapToGlobal(QPoint(0, 0));

	emotionPoint.setX(emotionPoint.x() + 170);
	emotionPoint.setY(emotionPoint.y() + 220);
	m_emotionWindow->move(emotionPoint);

}

//文本数据包格式：群聊标志 + 发信息员工QQ号 + 收信息员工QQ号（群qq号） + 信息类型 + 数据长度 + 数据（hello）
//表情数据包格式：群聊标志 + 发信息员工QQ号 + 收信息员工QQ号（群qq号） + 信息类型 + 表情个数 + images + 表情数据
//msgType 0表情信息 1文本信息 2文件信息
void TalkWindowShell::updateSendTcpMsg(QString& strData, int& msgType, QString fileName /*= ""*/)
{
	//获取当前活动聊天窗口
	TalkWindow* culTalkWindow = dynamic_cast<TalkWindow*>(ui.rightStackedWidget->currentWidget());
	QString talkId = culTalkWindow->getTalkId();

	QString strGroupFlag;		//1是群聊 0是单聊
	QString strSend;

	if (talkId.length() == 4)	//群QQ的长度
	{
		strGroupFlag = "1";
	}
	else
	{
		strGroupFlag = "0";
	}

	//获取文本信息长度  数字位数 求数字有几位数
	int dataLength = QString::number(strData.length()).length();
	const int sourceDataLength = strData.length();
	QString	strDataLength;

	if (msgType == 1)	//发送文本信息
	{
		//文本信息长度约定为5位
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
			QMessageBox::information(this, QString(u8"提示"), QString(u8"不合理的数据长度！"));
		}
		
		//文本数据包格式：群聊标志 + 发信息员工QQ号 + 收信息员工QQ号（群qq号） + 信息类型(1) + 数据长度 + 数据（hello）
		strSend = strGroupFlag + gLoginEmployeeID + talkId + "1" + strDataLength + strData;
	}
	else if (msgType == 0)	//发送表情信息
	{
		//表情数据包格式：群聊标志 + 发信息员工QQ号 + 收信息员工QQ号（群qq号） + 信息类型(0) + 表情个数 + images + 表情数据
		strSend = strGroupFlag + gLoginEmployeeID + talkId + "0" + strData;

	}
	else if (msgType == 2)
	{
		/*文件数据表格式：群聊标志 + 发信息员工QQ号 + 收信息员工QQ号（群qq号） + 信息类型(3) + 文件长度
		+ "bytes" + 文件名称 + "data_begin" + 文件内容*/
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
	数据包格式：
	文本数据包格式：群聊标志 + 发信息员工QQ号 + 收信息员工QQ号（群qq号） + 信息类型(1) + 数据长度 + 数据（hello）
	表情数据包格式：群聊标志 + 发信息员工QQ号 + 收信息员工QQ号（群qq号） + 信息类型(0) + 表情个数 + images + 表情数据
	文件数据表格式：群聊标志 + 发信息员工QQ号 + 收信息员工QQ号（群qq号） + 信息类型(3) + 文件长度 + "bytes" + 文件名称 + "data_begin" + 文件内容
	
	群聊标志占1位，0表示单聊，1表示群聊
	信息类型占1位，0表示表情信息，1表示文本信息，2表示文件信息
	QQ号占5位，群QQ号占4位，数据包占5位，表情包名称占3位
	（注意：当群聊标志为1时，则数据包没有收信息员工QQ号，而是收信息员工QQ号
			当群聊标志为1时，则数据包没有收信息群QQ号，而是收信息群QQ号

	群聊文本信息解析 1 10001 2001 1 00005 Hello
	单聊图片信息解析 0 10001 10002 0 1 images 060
	群聊文件信息解析 1 10005 2000 2 10 bytes test.txt data_begin helloworld

***********************************************************************************************************************************/

void TalkWindowShell::processPendingData()
{
	while (m_udpReceiver->hasPendingDatagrams())
	{
		const static int groupFlagWidth = 1;		//群聊标志占位
		const static int groupWidth = 4;			//群QQ号宽度
		const static int employeeWidth = 5;			//员工QQ号宽度
		const static int msgTypeWidth = 1;			//信息类型宽度
		const static int msgLengthWidth = 5;			//文本信息长度的宽度
		const static int pictureWidth = 3;			//表情图片信息的宽度

		QByteArray btData;
		btData.resize(m_udpReceiver->pendingDatagramSize());
		qint64 readSize = m_udpReceiver->readDatagram(btData.data(), btData.size());


		QString strData = btData.data();
		QString strWindowID;	//聊天窗口ID，群聊则是群号，单聊则是员工号
		QString strSendEmployeeID;		//发送端qq号
		QString strReceiveEmployeeID;	//接收端qq号
		QString strMsg;		//数据


		int msgLen = 0;		//数据长度
		int msgType = 1;	//数据类型

		strSendEmployeeID = strData.mid(groupFlagWidth, employeeWidth);
		
		//自己发的信息不做处理
		if (strSendEmployeeID == gLoginEmployeeID)
		{
			return;
		}
		if (btData[0] == '1')	//群聊
		{
			//群qq号
			strWindowID = strData.mid(groupFlagWidth + employeeWidth, groupWidth);

			QChar cMsgType = btData[groupFlagWidth + employeeWidth + groupWidth];
			if (cMsgType == '1')	//文本信息
			{
				msgType = 1;
				msgLen = strData.mid(groupFlagWidth + employeeWidth + groupWidth + msgTypeWidth, msgLengthWidth).toInt();
				strMsg = strData.mid(groupFlagWidth + employeeWidth + groupWidth + msgTypeWidth + msgLengthWidth, msgLen);
			}
			else if (cMsgType == '0')	//表情信息
			{
				msgType = 0;
				int posImages = strData.indexOf("images");
				strMsg = strData.right(strData.length() - posImages - QString("images").length());

			}
			else if (cMsgType == '2')		//文件信息
			{
				msgType = 2;
				int bytesWidth = QString("bytes").length();
				int posBytes = strData.indexOf("bytes");
				int posData_begin = strData.indexOf("data_begin");



				//文件名称
				QString fileName = strData.mid(posBytes + bytesWidth, posData_begin - posBytes - bytesWidth);
				gFileName = fileName;


				//文件内容
				int dataLengthWidth;
				int posData = posData_begin + QString("data_begin").length();
				strMsg = strData.mid(posData);
				gFileData = strMsg;
				
				//根据employeeID获取发送者姓名
				QString sender;
				int employeeID = strSendEmployeeID.toInt();
				QSqlQuery querySenderName(QString("SELECT employee_name FROM tab_employees WHERE employeeID = %1")
					.arg(employeeID));
				
				querySenderName.exec();
				if (querySenderName.first())
				{
					sender = querySenderName.value(0).toString();
				}

				//接受文件的后续操作。。
				ReceiveFile* recvFile = new ReceiveFile(this);
				connect(recvFile, &ReceiveFile::refuseFile, [this]() {
					return;
					});
				QString msgLabel = QString::fromLocal8Bit("收到来自") + sender +
					QString::fromLocal8Bit("发来的文件，是否接收？");
				recvFile->setMsg(msgLabel);
				recvFile->show();
			}

		}
		else	//单聊
		{
			strReceiveEmployeeID = strData.mid(groupFlagWidth + employeeWidth, employeeWidth);
			strWindowID = strSendEmployeeID;
			//不是发给自己的信息不做处理
			if (strReceiveEmployeeID != gLoginEmployeeID)
			{
				return;
			}
			//获取信息的类型
			QChar cMsgType = btData[groupFlagWidth + employeeWidth + employeeWidth];
			if (cMsgType == '1')		//文本信息
			{
				msgType = 1;

				//文本信息长度
				msgLen = strData.mid(groupFlagWidth + employeeWidth + employeeWidth + msgTypeWidth, msgLengthWidth).toInt();
				strMsg = strData.mid(groupFlagWidth + employeeWidth + employeeWidth + msgTypeWidth + msgLengthWidth, msgLen);
			}
			else if (cMsgType == '0')	//表情信息
			{
				msgType = 0;
				int posImages = strData.indexOf("images");
				int imagesWidth = QString("images").length();
				strMsg = strData.mid(posImages + imagesWidth);

			}
			else if (cMsgType == '2')	//文件信息
			{
				msgType = 2;
				
				int bytesWidth = QString("bytes").length();
				int posBytes = strData.indexOf("bytes");
				int data_beginWidth = QString("data_begin").length();
				int posData_begin = strData.indexOf("data_begin");

				//文件名称
				QString fileName = strData.mid(posBytes + bytesWidth, posData_begin - posBytes - bytesWidth);
				gFileName = fileName;

				//文件内容
				strMsg = strData.mid(posData_begin + data_beginWidth);
				gFileData = strMsg;

				//根据employeeID获取发送者姓名
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
				QString msgLabel = QString::fromLocal8Bit("收到来自") + sender +
					QString::fromLocal8Bit("发来的文件，是否接收？");
				recvFile->setMsg(msgLabel);
				recvFile->show();
			}
		}

		//将聊天窗口设为活动的窗口
		QWidget* widget = WindowManager::getInstance()->findWindowName(strWindowID);
		if (widget)	//聊天窗口存在
		{
			this->setCurrentWidget(widget);

			//同步激活左侧聊天窗口
			QListWidgetItem* item = m_talkWindowItemMap.key(widget);
			item->setSelected(true);

		}
		else	//聊天窗口未打开
		{
			return;
		}

		//文件信息另做处理
		if (msgType != 2)
		{
			int sendEmployeeID = strSendEmployeeID.toInt();
			handleReceivedMsg(sendEmployeeID, msgType, strMsg);
		}



	}
}
