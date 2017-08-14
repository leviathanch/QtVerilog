#ifndef VERILOGPARSER_H
#define VERILOGPARSER_H

#include <QObject>
#include "verilog_ast.hh"
#include "verilog.tab.hh"
#include "verilog.yy.hh"

class AstTree : public yyFlexLexer
{
private:
	verilog_source_tree *yy_verilog_source_tree;
public:
	AstTree();
	void add_library(ast_list *t);
	void add_config(ast_config_declaration *t);
	void add_source(ast_list *t);
	int yylex();
};

#endif // VERILOGPARSER_H
