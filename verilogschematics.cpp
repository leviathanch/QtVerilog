#include <QPainter>
#include "verilogschematics.h"
#include <iostream>

VerilogSchematics::VerilogSchematics(QWidget *parent) : QWidget(parent)
{
	code = new yy::VerilogCode();
	if(code->parse_file("../counter.rtl.v")) {
		code->showData();
	}
}

void VerilogSchematics::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.drawLine(0, 0, 10, 10);
}
