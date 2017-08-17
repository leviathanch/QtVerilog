#ifndef VERILOGSSANNER_HH
#define VERILOGSSANNER_HH

#ifndef yyFlexLexer
#include <FlexLexer.h>
#endif

#include "verilog_ast.hh"
#include "verilog_preprocessor.hh"
#include "verilog.yy.hh"

namespace yy {
class VerilogScanner : public yyFlexLexer
{
public:
	VerilogScanner(std::istream* arg_yyin = 0, std::ostream* arg_yyout = 0);
	VerilogScanner();
	int yylex(YYSTYPE* yylval, YYLTYPE* yylloc, yy::VerilogCode* code);
};
}

#endif // VERILOGSSANNER_HH
