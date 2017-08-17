#ifndef VERILOGCODE_H
#define VERILOGCODE_H

#include <fstream>
#include <sstream>
#include <ostream>

#include "verilog_ast.hh"
#include "verilog_preprocessor.hh"
#include "verilog_ast_common.hh"

namespace yy {
	class VerilogScanner;
	class VerilogCode
	{
	public:
		/*!
		  @brief Stores all information needed for the preprocessor.
		  @details This does not usually need to be accessed by the programmer, and
		  certainly should never be written to unless you are defining your own
		  default directives. For example, if I was writing my own simulated and
		  wanted to add my own "__IS_MY_SIMULATOR__" pre-defined macro, it can be
		  done by accessing this variable, and using the
		  verilog_preprocessor_macro_define function.
		  @note This is a global variable. Treat it with care!
		*/
		verilog_preprocessor_context * yy_preproc;

		/*!
		  @brief Global source tree object, used to store parsed constructs.
		  @details This is a global variable, initialised prior to calling
		  the verilog_parse function, into which all objects the parser finds are
		  stored.
		*/
		verilog_source_tree * yy_verilog_source_tree;

		//! The total number of memory allocations made.
		unsigned int memory_allocations = 0;

		//! The total number of bytes ever allocated using ast_alloc
		size_t       total_allocated = 0;

		//! Head of the linked list of allocated memory.
		ast_memory * memory_head = NULL;

		//! Walker for the linked list of allocated memory.
		ast_memory * walker = NULL;

		/// enable debug output in the flex scanner
		bool trace_scanning = true;

		/// enable debug output in the bison parser
		bool trace_parsing = true;

		/// construct a new parser driver context
		VerilogCode();

		/// stream name (file or input stream) used for error messages.
		QString streamname;

		void showData();

		/** Invoke the scanner and parser on a file. Use parse_stream with a
		 * std::ifstream if detection of file reading errors is required.
		 * @param filename	input file name
		 * @return		true if successfully parsed
		 */
		bool parse_file(QString filename);

		// To demonstrate pure handling of parse errors, instead of
		// simply dumping them on the standard error output, we will pass
		// them to the driver using the following two member functions.

		/** Error handling with associated line number. This can be modified to
		 * output the error e.g. to a dialog box. */
		void error(const class location& l, const std::string& m);

		/** General error handling. This can be modified to output the error
		 * e.g. to a dialog box. */
		void error(const std::string& m);

		class VerilogScanner* lexer;
		class VerilogParser *parser;

		void add_library(ast_list *t);
		void add_config(ast_config_declaration *t);
		void add_source(ast_list *t);

		// Parser Wrapper
		void verilog_parser_init();
		int verilog_parse_buffer(char * to_parse, int length);
		int verilog_parse_string(char * to_parse, int length);
		int verilog_parse_file(FILE * to_parse);

		// AST functions
		void ast_set_meta_info(ast_metadata * meta);
		/*!
	  @brief Creates a new ast primary which is part of a constant expression tree
			 with the supplied type and value.
	  @param [in] type - Self explanatory
	  */
		ast_primary * ast_new_constant_primary(ast_primary_value_type type);

		/*!
	  @brief Creates a new AST primary wrapper around a function call.
	  @param [in] call - The AST node representing a function call.
	  */
		ast_primary * ast_new_primary_function_call(ast_function_call * call);

		/*!
	  @brief Creates a new ast primary which is part of an expression tree
			 with the supplied type and value.
	  @param [in] type - Self explanatory
	  */
		ast_primary * ast_new_primary(ast_primary_value_type type);

		/*!
	  @brief Creates a new ast primary which is part of a constant expression tree
			 with the supplied type and value.
	  @param [in] type - Self explanatory
	  */
		ast_primary * ast_new_module_path_primary(ast_primary_value_type type);

		/*!
	  @brief A utility function for converting an ast expression tree back into
	  a string representation.
	  @param [in] exp - The expression to turn into a string.
	  */
		char * ast_expression_tostring(
			ast_expression * exp
			);

		/*!
	  @brief Creates and returns a new expression primary.
	  @details This is simply an expression instance wrapped around a
	  primary instance for the purposes of mirroring the expression tree gramamr.
	  Whether or not the expression is constant is denoted by the type member
	  of the passed primary.
	  @param [in] p - The primary to insert into the expression.
	  */
		ast_expression * ast_new_expression_primary(ast_primary * p);

		/*!
	  @brief Creates a new binary infix expression with the supplied operands.
	  @param [in] left - LHS of the infix operation.
	  @param [in] right - RHS of the infix operation.
	  @param [in] operation - What do we do?!
	  @param [in] attr - Attributes applied to the expression.
	  @param [in] constant - Is this a constant expression we can simplify?
	  */
		ast_expression * ast_new_binary_expression(ast_expression * left,
												   ast_expression * right,
												   ast_operator     operation,
												   ast_node_attributes * attr,
												   bool      constant);

		/*!
	  @brief Creates a new unary expression with the supplied operation.
	  @param [in] operand - The thing to operate on.
	  @param [in] operation - What do we do?!
	  @param [in] attr - Expression attributes.
	  @param [in] constant - Is this a constant expression we can simplify?
	  */
		ast_expression * ast_new_unary_expression(ast_primary    * operand,
												  ast_operator     operation,
												  ast_node_attributes * attr,
												  bool       constant);

		/*!
	  @brief Creates a new range expression with the supplied operands.
	  @param [in] left - The Upper range of the expression
	  @param [in] right - The lower range of the expression.
	  @details
	  For example, when specifying a simple bus in verilog:

	  @code
	  wire [31:0] bus_data;
	  @endcode

	  Then the `31` would go into left, and the `0` into the right.
	  */
		ast_expression * ast_new_range_expression(ast_expression * left,
												  ast_expression * right);

		/*!
	  @brief Creates a new range index expression with the supplied operands.
	  @param [in] left - The single expression index into an array.
	  @details Used to represent code like...

	  @code
	  wire [32:0] adder_result;
	  assign overflow = adder_result[32];
	  @endcode

	  Here, accessing the 32nd bit of `adder_result` is an index expression.
	  */
		ast_expression * ast_new_index_expression(ast_expression * left);

		/*!
	  @brief Creates a new string expression.
	  @param [in] string - The string. Duh.
	  */
		ast_expression * ast_new_string_expression(ast_string string);


		/*!
	  @brief Creates a new conditional expression node.
	  @param [in] condition - Decides which result expression is presented.
	  @param [in] if_true - executed if condition == true (!0)
	  @param [in] if_false - executed if condition == false (0).
	  @param [in] attr - Attributes
	  @note The condition is stored in the aux member, if_true in left, and if_false
	  on the right.
	  @details Can be used to represent ternary operations:

	  @code
	  assign stall = mem_error || mem_stall ? 1'b0 : global_stall;
	  @endcode

	  */
		ast_expression * ast_new_conditional_expression(ast_expression * condition,
														ast_expression * if_true,
														ast_expression * if_false,
														ast_node_attributes * attr);

