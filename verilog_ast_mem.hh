/*!
@file verilog_ast_mem.h
@brief Contains Declarations of datastructures and functions for helping to
manage dynamic memory allocation within the library.
*/

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#ifndef VERILOG_AST_MEM_H
#define VERILOG_AST_MEM_H

namespace yy {
  //! Typedef over ast_memory_T
  typedef struct ast_memory_t ast_memory;

  //! Stores information on some allocated memory as a linked list.
  struct ast_memory_t{
    size_t          size;   //!< Amount of memory allocated.
    void        *   data;   //!< Pointer to the allocated memory.
    ast_memory  *   next;   //!< Next element to be allocated.
  };
}

#endif

