#ifndef VERILOGSCANNER_H
#define VERILOGSCANNER_H

#include "verilog.l.hh"

class VerilogScanner : public yyFlexLexer
{
public:
	VerilogScanner(std::istream* arg_yyin = 0, std::ostream* arg_yyout = 0);
	VerilogScanner();
};

#endif // VERILOGSCANNER_H
