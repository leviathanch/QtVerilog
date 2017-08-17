/*!
@file verilog_preprocessor.h
@brief Contains function and data structures to support source code
       preprocessing.
*/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "verilog_ast.hh"
#include "verilog_ast_common.hh"

#ifndef VERILOG_PREPROCESSOR_H
#define VERILOG_PREPROCESSOR_H

namespace yy {
  /*!
@defgroup verilog-preprocessor Preprocessor
@{
@brief This module contains all code and information on the preprocessor and
how it works / is implemented.

@details

The preprocessor is implemented mostly as part of the lexer, with the
various compiler directives handled within the verilog_preprocessor_context
structure and it's associated functions.

*/

  // ----------------------- Default Net Type Directives ------------------

  /*!
@brief Keeps track of the points at which default net type directives are
encountered.
*/
  typedef struct verilog_default_net_type_t{
    unsigned int token_number;  //!< Token number of the directive.
    unsigned int line_number;   //!< Line number of the directive.
    ast_net_type type;          //!< The net type.
  } verilog_default_net_type;

  // ----------------------- Line Directives ------------------------------

  //! Describes a line directive.
  typedef struct verilog_line_directive_t{
    unsigned int  line;  //!< The line to set the current counter to.
    char *        file;  //!< The file we should pretend stuff comes from.
    unsigned char level; //!< Level of include depth.
  } verilog_line_directive;

  // ----------------------- Timescale Directives -------------------------

  //! Describes a simulation timescale directive.
  typedef struct verilog_timescale_directive_t{
	std::string scale;       //!< The timescale to simulate on.
	std::string precision;   //!< Precision of each timestep.
  } verilog_timescale_directive;

  // ----------------------- resetall directives --------------------------



  // ----------------------- Include Directives ---------------------------

  //! Stores information on an include directive.
  typedef struct verilog_include_directive_t{
    char       * filename;      //!< The file to include.
    unsigned int lineNumber;    //!< The line number of the directive.
    bool  file_found;    //!< Can we find the file?
  } verilog_include_directive;

  // ----------------------- `define Directives ---------------------------

  /*!
@brief A simple container for macro directives
*/
  typedef struct verilog_macro_directive_t{
    unsigned int line;      //!< Line number of the directive.
    char * macro_id;        //!< The name of the macro.
    char * macro_value;     //!< The value it expands to.
  } verilog_macro_directive;

  // ----------------------- Conditional Compilation Directives -----------

  /*!
@brief Stores information regarding a particular level of conditional
compilation.
@brief A stack of these is maintained, each one relating to a different
level of nested `ifdef or `ifndef statements.
*/
  typedef struct verilog_preprocessor_conditional_context_t{
    char        * condition;           //!< The definition to check for.
    int           line_number;         //!< Where the `ifdef came from.
    bool   condition_passed;    //!< Did the condition pass?
    bool   is_ndef;             //!< True if directive was `ifndef
    bool   wait_for_endif;      //!< Emit nothing more until `endif
  } verilog_preprocessor_conditional_context;

  // ----------------------- Preprocessor Context -------------------------

  /*
@brief Stores all of the contextual information used by the pre-processor.
@details Stores things like:
- Macro names and evaluations.
- Default net types.
- In Cell Defines.
- IF/ELSE pre-processor directives.
- Timescale directives
*/
  typedef struct verilog_preprocessor_context_t{
    bool     emit_e;           //!< Only emit tokens iff true.
    unsigned int    token_count;    //!< Keeps count of tokens processed.
    bool     in_cell_define; //!< TRUE iff we are in a cell define.

    char *          scratch;        //!< A scratch variable. DO NOT USE.

    ast_stack     * current_file;   //!< Stack of files currently being parsed.
    ast_hashtable * macrodefines;   //!< `define kvp matching.
    ast_list      * includes;       //!< Include directives.
    ast_list      * net_types;      //!< Storage for default nettype directives
    verilog_timescale_directive timescale; //!< Timescale information
    ast_primitive_strength unconnected_drive_pull; //!< nounconnectedrive
    ast_stack     * ifdefs;         //!< Storage for conditional compile stack.
    ast_list      * search_dirs;    //!< Where to look for include files.
  } verilog_preprocessor_context;

  /*! @} */
}

#endif

