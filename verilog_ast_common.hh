/*!
@file verilog_ast_common.h
@brief Contains Declarations of value-independent data structures like
       linked lists which are used in the ast.
*/

#include "stdarg.h"
#include "stdlib.h"
#include "string.h"

#include "verilog_ast_mem.hh"

#ifndef VERILOG_AST_COMMON_H
#define VERILOG_AST_COMMON_H

namespace yy {
  // --------------- Linked List ------------------------

  /*!
@defgroup ast-linked-lists Linked List
@{
@ingroup ast-utility
*/


  //! Typedef for the ast_list_element_t
  typedef struct ast_list_element_t ast_list_element;

  /*!
@brief Storage container for a single element in the linked list.
*/
  struct ast_list_element_t{
    ast_list_element * next;
	const void             * data;
  };


  /*!
@brief Container struct for the linked list data structure.
*/
  typedef struct ast_list_t {
    ast_list_element *  head;         //!< The "front" of the list.
    ast_list_element *  tail;         //!< The "end" of the list.
    ast_list_element *  walker;       //!< Used to "walk" along the list.
    unsigned int        items;        //!< Number of items in the list.
    unsigned int        current_item; //! Current position of walker in list.
  } ast_list;


  /*! @} */

  // ----------------------- Stack --------------------------------------

  /*!
@defgroup ast-stack Stack
@{
@ingroup ast-utility
*/

  //! Typedef for the ast_stack_element_t
  typedef struct ast_stack_element_t ast_stack_element;

  /*!
@brief Storage container for a single element in the stack
*/
  struct ast_stack_element_t{
    ast_stack_element * next;
    void             * data;
  };

  //! A very simple stack.
  typedef struct ast_stack_t{
    unsigned int          depth; //!< How many items are on the stack?
    ast_stack_element   * items; //!< The stack of items.
  } ast_stack;

  /*! @} */


  // ----------------------- Hash Table ---------------------------------


  /*!
@defgroup ast-hashtable Hash Table
@{
@ingroup ast-utility
@brief A *very* simple hash table implemented (for now) over a linked list.
@details This can be used for simple key-value pair storage. Current
access time is O(n) for a table with N elements in it.
@warning This is a *terrible* way to implement a hash table. It doesn't
even do any hashing!
@todo Re-implement over a proper hash table structure.
*/

  /*! @} */


  //! A single element in the hash table.
  typedef struct ast_hashtable_element_t{
    char * key; //!< The key for the element.
    void * data;    //!< The data associated with they key.
  } ast_hashtable_element;


  //! A hash table object.
  typedef struct ast_hashtable_t{
    ast_list * elements; //!< The items.
    unsigned int size;   //!< The number of elements in the table.
  } ast_hashtable;

  typedef enum ast_hashtable_result_e{
    HASH_SUCCESS        = 0,
    HASH_FAIL           = 1,
    HASH_KEY_COLLISION  = 2,
    HASH_KEY_NOT_FOUND  = 3
  } ast_hashtable_result;

}
#endif
