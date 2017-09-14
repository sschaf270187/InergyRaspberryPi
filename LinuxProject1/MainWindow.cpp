#include "MainWindow.h"
#include "ui_MainWindow.h"
//#include "qlcdnumber.h"
#include <QMessageBox>
#include "mcp3008spi.h"
#include <QSharedPointer>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	
//	dataTimer2.start(2000); // Interval 0 means to refresh as fast as possible
//	connect(&dataTimer2, SIGNAL(timeout()), this, SLOT(read_dht_data()));

	
//delete the files where data is stored on the pi
//REMOVE THESE LINES BEFORE THE FINAL PROGRAM IS COMPLETED


	FILE *e = fopen("/root/Desktop/temp1FData.txt", "w");
//	fprintf(e, "Contents Deleted\n");
	fclose(e);
	
	FILE *i = fopen("/root/Desktop/temp1CData.txt", "w");
	//	fprintf(i, "Contents Deleted\n");
	fclose(i);
	
	FILE *j = fopen("/root/Desktop/temp2FData.txt", "w");
	//	fprintf(j, "Contents Deleted\n");
	fclose(j);
	
	FILE *k = fopen("/root/Desktop/temp2CData.txt", "w");
	//	fprintf(k, "Contents Deleted\n");
	fclose(k);
	
	FILE *l = fopen("/root/Desktop/tempFBatteryData.txt", "w");
//	fprintf(l, "Contents Deleted\n");
	fclose(l);
	
	FILE *m = fopen("/root/Desktop/tempCBatteryData.txt", "w");
	//	fprintf(m, "Contents Deleted\n");
	fclose(m);
	
	FILE *n = fopen("/root/Desktop/potData.txt", "w");
//	fprintf(n, "Contents Deleted\n");
	fclose(n);
	
	FILE *g = fopen("/root/Desktop/pot2Data.txt", "w");
	fprintf(g, "Contents Deleted\n");
	fclose(g);
	
	FILE *h = fopen("/root/Desktop/humidityData.txt", "w");
	fprintf(h, "Contents Deleted\n");
	fclose(h);
	//***/

	
	graphDataFromPi("lifetime", "potInput", "/root/Desktop/potData.txt");

//The following are used to link the widgets to the graphing functions:
	//setupGraphTest(ui->widget);
	//setupGraphTest(ui->widget_2);
	
//Graphing the Real-Time Voltage Input
//	dataTimer = new QTimer(this);
	realTimeGraphSetup(ui->widget, 0);	
//	realTimeGraphSetup(ui->widget_2, 1);	
	//realTimeGraphSetup(ui->widget_2);
	
//LCD Setup Stuff
	LCDSetup(1);	//LCD Options: 1=controller1, 2=controller2, 3=battery
	LCDSetup(2);
	LCDSetup(3);
	LCDSetup(4);
	
/*********************************
	//GPIO LED Blink Test Stuff
	timer = new QTimer(this); // instantiate timer....make timer child of the QtGpio class
	rpiGpio = new mmapGpio(); // instantiate mmapGpio object
	rpiGpio->setPinDir(4, mmapGpio::OUTPUT); // set pin direction for GPIO4 & 17
	rpiGpio->setPinDir(17, mmapGpio::INPUT);
	connect(timer, SIGNAL(timeout()), this, SLOT(readInputPin())); //connect timer's timeout() signal to the readInputPin() slot method
	connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(btnON()));
	connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(btnOFF()));
	timer->start(250);
***********************************/
	