		/*!
	  @brief Creates a new (min,typical,maximum) expression.
	  @details If the mintypmax expression only specifies a typical value,
	  then the min and max arguments should be NULL, and only typ set.
	  @param [in] min - Minimum value in the distribution.
	  @param [in] typ - Typical / average.
	  @param [in] max - Maximum value in the distribution.
	  */
		ast_expression * ast_new_mintypmax_expression(ast_expression * min,
													  ast_expression * typ,
													  ast_expression * max);



		/*!
	  @brief Creates and returns as a pointer a new attribute descriptor.
	  @param [in] name - The name of the parameter/attribute.
	  @param [in] value - The value the attribute should take.
	  */
		ast_node_attributes * ast_new_attributes(
			ast_identifier name,
			ast_expression * value
			);

		/*!
	  @brief Creates and returns a new attribute node with the specified value
			 and name.
	  @param [inout] parent - Pointer to the node which represents the list of
							  attribute name,value pairs.
	  @param [in]    toadd  - The new attribute to add.
	  */
		void ast_append_attribute(ast_node_attributes * parent,
								  ast_node_attributes * toadd);



		/*!
	  @brief Creates and returns a new @ref ast_lvalue pointer, with the data type
			 being a single identifier of either @ref NET_IDENTIFIER or
			 @ref VAR_IDENTIFIER.
	  */
		ast_lvalue * ast_new_lvalue_id(ast_lvalue_type type, ast_identifier id);

		/*!
	  @brief Creates and returns a new @ref ast_lvalue pointer, with the data type
			 being a concatenation holder of either @ref NET_CONCATENATION or
			 @ref VAR_CONCATENATION.
	  */
		ast_lvalue * ast_new_lvalue_concat(ast_lvalue_type type, ast_concatenation*id);



		/*!
	  @brief A utility function for converting an ast expression primaries back into
	  a string representation.
	  @param [in] p - The expression primary to turn into a string.
	  */
		char * ast_primary_tostring(
			ast_primary * p
			);



		//! Returns the string representation of an operator;
		char * ast_operator_tostring(ast_operator op);

		/*!
	  @brief Creates and returns a new node representing a function call.
	  @param [in] arguments - list of elements of type ast_expression
	  representing the various parameters to the function. If the function has
	  no arguments, then it is an empty list, not NULL.
	  @param [in] id - Function identifier / name
	  @param [in] constant - Does this function return a constant value?
	  @param [in] system - Is this a system function?
	  @param [in] attr - Attributes for vendor specific tool features.
	  */
		ast_function_call * ast_new_function_call(ast_identifier  id,
												  bool     constant,
												  bool     system,
												  ast_node_attributes * attr,
												  ast_list      * arguments);




		/*!
	  @brief Creates a new AST concatenation element with the supplied type and
	  initial starting value.
	  @param [in] repeat - Used for replications or multiple_concatenation
	  @param [in] type - What sort of values are being concatenated?
	  @param [in] first_value - The first element of the concatentation.
	  @details Depending on the type supplied, the type of first_value
	  should be:
		  - CONCATENATION_EXPRESSION          : ast_expression
		  - CONCATENATION_CONSTANT_EXPRESSION : ast_expression
		  - CONCATENATION_NET                 : ast_identifier
		  - CONCATENATION_VARIABLE            : ast_identifer
		  - CONCATENATION_MODULE_PATH         : ast_identifier
	  */
		ast_concatenation * ast_new_concatenation(ast_concatenation_type type,
												  ast_expression * repeat,
												  void * first_value);

		/*!
	  @brief Creates and returns a new empty concatenation of the specified type.
	  @param [in] type - What sort of values are being concatenated?
	  */
		ast_concatenation * ast_new_empty_concatenation(ast_concatenation_type type);

		/*!
	  @brief Adds a new data element on to the *front* of a concatenation.
	  @param [inout] element - THe concantenation being extended,
	  @param [in] repeat - Is the concatenation repeated?
	  @param [in] data - The item to add to the concatenation sequence.
	  */
		void                ast_extend_concatenation(ast_concatenation * element,
													 ast_expression * repeat,
													 void * data);



		/*!
	  @brief Creates and returns a new path declaration type. Expects that the data
	  be filled in manually;
	  */
		ast_path_declaration * ast_new_path_declaration(ast_path_declaration_type type);

		/*!
	  @brief Creates and returns a pointer to a new simple parallel path declaration.
	  */
		ast_simple_parallel_path_declaration * ast_new_simple_parallel_path_declaration
		(
			ast_identifier      input_terminal,
			ast_operator        polarity,
			ast_identifier      output_terminal,
			ast_list        *   delay_value
			);


		/*!
	  @brief Creates and returns a pointer to a new simple full path declaration.
	  */
		ast_simple_full_path_declaration * ast_new_simple_full_path_declaration
		(
			ast_list        *   input_terminals,
			ast_operator        polarity,
			ast_list        *   output_terminals,
			ast_list        *   delay_value
			);

		/*!
	  @brief Describes a single edge sensitive path declaration
	  */
		ast_edge_sensitive_parallel_path_declaration *
		ast_new_edge_sensitive_parallel_path_declaration(
			ast_edge            edge,               //!< edge_identifier
			ast_identifier      input_terminal,     //!< specify_input_terminal_descriptor
			ast_operator        polarity,           //!< polarity_operator
			ast_identifier      output_terminal,    //!< specify_output_terminal_descriptor
			ast_expression  *   data_source,        //!< data_source_expression
			ast_list        *   delay_value         //!< path_delay_value
			);

		/*!
	  @brief Describes a parallel edge sensitive path declaration
	  */
		ast_edge_sensitive_full_path_declaration *
		ast_new_edge_sensitive_full_path_declaration(
			ast_edge            edge,               //!< edge_identifier
			ast_list        *   input_terminal,     //!< list_of_path_inputs
			ast_operator        polarity,           //!< polarity_operator
			ast_list        *   output_terminal,    //!< list_of_path_outputs
			ast_expression  *   data_source,        //!< data_source_expression
			ast_list        *   delay_value         //!< path_delay_value
			);




		/*!
	  @brief creates and returns a pointer to a new task-enable statement.
	  */
		ast_task_enable_statement * ast_new_task_enable_statement(
			ast_list        * expressions,
			ast_identifier    identifier,
			bool       is_system
			);



