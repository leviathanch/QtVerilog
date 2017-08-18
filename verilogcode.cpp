#include <string>
#include <fstream>
#include <istream>

#include "verilogscanner.hh"
#include "verilogcode.h"
#include "verilog.yy.hh"

namespace yy {
	class VerilogScanner;
	class VerilogParser;

	VerilogCode::VerilogCode() {
		yy_verilog_source_tree = NULL;
		yy_preproc = NULL;
		verilog_parser_init();
	}

	bool VerilogCode::parse_file(QString filename)
	{
		std::ifstream input;
		std::string stdfilename = filename.toStdString();
		int stat;

		trace_scanning = false;
		trace_parsing = false;

		streamname = filename;
		std::cout << "opening file " << stdfilename << std::endl;
		input.open(stdfilename,std::ios::in);
		std::cout << "opened file" << stdfilename << std::endl;

		lexer = new VerilogScanner(&input,&std::cout);
		lexer->set_debug(trace_scanning);

		parser = new VerilogParser(this);
		parser->set_debug_level(trace_parsing);
		stat=parser->parse();
		input.close();

		return stat;
	}

	void VerilogCode::error(const std::string& m)
	{
		std::cerr << m << std::endl;
	}

	void VerilogCode::add_config(ast_config_declaration *t)
	{
		ast_list_append(yy_verilog_source_tree->configs, t);
	}

	void VerilogCode::add_library(ast_list *t)
	{
		yy_verilog_source_tree->libraries = ast_list_concat(this->yy_verilog_source_tree->libraries, t);
	}

	void VerilogCode::add_source(ast_list *t)
	{
		for(unsigned int i = 0; i < t->items; i ++) {
			ast_source_item* toadd = (ast_source_item*)ast_list_get(t, i);
			if(toadd->type == SOURCE_MODULE) {
				std::cout << "adding module" << std::endl;
				ast_list_append(this->yy_verilog_source_tree->modules, toadd->module);
			} else if (toadd -> type == SOURCE_UDP) {
				ast_list_append(this->yy_verilog_source_tree->primitives, toadd->udp);
			} else {
				// Do nothing / unknown / unsupported type.
				std::cout << "line " << __LINE__ << " of " << __FILE__" - Unknown source item type: " << toadd->type << std::endl;
			}
		}
	}

	void VerilogCode::showData()
	{
		ast_module_declaration * module;
		verilog_source_tree *st = yy_verilog_source_tree;
		std::cout << "Modules: " << st->modules->items << std::endl;
		std::cout << "Primitives: " << st->primitives->items << std::endl;
		std::cout << "Libraries: " << st->libraries->items << std::endl;

		for(ast_list_element * m = st->modules->head; m; m=m->next) {
			module = (ast_module_declaration *)m->data;
			std::cout << module->identifier->identifier << std::endl;
		}
	}
}
