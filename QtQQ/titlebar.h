#pragma once

#include <QWidget>
#include <QLabel>
#include <QPushButton>

enum ButtonType
{
	MIN_BUTTON = 0,     //最小化及关闭按钮
	MIN_MAX_BUTTON,     //最小化、最大化及关闭按钮
	ONLY_CLOSE_BUTTON   //只有关闭按钮
};


//自定义标题栏
class TitleBar  : public QWidget
{
	Q_OBJECT

public:
	TitleBar(QWidget *parent = nullptr);
	~TitleBar();


	void setTitleIcon(const QString& filePath);			//设置标题栏图标
	void setTitleContent(const QString& titleContent);	//设置标题栏内容
	void setTitleWidth(int width);					//设置标题栏长度
	void setButtonType(ButtonType buttonType);		//设置标题栏按钮类型

	//保存、获取窗口最大化前的窗口位置及大小
	void saveRestoreInfo(const QPoint& point, const QSize& size);
	void getRestoreInfo(QPoint& point, QSize& size);



protected:
	void paintEvent(QPaintEvent* event) override;				//绘图事件
	void mouseDoubleClickEvent(QMouseEvent* event) override;	//鼠标双击事件
	void mousePressEvent(QMouseEvent* event) override;			//鼠标按下事件
	void mouseMoveEvent(QMouseEvent* event) override;			//鼠标移动事件
	void mouseReleaseEvent(QMouseEvent* event) override;		//鼠标松开事件


private:
	void initControl();				//初始化控件
	void initConnections();			//初始化信号与槽的连接
	void loadStyleSheet(const QString& sheetName);		//加载样式表


signals:
	//按钮发射的信号
	void signalButtonMinClicked();		//最小化按钮
	void signalButtonRestoreClicked();	//最大化还原按钮
	void signalButtonMaxClicked();		//最大化按钮
	void signalButtonCloseClicked();	//关闭按钮

private slots:
	//按钮响应的槽
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

	//最大化还原按钮变量（用于保存窗体位置及大小）
	QPoint m_restorePos;
	QSize m_restoreSize;

	//移动窗口的变量
	bool m_isPressed;
	QPoint m_startMovePos;

	QString m_titleContent;			//标题栏内容
	ButtonType m_buttonType;		//标题栏右上角类型
};
