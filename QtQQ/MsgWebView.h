#pragma once

#include <QWebEngineView>
#include <QDomNode>

class MsgHtmlObj :public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString msgLHtmlTmpl MEMBER m_msgLHtmlTmpl NOTIFY signalMsgHtml)
	Q_PROPERTY(QString msgRHtmlTmpl MEMBER m_msgRHtmlTmpl NOTIFY signalMsgHtml)


public:
	//发信人的头像路径
	MsgHtmlObj(QObject* parent, QString msgPicPath = "");

signals:
	void signalMsgHtml(const QString& html);

private:
	//初始化聊天网页
	void initHtmlTmpl();
	QString getMsgTmpHtml(const QString& code);

private:
	//别人发来的信息
	QString m_msgLHtmlTmpl;
	//自己发的信息
	QString m_msgRHtmlTmpl;
	//别人的头像
	QString m_msgLPicPath;


};


//重写此类是只对qrc中的QUrl进行处理
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
	//用于显示网页的其中html信息,第二个参数则是谁发的 用来识别对象 external_xxx 0代表自己
	void appendMsg(const QString& html, QString strObj = "0");


private:
	//解析html字符
	QList<QStringList> parseHtml(const QString& html);

	//Qt中所有Dom节点（属性、说明、文本等）都可以使用QDomNode进行表示
	//解析节点
	QList<QStringList> parseDocNode(const QDomNode& node);

signals:
	void signalSendMsg(QString& strData, int& msgType, QString sFile = "");

private:
	MsgHtmlObj* m_msgHtmlObj;
	QWebChannel* m_channel;

};