//The following few lines are used to initialize the signals and slots of the interface, 
// by calling the item names, and the signals and slot names used.
	connect(ui->radioButton_19, SIGNAL(clicked()), this, SLOT(RadioButton19Clicked()));
	connect(ui->radioButton_20, SIGNAL(clicked()), this, SLOT(RadioButton20Clicked()));	
	connect(ui->radioButton_21, SIGNAL(clicked()), this, SLOT(RadioButton21Clicked()));
	connect(ui->radioButton_22, SIGNAL(clicked()), this, SLOT(RadioButton22Clicked()));
	connect(ui->radioButton_23, SIGNAL(clicked()), this, SLOT(RadioButton23Clicked()));
	connect(ui->radioButton_24, SIGNAL(clicked()), this, SLOT(RadioButton24Clicked()));
	connect(ui->pushButton_11, SIGNAL(clicked()), this, SLOT(PushButton11Clicked()));
	connect(ui->pushButton_12, SIGNAL(clicked()), this, SLOT(PushButton12Clicked()));
	connect(ui->pushButton_14, SIGNAL(clicked()), this, SLOT(PushButton14Clicked()));
	connect(ui->pushButton_15, SIGNAL(clicked()), this, SLOT(PushButton15Clicked()));
	connect(ui->pushButton_16, SIGNAL(clicked()), this, SLOT(PushButton16Clicked()));
	connect(ui->pushButton_17, SIGNAL(clicked()), this, SLOT(PushButton17Clicked()));

}

MainWindow::~MainWindow()
{
    delete ui;
	delete rpiGpio;
}

//Original test example:
/*
void MainWindow::ButtonClicked()
{
	num2 += 10;
	//Testing stuff
    QMessageBox msgBox;
    msgBox.setText("Hello, World!"); 
    msgBox.setWindowTitle("VisualGDB Qt Demo");
    msgBox.exec();
	ui->lcdNumber->display(num2);
}
*/
 
void MainWindow::read_dht_data()
{
	uint8_t laststate	= HIGH;
	uint8_t counter		= 0;
	uint8_t j			= 0, i;
 
	data[0] = data[1] = data[2] = data[3] = data[4] = 0;
 
	/* pull pin down for 18 milliseconds */
	pinMode(DHT_PIN, OUTPUT);
	digitalWrite(DHT_PIN, LOW);
	delay(18);
 
	/* prepare to read the pin */
	pinMode(DHT_PIN, INPUT);
 
	/* detect change and read data */
	for (i = 0; i < MAX_TIMINGS; i++)
	{
		counter = 0;
		while (digitalRead(DHT_PIN) == laststate)
		{
			counter++;
			delayMicroseconds(1);
			if (counter == 255)
			{
				break;
			}
		}
		laststate = digitalRead(DHT_PIN);
 
		if (counter == 255)
			break; 
 
		/* ignore first 3 transitions */
		if ((i >= 4) && (i % 2 == 0))
		{
			/* shove each bit into the storage bytes */
			data[j / 8] <<= 1;
			if (counter > 16)
				data[j / 8] |= 1;
			j++;
		}
	}
 
	/*
	 * check we read 40 bits (8bit x 5 ) + verify checksum in the last byte
	 * print it out if data is good
	 */
	if ((j >= 40) &&
	     (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF)))
	{
		h = (float)((data[0] << 8) + data[1]) / 10;
		if (h > 100)
		{
			h = data[0];	// for DHT11
		}
		cel = (float)(((data[2] & 0x7F) << 8) + data[3]) / 10;
		if (cel > 125)
		{
			cel = data[2];	// for DHT11
		}
		if (data[2] & 0x80)
		{
			cel = -cel;
		}
		far = cel * conv + 32;
//		printf("Humidity = %.1f %% Temperature = %.1f *C (%.1f *F)\n", h, c, f);
	}
	else {
//		printf("Data not good, skip\n");
		ui->lcdNumber_7->display(111);
//		exit(1);
	}
}


void MainWindow::LCDSetup(int options)
{
	switch (options)
	{
	case 1:
		connect(&dataTimer1, SIGNAL(timeout()), this, SLOT(LCDControllerData1()));
		dataTimer1.start(1000); // Interval 0 means to refresh as fast as possible
		break;
	case 2:
		connect(&dataTimer1, SIGNAL(timeout()), this, SLOT(LCDControllerData2()));
		dataTimer1.start(1000); // Interval 0 means to refresh as fast as possible
		break;
	case 3:
		connect(&dataTimer1, SIGNAL(timeout()), this, SLOT(LCDBatteryData()));
		dataTimer1.start(1000); // Interval 0 means to refresh as fast as possible
		break;
	case 4:
		connect(&dataTimer1, SIGNAL(timeout()), this, SLOT(LCDHumidity()));
		dataTimer1.start(1000); // Interval 0 means to refresh as fast as possible
		break;

	default:
		connect(&dataTimer1, SIGNAL(timeout()), this, SLOT(LCDControllerData1()));
		dataTimer1.start(1000); // Interval 0 means to refresh as fast as possible
		break;
	}
}


