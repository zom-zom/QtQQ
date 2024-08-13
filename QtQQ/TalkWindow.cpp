#include "TalkWindow.h"
#include <QPushButton>
#include <QTreeWidgetItem>
#include <QToolTip>
#include <QFile>
#include <QMessageBox>
#include <QSqlQueryModel>
#include <QSqlQuery>

#include "CommonUtils.h"
#include "ContactItem.h"
#include "RootContactItem.h"
#include "WindowManager.h"
#include "SendFile.h"

TalkWindow::TalkWindow(QWidget* parent, const QString& uid/*, GroupType groupType*/)
	: QWidget(parent)
	,m_talkId(uid)
	/*,m_groupType(groupType)*/
{
	setAttribute(Qt::WA_DeleteOnClose);
	ui.setupUi(this);
	initControl();
	WindowManager::getInstance()->addWindowName(m_talkId,this);
}

TalkWindow::~TalkWindow()
{
	WindowManager::getInstance()->deleteWindowName(m_talkId);
}

void TalkWindow::addEmotionImage(int emotionNum)
{
	ui.textEdit->setFocus();
	ui.textEdit->addEmotionUrl(emotionNum);

}

void TalkWindow::setWindowName(const QString& name)
{
	ui.nameLabel->setText(name);
}

QString TalkWindow::getTalkId()
{
	return m_talkId;
}

void TalkWindow::onSetEmotionBtnStatus()
{

}

void TalkWindow::onSendBtnClicked(bool)
{
	if (ui.textEdit->toPlainText().isEmpty())
	{
		QToolTip::showText(this->mapToGlobal(QPoint(630, 660)), QString::fromLocal8Bit("���͵���Ϣ����Ϊ�գ�"),
			this, QRect(0, 0, 120, 100), 2000);
		return;
	}

	QString&& html = ui.textEdit->document()->toHtml();

	if (!html.contains(".png") && !html.contains("</span>"))
	{
		QString fontHtml;
		QString text = ui.textEdit->toPlainText();
		qDebug() << text;
		QFile file(":/Resources/MainWindow/MsgHtml/msgFont.txt");
		if (file.open(QIODevice::ReadOnly))
		{
			fontHtml = file.readAll();
			qDebug() << fontHtml;
			fontHtml.replace("%1", text);
			file.close();
			qDebug() << fontHtml;
		}
		else
		{
			QMessageBox::information(this, QString(u8"��ʾ"), QString("δ�ҵ������ļ� msgFont.txt!"));
			return;
		}

		if (!html.contains(fontHtml))
		{
			html.replace(text, fontHtml);
		}
	}

	//�˴����˸��ı��༭����Դ������Ѹ�ʽ������������Ի���Ҫ���и�ʽ�ָ����Ż���������Ϣ��
	//ui.textEdit->clear();
	ui.textEdit->clearText();
	ui.textEdit->deletAllEmotionImage();
	ui.msgWidget->appendMsg(html);	//����Ϣ�������html��Ϣ

}

void TalkWindow::onItemDoubleClicked(QTreeWidgetItem* item, int column)
{
	bool bIsChild = item->data(0, Qt::UserRole).toBool();
	if (bIsChild)
	{
		QString strPeopleName = m_groupPeopleMap.value(item);
		WindowManager::getInstance()->addNewTalkWindow(item->data(0, Qt::UserRole + 1).toString()/*, PTOP, strPeopleName*/);
	}
}

void TalkWindow::onFileOpenBtnClicked(bool)
{
	SendFile* sendFile = new SendFile(this);
	sendFile->show();
}

void TalkWindow::initControl()
{
	QList <int> rightWidgetSize;
	rightWidgetSize << 600 << 138;
	ui.bodySplitter->setSizes(rightWidgetSize);

	ui.textEdit->setFontPointSize(10);
	ui.textEdit->setFocus();

	connect(ui.sysmin, &QPushButton::clicked, dynamic_cast<BasicWindow*>(parent()), &TalkWindowShell::onShowMin);
	connect(ui.sysclose, &QPushButton::clicked, (const BasicWindow*)parent(), &TalkWindowShell::onShowClose);
	connect(ui.closeBtn, &QPushButton::clicked, (const BasicWindow*)parent(), &TalkWindowShell::onShowClose);

	connect(ui.faceBtn, &QPushButton::clicked, dynamic_cast<TalkWindowShell*>(parent()), &TalkWindowShell::onEmotionBtnClicked);
	connect(ui.sendBtn, &QPushButton::clicked, this, &TalkWindow::onSendBtnClicked);

	connect(ui.treeWidget, &QTreeWidget::itemDoubleClicked, this, &TalkWindow::onItemDoubleClicked);

	//���Ӱ���Enter����
	connect(ui.textEdit, &QMsgTextEdit::signalPressEnter, this, &TalkWindow::onSendBtnClicked);

	connect(ui.fileopenBtn, &QPushButton::clicked, this, &TalkWindow::onFileOpenBtnClicked);

	initGroupTalkStatus();
	if (m_isGroupTalk)
	{
		initGroupTalk();
	}
	else
	{
		initPtoPTalk();
	}


	/*
	switch (m_groupType)
	{
	case COMPANY:
	{
		initCompanyTalk();
	}
	break;
	case PERSONELGROUP:
	{
		initPersonelTalk();
	}
	break;
	case DEVELOPMENTGROUP:
	{
		initDevelopmentTalk();
	}
	break;
	case MARKETGROUP:
	{
		initMarketTalk();
	}
	break;
	default:	//����
	{
		initPtoPTalk();
	}
	break;

	}
	*/

}

