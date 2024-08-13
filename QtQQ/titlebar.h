#pragma once

#include <QWidget>
#include <QLabel>
#include <QPushButton>

enum ButtonType
{
	MIN_BUTTON = 0,     //��С�����رհ�ť
	MIN_MAX_BUTTON,     //��С������󻯼��رհ�ť
	ONLY_CLOSE_BUTTON   //ֻ�йرհ�ť
};


//�Զ��������
class TitleBar  : public QWidget
{
	Q_OBJECT

public:
	TitleBar(QWidget *parent = nullptr);
	~TitleBar();


	void setTitleIcon(const QString& filePath);			//���ñ�����ͼ��
	void setTitleContent(const QString& titleContent);	//���ñ���������
	void setTitleWidth(int width);					//���ñ���������
	void setButtonType(ButtonType buttonType);		//���ñ�������ť����

	//���桢��ȡ�������ǰ�Ĵ���λ�ü���С
	void saveRestoreInfo(const QPoint& point, const QSize& size);
	void getRestoreInfo(QPoint& point, QSize& size);



protected:
	void paintEvent(QPaintEvent* event) override;				//��ͼ�¼�
	void mouseDoubleClickEvent(QMouseEvent* event) override;	//���˫���¼�
	void mousePressEvent(QMouseEvent* event) override;			//��갴���¼�
	void mouseMoveEvent(QMouseEvent* event) override;			//����ƶ��¼�
	void mouseReleaseEvent(QMouseEvent* event) override;		//����ɿ��¼�


private:
	void initControl();				//��ʼ���ؼ�
	void initConnections();			//��ʼ���ź���۵�����
	void loadStyleSheet(const QString& sheetName);		//������ʽ��


signals:
	//��ť������ź�
	void signalButtonMinClicked();		//��С����ť
	void signalButtonRestoreClicked();	//��󻯻�ԭ��ť
	void signalButtonMaxClicked();		//��󻯰�ť
	void signalButtonCloseClicked();	//�رհ�ť

private slots:
	//��ť��Ӧ�Ĳ�
	void onButtonMinClicked();
	void onButtonRestoreClicked();
	void onButtonMaxClicked();
	void onButtonCloseClicked();


private:
	QLabel* m_pIcon;
	QLabel* m_pTitleContent;
	QPushButton* m_pButtonMax;
	QPushButton* m_pButtonRestore;
	QPushButton* m_pButtonMin;
	QPushButton* m_pButtonClose;

	//��󻯻�ԭ��ť���������ڱ��洰��λ�ü���С��
	QPoint m_restorePos;
	QSize m_restoreSize;

	//�ƶ����ڵı���
	bool m_isPressed;
	QPoint m_startMovePos;

	QString m_titleContent;			//����������
	ButtonType m_buttonType;		//���������Ͻ�����
};
