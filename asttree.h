#ifndef VERILOGPARSER_H
#define VERILOGPARSER_H

#include <QObject>
#include "verilog_ast.h"

class AstTree
{
public:
	AstTree();
	void add_library(ast_list *t);
	void add_config(ast_config_declaration *t);
};

#endif // VERILOGPARSER_H
