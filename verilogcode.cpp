#include <istream>
#include <string>

#include "verilogscanner.hh"
#include "verilogcode.h"
#include "verilog.yy.hh"

namespace yy {
	class VerilogScanner;
	class VerilogParser;

	VerilogCode::VerilogCode()
	{
	}

	bool VerilogCode::parse_stream(std::istream& in, const std::string& sname)
	{
		streamname = sname;

		lexer = new VerilogScanner(&in);
		lexer->set_debug(trace_scanning);

		parser = new VerilogParser(this);
		parser->set_debug_level(trace_parsing);

		return parser->parse();
	}

	bool VerilogCode::parse_file(const std::string &filename)
	{
		std::ifstream in(filename.c_str());
		if (!in.good()) return false;
		return parse_stream(in, filename);
	}

	bool VerilogCode::parse_string(const std::string &input, const std::string& sname)
	{
		std::istringstream iss(input);
		return parse_stream(iss, sname);
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
}
