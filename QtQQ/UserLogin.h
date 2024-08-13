#pragma once

#include "basicwindow.h"
#include "ui_UserLogin.h"
#include <QtWidgets/QWidget>
#include <QKeyEvent>


class UserLogin  : public BasicWindow
{
	Q_OBJECT

public:
	UserLogin(QWidget *parent = Q_NULLPTR);
	~UserLogin();

private:
	Ui::UserLogin ui;

protected:
	void keyPressEvent(QKeyEvent* event) override;
	void keyReleaseEvent(QKeyEvent* event) override;

private slots:
	void onLoginBtnClicked();
private:
	void initControl();
	
	bool connectMySql();		//连接数据库	
	bool verifyAccountCode(bool &isAccountLogin, QString &strAccount, QString& strEmployeeID);	//验证账号密码
	
	
};
