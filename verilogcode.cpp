#include <string>
#include <fstream>
#include <istream>

#include "verilogscanner.hh"
#include "verilogcode.h"
#include "verilog.yy.hh"

namespace yy {
	class VerilogScanner;
	class VerilogParser;

	VerilogCode::VerilogCode() {}

	bool VerilogCode::parse_file(QString filename)
	{
		bool ret;
		streamname = filename;
		std::filebuf fb;
		if (fb.open (filename.toStdString(),std::ios::in)) {
			std::istream is(&fb);

			lexer = new VerilogScanner(&is);
			lexer->set_debug(trace_scanning);

			parser = new VerilogParser(this);
			parser->set_debug_level(trace_parsing);

			ret=parser->parse();
		}
		std::cout << "done parsing" << std::endl;
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
				ast_list_append(this->yy_verilog_source_tree -> modules, toadd->module);
			} else if (toadd -> type == SOURCE_UDP) {
				ast_list_append(this->yy_verilog_source_tree->primitives, toadd->udp);
			} else {
				// Do nothing / unknown / unsupported type.
				printf("line %d of %s - Unknown source item type: %d",
				__LINE__,
				__FILE__,
				toadd -> type);
			}
		}
	}

	void VerilogCode::showData()
	{
		verilog_source_tree *st = yy_verilog_source_tree;
		ast_list* m = st->modules;
		for(ast_list_element *e=m->head;e;e=e->next) {
		}
	}
}
