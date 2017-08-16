#ifndef VERILOGSCANNER_H
#define VERILOGSCANNER_H

#include "verilog.ll.hh"

namespace yy {
	class VerilogScanner : public yyFlexLexer
	{
	public:
		VerilogScanner(std::istream* arg_yyin = 0, std::ostream* arg_yyout = 0);
		VerilogScanner();
		int lex(yy::VerilogParser::semantic_type*,yy::VerilogParser::location_type*,yy::VerilogCode*,yy::AstTree*);
	};
}

#endif // VERILOGSCANNER_H