void MainWindow::LCDHumidity()
{
	char *file = "/root/Desktop/humidityData.txt";
	writeDataToPi(file, h);
	ui->lcdNumber_7->display(h);
}

void MainWindow::LCDControllerData1()
{

//	float valC, valF;
	float val = readSPI(1, 1);
//	valC = val;
//	valF = val;
	
	val = (100*(val) - 50);

	char *file = "/root/Desktop/temp1CData.txt";
	writeDataToPi(file, val);
	if (cController1 == 1)
	{
		ui->lcdNumber->display(val);
	}

//	valF = (100*(valF) - 50);
	val = ((val)*(conv) + 32);

	char *file1 = "/root/Desktop/temp1FData.txt";
	writeDataToPi(file1, val);
	if(cController1 == 0)
	{
		ui->lcdNumber->display(val);
	}
}

void MainWindow::LCDControllerData2()
{

	float val = readSPI(1, 2);

	val = (100*(val) - 50);
	char *file = "/root/Desktop/temp2CData.txt";
	writeDataToPi(file, val);

	if(cController2 == 1)
	{
		ui->lcdNumber_2->display(val);
	}
	
	val = ((val)*(conv) + 32);
	char *file1 = "/root/Desktop/temp2FData.txt";
	writeDataToPi(file1, val);

	if(cController2 == 0)
	{
		ui->lcdNumber_2->display(val);
	}
	
	
/*****	
	if (cController2 == 1)
	{
//		count2++;
		float val = readSPI(1, 2);
		val = (100*(val) - 50);
//		val = (val + prevVal1) / count2;
		ui->lcdNumber_2->display(val);
//		prevVal1 = val;
//		if (count2 <= 15)
//		{
//			count2 = 1;
//		}
	}
	else
	{
//		count2f++;
		float val = readSPI(1, 2);
		val = (100*(val) - 50);
		val = ((val)*(conv) + 32);
//		val = (val + prevVal2f) / count2f;
		ui->lcdNumber_2->display(val);
//		prevVal2f = val;
//		if (count2f <= 15)
//		{
//			count2f = 1;
//		}
	}
*****/
}

void MainWindow::LCDBatteryData()
{
	float val = readSPI(1, 3);
	
	val = (100*(val) - 50);
	char *file = "/root/Desktop/tempCBatteryData.txt";
	writeDataToPi(file, val);

	if (cBattery == 1)
	{
		ui->lcdNumber_9->display(val);
	}
	
	val = ((val)*(conv) + 32);
	char *file1 = "/root/Desktop/tempFBatteryData.txt";
	writeDataToPi(file1, val);

	if (cBattery == 0)
	{
		ui->lcdNumber_9->display(val);
	}
	
	/****
	if (cBattery == 1)
	{
		count3++;
		float val = readSPI(1, 3);
		val = (100*(val) - 50);
		val = (val + prevVal3)/count3;
		ui->lcdNumber_9->display(val);
		prevVal3 = val;
		if (count3 <= 15)
		{
			count3 = 1;
		}
	}
	else
	{
		count3f++;
		float val = readSPI(1, 3);
		val = (100*(val) - 50) ;
		val = ((val)*(conv) + 32);
		val = (val + prevVal3f) / count3f;
		ui->lcdNumber_9->display(val);
		prevVal3f = val;
		if (count3f <= 15)
		{
			count3f = 1;
		}
	}
	****/
}
//Controller #1 C
void MainWindow::RadioButton19Clicked()
{
	cController1 = 1;
}

//Controller #1 F
void MainWindow::RadioButton20Clicked()
{

	cController1 = 0;
	//ui->lcdNumber_2->display(f);
	
}

//Controller #2 C
void MainWindow::RadioButton21Clicked()
{
	cController2 = 1;
	//ui->lcdNumber_2->display(c);
	
}

