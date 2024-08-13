#include "CCMainWindow.h"
#include "SkinWindow.h"
#include "SysTray.h"
#include "NotifyManager.h"
#include "RootContactItem.h"
#include "WindowManager.h"
#include "TalkWindowShell.h"
#include "ContactItem.h"


#include <QApplication>
#include <QPainter>
#include <QProxyStyle>
#include <QTimer>
#include <QEvent>
#include <QString>
#include <QDebug>
#include <QTreeWidgetItem>
#include <QMouseEvent>
#include <QSqlQuery>
#include <QMessageBox>

QString gStrLoginHeadPath;
//extern QString gLoginEmployeeID;


class CustomProxyStyle :public QProxyStyle
{
public:
    virtual void drawPrimitive(PrimitiveElement element, const QStyleOption* option,
        QPainter* painter, const QWidget* widget = nullptr) const
    {
        if (element == PE_FrameFocusRect)
            return;
        else
            QProxyStyle::drawPrimitive(element, option, painter, widget);
    }

};


CCMainWindow::CCMainWindow(QString employeeID, QString account, bool isAccountLogin, QWidget* parent)
    : BasicWindow(parent)
    , m_employeeID(employeeID)
    , m_isAccountLogin(isAccountLogin)
    , m_account(account)
{
    //setWindowFlags(Qt::FramelessWindowHint);
    ui.setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose);//����������û�����ã�close()�����ú�hide()��setvisible(false)һ����ֻ�����ش��ڶ�����ѣ����������ٸö���

    setWindowFlags(windowFlags() | Qt::Tool);
    loadStyleSheet("CCMainWindow");
    initControl();
    initTimer();
    this->setAttribute(Qt::WA_QuitOnClose, true);//Qt::WA_QuitOnClose���Ե���˼��ָ�����һ������ر�ʱ�˳�������ͨ������lastWindowClosed�źţ������͸��ź�ʱ�������ִ��quit����


}

CCMainWindow::~CCMainWindow()
{}

void CCMainWindow::initTimer()
{

    QTimer* timer = new QTimer(this);
    timer->setInterval(500);
    connect(timer, &QTimer::timeout, [this] {
        static int level = 0;
        if (level == 99)
            level = 0;
        level++;
        setLevelPixmap(level);
        });
    timer->start();
}

void CCMainWindow::initControl()
{
    //����ȡ����ʱ�����Ʊ߿�
    ui.treeWidget->setStyle(new CustomProxyStyle);
    setLevelPixmap(22);

    //setHeadPixmap(":/Resources/MainWindow/girl1.jpg");

    setHeadPixmap(getHeadPicturePath());


    setStatusMenuIcon(":/Resources/MainWindow/StatusSucceeded.png");


    
    QHBoxLayout* appupLayout = new QHBoxLayout;
    appupLayout->setContentsMargins(0, 0, 0, 0);
    appupLayout->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_7.png", "app_7"));
    appupLayout->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_2.png", "app_2"));
    appupLayout->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_3.png", "app_3"));
    appupLayout->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_4.png", "app_4"));
    appupLayout->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_5.png", "app_5"));
    appupLayout->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_6.png", "app_6"));
	appupLayout->addStretch();
    appupLayout->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/skin.png", "app_skin"));
    appupLayout->setSpacing(2);
    ui.appWidget->setLayout(appupLayout);

    ui.bottomLayout_up->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_10.png", "app_10"));
    ui.bottomLayout_up->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_8.png", "app_8"));
    ui.bottomLayout_up->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_11.png", "app_11"));
    ui.bottomLayout_up->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_9.png", "app_9"));
    ui.bottomLayout_up->addStretch();

    //����ǩ��
    ui.lineEdit->installEventFilter(this);
    //��������
    ui.searchLineEdit->installEventFilter(this);

	connect(ui.sysclose, SIGNAL(clicked()), this, SLOT(onButtonCloseClicked()));
	connect(ui.sysmin, SIGNAL(clicked(bool)), this, SLOT(onShowHide(bool)));
    connect(NotifyManager::getInstance(), &NotifyManager::signalSkinChanged, [this] {
        updateSearchStyle();
        });


    SysTray* systray = new SysTray(this);
    initContactTree();
}

