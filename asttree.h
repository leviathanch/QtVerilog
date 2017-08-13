#ifndef VERILOGPARSER_H
#define VERILOGPARSER_H

#include <QObject>
#include "verilog_ast.h"

class AstTree
{
private:
	verilog_source_tree *yy_verilog_source_tree;
public:
	AstTree();
	void add_library(ast_list *t);
	void add_config(ast_config_declaration *t);
	void add_source(ast_list *t);
};

#endif // VERILOGPARSER_H
