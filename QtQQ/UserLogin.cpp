#include "UserLogin.h"
#include "CCMainWindow.h"
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>



//��½��qq��
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
		QMessageBox::information(NULL, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("�������ݿ�ʧ��!"));
	}


}


bool UserLogin::connectMySql()
{
	QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
	db.setDatabaseName("qtqq");		//���ݿ�����
	db.setHostName("localhost");	//������
	db.setUserName("root");			//�û���
	db.setPassword("hdw123");		//����
	db.setPort(3306);				//�˿�

	if (db.open())
		return true;

	return false;
}

bool UserLogin::verifyAccountCode(bool& isAccountLogin, QString& strAccount, QString& strEmployeeID)
{
	QString strAccountInput = ui.editUserAccount->text();
	QString strCodeInput = ui.editPassword->text();


	//����Ա����(qq�ŵ�½)
	QString strSqlCode = QString("SELECT code, account FROM tab_accounts WHERE employeeID = %1").arg(strAccountInput);
	QSqlQuery queryEmployeeID(strSqlCode);
	queryEmployeeID.exec();
	

	if (queryEmployeeID.first())	//ָ��������һ��
	{
		
		//���ݿ���qq�Ŷ�Ӧ������   value(x)Ϊ�Ѳ�ĵ�x�в���
		QString strCode = queryEmployeeID.value(0).toString();
		
		if (strCode == strCodeInput)
		{
			isAccountLogin = false;
			strEmployeeID = strAccountInput;
			strAccount = queryEmployeeID.value(1).toString();	//ֱ�ӻ�ȡ�˺�
			return true;
		}
		else
			return false;
	}
	
	//�˺ŵ�½(�û�����½)
	strSqlCode = QString("SELECT code, employeeID FROM tab_accounts WHERE account = '%1' ").arg(strAccountInput);
	QSqlQuery queryAccount(strSqlCode);
	queryAccount.exec();
	if (queryAccount.first())
	{
		QString strCode = queryAccount.value(0).toString();
		//QString strAccountID = queryAccount.value(1).toString();
        if (true/*strCode == strCodeInput*/)
		{
			//gLoginEmployeeID = strAccountID;		//ȡ��ȫ�ֱ�����ȡqq�˺�
			strAccount = strAccountInput;			//����qq�ŵ�½��ֵ�˺�
			isAccountLogin = true;					//�˴�Ϊ�˺ŵ�½����Ϊ��
			strEmployeeID = queryAccount.value(1).toString();	//ֱ�ӻ�ȡqq��
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
	QString strAccount;		//�˺�
	QString strEmployeeID;	//qq��

	if (!verifyAccountCode(isAccountLogin, strAccount, strEmployeeID))
	{
		QMessageBox::information(NULL, QString(u8"��ʾ"), QString(u8"�˺����������������������룡"));
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
