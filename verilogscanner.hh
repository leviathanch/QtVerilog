#ifndef VERILOGSSANNER_HH
#define VERILOGSSANNER_HH

#ifndef yyFlexLexer
#include <FlexLexer.h>
#endif

#include <cstdio>

#include "verilog_ast.hh"
#include "verilog_preprocessor.hh"
#include "verilog.yy.hh"

namespace yy {
class VerilogScanner : public yyFlexLexer
{
public:
	explicit VerilogScanner(std::istream* in, std::ostream* out);
	int yylex(YYSTYPE* yylval, YYLTYPE* yylloc, yy::VerilogCode* code);
};
}

#endif // VERILOGSSANNER_HH
