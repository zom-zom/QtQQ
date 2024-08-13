#pragma once

#include <QWebEngineView>
#include <QDomNode>

class MsgHtmlObj :public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString msgLHtmlTmpl MEMBER m_msgLHtmlTmpl NOTIFY signalMsgHtml)
	Q_PROPERTY(QString msgRHtmlTmpl MEMBER m_msgRHtmlTmpl NOTIFY signalMsgHtml)


public:
	//�����˵�ͷ��·��
	MsgHtmlObj(QObject* parent, QString msgPicPath = "");

signals:
	void signalMsgHtml(const QString& html);

private:
	//��ʼ��������ҳ
	void initHtmlTmpl();
	QString getMsgTmpHtml(const QString& code);

private:
	//���˷�������Ϣ
	QString m_msgLHtmlTmpl;
	//�Լ�������Ϣ
	QString m_msgRHtmlTmpl;
	//���˵�ͷ��
	QString m_msgLPicPath;


};


//��д������ֻ��qrc�е�QUrl���д���
class MsgWebPage :public QWebEnginePage
{
	Q_OBJECT

public:


	MsgWebPage(QObject* parent = nullptr) :QWebEnginePage(parent) {};

protected:
	bool acceptNavigationRequest(const QUrl& url, NavigationType type, bool isMainFrame) override;

};

class MsgWebView  : public QWebEngineView
{
	Q_OBJECT

public:
	MsgWebView(QWidget *parent);
	~MsgWebView();

public:
	//������ʾ��ҳ������html��Ϣ,�ڶ�����������˭���� ����ʶ����� external_xxx 0�����Լ�
	void appendMsg(const QString& html, QString strObj = "0");


private:
	//����html�ַ�
	QList<QStringList> parseHtml(const QString& html);

	//Qt������Dom�ڵ㣨���ԡ�˵�����ı��ȣ�������ʹ��QDomNode���б�ʾ
	//�����ڵ�
	QList<QStringList> parseDocNode(const QDomNode& node);

signals:
	void signalSendMsg(QString& strData, int& msgType, QString sFile = "");

private:
	MsgHtmlObj* m_msgHtmlObj;
	QWebChannel* m_channel;

};
