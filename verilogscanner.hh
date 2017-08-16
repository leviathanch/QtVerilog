#ifndef VERILOGSSANNER_HH
#define VERILOGSSANNER_HH

#ifndef yyFlexLexer
#include <FlexLexer.h>
#endif

#include "verilog.yy.hh"

namespace yy {
class VerilogScanner : public yyFlexLexer
{
public:
	VerilogScanner(std::istream* arg_yyin = 0, std::ostream* arg_yyout = 0);
	VerilogScanner();
	int lex(YYSTYPE*,yy::VerilogParser::location_type*,YYLTYPE*,yy::AstTree*);
	int yylex(yy::VerilogParser::semantic_type*, yy::VerilogParser::location_type*, yy::VerilogCode*&, yy::AstTree*&);
	int yylex();
	YYSTYPE* yyget_lval();
};
}

#endif // VERILOGSSANNER_HH