		/*!
	  @brief Creates and returns a new forever loop statement.
	  @param inner_statement - Pointer to the inner body of statements which
	  make upt the loop body.
	  */
		ast_loop_statement * ast_new_forever_loop_statement(
			ast_statement * inner_statement
			);

		/*!
	  @brief Creates and returns a new for loop statement.
	  @param inner_statements - Pointer to the inner body of statements which
	  make up the loop body.
	  @param initial_condition - Assignement which sets up the initial condition
	  of the iterator.
	  @param modify_assignment - How the iterator variable changes with each
	  loop iteration.
	  @param continue_condition - Expression which governs whether the loop should
	  continue or break.
	  */
		ast_loop_statement * ast_new_for_loop_statement(
			ast_statement  * inner_statements,
			ast_single_assignment * initial_condition,
			ast_single_assignment * modify_assignment,
			ast_expression * continue_condition
			);

		/*!
	  @brief Creates and returns a new generate loop statement.
	  @param inner_statements - Pointer to the inner body of statements which
	  make up the loop body.
	  @param initial_condition - Assignement which sets up the initial condition
	  of the iterator.
	  @param modify_assignment - How the iterator variable changes with each
	  loop iteration.
	  @param continue_condition - Expression which governs whether the loop should
	  continue or break.
	  */
		ast_loop_statement * ast_new_generate_loop_statement(
			ast_list              * inner_statements,
			ast_single_assignment * initial_condition,
			ast_single_assignment * modify_assignment,
			ast_expression        * continue_condition
			);

		/*!
	  @brief Creates and returns a while loop statement.
	  @param inner_statement - Pointer to the inner body of statements which
	  make upt the loop body.
	  @param continue_condition - Expression which governs whether the loop should
	  continue or break.
	  */
		ast_loop_statement * ast_new_while_loop_statement(
			ast_statement  * inner_statement,
			ast_expression * continue_condition
			);

		/*!
	  @brief Creates and returns a repeat loop statement.
	  @param inner_statement - Pointer to the inner body of statements which
	  make upt the loop body.
	  @param continue_condition - Expression which governs whether the loop should
	  continue or break.
	  */
		ast_loop_statement * ast_new_repeat_loop_statement(
			ast_statement  * inner_statement,
			ast_expression * continue_condition
			);



		/*!
	  @brief Create and return a new item in a cast statement.
	  @param conditions - The conditions on which the item is executed.
	  @param body - Executes when any of the conditions are met.
	  */
		ast_case_item * ast_new_case_item(ast_list      * conditions,
										  ast_statement * body);


		/*!
	  @brief Creates and returns a new case statement.
	  @param expression - The expression evaluated to select a case.
	  @param cases - list of possible cases.
	  */
		ast_case_statement * ast_new_case_statement(ast_expression * expression,
													ast_list       * cases,
													ast_case_statement_type type);



		/*!
	  @brief Creates and returns a new conditional statement.
	  @param statement - what to run if the condition holds true.
	  @param condtion  - the condition on which statement is run.
	  */
		ast_conditional_statement * ast_new_conditional_statement(
			ast_statement * statement,
			ast_expression * condition
			);

		/*!
	  @brief Creates a new if-then-else-then statement.
	  @param if_condition - the conditional statement.
	  @param else_condition - What to do if no conditional statements are executed.
	  This can be NULL.
	  @details This node also supports "if then elseif then else then" statements,
	  and uses the ast_extend_if_else function to append a new
	  ast_conditional_statement to the end of a list of if-else conditions.
	  Priority of exectuion is given to items added first.
	  */
		ast_if_else * ast_new_if_else(
			ast_conditional_statement * if_condition,
			ast_statement             * else_condition
			);


		/*!
	  @brief Adds an additional conditional (ha..) to an existing if-else
	  statement.
	  @param conditional_statements - the existing if-else tree.
	  @param new_statement - The new statement to add at the end of the existing
	  if-then conditions, but before any else_condtion.
	  */
		void  ast_extend_if_else(
			ast_if_else                 * conditional_statements,
			ast_list                    * new_statement
			);



		/*!
	  @brief Creates and returns a new wait statement.
	  */
		ast_wait_statement * ast_new_wait_statement(
			ast_expression * wait_for,
			ast_statement  * statement
			);

		/*!
	  @brief Creates a new event expression node
	  @param trigger_edge - the edge on which the trigger is activated.
	  @param expression - the expression to monitor the waveforms of.
	  */
		ast_event_expression * ast_new_event_expression(
			ast_edge trigger_edge,
			ast_expression * expression
			);

		/*!
	  @brief Creates a new event expression node, which is itself a sequence of
	  sub-expressions.
	  */
		ast_event_expression * ast_new_event_expression_sequence(
			ast_event_expression * left,
			ast_event_expression * right
			);

		/*!
	  @brief Creates and returns a new event control specifier.
	  */
		ast_event_control * ast_new_event_control(
			ast_event_control_type type,
			ast_event_expression * expression
			);

		/*!
	  @brief creates and returns a new delay control statement.
	  */
		ast_delay_ctrl * ast_new_delay_ctrl_value(ast_delay_value * value);

		/*!
	  @brief creates and returns a new delay control statement.
	  */
		ast_delay_ctrl * ast_new_delay_ctrl_mintypmax(
			ast_expression * mintypmax
			);

		/*!
	  @brief Creates and returns a new timing control statement node.
	  */
		ast_timing_control_statement * ast_new_timing_control_statement_delay(
			ast_timing_control_statement_type   type,
			ast_statement                     * statement,
			ast_delay_ctrl                    * delay_ctrl
			);

		/*!
	  @brief Creates and returns a new timing control statement node.
	  */
		ast_timing_control_statement * ast_new_timing_control_statement_event(
			ast_timing_control_statement_type   type,
			ast_expression                    * repeat,
			ast_statement                     * statement,
			ast_event_control                 * event_ctrl
			);



		/*!
	  @brief Creates and returns a new continuous assignment.
	  */
		ast_single_assignment * ast_new_single_assignment(
			ast_lvalue         * lval,          //!< The thing being assigned to.
			ast_expression     * expression //!< The value it takes on.
			);

		/*!
	  @brief Creates a new hybrid assignment of the specified type.
	  */
		ast_assignment * ast_new_hybrid_assignment(
			ast_hybrid_assignment_type type,    //!< The assignment type.
			ast_single_assignment * assignment  //!< The things being assigned.
			);

		/*!
	  @brief Creates a new hybrid assignment of the specified type.
	  */
		ast_assignment * ast_new_hybrid_lval_assignment(
			ast_hybrid_assignment_type type, //!< FORCE or (DE)ASSIGN.
			ast_lvalue * lval   //!< The thing to change.
			);

