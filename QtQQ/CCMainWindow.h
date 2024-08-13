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
    void setUserName(const QString& userName);              //设置用户名
    void setLevelPixmap(int level);                         //设置等级
    void setHeadPixmap(const QString& headPath);            //设置头像
    void setStatusMenuIcon(const QString& statusPath);      //设置状态
    //添加应用部件(app图片路径，app部件对象名)
    QWidget* addOtherAppExtension(const QString& appPath, const QString& appName);


private:
	void initTimer();   //初始化计时器
	void initControl();
	void initContactTree();         //初始化联系人列表
	void updateSearchStyle();       //更新搜索样式
    void addCompanyDeps(QTreeWidgetItem* pRootGroupItem, const int& depID);
    QString getHeadPicturePath();   //获取头像

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
    
    bool m_isAccountLogin;      //是否是账号或者qq号
    QString m_account;          //登陆的账号
    QString m_employeeID;       //登陆的qq号

    //所有分组的分组项
    //QMap <QTreeWidgetItem*, QString> m_groupMap;
};

