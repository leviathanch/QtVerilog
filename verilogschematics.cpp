#include <QPainter>
#include "verilogschematics.h"

VerilogSchematics::VerilogSchematics(QWidget *parent) : QWidget(parent)
{
	code = new yy::VerilogCode();
}

void VerilogSchematics::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.drawLine(0, 0, 10, 10);
}