		/*!
	  @brief Creates and returns a new blocking procedural assignment object.
	  */
		ast_assignment * ast_new_blocking_assignment(
			ast_lvalue * lval, //!< The net/variable being assigned to.
			ast_expression  * expression,   //!< Assign it this value.
			ast_timing_control_statement* delay_or_event //!< The timing module
			);

		/*!
	  @brief Creates and returns a new nonblocking procedural assignment object.
	  */
		ast_assignment * ast_new_nonblocking_assignment(
			ast_lvalue * lval,  //!< The net/variable being assigned to.
			ast_expression  * expression,   //!< The value it will take.
			ast_timing_control_statement * delay_or_event //!< Timing model.
			);


		/*!
	  @brief Creates and returns a new continuous assignment object.
	  @brief All of the assignments will have the same drive strength and
	  signal delay properties.
	  */
		ast_assignment * ast_new_continuous_assignment(
			ast_list * assignments, //!< The list of assignments to make.
			ast_drive_strength * strength, //!< The drive strength
			ast_delay3 * delay  //!< Delay in making the assignment.
			);

		/*!
	  @brief Creates and returns a new statement block of the specified type
	  @note The `trigger` member of the returned ast_statement_block will always be
	  NULL. This is because we don't find out what sort of block this is until
	  further up the parse tree.
	  */
		ast_statement_block * ast_new_statement_block(
			ast_block_type   type,
			ast_identifier   block_identifier,
			ast_list       * declarations,
			ast_list       * statements
			);



		//! Creates and returns a pointer to a new disable statement.
		ast_disable_statement * ast_new_disable_statement(ast_identifier   id);
		/*!
	  @brief Creates a new AST statement and returns it.
	  @note Requires the data field of the union to be filled out manually.
	  */
		ast_statement * ast_new_statement(
			ast_node_attributes * attr,
			bool         is_function_statement,
			void             *  data,
			ast_statement_type  type
			);
		/*!
	  @brief Creates and returns a new pulse control data structure.
	  */
		ast_pulse_control_specparam * ast_new_pulse_control_specparam(
			ast_expression * reject_limit,
			ast_expression * error_limit
			);

		/*!
	  @brief Creates and returns a new range or dimension representation node.
	  */
		ast_range * ast_new_range(
			ast_expression * upper,
			ast_expression * lower
			);


		//! Creates a new initial statement node.
		ast_udp_initial_statement * ast_new_udp_initial_statement(
			ast_identifier   output_port,
			ast_number     * initial_value
			);


		//! Creates and returns a new sequential UDP body representation.
		ast_udp_body * ast_new_udp_sequential_body(
			ast_udp_initial_statement * initial_statement,
			ast_list                  * sequential_entries
			);

		//! Creates and returns a new combinatorial UDP body representation.
		ast_udp_body * ast_new_udp_combinatoral_body(
			ast_list                  * combinatorial_entries
			);

		//! Creates a new combinatorial entry for a UDP node.
		ast_udp_combinatorial_entry * ast_new_udp_combinatoral_entry(
			ast_list * input_levels,
			ast_udp_next_state output_symbol
			);

		//! Creates a new sequntial body entry for a UDP node.
		ast_udp_sequential_entry * ast_new_udp_sequential_entry(
			ast_udp_seqential_entry_prefix prefix_type,
			ast_list    *                  levels_or_edges,
			ast_level_symbol               current_state,
			ast_udp_next_state             output
			);

		/*!
	  @brief Creates a new UDP port AST node
	  @details
	  @returns A pointer to the new port
	  */
		ast_udp_port * ast_new_udp_port(
			ast_port_direction    direction,
			ast_identifier        identifier, //!< The udp being instanced.
			ast_node_attributes * attributes,
			bool           reg,
			ast_expression      * default_value
			);

		/*!
	  @brief Creates a new UDP input port AST node
	  @details
	  @returns A pointer to the new port
	  */
		ast_udp_port * ast_new_udp_input_port(
			ast_list            * identifiers,
			ast_node_attributes * attributes
			);

		/*!
	  @brief Creates a new UDP declaration node
	  @details
	  @note the first element of the ports list should be the output terminal.
	  @returns A pointer to the new node.
	  */
		ast_udp_declaration * ast_new_udp_declaration(
			ast_node_attributes * attributes,
			ast_identifier        identifier,
			ast_list            * ports,
			ast_udp_body        * body
			);

		/*!
	  @brief Creates a new instance of a UDP.
	  @details
	  @returns A pointer to the new instance.
	  */
		ast_udp_instance * ast_new_udp_instance(
			ast_identifier        identifier,
			ast_range           * range,
			ast_lvalue          * output,
			ast_list            * inputs
			);

		/*!
	  @brief Creates a new list of UDP instances with shared properties.
	  @details
	  @returns A pointer to the new list.
	  */
		ast_udp_instantiation * ast_new_udp_instantiation(
			ast_list            * instances,
			ast_identifier        identifier, //!< The UDP being instanced
			ast_drive_strength  * drive_strength,
			ast_delay2          * delay
			);

		//! Creates and returns a new block of generate items.
		ast_generate_block * ast_new_generate_block(
			ast_identifier   identifier,
			ast_list       * generate_items
			);

		/*!
	  @brief Creates and returns a new item which exists inside a generate statement.
	  @note the void* type of the construct parameter allows for a single
	  constructor function rather than one per member of the union inside the
	  ast_generate_item structure.
	  */
		ast_statement * ast_new_generate_item(
			ast_statement_type type,
			void    *          construct
			);

		/*!
	  @brief Creates and returns a new set of module instances with shared
	  parameters.
	  */
		ast_module_instantiation * ast_new_module_instantiation(
			ast_identifier          module_identifer,
			ast_list              * module_parameters,
			ast_list              * module_instances
			);

		/*!
	  @brief Creates and returns a new instance of a module with a given identifer
	  and set of port connections.
	  */
		ast_module_instance * ast_new_module_instance(
			ast_identifier          instance_identifier,
			ast_list              * port_connections
			);

		/*!
	  @brief Creates and returns a new port connection representation.
	  @param port_name - The port being assigned to.
	  @param expression - The thing inside the module the port connects to.
	  @note This is also used for module parameter assignments.
	  */
		ast_port_connection * ast_new_named_port_connection(
			ast_identifier   port_name,
			ast_expression * expression
			);


		//! Instances a new switch type with a delay3.
		ast_switch_gate * ast_new_switch_gate_d3(
			ast_switchtype type,
			ast_delay3     * delay
			);

		//! Instances a new switch type with a delay2.
		ast_switch_gate * ast_new_switch_gate_d2(
			ast_switchtype type,
			ast_delay2     * delay
			);

