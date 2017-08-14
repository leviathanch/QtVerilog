#include <QPainter>
#include "verilogschematics.h"
#include "asttree.hh"

VerilogSchematics::VerilogSchematics(QWidget *parent) : QWidget(parent)
{
	ast = new AstTree();
}

void VerilogSchematics::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.drawLine(0, 0, 10, 10);
}
