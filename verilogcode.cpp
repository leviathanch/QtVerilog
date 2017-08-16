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
		ast = new AstTree();
	}

	bool VerilogCode::parse_stream(std::istream& in, const std::string& sname)
	{
		streamname = sname;

		VerilogScanner *scanner = new VerilogScanner(&in);
		//scanner->set_debug(trace_scanning);
		this->lexer = scanner;

		//yy::VerilogParser parser(*this);
		VerilogParser *parser = new VerilogParser(this, ast);

		//parser.set_debug_level(trace_parsing);
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
}
