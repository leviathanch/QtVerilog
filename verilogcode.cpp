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
		bool ret;
		std::string stdfilename = filename.toStdString();
		trace_scanning = false;
		trace_parsing = false;

		streamname = filename;
		std::filebuf fb;
		std::cout << "opening file " << stdfilename << std::endl;
		if(fb.open(stdfilename,std::ios::in)) {
			std::cout << "opened file" << stdfilename << std::endl;
			std::istream is(&fb);

			lexer = new VerilogScanner(&is,&std::cout);
			lexer->set_debug(trace_scanning);

			parser = new VerilogParser(this);
			parser->set_debug_level(trace_parsing);

			ret=parser->parse();
			fb.close();
			std::cout << "done parsing" << std::endl;
		}
		return ret;
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
		for(ast_list_element * m = st->modules->head; m; m=m->next) {
			module = (ast_module_declaration *)m->data;
			std::cout << module->identifier->identifier << std::endl;
		}
	}
}
