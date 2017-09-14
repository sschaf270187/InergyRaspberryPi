#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"
#include "newwindow.h"
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include "C:\Users\Sammy\Documents\visualstudio2015\Projects\LinuxProject1\LinuxProject1\mmapGpio.h"
#include <QWidget>
#include <stdio.h>
#include <fstream>
#include "wiringPi.h"

using namespace std;

#define MAX_TIMINGS	85
#define DHT_PIN		4	/* GPIO-22 */

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT
    
public :
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
	//explicit QtGpio(QWidget *parent = 0); // QtGpio constructor...must take on a parent parameter that defaults to NULL
	//~QtGpio(); // QtGpio destructor
	
	//void setupGraphTest(QCustomPlot *customPlot);
	void realTimeGraphSetup(QCustomPlot *customPlot, int options);
	void LCDSetup(int options);
	void writeDataToPi(char *filename, float value);
	void graphDataFromPi(string timePeriod, string dataType, const char* filename);
	float readSPI(int spiNum, int channel);
	void graphPoints(QCustomPlot *customPlot, double yVal[], double xVal[], long size, unsigned long time, string xLabel, string yLabel);

	public slots :
	//Schedule Window
	void openNewWindow();

//	void readInputPin(void);

	
	protected slots :
	//    void ButtonClicked();
	void RadioButton19Clicked();
	void RadioButton20Clicked();
	void RadioButton21Clicked();
	void RadioButton22Clicked();
	void RadioButton23Clicked();
	void RadioButton24Clicked();
	void PushButton11Clicked();
	void PushButton12Clicked();
	void PushButton14Clicked();
	void PushButton15Clicked();
	void PushButton16Clicked();
	void PushButton17Clicked();
	//Graph RTD
	void realtimeDataSlot0();
	void realtimeDataSlot1();
	//GPIO Stuff
//	void btnON();
//	void btnOFF();
	//LCD Data
	void LCDHumidity();
	void LCDControllerData1();
	void LCDControllerData2();	
	void LCDBatteryData();	
	void read_dht_data();

	
private:
    Ui::MainWindow *ui;
	NewWindow *myNewWindow;
	
	int cController1 = 0;
	int cController2 = 0;
	int cBattery = 0;
//	int num2 = 0;
	int num3 = 7;
	int num4 = 5;
	//GPIO Stuff
	//QPushButton *onBtn, *offBtn; // We need two Buttons
	//QLabel *lblBtnState, *lblInputPinState; // two labels
	QTimer *timer; // a timer
	QTimer dataTimer;
	QTimer dataTimer1;
	QTimer dataTimer2;
	mmapGpio *rpiGpio; // a reference to the mmapGpio class to control the GPIO
//	int count1 = 0;
//	int count2 = 0;
//	int count3 = 0;
	int count4 = 0;
	float prevVal1 = 0;
	float prevVal2 = 0;
	float prevVal3 = 0;
	float prevVal4 = 0;
//	int count1f = 0;
//	int count2f = 0;
//	int count3f = 0;
	int count4f = 0;
	float prevVal1f = 0;
	float prevVal2f = 0;
	float prevVal3f = 0;
	float prevVal4f = 0;
	const float conv = 1.8;
	unsigned long i;
	int secCounter1 = 0;
	int secCounter2 = 0;
	
	int data[5] = { 0, 0, 0, 0, 0 }; //array for reading the humidity sensor (DHT22)
	float cel, h, far; //used when reading the DHT22
	
//	long zer;
//	double a[60000] = { 0 }, b[60000] = { 0 }, c[60000] = { 0 }, d[60000] = { 0 }, e[60000] = { 0 }, f[60000] = { 0 }, secTime[60000] = { 0 };

};

#endif // MAINWINDOW_H