		//! Creates and returns a new structure describing primitive net strength.
		ast_primitive_pull_strength * ast_new_primitive_pull_strength(
			ast_pull_direction       direction,
			ast_primitive_strength   strength_1,
			ast_primitive_strength   strength_0
			);

		//! Create and return a new pull strength indicator for 1 and 0.
		ast_pull_strength * ast_new_pull_stregth(
			ast_primitive_strength strength_1,
			ast_primitive_strength strength_2
			);

		//! Creates collection of n-input gates with the same type and properties.
		ast_n_input_gate_instances * ast_new_n_input_gate_instances(
			ast_gatetype_n_input    type,
			ast_delay3            * delay,
			ast_drive_strength    * drive_strength,
			ast_list              * instances
			);

		//! Creates collection of enable gates with the same type and properties.
		ast_enable_gate_instances * ast_new_enable_gate_instances(
			ast_gatetype_n_input    type,
			ast_delay3            * delay,
			ast_drive_strength    * drive_strength,
			ast_list              * instances
			);

		/*!
	  @brief Creates and returns a new n_output gate instance.
	  @see ast_n_output_gate_instances
	  */
		ast_n_output_gate_instance * ast_new_n_output_gate_instance(
			ast_identifier                name,
			ast_list                    * outputs,
			ast_expression              * input
			);

		/*!
	  @brief Creates and returns a set of n_output gates with the same properties.
	  */
		ast_n_output_gate_instances * ast_new_n_output_gate_instances(
			ast_n_output_gatetype         type,
			ast_delay2                  * delay,
			ast_drive_strength          * drive_strength,
			ast_list                    * instances
			);

		/*!
	  @brief Creates and returns a collection of pass enable switches.
	  */
		ast_pass_enable_switches * ast_new_pass_enable_switches(
			ast_pass_enable_switchtype    type,
			ast_delay2                  * delay,
			ast_list                    * switches
			);

		/*!
	  @brief Creates and returns a new pass enable switch instance.
	  */
		ast_pass_enable_switch * ast_new_pass_enable_switch(
			ast_identifier      name,
			ast_lvalue        * terminal_1,
			ast_lvalue        * terminal_2,
			ast_expression    * enable
			);

		/*! @brief Describes a single pull gate instance.*/
		ast_pull_gate_instance * ast_new_pull_gate_instance(
			ast_identifier      name,
			ast_lvalue        * output_terminal
			);

		/*! @brief A single pass transistor instance.*/
		ast_pass_switch_instance * ast_new_pass_switch_instance(
			ast_identifier      name,
			ast_lvalue        * terminal_1,
			ast_lvalue        * terminal_2
			);

		/*! @brief An N-input gate instance. e.g. 3-to-1 NAND.*/
		ast_n_input_gate_instance * ast_new_n_input_gate_instance(
			ast_identifier      name,
			ast_list          * input_terminals,
			ast_lvalue        * output_terminal
			);

		/*! @brief A single Enable gate instance.*/
		ast_enable_gate_instance * ast_new_enable_gate_instance(
			ast_identifier      name,
			ast_lvalue        * output_terminal,
			ast_expression    * enable_terminal,
			ast_expression    * input_terminal
			);

		/*! @brief A single MOS switch (transistor) instance.*/
		ast_mos_switch_instance * ast_new_mos_switch_instance(
			ast_identifier      name,
			ast_lvalue        * output_terminal,
			ast_expression    * enable_terminal,
			ast_expression    * input_terminal
			);

		/*! @brief A single CMOS switch (transistor) instance.*/
		ast_cmos_switch_instance * ast_new_cmos_switch_instance(
			ast_identifier      name,
			ast_lvalue        * output_terminal,
			ast_expression    * ncontrol_terminal,
			ast_expression    * pcontrol_terminal,
			ast_expression    * input_terminal
			);

		/*!
	  @brief Creates and returns a new configuration rule statment node.
	  @details If is_default is TRUE then clause_2 is NULL.
	  */
		ast_config_rule_statement * ast_new_config_rule_statement(
			bool    is_default,
			ast_identifier clause_1,    //!< The first grammar clause.
			ast_identifier clause_2     //!< The second grammar clause.
			);
		/*!
	  @brief Creates and returns a new config declaration node.
	  */
		ast_config_declaration * ast_new_config_declaration(
			ast_identifier  identifier,
			ast_identifier  design_statement,
			ast_list      * rule_statements
			);



		/*!
	  @brief Creates a new library declaration node.
	  */
		ast_library_declaration * ast_new_library_declaration(
			ast_identifier  identifier,
			ast_list      * file_paths,
			ast_list      * incdirs
			);

		/*!
	  @brief creates and returns a new collection of AST switches.
	  */
		ast_switches * ast_new_switches(ast_switch_gate * type, ast_list * switches);

		/*!
	  @brief Creates and returns a new gate instantiation descriptor.
	  @details Expects the data fields to be filled out manually after the structure
	  is returned.
	  */
		ast_gate_instantiation * ast_new_gate_instantiation(ast_gate_type type);

		/*!
	  @brief Create a new delay value.
	  */
		ast_delay_value * ast_new_delay_value(
			ast_delay_value_type type,
			void * data
			);

		/*!
	  @brief Create a new delay3 instance.
	  */
		ast_delay3 * ast_new_delay3(
			ast_delay_value * min,
			ast_delay_value * avg,
			ast_delay_value * max
			);

		/*!
	  @brief Create a new delay2 instance.
	  */
		ast_delay2 * ast_new_delay2(
			ast_delay_value * min,
			ast_delay_value * max
			);


		/*!
	  @brief Creates a new net declaration object.
	  @details Turns a generic "type declaration" object into a net_declration
	  object and discards un-needed member fields.
	  @returns A set of ast_net_declaration types as a list, one for each identifer
	  in the original type declaration object.
	  */
		ast_list * ast_new_net_declaration(
			ast_type_declaration * type_dec
			);

		/*!
	  @brief Creates a new reg declaration object.
	  @details Turns a generic "type declaration" object into a reg_declration
	  object and discards un-needed member fields.
	  @returns A set of ast_reg_declaration types as a list, one for each identifer
	  in the original type declaration object.
	  */
		ast_list * ast_new_reg_declaration(
			ast_type_declaration * type_dec
			);

		/*!
	  @brief Creates a new variable declaration object.
	  @details Turns a generic "var declaration" object into a var_declration
	  object and discards un-needed member fields.
	  @returns A set of ast_var_declaration types as a list, one for each identifer
	  in the original type declaration object.
	  */
		ast_list * ast_new_var_declaration(
			ast_type_declaration * type_dec
			);


