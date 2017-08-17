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
	VerilogScanner();
	VerilogScanner(std::ifstream input);
	int yylex(YYSTYPE* yylval, YYLTYPE* yylloc, yy::VerilogCode* code);
};
}

#endif // VERILOGSSANNER_HH
