/*!
@file verilog_ast_mem.c
@brief Contains definitions of datastructures and functions for helping to
manage dynamic memory allocation within the library.
*/

#include "verilog_ast_mem.hh"
#include "verilogcode.h"

namespace yy {
  /*!
@defgroup ast-utility-mem-manage Memory Management
@{
    @brief Helps to manage memory allocated during AST construction.
@ingroup ast-utility
*/

  /*!
@brief A simple wrapper around calloc.
@details Makes it very easy to clean up afterward using the @ref ast_free_all
function.
@param [in] num - Number of elements to allocate space for.
@param [in] size - The size of each element being allocated.
@returns A pointer to the start of the block of memory allocated.
*/
  void * VerilogCode::ast_calloc(size_t num, size_t size)
  {
    // This is the memory the user asked for.
    void * data = calloc(num,size);

    memory_allocations += 1;

    // Add the allocated memory to tracking data structure.
    if(walker == NULL)
      {
        memory_head = (ast_memory *)calloc(1,sizeof(ast_memory));
        memory_head->size = num * size;
        total_allocated += memory_head->size;
        memory_head->data = data;
        memory_head->next = NULL;
        walker      = memory_head;
      }
    else
      {
        walker->next = (ast_memory *)calloc(1,sizeof(ast_memory));
        walker->next->size = num * size;
        total_allocated += walker->size;
        walker->next->data = data;
        walker->next->next = NULL;
        walker         = walker->next;
      }


    return data;
  }

  /*!
@brief Frees all memory allocated using @ref ast_calloc.
@details Free's all data stored in the linked list pointed to by the
@ref head variable.
@post @ref walker and @ref head are NULL. All memory allocated by ast_calloc
has been freed.
*/
  void VerilogCode::ast_free_all()
  {
    printf("Freeing data for %u memory allocations.\n", memory_allocations);
    size_t total_freed = 0;

    while(memory_head != NULL)
      {
        walker = memory_head->next;
        total_freed += memory_head->size;

        free(memory_head->data);
        free(memory_head);

        memory_head = walker;
      }

    printf("\tFree'd %lu bytes of %lu bytes allocated.\n",
           total_freed, total_allocated);
    printf("\tBytes remaining: %lu\n", total_allocated - total_freed);
  }


  char * VerilogCode::ast_strdup(QString in)
  {
        QByteArray array = in.toLocal8Bit();
        char* buffer = array.data();
        int len = strlen(buffer);
        char * tr = (char *)ast_calloc(len+1,sizeof(char));
        memcpy(tr,buffer,len);
        return tr;
  }

  char * VerilogCode::ast_strdup(char * in)
  {
    size_t len = strlen(in);
    char * tr = (char *)ast_calloc(len+1,sizeof(char));
    memcpy(tr,in,len);
    return tr;
  }

  /*!@}*/
}