		/*!
	  @brief Creates and returns a node to represent the declaration of a new
	  module item construct.
	  @param [in] type - What sort of item is contained in the returned structure.
	  From this, we know which members of the ast_type_declaration make sense and
	  are safe to access.
	  @details Because of the complex nature of the grammar for these declarations,
	  (bourne from the number of optional modifiers) no single constructor function
	  is provided. Rather, one can create a new type declaration of a
	  known type, but must otherwise fill out the data members as they go along.
	  All pointer members are initialised to NULL, and all boolean members will
	  initially be false.
	  */
		ast_type_declaration * ast_new_type_declaration(ast_declaration_type type);



		/*!
	  @brief creates and returns a new set of parameter declarations of the same type
	  @param [in] assignments - The list of individual assignments.
	  @param [in] signed_values - are the bit vectors signed?
	  @param [in] range - Bit range
	  @param [in] type - type of the parameters.
	  */
		ast_parameter_declarations * ast_new_parameter_declarations(
			ast_list        * assignments,
			bool       signed_values,
			bool       local,
			ast_range       * range,
			ast_parameter_type  type
			);



		/*!
	  @brief Creates and returns a new port declaration representation.
	  */
		ast_port_declaration * ast_new_port_declaration(
			ast_port_direction  direction,      //!< [in] Input / output / inout etc.
			ast_net_type        net_type,       //!< [in] Wire/reg etc
			bool         net_signed,     //!< [in] Signed value?
			bool         is_reg,         //!< [in] Is explicitly a "reg"
			bool         is_variable,    //!< [in] Variable or net?
			ast_range         * range,          //!< [in] Bus width.
			ast_list          * port_names      //!< [in] The names of the ports.
			);



		/*!
	  @brief Creates and returns a new object storing either a range or a type.
	  @param [in] is_range - IFF true then the structure's union contains a
	  range structure, otherwise it contains a type structure.
	  */
		ast_range_or_type * ast_new_range_or_type(bool is_range);

		/*!
	  @brief Creates and returns a function declaration node.
	  */
		ast_function_declaration * ast_new_function_declaration(
			bool         automatic,         //!< Is automatic?
			bool         is_signed,         //!< Is the returned value signed?
			bool         function_or_block, //!< IFF true statements is list of function_item_declaration else list of block_item_declaration.
			ast_range_or_type  *rot,               //!< Range or type.
			ast_identifier      identifier,        //!< Function name.
			ast_list           *item_declarations, //!< Internal variable declarations.
			ast_statement      *statements         //!< Executable statements.
			);

		/*!
	  @brief Creates and returns a new representation of a task or function
	  argument.
	  */
		ast_task_port * ast_new_task_port(
			ast_port_direction direction,   //!< Input or output to the port.
			bool        reg,         //!< Is is a registered value?
			bool        is_signed,   //!< Does it represent a signed value?
			ast_range        * range,       //!< Bit or item range for arrays.
			ast_task_port_type type,        //!< Data type (if any)
			ast_list         * identifiers //!< The list of port names.
			);

		/*!
	  @brief Creates and returns a new function item declaration.
	  @note All member fields must be filled out manaully. THis function just
	  ensures the memory is allocated properly.
	  */
		ast_function_item_declaration * ast_new_function_item_declaration();

		/*!
	  @brief Creates and returns a new task declaration statement.
	  */
		ast_task_declaration * ast_new_task_declaration(
			bool         automatic,      //!< Automatic iff TRUE
			ast_identifier      identifier,     //!< The task name.
			ast_list        *   ports,          //!< Arguments to the task.
			ast_list        *   declarations,   //!< Internal variable declarations.
			ast_statement   *   statements      //!< The body of the task.
			);

		/*!
	  @brief Creates and returns a new block register declaration descriptor.
	  */
		ast_block_reg_declaration * ast_new_block_reg_declaration(
			bool   is_signed,    //!< Do they represent signed values?
			ast_range   * range,        //!< Are these vectors of registers?
			ast_list    * identifiers   //!< list of reg names with same properties.
			);

		/*!
	  @brief Creates and returns a new block item declaration of the specified type.
	  @note Expects the relevant union member to be set manually.
	  */
		ast_block_item_declaration * ast_new_block_item_declaration(
			ast_block_item_declaration_type type, //!< The item type.
			ast_node_attributes             * attributes //!< Tool specific attributes.
			);

		/*!
	  @brief Simply returns the fully qualified representation of an identifier as
	  a string.
	  @details Where the identifier is "simple" or a system id, then the identifier
	  will just be returned as a character array. Where it is a hierarchical
	  idenifier, then a dot separated string of all identifiers in the hierarchy
	  will be returned.
	  @param [in] id - The identifier object to return a string representation of.
	  @returns A copy of the identifiers full name, as a null terminated character
	  array.
	  */
		char * ast_identifier_tostring(ast_identifier id);

		/*!
	  @brief Acts like strcmp but works on ast identifiers.
	  */
		int ast_identifier_cmp(
			ast_identifier a,
			ast_identifier b
			);

		/*!
	  @brief Creates and returns a new node representing an identifier.
	  @details By default, the returned identifier has the ID_UNKNOWN type,
	  and this is set later when the parser winds back up and knows which rules
	  to follow.
	  Also, the is_system member is set to false. If you want a new system
	  idenifier instance, use the @ref ast_new_system_identifier function.
	  */
		ast_identifier ast_new_identifier(
			QString        identifier,  //!< String text of the identifier.
			unsigned int   from_line    //!< THe line the idenifier came from.
			);


		/*!
	  @brief Creates and returns a new node representing an identifier.
	  @details By default, the returned identifier has the ID_SYSTEM_* type,
	  */
		ast_identifier ast_new_system_identifier(
			char         * identifier,  //!< String text of the identifier.
			unsigned int   from_line    //!< THe line the idenifier came from.
			);

		/*!
	  @brief Used to construct linked lists of hierarchical identifiers.
	  @details The child node is linked to the next field of the parent,
	  and the parent field returned.
	  @param [in] child - The child to add to the hierarchy.
	  @param [inout] parent - The parent identifier.
	  */
		ast_identifier ast_append_identifier(
			ast_identifier parent,
			ast_identifier child
			);

		/*!
	  @brief Used to set the range field of an identifier.
	  @param [inout] id - The identifier to set the range for.
	  @param [in] range - The range the identifier refers to.
	  @post Also sets the range_or_idx member to ID_HAS_RANGE
	  */
		void ast_identifier_set_range(
			ast_identifier    id,
			ast_range       * range
			);

		/*!
	  @brief Used to set the index field of an identifier.
	  @param [inout] id - The identifier to set the index for.
	  @param [in] index - The index the identifier refers to.
	  @post Also sets the range_or_idx member to ID_HAS_INDEX
	  */
		void ast_identifier_set_index(
			ast_identifier    id,
			ast_expression  * index
			);