//Controller #2 F
void MainWindow::RadioButton22Clicked()
{
	cController2 = 0;
}

//Battery C
void MainWindow::RadioButton23Clicked()
{
	cBattery = 1;
}

//Battery F
void MainWindow::RadioButton24Clicked()
{
	cBattery = 0;
}

/***************************
//Graph Display TEST
void MainWindow::setupGraphTest(QCustomPlot *customPlot)
{
	
	QVector<double> x(101), y(101); // initialize with entries 0..100
	for (int i = 0; i < 101; ++i)
	{
		x[i] = i / 50.0 - 1; // x goes from -1 to 1
		y[i] = x[i]*x[i]; // let's plot a quadratic function
	}
	// create graph and assign data to it:
	customPlot->addGraph();
	customPlot->graph(0)->setData(x, y);
	// give the axes some labels:
	customPlot->xAxis->setLabel("x");
	customPlot->yAxis->setLabel("y");
	// set axes ranges, so we see all data:
	customPlot->xAxis->setRange(-1, 1);
	customPlot->yAxis->setRange(0, 1);
	customPlot->replot();
}
*****************************/

//Store the data
void MainWindow::writeDataToPi(char *filename, float value)
{

	FILE *f = fopen(filename, "a");
	if (f == NULL)
	{
		printf("Error opening file!\n");
		exit(1);
	}

	fprintf(f, "%f\t", value); //write data value to file(voltage, temp, power, etc.)
	int days = 0;
	int hr = 0;
	int min = 0;
	int sec = 0;
	QTime curtime(QTime::currentTime());
	QDate curdate(QDate::currentDate());
	
	days = curdate.dayOfYear();

	hr = curtime.hour();

	min = curtime.minute();

	sec = curtime.second();
	
	fprintf(f, "%i   %i   %i   %i\n", days, hr, min, sec); //write the time to the file in the format- Day Hour Minute Second

	fclose(f);
}