void CCMainWindow::initContactTree()
{
    //չ��������ʱ���ź�
    connect(ui.treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)),
        this, SLOT(onItemClicked(QTreeWidgetItem*, int)));
    connect(ui.treeWidget, SIGNAL(itemExpanded(QTreeWidgetItem*)),
        this, SLOT(onItemExpanded(QTreeWidgetItem*)));
    connect(ui.treeWidget, SIGNAL(itemCollapsed(QTreeWidgetItem*)),
        this, SLOT(onItemCollapsed(QTreeWidgetItem*)));
    connect(ui.treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)),
        this, SLOT(onItemDoubleClicked(QTreeWidgetItem*, int)));

    //���ڵ�
    QTreeWidgetItem* pRootGroupItem = new QTreeWidgetItem;
    pRootGroupItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
    pRootGroupItem->setData(0, Qt::UserRole, 0);    //����������Ϊ0


	RootContactItem* pItemName = new RootContactItem(true, ui.treeWidget);
	QString strGroupName = QString::fromLocal8Bit("ţţ�Ƽ�");
	pItemName->setText(strGroupName);

    //�������ڵ�
    ui.treeWidget->addTopLevelItem(pRootGroupItem);
    ui.treeWidget->setItemWidget(pRootGroupItem, 0, pItemName);

    //��ȡ��˾����ID
    QSqlQuery queryCompDepID(QString("SELECT departmentID FROM tab_department WHERE department_name = '%1'").arg(u8"��˾Ⱥ"));
    queryCompDepID.exec();
    if (queryCompDepID.first())
    {
        int compDepID = queryCompDepID.value(0).toInt();
		addCompanyDeps(pRootGroupItem, compDepID);
    }
    else
    {
        QMessageBox::information(NULL, QString(u8"��ʾ"), QString(u8"���ҹ�˾IDʧ��"));
    }


	QSqlQuery querySelfDepID(QString("SELECT departmentID FROM tab_employees WHERE employeeID = %1").arg(m_employeeID));
	querySelfDepID.exec();
	if (querySelfDepID.first())
	{
		int SelfDepID = querySelfDepID.value(0).toInt();
		addCompanyDeps(pRootGroupItem, SelfDepID);
	}
	else
	{
		QMessageBox::information(NULL, QString(u8"��ʾ"), QString(u8"����Ա������Ⱥʧ��"));
	}


    ////��˾����
    //QStringList sCompDeps;
    //sCompDeps << QString::fromLocal8Bit("��˾Ⱥ");
    //sCompDeps << QString::fromLocal8Bit("���²�");
    //sCompDeps << QString::fromLocal8Bit("�з���");
    //sCompDeps << QString::fromLocal8Bit("�г���");

    //for (int nIndex = 0; nIndex < sCompDeps.length(); nIndex++)
    //{
    //    addCompanyDeps(pRootGroupItem, sCompDeps.at(nIndex));
    //}
}

void CCMainWindow::updateSearchStyle()
{
    ui.searchWidget->setStyleSheet(QString("QWidget#searchWidget{background-color:rgba(%1,%2,%3,50);border-bottom:1px solid rgba(%1,%2,%3,30)}\
				QPushButton#searchBtn{border-image:url(:/Resources/MainWindow/search/search_icon.png)}")
        .arg(m_colorBackGround.red()).arg(m_colorBackGround.green()).arg(m_colorBackGround.blue()));
}

void CCMainWindow::addCompanyDeps(QTreeWidgetItem* pRootGroupItem,const int& depID)
{
    QTreeWidgetItem* pChild = new QTreeWidgetItem;
    

    QPixmap pix;
    pix.load(":/Resources/MainWindow/head_mask.png");
    //����ӽڵ�
    pChild->setData(0, Qt::UserRole, 1);    //����������Ϊ1
    pChild->setData(0, Qt::UserRole + 1, depID); //���ò���ID

    //��ȡ��˾������ͷ��
    QPixmap groupPix;
    QSqlQuery queryPicture(QString("SELECT picture FROM tab_department WHERE departmentID = %1").arg(depID));
    queryPicture.exec();
    if (queryPicture.next())
    {
        qDebug() << queryPicture.value(0).toString();
        groupPix.load(queryPicture.value(0).toString());
    }
    //��ȡ��������
    QString strDepName;
    QSqlQuery queryName(QString("SELECT department_name FROM tab_department WHERE departmentID = %1").arg(depID));
    queryName.exec();
    if (queryName.next())
    {
        qDebug () << queryName.value(0).toString();
        strDepName = queryName.value(0).toString();
    }
    //test ����ͷ��
    QPixmap testPixmap;
    testPixmap.load(QString("E:\\qtqq_images\\personel.jpg"));
    //QPixmap(QString("E:\qtqq_images\gsq.jpg")
    ContactItem* pContactItem = new ContactItem(ui.treeWidget);
    pContactItem->setHeadPixmap(getRoundImage(groupPix, pix,
        pContactItem->getHeadLabelSize()));
    pContactItem->setUserName(strDepName);
    pRootGroupItem->addChild(pChild);
    ui.treeWidget->setItemWidget(pChild, 0, pContactItem);

   // m_groupMap.insert(pChild, strDepName);

}

