#include "WindowManager.h"
#include "TalkWindow.h"
#include "TalkWindowItem.h"
#include <QSqlQueryModel>

//����ģʽ������ȫ�־�̬����
Q_GLOBAL_STATIC(WindowManager, theInstance);



WindowManager::WindowManager()
	:QObject(nullptr)
	,m_talkwindowshell(nullptr)
{

}

WindowManager::~WindowManager()
{
}

QWidget* WindowManager::findWindowName(const QString& qsWindowName)
{
	if (m_windowMap.contains(qsWindowName))
	{
		return m_windowMap.value(qsWindowName);
	}
	return nullptr;
}

void WindowManager::deleteWindowName(const QString& qsWindowName)
{
	m_windowMap.remove(qsWindowName);
}

void WindowManager::addWindowName(const QString& qsWindowName, QWidget* qWidget)
{
	if (!m_windowMap.contains(qsWindowName))
	{
		m_windowMap.insert(qsWindowName, qWidget);
	}
}

WindowManager* WindowManager::getInstance()
{
	return theInstance;
}

void WindowManager::addNewTalkWindow(const QString& uid/*, GroupType groupType, const QString& strPepole*/)
{
	if (m_talkwindowshell == nullptr)
	{
		m_talkwindowshell = new TalkWindowShell;
		connect(m_talkwindowshell, &TalkWindowShell::destroyed, [this](QObject* obj) {
			m_talkwindowshell = nullptr;
			});
	}

	QWidget* widget = findWindowName(uid);
	if (!widget)
	{
		m_strCreatingTalkId = uid;
		TalkWindow* talkwindow = new TalkWindow(m_talkwindowshell, uid/*, groupType*/);
		TalkWindowItem* talkwindowItem = new TalkWindowItem(talkwindow);

		m_strCreatingTalkId = "";

		//�ж���Ⱥ�Ļ��ǵ���
		QSqlQueryModel sqlDepModel;
		QString strSql = QString("SELECT department_name, sign FROM tab_department WHERE departmentID = %1").arg(uid);
		sqlDepModel.setQuery(strSql);
		int rows = sqlDepModel.rowCount();

		//����
		if (rows == 0)
		{
			QString sql = QString("SELECT employee_name, employee_sign FROM tab_employees WHERE employeeID = %1").arg(uid);
			sqlDepModel.setQuery(sql);
		}
		
		QModelIndex nameIndex, signIndex;
		nameIndex = sqlDepModel.index(0, 0);	//0��0��
		signIndex = sqlDepModel.index(0, 1);		//0��1��
		QString strWindowName = sqlDepModel.data(signIndex).toString();
		QString strMsgLabel = sqlDepModel.data(nameIndex).toString();

		talkwindow->setWindowName(strWindowName);			//��������
		talkwindowItem->setMsgLabelContent(strMsgLabel);	//�����ϵ���ı���ʾ
		m_talkwindowshell->addTalkWindow(talkwindow, talkwindowItem, uid);


		/*
		switch (groupType) {
		case COMPANY:
		{
			talkwindow->setWindowName(QStringLiteral("ţţ�Ƽ�-Խ����Խӵ��"));
			talkwindowitem->setMsgLabelContent(QStringLiteral("��˾Ⱥ"));
			break;
		}
		case PERSONELGROUP:
		{
			talkwindow->setWindowName(QStringLiteral("���ġ����ġ����ģ���������"));
			talkwindowitem->setMsgLabelContent(QStringLiteral("����Ⱥ"));
			break;
		}
		case DEVELOPMENTGROUP:
		{
			talkwindow->setWindowName(QStringLiteral("ֻ�����ֱ�����ԣ�һ�������찤��ģ�һ����û���õ�"));
			talkwindowitem->setMsgLabelContent(QStringLiteral("�з�Ⱥ"));
			break;
		}
		case MARKETGROUP:
		{
			talkwindow->setWindowName(QStringLiteral("���칤����Ŭ��������Ŭ���ҹ���"));
			talkwindowitem->setMsgLabelContent(QStringLiteral("�г�Ⱥ"));
			break;
		}
		case PTOP:
		{
			talkwindow->setWindowName(QStringLiteral(""));
			talkwindowitem->setMsgLabelContent(strPepole);
			break;
		}
		default:
			break;
		}

		m_talkwindowshell->addTalkWindow(talkwindow, talkwindowitem, groupType);
		*/
	}
	else
	{
		//��������б���Ϊ��ѡ��
		QListWidgetItem* item = m_talkwindowshell->getTalkWindowItemMap().key(widget);
		item->setSelected(true);
		
		//�����Ҳ൱ǰ���촰��
		m_talkwindowshell->setCurrentWidget(widget);
	}
	m_talkwindowshell->show();
	m_talkwindowshell->activateWindow();
}

TalkWindowShell* WindowManager::getTalkWindowShell()
{
	return m_talkwindowshell;
}

QString WindowManager::getCreatingTalkId()
{
	return m_strCreatingTalkId;
}