//Read the stored data
void MainWindow::graphDataFromPi(string timePeriod, string dataType, const char* filename)
{
	long start = 0;
	long end = 0;
	unsigned long timeLimit = 604800;
	float currentTime = 0;
	double currentTimeLong = 0;
	float value = 0;
	int day = 0;
	int hour = 0;
	int minute = 0;
	int second = 0;
	float dayFraction = 0;
	float hourFraction = 0;
	float minuteFraction = 0;
	float secondFraction = 0;
	int time = 0; //time indicator, 0 = one day, 1 = one week, 2 = one month, 3 = total lifetime
	float totalTime = 0; //the amount of time passed, expressed as a fraction, where 1 = one year
	unsigned long totalTimeLong = 0;
	i = 0;
	float dayF = 365;
	float hourF = 8760;
	float minuteF = 525600;
	float secondF = 31536000;
//	double a[60000] = { 0 }, b[60000] = { 0 }, c[60000] = { 0 }, d[60000] = { 0 }, e[60000] = { 0 };// f[60000] = { 0 }; // secTime[60000] = { 0 };
	double *secTime = (double*) malloc(200000000); //200000000 = 6.34 years worth of data
	double *f = (double*) malloc(200000000);
	double *a = (double*) malloc(200000000);
	double *b = (double*) malloc(200000000);
	double *c = (double*) malloc(200000000);
	double *d = (double*) malloc(200000000);
	double *e = (double*) malloc(200000000);

	
	ifstream infile;
	infile.open(filename);//open the text file
	if (!infile) {
		cout << "Unable to open file";
		exit(1); // terminate with error
	}


	while (!infile.eof())
	{
	//To make five arrays for each column (a for 1st column, b for 2nd....)
		infile >> value >> day >> hour >> minute >> second;
//		infile >> value;
		if (dataType == "potInput")
		{
			value = value * 1000;	
		}
		a[i] = value;
		b[i] = day;
		c[i] = hour;
		d[i] = minute;
		e[i] = second;
		f[i] = i;
		
		currentTime = (day / dayF) + (hour / hourF) + (minute / minuteF) + (second / secondF);
		currentTimeLong = currentTime * 31536000;
		secTime[i] = currentTimeLong;
		i++;
	}  
	i--;
	dayFraction = b[i] - b[0];
	hourFraction = c[i] - c[0];
	minuteFraction = d[i] - d[0];
	secondFraction = e[i] - e[0];
	

	totalTime = (dayFraction / dayF) + (hourFraction / hourF) + (minuteFraction / minuteF) + (secondFraction / secondF);
//	totalTime = (dayFraction / 365) + (hourFraction / 8395) + (minuteFraction / 495305) + (secondFraction / 29718300);
	totalTimeLong = totalTime * 31536000;
	// one week = 604800
	// one month = 2592000
	// one year = 31536000
	
	if (timePeriod == "week")
	{
		timeLimit = 604800;
	}
	else if (timePeriod == "month")
	{
		timeLimit = 2592000;
	}
	else if (timePeriod == "year")
	{
		timeLimit = 31536000;
	}
	else if (timePeriod == "lifetime")
	{
		timeLimit = secTime[i] - secTime[0];
	}
	
	free(b);
	free(c);
	free(d);
	free(e);
	
	start = secTime[i] - timeLimit;
	end = secTime[i];
//	unsigned long timeLim[timeLimit] = { 0 }, actualVals[timeLimit] = { 0 };	
	double *timeLim = (double*) malloc(200000000);
	double *actualVals = (double*) malloc(200000000);

	
	for (long zer = 0; zer < timeLimit; zer++) // set all elements in the array to zero
	{
		actualVals[zer] = 0;
	}


	long k = 0;
	long adjTime = 0;
	while (start <= end)
	{
		adjTime = start;
		while (adjTime < 0)
		{
			adjTime = adjTime + 31536000;
		}
		timeLim[k] = adjTime;
		k++;
		start++;
	}

	
	for (long x = 0; x < timeLimit; x++)
	{
		double val1 = timeLim[x]; //array of currentTime-timeLimit to currentTime (# of seconds for time limit)
		for (long y = 0; y < i; y++)
		{
			double val2 = secTime[y]; //array of seconds at which data was recorded
			if (val2 == val1) //if a time where data was recorded = a needed time, then set actual values = recorded value at that time
			{
				actualVals[x] = a[y];
				a[y] = 0; //this way everytime a value is written over it is essentially deleted,
						  // meaning that the repeating seconds(in different years) will be encountered sequentially
			}
		}
	} //now timeLim holds all the times, and actualVals holds all the values, adjusted for time

	
	if ((timePeriod == "lifetime") && (dataType == "potInput"))
	{
		graphPoints(ui->widget_2, actualVals, timeLim, timeLimit, totalTime, "lifetime(in seconds)", "mV");

	}
	else 
		if ((timePeriod == "month") && (dataType == "potInput"))
	{
		graphPoints(ui->widget_2, actualVals, timeLim, timeLimit, totalTime, "month(in seconds)", "mV");

	}
	else 
		if ((timePeriod == "lifetime") && (dataType == "tempurature(F)"))
	{
		graphPoints(ui->widget_2, actualVals, timeLim, timeLimit, totalTime, "lifetime(in seconds)", "tempurature(F)");

	}
	else
		if ((timePeriod == "lifetime") && (dataType == "tempurature(C)"))
	{
		graphPoints(ui->widget_2, actualVals, timeLim, timeLimit, totalTime, "lifetime(in seconds)", "tempurature(C)");

	}
	
	free(secTime);
	free(a);

	free(f);
	free(timeLim);
	free(actualVals);
	infile.close();
}

void MainWindow::graphPoints(QCustomPlot *customPlot, double yVal[], double xVal[], long size, unsigned long time, string xLabel, string yLabel)
{
	customPlot->addGraph();
	customPlot->graph(0)->data()->clear();
	customPlot->graph()->setLineStyle((QCPGraph::LineStyle)0);
	customPlot->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));

//	double *x1 = (double*) malloc(200000000);
//	double *y1 = (double*) malloc(200000000);
	// generate some data:
//	QVector<double> x1(size), y1(size); // initialize with entries 0..size
	for (long x = 0; x < size; x++)
	{
		customPlot->graph(0)->addData(xVal[x], yVal[x]);
	
		
		if (size >= 500)
		{
			x = x + 9;
		}
		else if (size >= 1000)
		{
			x = x + 19;
		}
		else if (size >= 10000) // 2.77 hours
		{
			x = x + 49; // every 50 seconds
		}
		else if (size >= 100000) // 27.7 hours (1.2 days)
		{
			x = x + 119; // every 120 seconds (2 minutes)
		}
		else if (size >= 1000000) //277 hours (11.6 days)
		{
			x = x + 599; // every 600 seconds (5 minutes)
		}
		else if (size >= 10000000) // 2770 hours (115.7 days)
		{
			x = x + 1799; // every 1800 seconds (30 mins)
		}
//		customPlot->replot();
	}
	
//	if (xLabel == "lifetime(in seconds)")
//	{
		//nothing
//	}
//	else if (xLabel == "month(in days)")
//	{
		// one month = 2592000
//		time -  ;
//	}
	
	// create graph and assign data to it:

	double lower = xVal[1]; //18403200
	double upper = xVal[size - 1]; //18576000 
	
	// give the axes some labels:
	if (xLabel == "lifetime(in seconds)")
	{
		customPlot->xAxis->setLabel("lifetime(in seconds)");
//		customPlot->xAxis->setRange(0, size);
		customPlot->xAxis->setRange(lower, upper);
	}
	else
		if (xLabel == "month(in days)")
	{
		customPlot->xAxis->setLabel("month(in days)");
		customPlot->xAxis->setRange(lower, upper);
	}
	
	
	if (yLabel == "mV")
	{
		customPlot->yAxis->setLabel("mV");
		customPlot->yAxis->setRange(0, 5000);
	}
	else
		if (yLabel == "tempurature(F)")
	{
		customPlot->yAxis->setLabel("Degrees F");
		customPlot->yAxis->setRange(50, 110);
	}
	else
		if (yLabel == "tempurature(C)")
	{
		customPlot->yAxis->setLabel("Degrees C");
		customPlot->yAxis->setRange(0, 50);
	}
//	customPlot->xAxis->setLabel("lifetime(in minutes)");
//	customPlot->yAxis->setLabel("mV");
	// set axes ranges, so we see all data:
//	customPlot->xAxis->setRange(0, xRange);
//	customPlot->yAxis->setRange(0, 5000);
	customPlot->replot();
//	free(x1);
//	free(y1);
}
	

//Read the SPI data
float MainWindow::readSPI(int spiNum, int channel)
{
	if (spiNum == 1)
	{
		mcp3008Spi a2d("/dev/spidev0.1", SPI_MODE_1, 1000, 8); 
		
		int a2dVal = 0;
		int a2dChannel = channel;
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
		return val;
	}
	else
	{
		mcp3008Spi a2d("/dev/spidev0.0", SPI_MODE_0, 1000, 8);

		int a2dVal = 0;
		int a2dChannel = channel;
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
		return val;
	}
}

//Real Time Graph
void MainWindow::realTimeGraphSetup(QCustomPlot *customPlot, int options)
{
	
	customPlot->addGraph(); // blue line
	customPlot->graph(0)->setPen(QPen(QColor(40, 110, 255)));

	customPlot->axisRect()->setupFullAxesBox();
	customPlot->yAxis->setRange(-0.2, 4.2);
  
	// make left and bottom axes transfer their ranges to right and top axes:
	connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
	connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));
  

	// setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
	switch (options)
	{
	
	case 0 : 
		connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot0()));
		dataTimer.start(0); // Interval 0 means to refresh as fast as possible
		break;
	case 1 : 
		connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot1()));
		dataTimer.start(0); // Interval 0 means to refresh as fast as possible
		break;
	default:
		connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot0()));
		dataTimer.start(0); // Interval 0 means to refresh as fast as possible
	}

//	connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot1()));
//	dataTimer.start(0); // Interval 0 means to refresh as fast as possible

}

