#ifndef VERILOGSCHEMATICS_H
#define VERILOGSCHEMATICS_H

#include <QWidget>
#include "asttree.hh"

class VerilogSchematics : public QWidget
{
	Q_OBJECT
private:
	AstTree *ast;
public:
	explicit VerilogSchematics(QWidget *parent = nullptr);

signals:

public slots:

protected:
	void paintEvent(QPaintEvent *event);
};

#endif // VERILOGSCHEMATICS_H
