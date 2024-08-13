#include "UserLogin.h"
#include "CCMainWindow.h"
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>



//登陆者qq号
QString gLoginEmployeeID;


UserLogin::UserLogin(QWidget *parent)
	: BasicWindow(parent)
{
	ui.setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose);
	initTitleBar();
	setTitleBarTitle("", ":/Resources/MainWindow/qqlogoclassic.png");
	loadStyleSheet("UserLogin"); 
	initControl();
}

UserLogin::~UserLogin()
{}

void UserLogin::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
	{
		onLoginBtnClicked();
	}
	__super::keyPressEvent(event);
}

void UserLogin::keyReleaseEvent(QKeyEvent* event)
{
	__super::keyReleaseEvent(event);
}

void UserLogin::initControl()
{
	QLabel* headlabel = new QLabel(this);
	headlabel->setFixedSize(68, 68);
	QPixmap pix(":/Resources/MainWindow/head_mask.png");
	headlabel->setPixmap(getRoundImage(QPixmap(":/Resources/MainWindow/app/logo.ico"), pix, headlabel->size()));
	headlabel->move((width() - pix.width()) / 2, ui.titleWidget->height() - 34);
	connect(ui.loginBtn, &QPushButton::clicked, this, &UserLogin::onLoginBtnClicked);

	if (!connectMySql())
	{
		QMessageBox::information(NULL, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("连接数据库失败!"));
	}


}


bool UserLogin::connectMySql()
{
	QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
	db.setDatabaseName("qtqq");		//数据库名称
	db.setHostName("localhost");	//主机名
	db.setUserName("root");			//用户名
	db.setPassword("hdw123");		//密码
	db.setPort(3306);				//端口

	if (db.open())
		return true;

	return false;
}

bool UserLogin::verifyAccountCode(bool& isAccountLogin, QString& strAccount, QString& strEmployeeID)
{
	QString strAccountInput = ui.editUserAccount->text();
	QString strCodeInput = ui.editPassword->text();


	//输入员工号(qq号登陆)
	QString strSqlCode = QString("SELECT code, account FROM tab_accounts WHERE employeeID = %1").arg(strAccountInput);
	QSqlQuery queryEmployeeID(strSqlCode);
	queryEmployeeID.exec();
	

	if (queryEmployeeID.first())	//指向结果集第一条
	{
		
		//数据库中qq号对应的密码   value(x)为搜查的第x列参数
		QString strCode = queryEmployeeID.value(0).toString();
		
		if (strCode == strCodeInput)
		{
			isAccountLogin = false;
			strEmployeeID = strAccountInput;
			strAccount = queryEmployeeID.value(1).toString();	//直接获取账号
			return true;
		}
		else
			return false;
	}
	
	//账号登陆(用户名登陆)
	strSqlCode = QString("SELECT code, employeeID FROM tab_accounts WHERE account = '%1' ").arg(strAccountInput);
	QSqlQuery queryAccount(strSqlCode);
	queryAccount.exec();
	if (queryAccount.first())
	{
		QString strCode = queryAccount.value(0).toString();
		//QString strAccountID = queryAccount.value(1).toString();
        if (true/*strCode == strCodeInput*/)
		{
			//gLoginEmployeeID = strAccountID;		//取消全局变量获取qq账号
			strAccount = strAccountInput;			//不是qq号登陆传值账号
			isAccountLogin = true;					//此处为账号登陆所以为真
			strEmployeeID = queryAccount.value(1).toString();	//直接获取qq号
			return true;
		}
		else
			return false;
	}
	 

	return false;
	
}

void UserLogin::onLoginBtnClicked()
{
	bool isAccountLogin;
	QString strAccount;		//账号
	QString strEmployeeID;	//qq号

	if (!verifyAccountCode(isAccountLogin, strAccount, strEmployeeID))
	{
		QMessageBox::information(NULL, QString(u8"提示"), QString(u8"账号密码输入有误，请重新输入！"));
		ui.editUserAccount->clear();
		ui.editPassword->clear();

		return;
	}
	gLoginEmployeeID = strEmployeeID;

	QString strSqlStatus = QString("UPDATE tab_employees SET online = 2 WHERE employeeID = %1").arg(gLoginEmployeeID);
	QSqlQuery sqlStatus(strSqlStatus);
	sqlStatus.exec();

	this->close();
	CCMainWindow* mainwindow = new CCMainWindow(strEmployeeID, strAccount, isAccountLogin);
	mainwindow->show();

}