void MainWindow::realtimeDataSlot0()
{
	//int secCounter1 = 0;
	int division = 0;
	float val = readSPI(0, 0);

	//Graph the calculated data "val"
	static QTime time(QTime::currentTime());
	// calculate two new data points:
	double key = time.elapsed() / 1000.0; // time elapsed since start of demo, in seconds
	static double lastPointKey = 0;
	if (key - lastPointKey > 0.2) // at most add point every 20 ms
	{
		secCounter1++;
		division = secCounter1 % 5;
		if (division == 0)
		{
			char *file = "/root/Desktop/potData.txt";
			writeDataToPi(file, val);
		}
	  // add data to lines:
		ui->widget->graph(0)->addData(key, val);
//		ui->widget->graph(1)->addData(key, val2);
		//ui->widget->graph(1)->addData(key, qCos(key) + qrand() / (double)RAND_MAX * 0.5*qSin(key / 0.4364));
		// rescale value (vertical) axis to fit the current data:
		//ui->widget->graph(0)->rescaleValueAxis();
		//ui->widget->graph(1)->rescaleValueAxis(true);
		lastPointKey = key;
	}
	// make key axis range scroll with the data (at a constant range size of 8):
	ui->widget->xAxis->setRange(key, 8, Qt::AlignRight);
	ui->widget->replot();

}

void MainWindow::realtimeDataSlot1()
{
	//int secCounter2 = 0;
	int division = 0;
	float val = readSPI(1, 0);
	
	//Graph the calculated data "val"
	static QTime time(QTime::currentTime());
	// calculate two new data points:
	double key = time.elapsed() / 1000.0; // time elapsed since start of demo, in seconds
	static double lastPointKey = 0;
	if (key - lastPointKey > 0.2) // at most add point every 2 ms
	{
		secCounter2++;
		division = secCounter2 % 5;
		if (division == 0)
		{
			char *file = "/root/Desktop/pot2Data.txt";
			writeDataToPi(file, val);
		}
	  // add data to lines:
		ui->widget_2->graph(0)->addData(key, val);
		lastPointKey = key;
	}
	// make key axis range scroll with the data (at a constant range size of 8):
	ui->widget_2->xAxis->setRange(key, 8, Qt::AlignRight);
	ui->widget_2->replot();

}
	
//Schedule window TEST, buttons 11, 12, 14, 15, 16, 17:
void MainWindow::PushButton11Clicked()
{
	graphDataFromPi("lifetime", "potInput", "/root/Desktop/potData.txt");
	//openNewWindow();
}


void MainWindow::PushButton12Clicked()
{
	graphDataFromPi("lifetime", "tempurature(F)", "/root/Desktop/temp1FData.txt");
	//openNewWindow();
}

void MainWindow::PushButton14Clicked()
{
	graphDataFromPi("lifetime", "tempurature(C)", "/root/Desktop/temp1CData.txt");
//	openNewWindow();
}

void MainWindow::PushButton15Clicked()
{
	graphDataFromPi("week", "tempurature(C)", "/root/Desktop/temp1CData.txt");
//	openNewWindow();
}

void MainWindow::PushButton16Clicked()
{
	openNewWindow();
}

void MainWindow::PushButton17Clicked()
{
	openNewWindow();
}


//Schedule popup window TEST:
void MainWindow::openNewWindow()
{
	myNewWindow = new NewWindow();

	myNewWindow->show();
}

/****************************************
//GPIO Functions
void MainWindow::btnON() {
	//lblBtnState->setText("GPIO 4 ON");
	rpiGpio->writePinHigh(4);
}


void MainWindow::btnOFF()  {
	//lblBtnState->setText("GPIO 4 OFF");
	rpiGpio->writePinLow(4);
}


void MainWindow::readInputPin(void) {
	unsigned int pinVal;
	pinVal = rpiGpio->readPin(17);
//	if (pinVal == mmapGpio::HIGH)
		//lblInputPinState->setText("GPIO17 is HIGH...PushButton not pressed");
//		ui->lcdNumber_9->display(1);
//	else
		//lblInputPinState->setText("GPIO17 is LOW...PushButton pressed");
//		ui->lcdNumber_9->display(0);
	
}
*******************************************/