		/*! @} */

		//! Creates and returns a new library description object.
		ast_library_descriptions * ast_new_library_description(
			ast_library_item_type type
			);
		/*!
	  @brief Creates and returns a new module item descriptor.
	  @param [in] attributes - Tool specific attributes.
	  @param [in] type - What sort of module item is being represented?
	  @note Expects the relevant union member to be set based on the type manually.
	  */
		ast_module_item * ast_new_module_item(
			ast_node_attributes * attributes,
			ast_module_item_type  type
			);

		/*! @} */

		/*!
	  @brief Takes a body statement (type = STM_BLK) and splits it into it's event
	  trigger and statements.
	  */
		ast_statement_block * ast_extract_statement_block(
			ast_statement_type  type,
			ast_statement     * body
			);

		/*!
	  @brief Creates a new module instantiation.
	  @param [in] attributes - Tool specific attributes.
	  @param [in] identifier - The full module name
	  @param [in] parameters - List of parameters to the module
	  @param [in] ports      - List of module ports.
	  @param [in] constructs - The internal constructs such as tasks, procedures and
							   internal instantiations.
	  */
		ast_module_declaration * ast_new_module_declaration(
			ast_node_attributes * attributes,
			ast_identifier        identifier,
			ast_list            * parameters,
			ast_list            * ports,
			ast_list            * constructs
			);

		/*!
	  @brief Creates and returns a new source item representation.
	  @param [in] type - The type of the source item to be represented.
	  @note Expects the union member of the returned ast_source_item to be
	  set manually.
	  */
		ast_source_item * ast_new_source_item(ast_source_item_type type);

		/*!
	  @brief Creates a new number representation object.
	  */
		ast_number * ast_new_number(
			ast_number_base base,   //!< What is the base of the number.
			ast_number_representation representation,   //!< How to interepret digits.
			char  * digits  //!< The string token representing the number.
			);

		/*!
	  @brief A utility function for converting an ast number into a string.
	  @param [in] n - The number to turn into a string.
	  */
		char * ast_number_tostring(
			ast_number * n
			);

		/*!
	  @brief Creates and returns a new, empty source tree.
	  @details This should be called ahead of parsing anything, so we will
	  have an object to put parsed constructs into.
	  */
		verilog_source_tree * verilog_new_source_tree();

		/*!
	  @brief Releases a source tree object from memory.
	  @details Frees the top level source tree object, and all of it's child
	  ast_* objects by calling the ast_free_all function.
	  @param [in] tofree - The source tree to be free'd
	  */
		void verilog_free_source_tree(
			verilog_source_tree * tofree
			);

		ast_node * ast_node_new();

		//! Duplicates the supplied null terminated string.
		char * ast_strdup(char * in);
		char * ast_strdup(QString in);

		//! Iterates over all allocated memory and frees it.
		void ast_free_all();

		/*!
	  @brief A simple wrapper around calloc.
	  @details This function is identical to calloc, but uses the head and
	  walker variables above to keep a linked list of all heap memory that the
	  AST construction allocates. This makes it very easy to clean up afterward
	  using the @ref ast_free_all function.
	  @param [in] num - Number of elements to allocate space for.
	  @param [in] size - The size of each element being allocated.
	  @returns A pointer to the start of the block of memory allocated.
	  */
		void * ast_calloc(size_t num, size_t size);

		/*!
	  @brief Creates and returns a pointer to a new linked list.
	  */
		ast_list * ast_list_new ();

		/*!
	  @brief Frees the memory of the supplied linked list.
	  @note Does not free the memory of the data elements in the list, only
	  the list construct itself.
	  */
		void       ast_list_free(ast_list * list);

		/*!
	  @brief Adds a new item to the end of a linked list.
	  */
		void       ast_list_append(ast_list * list, const void * data);

		/*!
	  @brief Adds a new item to the front of a linked list.
	  */
		void       ast_list_preappend(ast_list * list, void * data);

		/*!
	  @brief Finds and returns the i'th item in the linked list.
	  @details Returns a void* pointer. The programmer must be sure to cast this
	  as the correct type.
	  */
		const void *    ast_list_get(ast_list * list, unsigned int item);

		/*!
	  @brief Removes the i'th item from a linked list.
	  */
		void      ast_list_remove_at(ast_list * list, unsigned int i);

		/*!
	  @brief concatenates the two supplied lists into one.
	  @param head - This will form the "front" of the new list.
	  @param tail - This will form the "end" of the new list.
	  @details This function takes all the elements in tail and appends them
	  to those in head. The tail argument is then released from memory, and the
	  original head pointer is returned, with all data items still in tact.
	  */
		ast_list *    ast_list_concat(ast_list * head, ast_list * tail);


		/*!
	  @brief Searches the list, returning true or false if the data item supplied is
	  contained within it.
	  @details Performs a *pointer* comparison. That is, if the internal list
	  pointer has the same address as the supplied data pointer, the item is
	  considered to be found.
	  */
		int ast_list_contains(
			ast_list * list,
			void * data
			);

		/*!
	  @brief Creates and returns a new stack object.
	  */
		ast_stack * ast_stack_new();

		/*!
	  @brief Free the stack, but not it's contents
	  */
		void ast_stack_free(ast_stack * stack);

		/*!
	  @brief Push a new item to the top of the stack.
	  @param [inout] stack - The stack to push to.
	  @param [in]    item  - The thing to push onto the stack.
	  */
		void ast_stack_push(
			ast_stack * stack,
			void      * item
			);

		/*!
	  @brief Pop the top item from the top of the stack.
	  @param [inout] stack - The stack to pop from.
	  */
		void * ast_stack_pop(
			ast_stack * stack
			);

		/*!
	  @brief Peek at the top item on the top of the stack.
	  @param [inout] stack - The stack to peek at
	  */
		void * ast_stack_peek(
			ast_stack * stack
			);

		/*!
	  @brief Peek at the item *below* the top item on the top of the stack.
	  @param [inout] stack - The stack to peek into
	  */
		void * ast_stack_peek2(
			ast_stack * stack
			);

		//! Creates and returns a new hashtable.
		ast_hashtable * ast_hashtable_new();

		//! Frees an existing hashtable, but not it's contents, only the structure.
		void  ast_hashtable_free(
			ast_hashtable * table  //!< The table to free.
			);

		//! Inserts a new item into the hashtable.
		ast_hashtable_result ast_hashtable_insert(
			ast_hashtable * table, //!< The table to insert into.
			char          * key,   //!< The key to insert with.
			void          * value  //!< The data being added.
			);

