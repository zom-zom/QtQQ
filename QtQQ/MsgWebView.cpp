#include "MsgWebView.h"
#include <QFile>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonDocument>
#include <QWebChannel>
#include <QSqlQueryModel>

#include "TalkWindowShell.h"
#include "WindowManager.h"


extern QString gStrLoginHeadPath;

MsgHtmlObj::MsgHtmlObj(QObject* parent, QString msgLPicPath):QObject(parent)
{
	m_msgLPicPath = msgLPicPath;
	initHtmlTmpl();
}


void MsgHtmlObj::initHtmlTmpl()
{
	m_msgLHtmlTmpl = getMsgTmpHtml("msgleftTmpl");
	m_msgLHtmlTmpl.replace("%1", m_msgLPicPath);

	m_msgRHtmlTmpl = getMsgTmpHtml("msgrightTmpl");
	m_msgRHtmlTmpl.replace("%1", gStrLoginHeadPath);

}



QString MsgHtmlObj::getMsgTmpHtml(const QString& code)
{
	QFile file(":/Resources/MainWindow/MsgHtml/" + code + ".html");
	file.open(QFile::ReadOnly);
	QString strData;
	if (file.isOpen())
	{
		strData = QLatin1String(file.readAll());
	}
	else
	{
		QMessageBox::information(nullptr, "Tips", "Failed to init html");
		return QString("");
	}
	file.close();
	return strData;
}

bool MsgWebPage::acceptNavigationRequest(const QUrl& url, NavigationType type, bool isMainFrame)
{
	//������qrc:/*.html
	if (url.scheme() == QString("qrc"))
		return true;
	else
		return false;

}




//QWebEngineView��ͼ������ʾ��ҳ����
MsgWebView::MsgWebView(QWidget *parent)
	: QWebEngineView(parent)
	, m_channel(new QWebChannel(this))
{
	MsgWebPage* page = new MsgWebPage(this);
	setPage(page);

	m_msgHtmlObj = new MsgHtmlObj(this);
	m_channel->registerObject("external0", m_msgHtmlObj);

	TalkWindowShell* talkwindowShell = WindowManager::getInstance()->getTalkWindowShell();
	connect(this, &MsgWebView::signalSendMsg, talkwindowShell, &TalkWindowShell::updateSendTcpMsg);

	//��ǰ���ڹ��������촰�ڵ�ID��qq�ţ�
	QString strTalkId = WindowManager::getInstance()->getCreatingTalkId();

	QSqlQueryModel queryEmployeeModel;
	QString strEmployeeID, strPicturePath;
	QString strExternal;
	bool isGroupTalk = false;
	
	//��ȡ��˾ȺID
	queryEmployeeModel.setQuery(QString("SELECT departmentID FROM tab_department WHERE department_name = '%1'")
		.arg(QStringLiteral("��˾Ⱥ")));
	QModelIndex companyIndex = queryEmployeeModel.index(0, 0);
	QString strCompanyID = queryEmployeeModel.data(companyIndex).toString();

	if (strTalkId == strCompanyID)	//��˾Ⱥ��
	{
		isGroupTalk = true;
		queryEmployeeModel.setQuery("SELECT employeeID, picture FROM tab_employees WHERE status = 1");
	}
	else
	{
		if (strTalkId.length() == 4)	//����Ⱥ��
		{
			isGroupTalk = true;
			queryEmployeeModel.setQuery(QString("SELECT employeeID, picture FROM tab_employees WHERE status = 1 AND departmentID = %1").arg(strTalkId));

		}
		else	//��������
 		{
			queryEmployeeModel.setQuery(QString("SELECT picture FROM tab_employees WHERE status = 1 AND employeeID = %1").arg(strTalkId));
			
			QModelIndex index = queryEmployeeModel.index(0, 0);
			strPicturePath = queryEmployeeModel.data(index).toString();

			strExternal = "external_" + strTalkId;
			MsgHtmlObj* msgHtmlObj = new MsgHtmlObj(this, strPicturePath);
			m_channel->registerObject(strExternal, msgHtmlObj);
		}
	}

	if (isGroupTalk)
	{
		QModelIndex employeeModelIndex, pictureModelIndex;
		int rows = queryEmployeeModel.rowCount();
		for (int i = 0; i < rows; i++)
		{
			employeeModelIndex = queryEmployeeModel.index(i, 0);
			pictureModelIndex = queryEmployeeModel.index(i, 1);

			strEmployeeID = queryEmployeeModel.data(employeeModelIndex).toString();
			strPicturePath = queryEmployeeModel.data(pictureModelIndex).toString();

			strExternal = "external_" + strEmployeeID;

			MsgHtmlObj* msgHtmlObj = new MsgHtmlObj(this, strPicturePath);
			m_channel->registerObject(strExternal, msgHtmlObj);
		}
	}

	this->page()->setWebChannel(m_channel);

	//��ʼ������Ϣ��ҳҳ��
	this->load(QUrl("qrc:/Resources/MainWindow/MsgHtml/msgTmpl.html"));




}

