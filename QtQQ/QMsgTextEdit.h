#pragma once

#include <QTextEdit>
#include <QKeyEvent>

class QMsgTextEdit  : public QTextEdit
{
	Q_OBJECT

public:
	QMsgTextEdit(QWidget* parent = nullptr);
	~QMsgTextEdit();

private slots:
	void onEmotionImageFrameChange(int frame);//�����ı�ʱ��Ӧ�Ĳۺ���

public:
	void addEmotionUrl(int emotionNum);
	void deletAllEmotionImage();
	void clearText();

protected:
	void keyPressEvent(QKeyEvent* event) override;

signals:
	void signalPressEnter(bool = false);


private:
	QList<QString> m_listEmotionUrl;

	QMap<QMovie*, QString> m_emotionMap;

};