		//! Returns an item from the hashtable.
		ast_hashtable_result ast_hashtable_get(
			ast_hashtable * table, //!< The table to fetch from.
			char          * key,   //!< The key of the data to fetch.
			void         ** value  //!< [out] The data being returned.
			);

		//! Removes a key value pair from the hashtable.
		ast_hashtable_result ast_hashtable_delete(
			ast_hashtable * table, //!< The table to delete from.
			char          * key    //!< The key to delete.
			);

		//! Updates an existing item in the hashtable.
		ast_hashtable_result ast_hashtable_update(
			ast_hashtable * table, //!< The table to update.
			char          * key,   //!< The key to update with.
			void          * value  //!< The new data item to update.
			);

			/*!
		@defgroup ast-utility-modules Module Resoloution & Searching
		@{
		@brief Functions for resolving module names.
		@ingroup ast-utility
		*/

			/*!
		@brief searches across an entire verilog source tree, resolving module
		identifiers to their declarations.
		*/
			void verilog_resolve_modules(
					verilog_source_tree * source
					);

			/*!
		@brief Searches the list of modules in the parsed source tree, returning the
		one that matches the passed identifer.
		@returns The matching module declaration, or NULL if no such declaration
		exists.
		*/
			ast_module_declaration * verilog_find_module_declaration(
					verilog_source_tree * source,
					ast_identifier module_name
					);

			// -------------------------------------------------------------------------


			/*!
		@brief Returns a list of module declarations, representing the different types
		of module which this parent instantiates.
		@details Each child module will appear once in the returned list. That is, if
		a register instantiates eight flip flop modules, then the flip flop module
		will appear only once in the returned list.
		@returns a list of elements of type ast_module_declaration.
		@pre The verilog_resolve_modules function has been called on the source tree
		to which the passed module belongs.
		@see verilog_modules_get_children
		*/
			ast_list * verilog_module_get_children(
					ast_module_declaration * module
					);


			/*!
		@brief Finds the child modules for all modules in a source tree.
		@returns A hash table, keyed by the module identifiers, of lists of
		module children.
		@pre The verilog_resolve_modules function has been called on the source tree
		to which the passed module belongs.
		@see verilog_module_get_children
		*/
			ast_hashtable * verilog_modules_get_children(
					verilog_source_tree * source
					);

	/*! @} */

	//! Creates and returns a new default net type directive.
		verilog_default_net_type * verilog_new_default_net_type(
			unsigned int token_number,  //!< Token number of the directive.
			unsigned int line_number,   //!< Line number of the directive.
			ast_net_type type           //!< The net type.
			);

		/*!
	  @brief Instructs the preprocessor to register a new macro definition.
	  */
		void verilog_preprocessor_macro_define(
			unsigned int line,  //!< The line the defininition comes from.
			char * macro_name,  //!< The macro identifier.
			char * macro_text,  //!< The value the macro expands to.
			size_t text_len     //!< Length in bytes of macro_text.
			);

		/*!
	  @brief Removes a macro definition from the preprocessors lookup table.
	  */
		void verilog_preprocessor_macro_undefine(
			char * macro_name //!< The name of the macro to remove.
			);

		//! Creates and returns a new conditional context.
		verilog_preprocessor_conditional_context *
		verilog_preprocessor_new_conditional_context(
			char        * condition,          //!< The definition to check for.
			int           line_number         //!< Where the `ifdef came from.
			);

		/*!
	  @brief Handles an ifdef statement being encountered.
	  @param [in] macro_name - The macro to test if defined or not.
	  @param [in] lineno - The line the directive occurs on.
	  @param [in] is_ndef - TRUE IFF the directive is `ifndef. Else the directive
	  is `ifdef and this should be FALSE.
	  */
		void verilog_preprocessor_ifdef (
			char * macro_name,
			unsigned int lineno,    //!< line number of the directive.
			bool is_ndef     //!< Is this an ifndef or ifdef directive.
			);

		/*!
	  @brief Handles an elseif statement being encountered.
	  @param [in] macro_name - The macro to test if defined or not.
	  @param [in] lineno - The line the directive occurs on.
	  */
		void verilog_preprocessor_elseif(char * macro_name, unsigned int lineno);

		/*!
	  @brief Handles an else statement being encountered.
	  @param [in] lineno - The line the directive occurs on.
	  */
		void verilog_preprocessor_else  (unsigned int lineno);

		/*!
	  @brief Handles an else statement being encountered.
	  @param [in] lineno - The line the directive occurs on.
	  */
		void verilog_preprocessor_endif (unsigned int lineno);

		/*!
	  @brief Creates a new pre-processor context.
	  @details This is called *once* at the beginning of a parse call.
	  */
		verilog_preprocessor_context * verilog_new_preprocessor_context();

		/*!
	  @brief Clears the stack of files being parsed, and sets the current file to
	  the supplied string.
	  @param [inout] preproc - The context who's file name is being set.
	  @param [in] file - The file path to put as the current file.
	  */
		void verilog_preprocessor_set_file(
			verilog_preprocessor_context * preproc,
			char * file
			);

		/*!
	  @brief Returns the file currently being parsed by the context, or NULL
	  @param [in] preproc - The context to get the current file for.
	  */
		char * verilog_preprocessor_current_file(
			verilog_preprocessor_context * preproc
			);

		/*!
	  @brief Frees a preprocessor context and all child constructs.
	  */
		void verilog_free_preprocessor_context(
			verilog_preprocessor_context * tofree
			);


		/*!
	  @brief Tells the preprocessor we are now defining PLI modules and to tag
			 them as such.
	  */
		void verilog_preproc_enter_cell_define();


		/*!
	  @brief Tells the preprocessor we are no longer defining PLI modules.
	  */
		void verilog_preproc_exit_cell_define();

		/*!
	  @brief Registers a new default net type directive.
	  */
		void verilog_preproc_default_net(
			unsigned int token_number,  //!< Token number of the directive.
			unsigned int line_number,   //!< Line number of the directive.
			ast_net_type type           //!< The net type.
			);

		/*!
	  @brief Handles the encounter of a `resetall directive as described in annex
	  19.6 of the spec.
	  */
		void verilog_preprocessor_resetall();

		// ----------------------- Connected Drive Directives -------------------

		/*!
	  @brief Handles the entering of a no-unconnected drive directive.
	  @param [in] direction -  Where should an unconnected line be pulled?
	  */
		void verilog_preprocessor_nounconnected_drive(ast_primitive_strength direction);

		/*!
	  @brief Handles the encounter of an include directive.
	  @returns A pointer to the newly created directive reference.
	  */
		verilog_include_directive * verilog_preprocessor_include(
			char * filename,        //<! The file to include.
			unsigned int lineNumber //!< The line number of the directive.
			);


	};
}

#endif // VERILOGCODE_H
