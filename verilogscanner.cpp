#include "verilogscanner.hh"

namespace yy {
	VerilogScanner::VerilogScanner(std::istream* in = 0, std::ostream* out = &std::cout) : yyFlexLexer(in, 0)
	{
	}
}

int yyFlexLexer::yylex()
{
  return 1;
}

int yyFlexLexer::yywrap()
{
  return 1;
}
