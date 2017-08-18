
/*!
@file verilog_parser_wrapper.c
@brief Contains implementations of functions declared in verilog_parser.h
*/

#include "verilogcode.h"
//#include "verilog_ast.hh"
#include "verilog.yy.hh"

namespace yy {
	void VerilogCode::verilog_parser_init()
	{
		if(yy_preproc == NULL)
		{
			//printf("Added new preprocessor context\n");
			yy_preproc = verilog_new_preprocessor_context();
		}
		if(yy_verilog_source_tree == NULL)
		{
			//printf("Added new source tree\n");
			yy_verilog_source_tree = verilog_new_source_tree();
		}
	}
}
