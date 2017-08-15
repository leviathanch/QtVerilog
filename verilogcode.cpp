#include <istream>
#include <string>

#include "verilogcode.h"
#include "verilog.y.hh"
#include "verilogscanner.h"

namespace yy {
	VerilogCode::VerilogCode()
	{
		ast = new AstTree();
	}

	bool VerilogCode::parse_stream(std::istream& in, const std::string& sname)
	{
		streamname = sname;

		yy::VerilogScanner *scanner = new yy::VerilogScanner(&in);
		scanner->set_debug(trace_scanning);
		this->lexer = scanner;

		//yy::VerilogParser parser(*this);
		yy::VerilogParser *parser = new yy::VerilogParser(this, ast);
		parser->parse();

		//parser.set_debug_level(trace_parsing);
		//return (parser.parse() == 0);
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

	void VerilogCode::error(const class location& l, const std::string& m)
	{
	}

	void VerilogCode::error(const std::string& m)
	{
		std::cerr << m << std::endl;
	}
}
