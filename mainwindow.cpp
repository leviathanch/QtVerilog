#include "mainwindow.h"
#include "verilogschematics.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	VerilogSchematics *schematics = new VerilogSchematics();
	ui->setupUi(this);
	ui->workbench->addWidget(schematics);
}

MainWindow::~MainWindow()
{
	delete ui;
}
