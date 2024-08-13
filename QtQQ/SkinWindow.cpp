#include "SkinWindow.h"
#include "QClickLabel.h"
#include "NotifyManager.h"

SkinWindow::SkinWindow(QWidget *parent)
	: BasicWindow(parent)
{
	ui.setupUi(this);
	loadStyleSheet("SkinWindow");
	setAttribute(Qt::WA_DeleteOnClose);
	this->setAttribute(Qt::WA_QuitOnClose, false);//Qt::WA_QuitOnClose属性的意思是指在最后一个窗体关闭时退出程序，是通过发送lastWindowClosed信号，当发送该信号时，程序会执行quit函数
	initControl();
}

SkinWindow::~SkinWindow()
{}

void SkinWindow::initControl()
{
	QList<QColor> colorList = {
		QColor(22,154,218), QColor(40,138,221),  QColor(49,166,107), QColor(218,67,68),
		QColor(177,99,158), QColor(107,81,92),   QColor(89,92,160),  QColor(21,156,199),
		QColor(79,169,172), QColor(155,183,154), QColor(128,77,77),  QColor(240,188,189)
	};

	for (int row = 0; row < 3; row++)
	{
		for (int column = 0; column < 4; column++)
		{
			QClickLabel* label = new QClickLabel(this);
			label->setCursor(Qt::PointingHandCursor);//设置成手指形状
			//通过lambda表达式进行通告换肤
			connect(label, &QClickLabel::clicked, [row, column, colorList] {
				NotifyManager::getInstance()->notifyOtherWindowChangeSkin(colorList.at(row * 4 + column));
				});
			label->setFixedSize(84, 84);
			
			QPalette palette;
			palette.setColor(QPalette::Background, colorList.at(row * 4 + column));
			label->setAutoFillBackground(true);
			label->setPalette(palette);

			ui.gridLayout->addWidget(label, row, column);

		}

	}

	connect(ui.sysmin, SIGNAL(clicked()), this, SLOT(onShowMin()));
	connect(ui.sysclose, SIGNAL(clicked()), this, SLOT(onButtonCloseClicked()));
}

void SkinWindow::onButtonCloseClicked()
{
	close();
}