QString CCMainWindow::getHeadPicturePath()
{
    QString strPicturePath;
//qq�ŵ�½

	QSqlQuery queryPicture(QString("SELECT picture, status FROM tab_employees WHERE employeeID = %1").arg(m_employeeID));
	queryPicture.exec();
	queryPicture.next();
	strPicturePath = queryPicture.value(0).toString();

    gStrLoginHeadPath = strPicturePath;
    return strPicturePath;
}

void CCMainWindow::setUserName(const QString& userName)
{
    ui.nameLabel->adjustSize();
    //�ı���������ʡ��
    QString name = ui.nameLabel->fontMetrics().elidedText(userName, Qt::ElideRight, ui.nameLabel->width());
    ui.nameLabel->setText(name);
}

void CCMainWindow::setLevelPixmap(int level)
{
    QPixmap levelPixmap(ui.levelBtn->size());
    levelPixmap.fill(Qt::transparent);
    
    QPainter painter(&levelPixmap);
    painter.drawPixmap(0, 4, QPixmap(":/Resources/MainWindow/lv.png"));

    int unitNum = level % 10;   //��λ��
    int tenNum = level / 10;    //ʮλ��

    //ʮλ,��ȡͼƬ�еĲ��ֽ��л���
    //drawPixmap(���Ƶ�x�����Ƶ�y��ͼƬ��ͼƬ���Ͻ�x��ͼƬ���Ͻ�y�������Ŀ�ȣ������ĸ߶�)

    painter.drawPixmap(10, 4, QPixmap(":/Resources/MainWindow/levelvalue.png"), tenNum * 6, 0, 6, 7);
    //��λ
    painter.drawPixmap(16, 4, QPixmap(":/Resources/MainWindow/levelvalue.png"), unitNum * 6, 0, 6, 7);

    ui.levelBtn->setIcon(levelPixmap);
    ui.levelBtn->setIconSize(ui.levelBtn->size());


}


void CCMainWindow::setHeadPixmap(const QString& headPath)
{
    QPixmap pix;
    pix.load(":/Resources/MainWindow/head_mask.png");
    ui.headLabel->setPixmap(getRoundImage(QPixmap(headPath), pix, ui.headLabel->size()));

}

void CCMainWindow::setStatusMenuIcon(const QString& statusPath)
{
    QPixmap statusBtnPixmap(ui.statusBtn->size());
    statusBtnPixmap.fill(Qt::transparent);

    QPainter painter(&statusBtnPixmap);
    painter.drawPixmap(4, 4, QPixmap(statusPath));

    ui.statusBtn->setIcon(statusBtnPixmap);
    ui.statusBtn->setIconSize(ui.statusBtn->size());

}

QWidget* CCMainWindow::addOtherAppExtension(const QString& appPath, const QString& appName)
{
    QPushButton* btn = new QPushButton(this);
    btn->setFixedSize(20, 20);

    QPixmap pixmap(btn->size());
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    QPixmap appPixmap(appPath);
    painter.drawPixmap((btn->width() - appPixmap.width()) / 2, (btn->height() - appPixmap.height()) / 2, appPixmap);
    btn->setIcon(pixmap);
    btn->setIconSize(btn->size());
    btn->setObjectName(appName);
    btn->setProperty("hasborder", true);

    connect(btn, &QPushButton::clicked, this, &CCMainWindow::onAppIconClicked);
    return btn;

}


void CCMainWindow::resizeEvent(QResizeEvent* event)
{
    QSqlQuery queryName(QString("SELECT employee_name FROM tab_employees WHERE employeeID = %1").arg(m_employeeID));
    queryName.next();
    setUserName(queryName.value(0).toString());
    BasicWindow::resizeEvent(event);

}