void TalkWindow::initGroupTalkStatus()
{
	QSqlQueryModel sqlDepModel;
	QString strSql = QString("SELECT * FROM tab_department WHERE departmentID = %1").arg(m_talkId);
	sqlDepModel.setQuery(strSql);
	
	int rows = sqlDepModel.rowCount();
	if (rows == 0)//��������
		m_isGroupTalk = false;
	else
		m_isGroupTalk = true;




}

int TalkWindow::getCompDepID()
{
	QSqlQuery queryDepID(QString("SELECT departmentID FROM tab_department WHERE department_name = '%1'").arg(u8"��˾Ⱥ"));
	queryDepID.exec();
	queryDepID.first();
	
	return queryDepID.value(0).toInt();

}

void TalkWindow::initGroupTalk()
{
	QTreeWidgetItem* pRootItem = new QTreeWidgetItem();
	pRootItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);

	pRootItem->setData(0, Qt::UserRole, 0);
	RootContactItem* pItemName = new RootContactItem(false, ui.treeWidget);

	ui.treeWidget->setFixedHeight(646);	//shell�߶� - shellͷ�ߣ�talkwindow titleWidget��

	

	//��ǰ�����Ⱥ����������
	QString strGroupName;
	QSqlQuery queryGroupName(QString("SELECT department_name FROM tab_department WHERE departmentID = %1").arg(m_talkId));
	queryGroupName.exec();
	if (queryGroupName.next())
	{
		strGroupName = queryGroupName.value(0).toString();
	}


	QSqlQueryModel queryEmployeeModel;

	if (getCompDepID() == m_talkId.toInt())	//��˾Ⱥ
	{
		queryEmployeeModel.setQuery(QString("SELECT employeeID FROM tab_employees WHERE status = 1"));
	}
	else	//����Ⱥ
	{
		queryEmployeeModel.setQuery(QString("SELECT employeeID FROM tab_employees WHERE status = 1 AND departmentID = %1").arg(m_talkId));
	}

	int nEmployeeNum = queryEmployeeModel.rowCount();



	QString qsGroupName = QString::fromLocal8Bit("%1 %2/%3").arg(strGroupName).arg(0).arg(nEmployeeNum);
	pItemName->setText(qsGroupName);


	//�������ڵ�
	ui.treeWidget->addTopLevelItem(pRootItem);
	ui.treeWidget->setItemWidget(pRootItem, 0, pItemName);

	//չ��
	pRootItem->setExpanded(true);

	for (int i = 0; i < nEmployeeNum; ++i)
	{
		QModelIndex modelIndex = queryEmployeeModel.index(i, 0);

		int employeeID = queryEmployeeModel.data(modelIndex).toInt();



		//����ӽڵ�
		addPeopInfo(pRootItem,employeeID);
	}
}

