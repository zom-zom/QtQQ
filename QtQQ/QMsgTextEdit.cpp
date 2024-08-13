#include "QMsgTextEdit.h"
#include <QMovie>

QMsgTextEdit::QMsgTextEdit(QWidget *parent)
	: QTextEdit(parent)
{
}

QMsgTextEdit::~QMsgTextEdit()
{
	deletAllEmotionImage();
}

void QMsgTextEdit::onEmotionImageFrameChange(int frame)
{

	//ǿ��ת��ΪQMovie*  ��ȡ�ĸ����������˸ı�
	QMovie* movie = qobject_cast<QMovie*>(sender());
	//ͼƬ������Դ
	document()->addResource(QTextDocument::ImageResource,
		QUrl(m_emotionMap.value(movie)), movie->currentPixmap());
}

void QMsgTextEdit::addEmotionUrl(int emotionNum)
{
	const QString& imageName = QString("qrc:/Resources/MainWindow/emotion/%1.png").arg(emotionNum);
	const QString& flagName = QString("%1").arg(imageName);
	//const QString& flagName = imageName;
	//?
	insertHtml(QString("<img src='%1' />").arg(flagName));
	if (m_listEmotionUrl.contains(imageName))
	{
		return;
	}
	else
	{
		m_listEmotionUrl.append(imageName);
	}

	QMovie* apngMovie = new QMovie(imageName, "apng", this);
	m_emotionMap.insert(apngMovie, flagName);

	//����֡�ı�ʱ������ź�
	connect(apngMovie, &QMovie::frameChanged, this, &QMsgTextEdit::onEmotionImageFrameChange);
	apngMovie->start();
	updateGeometry();


}

void QMsgTextEdit::deletAllEmotionImage()
{
	for (auto itor = m_emotionMap.constBegin(); itor != m_emotionMap.constEnd(); ++itor)
	{
		delete itor.key();
	}
	m_emotionMap.clear();
}

void QMsgTextEdit::clearText()
{
	clear();
	//setFocus();
	//�ָ���ʼ��ʽ
	QFont f;
	f.setFamily("΢���ź�");
	f.setPointSize(10);
	selectAll();
	setCurrentFont(f);
	setTextColor(Qt::black);
	setTextBackgroundColor(Qt::white);
}

void QMsgTextEdit::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
	{
		emit signalPressEnter();
		//onSendBtnClicked(false);
	}
	__super::keyPressEvent(event);
}