bool CCMainWindow::eventFilter(QObject* obj, QEvent* event)
{
    if (ui.searchLineEdit == obj)
    {
        if (event->type() == QEvent::FocusIn)
        {
			ui.searchWidget->setStyleSheet(QString("\
				QWidget#searchWidget{background-color:rgb(255,255,255);border-bottom:1px solid rgba(%1,%2,%3,100)}\
				QPushButton#searchBtn{border-image:url(:/Resources/MainWindow/search/main_search_deldown.png)}\
				QPushButton#searchBtn:hover{border-image:url(:/Resources/MainWindow/search/main_search_delhighlight.png)}\
				QPushButton#searchBtn:pressed{border-image:url(:/Resources/MainWindow/search/main_search_delhighdown.png)}\
				").arg(m_colorBackGround.red()).arg(m_colorBackGround.green()).arg(m_colorBackGround.blue())
			);
        }
    }
    return false;
}

void CCMainWindow::mousePressEvent(QMouseEvent* event)
{
    if (qApp->widgetAt(event->pos()) != ui.searchLineEdit && ui.searchLineEdit->hasFocus())
    {
        ui.searchLineEdit->clearFocus();
    }
    else if (qApp->widgetAt(event->pos()) != ui.lineEdit && ui.lineEdit->hasFocus())
    {
        ui.lineEdit->clearFocus();
    }
    __super::mousePressEvent(event);
}

void CCMainWindow::onAppIconClicked()
{
    if (sender()->objectName() == "app_skin")
    {
        SkinWindow* skinWindow = new SkinWindow;
        skinWindow->show();
    }
}

void CCMainWindow::onItemClicked(QTreeWidgetItem* item, int)
{
    bool bIsChild = item->data(0, Qt::UserRole).toBool();
    if (!bIsChild)
    {
        item->setExpanded(!item->isExpanded());  //δչ����չ������

    }
}

void CCMainWindow::onItemExpanded(QTreeWidgetItem* item)
{
	bool bIsChild = item->data(0, Qt::UserRole).toBool();
	if (!bIsChild)
	{
		//���԰�ȫ�Ľ����ָ����������������ǵļ̳в��ת����   ����ָ�������Ч������Ϊ0�����Զ�ֵΪ0��ָ��Ӧ��dynamic_cast�������Ȼ��0������ָ��һ������  ��ֻ���ܻ���������ָ������õ�����ת����
		RootContactItem* prootItem = dynamic_cast<RootContactItem*>(ui.treeWidget->itemWidget(item, 0));
		if (prootItem)
		{
			prootItem->setExpanded(true);
		}
	}
}

void CCMainWindow::onItemCollapsed(QTreeWidgetItem* item)
{
	bool bIsChild = item->data(0, Qt::UserRole).toBool();
	if (!bIsChild)
	{
		RootContactItem* prootItem = dynamic_cast<RootContactItem*>(ui.treeWidget->itemWidget(item, 0));
		if (prootItem)
		{
			prootItem->setExpanded(false);
		}
	}
}

void CCMainWindow::onItemDoubleClicked(QTreeWidgetItem* item, int)
{
    bool bIsChild = item->data(0, Qt::UserRole).toBool();
	if (bIsChild)
	{
        WindowManager::getInstance()->addNewTalkWindow(item->data(0, Qt::UserRole + 1).toString());

        //QString strGroup = m_groupMap.value(item);
/*
        if (strGroup == QString::fromLocal8Bit("��˾Ⱥ"))
        {
            WindowManager::getInstance()->addNewTalkWindow(item->data(0,Qt::UserRole+1).toString(), COMPANY);
        } 
        else if(strGroup == QString::fromLocal8Bit("���²�"))
        {
			WindowManager::getInstance()->addNewTalkWindow(item->data(0, Qt::UserRole + 1).toString(), PERSONELGROUP);

        }
		else if (strGroup == QString::fromLocal8Bit("�з���"))
		{
			WindowManager::getInstance()->addNewTalkWindow(item->data(0, Qt::UserRole + 1).toString(), DEVELOPMENTGROUP);

		}
		else if (strGroup == QString::fromLocal8Bit("�г���"))
		{
			WindowManager::getInstance()->addNewTalkWindow(item->data(0, Qt::UserRole + 1).toString(), MARKETGROUP);
		}
        */
	}


}

void CCMainWindow::onButtonCloseClicked()
{
    close();
    //qApp->quit();
}
