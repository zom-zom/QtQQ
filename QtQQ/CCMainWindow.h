#pragma once

#include "basicwindow.h"
#include "ui_CCMainWindow.h"

class CCMainWindow : public BasicWindow
{
    Q_OBJECT

public:
    CCMainWindow(QString employeeID, QString account, bool isAccountLogin, QWidget *parent = nullptr);
    ~CCMainWindow();


public:
    void setUserName(const QString& userName);              //�����û���
    void setLevelPixmap(int level);                         //���õȼ�
    void setHeadPixmap(const QString& headPath);            //����ͷ��
    void setStatusMenuIcon(const QString& statusPath);      //����״̬
    //���Ӧ�ò���(appͼƬ·����app����������)
    QWidget* addOtherAppExtension(const QString& appPath, const QString& appName);


private:
	void initTimer();   //��ʼ����ʱ��
	void initControl();
	void initContactTree();         //��ʼ����ϵ���б�
	void updateSearchStyle();       //����������ʽ
    void addCompanyDeps(QTreeWidgetItem* pRootGroupItem, const int& depID);
    QString getHeadPicturePath();   //��ȡͷ��

private:
    void resizeEvent(QResizeEvent* event) override;
    bool eventFilter(QObject* obj, QEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;


private slots:
    void onButtonCloseClicked();
    void onAppIconClicked();

    void onItemClicked(QTreeWidgetItem* item, int);
    void onItemExpanded(QTreeWidgetItem* item);
    void onItemCollapsed(QTreeWidgetItem* item);
    void onItemDoubleClicked(QTreeWidgetItem* item, int);

    

private:
    Ui::CCMainWindowClass ui;
    
    bool m_isAccountLogin;      //�Ƿ����˺Ż���qq��
    QString m_account;          //��½���˺�
    QString m_employeeID;       //��½��qq��

    //���з���ķ�����
    //QMap <QTreeWidgetItem*, QString> m_groupMap;
};

