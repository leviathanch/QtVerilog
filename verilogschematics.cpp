#include "verilogschematics.h"
#include <QPainter>

VerilogSchematics::VerilogSchematics(QWidget *parent) : QWidget(parent)
{
}

void VerilogSchematics::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.drawLine(0, 0, 10, 10);
}
