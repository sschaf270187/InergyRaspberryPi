
#ifndef NEWWINDOW_H
#define NEWWINDOW_H

#include <QMainWindow>
#include <QTimer>

namespace Ui{
	class NewWindow;
}

class NewWindow : public QMainWindow
{
	Q_OBJECT
		
	public :
		explicit NewWindow(QWidget *parent = 0);
		~NewWindow();
	
	protected slots :
	void pushButtonClicked();
	
	private :
	Ui::NewWindow *ui;
	int num1 = 11111;
	QTimer *timer; // a timer

};

#endif // NEWWINDOW.H
