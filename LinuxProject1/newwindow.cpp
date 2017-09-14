#include "newwindow.h"
#include "ui_ui_newwindow.h"
#include "MainWindow.h"
#include "mcp3008spi.h"

NewWindow::NewWindow(QWidget *parent) :
	QMainWindow(parent), 
	ui(new Ui::NewWindow)
{
	ui->setupUi(this);
	
	timer = new QTimer(this);
	timer->start(250);
	
	connect(timer, SIGNAL(timeout()), this, SLOT(pushButtonClicked()));
}

NewWindow::~NewWindow()
{
	delete ui;
}

void NewWindow::pushButtonClicked()
{
	//ui->lcdNumber->display(num1);

	mcp3008Spi a2d("/dev/spidev0.0", SPI_MODE_0, 1000000, 8);

	int a2dVal = 0;
	int a2dChannel = 0;
	unsigned char data[3];
	float val = 0;
	float val2 = 0;
	float factor = 312;


	data[0] = 1;  //  first byte transmitted -> start bit
	data[1] = 0b10000000 | (((a2dChannel & 7) << 4)); // second byte transmitted -> (SGL/DIF = 1, D2=D1=D0=0)
	data[2] = 0; // third byte transmitted....don't care

	a2d.spiWriteRead(data, sizeof(data));


	a2dVal = 0;
	a2dVal = (data[1] << 8) & 0b1100000000; //merge data[1] & data[2] to get result
	a2dVal |=  (data[2] & 0xff);
	//sleep(1);
	val = (float)a2dVal / factor;

	ui->lcdNumber->display(val);
}