/*
void TalkWindow::initCompanyTalk()
{
	QTreeWidgetItem* pRootItem = new QTreeWidgetItem();
	pRootItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
	
	pRootItem->setData(0, Qt::UserRole, 0);
	RootContactItem* pItemName = new RootContactItem(false, ui.treeWidget);

	ui.treeWidget->setFixedHeight(646);	//shell�߶� - shellͷ�ߣ�talkwindow titleWidget��
	//ui.widget->setBaseSize(280, 355);//���ó�ʼ���
	int nEmployeeNum = 50;
	QString qsGroupName = QString::fromLocal8Bit("��˾Ⱥ %1/%2").arg(0).arg(nEmployeeNum);
	pItemName->setText(qsGroupName);


	//�������ڵ�
	ui.treeWidget->addTopLevelItem(pRootItem);
	ui.treeWidget->setItemWidget(pRootItem, 0, pItemName);

	//չ��
	pRootItem->setExpanded(true);

	for (int i = 0; i < nEmployeeNum; ++i)
	{
		addPeopInfo(pRootItem);
	}



}

void TalkWindow::initPersonelTalk()
{
	QTreeWidgetItem* pRootItem = new QTreeWidgetItem();
	pRootItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);

	pRootItem->setData(0, Qt::UserRole, 0);
	RootContactItem* pItemName = new RootContactItem(false, ui.treeWidget);

	ui.treeWidget->setFixedHeight(646);	//shell�߶� - shellͷ�ߣ�talkwindow titleWidget��
	//ui.widget->setBaseSize(280, 355);//���ó�ʼ���
	int nEmployeeNum = 50;
	QString qsGroupName = QString::fromLocal8Bit("����Ⱥ %1/%2").arg(0).arg(nEmployeeNum);
	pItemName->setText(qsGroupName);


	//�������ڵ�
	ui.treeWidget->addTopLevelItem(pRootItem);
	ui.treeWidget->setItemWidget(pRootItem, 0, pItemName);

	//չ��
	pRootItem->setExpanded(true);

	for (int i = 0; i < nEmployeeNum; ++i)
	{
		addPeopInfo(pRootItem);
	}
}

void TalkWindow::initDevelopmentTalk()
{
	QTreeWidgetItem* pRootItem = new QTreeWidgetItem();
	pRootItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);

	pRootItem->setData(0, Qt::UserRole, 0);
	RootContactItem* pItemName = new RootContactItem(false, ui.treeWidget);

	ui.treeWidget->setFixedHeight(646);	//shell�߶� - shellͷ�ߣ�talkwindow titleWidget��
	//ui.widget->setBaseSize(280, 355);//���ó�ʼ���
	int nEmployeeNum = 15;
	QString qsGroupName = QString::fromLocal8Bit("����Ⱥ %1/%2").arg(0).arg(nEmployeeNum);
	pItemName->setText(qsGroupName);


	//�������ڵ�
	ui.treeWidget->addTopLevelItem(pRootItem);
	ui.treeWidget->setItemWidget(pRootItem, 0, pItemName);

	//չ��
	pRootItem->setExpanded(true);

	for (int i = 0; i < nEmployeeNum; ++i)
	{
		addPeopInfo(pRootItem);
	}
}

void TalkWindow::initMarketTalk()
{
	QTreeWidgetItem* pRootItem = new QTreeWidgetItem();
	pRootItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);

	pRootItem->setData(0, Qt::UserRole, 0);
	RootContactItem* pItemName = new RootContactItem(false, ui.treeWidget);

	ui.treeWidget->setFixedHeight(646);	//shell�߶� - shellͷ�ߣ�talkwindow titleWidget��
	//ui.widget->setBaseSize(280, 355);//���ó�ʼ���
	int nEmployeeNum = 8;
	QString qsGroupName = QString::fromLocal8Bit("�г�Ⱥ %1/%2").arg(0).arg(nEmployeeNum);
	pItemName->setText(qsGroupName);


	//�������ڵ�
	ui.treeWidget->addTopLevelItem(pRootItem);
	ui.treeWidget->setItemWidget(pRootItem, 0, pItemName);

	//չ��
	pRootItem->setExpanded(true);

	for (int i = 0; i < nEmployeeNum; ++i)
	{
		addPeopInfo(pRootItem);
	}
}
*/
void TalkWindow::initPtoPTalk()
{
	QPixmap pixSkin;
	pixSkin.load(":/Resources/MainWindow/skin.png");

	ui.widget->setFixedSize(pixSkin.size());

	QLabel* skinLabel = new QLabel(ui.widget);
	skinLabel->setPixmap(pixSkin);
	skinLabel->setFixedSize(ui.widget->size());
}

void TalkWindow::addPeopInfo(QTreeWidgetItem* pRootGroupItem, int employeeID)
{
	QTreeWidgetItem* pChild = new QTreeWidgetItem();
	


	//����ӽڵ�
	pChild->setData(0, Qt::UserRole, 1);
	pChild->setData(0, Qt::UserRole + 1, employeeID);

	ContactItem* pContactItem = new ContactItem(ui.treeWidget);

	//��ȡ����ǩ����ͷ��
	QString strName, strSign, strPicturePath;
	QSqlQueryModel queryInfoModel;
	queryInfoModel.setQuery(QString(
		"SELECT employee_name, employee_sign, picture FROM tab_employees WHERE employeeID = %1"
	).arg(employeeID));

	QModelIndex nameIndex, signIndex, pictureIndex;
	nameIndex = queryInfoModel.index(0, 0);
	signIndex = queryInfoModel.index(0, 1);
	pictureIndex = queryInfoModel.index(0, 2);

	strName = queryInfoModel.data(nameIndex).toString();
	strSign = queryInfoModel.data(signIndex).toString();
	strPicturePath = queryInfoModel.data(pictureIndex).toString();

	QPixmap pix;
	pix.load(":/Resources/MainWindow/head_mask.png");
	QImage imageHead(strPicturePath);
	//QImage imageHead;
	//imageHead.load(strPicturePath);

	pContactItem->setHeadPixmap(CommonUtils::getRoundImage(QPixmap::fromImage(imageHead), pix, pContactItem->getHeadLabelSize()));
	pContactItem->setUserName(strName);
	pContactItem->setSignName(strSign);

	pRootGroupItem->addChild(pChild);
	ui.treeWidget->setItemWidget(pChild, 0, pContactItem);

	QString str = pContactItem->getUserName();
	m_groupPeopleMap.insert(pChild, str);

}

