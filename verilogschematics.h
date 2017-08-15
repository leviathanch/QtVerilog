#ifndef VERILOGSCHEMATICS_H
#define VERILOGSCHEMATICS_H

#include <QWidget>
#include "verilogcode.h"

class VerilogSchematics : public QWidget
{
	Q_OBJECT
private:
	yy::VerilogCode *code;
public:
	explicit VerilogSchematics(QWidget *parent = nullptr);
signals:

public slots:

protected:
	void paintEvent(QPaintEvent *event);
};

#endif // VERILOGSCHEMATICS_H
