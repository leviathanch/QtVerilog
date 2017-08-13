#include "asttree.h"

AstTree::AstTree()
{
	yy_verilog_source_tree = NULL;
}

void AstTree::add_config(ast_config_declaration *t)
{
	ast_list_append(this->yy_verilog_source_tree->configs, t);
}

void AstTree::add_library(ast_list *t)
{
	this->yy_verilog_source_tree->libraries = ast_list_concat(this->yy_verilog_source_tree->libraries, t);
}

void AstTree::add_source(ast_list *t)
{
  for(unsigned int i = 0; i < t->items; i ++)
{
        ast_source_item* toadd = (ast_source_item*)ast_list_get(t, i);

        if(toadd -> type == SOURCE_MODULE)
{
    ast_list_append(this->yy_verilog_source_tree -> modules, toadd->module);
}
else if (toadd -> type == SOURCE_UDP)
{
    ast_list_append(this->yy_verilog_source_tree->primitives, toadd->udp);
}
else
{
    // Do nothing / unknown / unsupported type.
    printf("line %d of %s - Unknown source item type: %d",
        __LINE__,
        __FILE__,
        toadd -> type);
}
}
}
