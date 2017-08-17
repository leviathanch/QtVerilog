#include "verilogscanner.hh"

namespace yy {
	VerilogScanner::VerilogScanner()
	{
	}

	VerilogScanner::VerilogScanner(std::ifstream input)
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