MsgWebView::~MsgWebView()
{}


void MsgWebView::appendMsg(const QString& html, QString strObj)
{
	QJsonObject msgObj;
	QString qsMsg;
	const QList<QStringList> msgList = parseHtml(html);//����html

	int imageNum = 0;
	int msgType = 1;	//��Ϣ����: 0�Ǳ���  1���ı�  2���ļ�
	bool isImageMsg = false;
	QString strData;	//���͵�����



	for (int i = 0; i < msgList.size(); i++)
	{
		if (msgList.at(i).at(0) == "img")
		{
			isImageMsg = true;
			QString imagePath = msgList.at(i).at(1); 
			QPixmap pixmap;

			//��ȡ��������λ��
			QString strEmotionPath = "qrc:/Resources/MainWindow/emotion/";
			int pos = strEmotionPath.size();
			
			
			//��ȡ��������
			QString strEmotionName = imagePath.mid(pos);
			strEmotionName.replace(".png", "");



			//���ݱ������Ƶĳ��Ƚ������ñ������ݣ�����3λ����3λ����1 ��001
			int emotionNameL = strEmotionName.length();
			if (emotionNameL == 1)
			{
				strData = strData + "00" + strEmotionName;
			}
			else if (emotionNameL == 2)
			{
				strData = strData + "0" + strEmotionName;
			}
			else if (emotionNameL == 3)
			{
				strData = strData + strEmotionName;
			}

			msgType = 0;
			imageNum++;

			if (imagePath.left(3) == "qrc")
			{
				pixmap.load(imagePath.mid(3));
			}
			else
			{
				pixmap.load(imagePath);
			}



			//����ͼƬhtml��ʽ�ı����
			QString imgPath = QString("<img src=\"%1\" width=\"%2\" height=\"%3\"/>")
				.arg(imagePath).arg(pixmap.width()).arg(pixmap.height());

			qsMsg += imgPath;
		}
		if (msgList.at(i).at(0) == "text")
		{
			qsMsg += msgList.at(i).at(1);
			strData = qsMsg;
		}
	}
	msgObj.insert("MSG",qsMsg);
	const QString& Msg = QJsonDocument(msgObj).toJson(QJsonDocument::Compact);

	if (strObj.toInt() == 0)
	{
		this->page()->runJavaScript(QString("appendHtml0(%1)").arg(Msg));
		if (isImageMsg)
		{
			strData = QString::number(imageNum) + "images" + strData;
		}
		//����
		emit signalSendMsg(strData, msgType);

	}
	else	//����
	{
		qDebug() << u8"�յ�����: " << strObj << u8"   ��Ϣ�� " << Msg;
		this->page()->runJavaScript(QString("recvHtml_%1(%2)").arg(strObj).arg(Msg));
	}

	//this->page()->runJavaScript(QString("appendHtml(%1)").arg(Msg));
	//emit signalSendMsg(strData, msgType);


}

QList<QStringList> MsgWebView::parseHtml(const QString& html)
{
	QDomDocument doc;
	doc.setContent(html);
	const QDomElement& root = doc.documentElement();//�ڵ�Ԫ��
	//ֱ�ӻ�ȡ��һ����Ԫ��
	const QDomNode& node = root.firstChildElement("body");



	//���ý����ڵ�
	return parseDocNode(node);
}

QList<QStringList> MsgWebView::parseDocNode(const QDomNode& node)
{
	QList<QStringList> attribute;
	const QDomNodeList& list = node.childNodes();	//���������ӽڵ�

	for (int i = 0; i < list.count(); i++)
	{
		const QDomNode& node = list.at(i);

		if (node.isElement())
		{
			//ת����Ԫ��
			const QDomElement& element = node.toElement();
			if (element.tagName() == "img")
			{
				QStringList attributeList;
				attributeList << "img" << element.attribute("src");
				attribute << attributeList;
			}
			if (element.tagName() == "span")
			{
				QStringList attributeList;
				attributeList << "text" << element.text();
				attribute << attributeList;

			}
			if (node.hasChildNodes())
			{
				//������ӽ������ӽڵ�����б�
				attribute << parseDocNode(node);
			}

		}

	}

	return attribute;
}

