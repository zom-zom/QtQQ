#include "ReceiveFile.h"
#include <QFileDialog>
#include <QMessageBox>


extern QString gFileName;
extern QString gFileData;

ReceiveFile::ReceiveFile(QWidget *parent)
	: BasicWindow(parent)
{
	ui.setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose);
	initTitleBar();
	setTitleBarTitle("", ":/Resources/MainWindow/qqlogoclassic.png");
	loadStyleSheet("ReceiveFile");

	this->move(100, 400);

	connect(_titleBar, &TitleBar::signalButtonCloseClicked, this, &ReceiveFile::refuseFile);

}

ReceiveFile::~ReceiveFile()
{}

void ReceiveFile::setMsg(QString& msgLabel)
{
	ui.label->setText(msgLabel);
}

void ReceiveFile::on_cancelBtn_clicked()
{
	emit refuseFile();
	this->close();
}

void ReceiveFile::on_okBtn_clicked()
{
	this->close();

	//��ȡ��Ҫ��·��
	QString fileDirPath = QFileDialog::getExistingDirectory(nullptr, QStringLiteral("�ļ�����·��"), "/");

	QString filePath = fileDirPath + "/" + gFileName;
	
	QFile file(filePath);

	if (file.open(QIODevice::WriteOnly))
	{
		file.write(gFileData.toUtf8());
		file.close();
		QMessageBox::information(nullptr, QStringLiteral("��ʾ"), QStringLiteral("�ļ����ճɹ���"));
	}
	else
	{
		QMessageBox::information(nullptr, QStringLiteral("��ʾ"), QStringLiteral("�ļ�����ʧ�ܣ�"));

	}

}
