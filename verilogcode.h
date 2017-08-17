#ifndef VERILOGCODE_H
#define VERILOGCODE_H

#include <fstream>
#include <sstream>
#include <ostream>

#include "verilog_ast.hh"
#include "verilog_preprocessor.hh"

namespace yy {
	class VerilogScanner;
	class VerilogCode
	{
	private:
		verilog_preprocessor_context * yy_preproc; // preprocessor

		/*!
		  @brief Global source tree object, used to store parsed constructs.
		  @details This is a global variable, initialised prior to calling
		  the verilog_parse function, into which all objects the parser finds are
		  stored.
		*/
		verilog_source_tree * yy_verilog_source_tree;

		/// enable debug output in the flex scanner
		bool trace_scanning;

		/// enable debug output in the bison parser
		bool trace_parsing;

	public:
		/// construct a new parser driver context
		VerilogCode();

		/// stream name (file or input stream) used for error messages.
		std::string streamname;

		/** Invoke the scanner and parser for a stream.
		 * @param in	input stream
		 * @param sname	stream name for error messages
		 * @return		true if successfully parsed
		 */
		bool parse_stream(std::istream& in, const std::string& sname = "stream input");

		/** Invoke the scanner and parser on an input string.
		 * @param input	input string
		 * @param sname	stream name for error messages
		 * @return		true if successfully parsed
		 */
		bool parse_string(const std::string& input, const std::string& sname = "string stream");

		/** Invoke the scanner and parser on a file. Use parse_stream with a
		 * std::ifstream if detection of file reading errors is required.
		 * @param filename	input file name
		 * @return		true if successfully parsed
		 */
		bool parse_file(const std::string& filename);

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

	protected:
		// Parser Wrapper
		void verilog_parser_init();
		int verilog_parse_buffer(char * to_parse, int length);
		int verilog_parse_string(char * to_parse, int length);
		int verilog_parse_file(FILE * to_parse);

		// AST functions
		void ast_set_meta_info(ast_metadata * meta);
		ast_node_attributes * ast_new_attributes(ast_identifier name, ast_expression * value);
		void ast_append_attribute(ast_node_attributes * parent, ast_node_attributes * toadd);
		ast_lvalue * ast_new_lvalue_id(ast_lvalue_type type, ast_identifier id);
		ast_lvalue * ast_new_lvalue_concat(ast_lvalue_type type, ast_concatenation*concat);
		ast_primary * ast_new_constant_primary(ast_primary_value_type type);
		ast_primary * ast_new_primary_function_call(ast_function_call * call);
		ast_primary * ast_new_primary(ast_primary_value_type type);
		ast_primary * ast_new_module_path_primary(ast_primary_value_type type);
		ast_expression * ast_new_unary_expression(ast_primary * operand, ast_operator operation, ast_node_attributes * attr, bool constant);
		ast_expression * ast_new_expression_primary(ast_primary * p);
		ast_expression * ast_new_range_expression(ast_expression * left, ast_expression * right);
		ast_expression * ast_new_index_expression(ast_expression * left);
		ast_expression * ast_new_binary_expression(ast_expression * left, ast_expression * right, ast_operator operation, ast_node_attributes * attr, bool constant);
		ast_expression * ast_new_string_expression(ast_string);
		ast_expression * ast_new_conditional_expression(ast_expression*, ast_expression*, ast_expression*, ast_node_attributes*);
		ast_expression * ast_new_mintypmax_expression(ast_expression*, ast_expression*, ast_expression*);
		char * ast_expression_tostring(ast_expression * exp);
		char * ast_operator_tostring(ast_operator op);
		char * ast_primary_tostring(ast_primary * p);
		ast_function_call * ast_new_function_call(ast_identifier, bool, bool, ast_node_attributes*, ast_list*);
		ast_concatenation * ast_new_concatenation(ast_concatenation_type, ast_expression*, void*);
		ast_concatenation * ast_new_empty_concatenation(ast_concatenation_type);
		ast_path_declaration * ast_new_path_declaration(ast_path_declaration_type);
		ast_simple_parallel_path_declaration * ast_new_simple_parallel_path_declaration(ast_identifier, ast_operator, ast_identifier, ast_list*);
		ast_simple_full_path_declaration * ast_new_simple_full_path_declaration(ast_list*, ast_operator, ast_list*, ast_list*);
		ast_edge_sensitive_parallel_path_declaration * ast_new_edge_sensitive_parallel_path_declaration(ast_edge, ast_identifier, ast_operator, ast_identifier, ast_expression*, ast_list*);
		ast_edge_sensitive_full_path_declaration * ast_new_edge_sensitive_full_path_declaration(ast_edge, ast_list*, ast_operator, ast_list*, ast_expression*, ast_list*);
		ast_task_enable_statement* ast_new_task_enable_statement(ast_list*, ast_identifier, bool);
		ast_loop_statement* ast_new_forever_loop_statement(ast_statement*);
		ast_loop_statement* ast_new_for_loop_statement(ast_statement*, ast_single_assignment*, ast_single_assignment*, ast_expression*);
		ast_loop_statement* ast_new_generate_loop_statement(ast_list*, ast_single_assignment*, ast_single_assignment*, ast_expression*);
		ast_loop_statement* ast_new_while_loop_statement(ast_statement*, ast_expression*);
		ast_loop_statement* ast_new_repeat_loop_statement(ast_statement*, ast_expression*);
		ast_case_item * ast_new_case_item(ast_list * conditions, ast_statement * body);
		ast_case_statement* ast_new_case_statement(ast_expression*, ast_list*, ast_case_statement_type);
		ast_conditional_statement* ast_new_conditional_statement(ast_statement*, ast_expression*);
		ast_if_else* ast_new_if_else(ast_conditional_statement*, ast_statement*);
		ast_wait_statement* ast_new_wait_statement(ast_expression*, ast_statement*);
		ast_event_control* ast_new_event_control(ast_event_control_type, ast_event_expression*);
		ast_delay_ctrl * ast_new_delay_ctrl_value(ast_delay_value * value);
		ast_delay_ctrl * ast_new_delay_ctrl_mintypmax(ast_expression*);
		ast_timing_control_statement* ast_new_timing_control_statement_delay(ast_timing_control_statement_type, ast_statement*, ast_delay_ctrl*);
		ast_timing_control_statement* ast_new_timing_control_statement_event(ast_timing_control_statement_type, ast_expression*, ast_statement*, ast_event_control*);
		ast_single_assignment* ast_new_single_assignment(ast_lvalue*, ast_expression*);
		ast_assignment* ast_new_hybrid_assignment(ast_hybrid_assignment_type, ast_single_assignment*);
		ast_assignment* ast_new_hybrid_lval_assignment(ast_hybrid_assignment_type, ast_lvalue*);
		ast_assignment* ast_new_blocking_assignment(ast_lvalue*, ast_expression*, ast_timing_control_statement*);
		ast_assignment* ast_new_nonblocking_assignment(ast_lvalue*, ast_expression*, ast_timing_control_statement*);
		ast_assignment* ast_new_continuous_assignment(ast_list*, ast_drive_strength*, ast_delay3*);
		ast_statement_block* ast_new_statement_block(ast_block_type, ast_identifier, ast_list*, ast_list*);
		ast_disable_statement * ast_new_disable_statement(ast_identifier id);
		ast_statement * ast_new_statement(ast_node_attributes*, bool, void*, ast_statement_type);
		ast_udp_port* ast_new_udp_port(ast_port_direction, ast_identifier, ast_node_attributes*, bool, ast_expression*);
		ast_udp_port* ast_new_udp_input_port(ast_list*, ast_node_attributes*);
		ast_udp_declaration* ast_new_udp_declaration(ast_node_attributes*, ast_identifier, ast_list*, ast_udp_body*);
		ast_udp_instance* ast_new_udp_instance(ast_identifier, ast_range*, ast_lvalue*, ast_list*);
		ast_udp_instantiation* ast_new_udp_instantiation(ast_list*, ast_identifier, ast_drive_strength*, ast_delay2*);
		ast_udp_initial_statement* ast_new_udp_initial_statement(ast_identifier, ast_number*);
		ast_udp_body* ast_new_udp_sequential_body(ast_udp_initial_statement*, ast_list*);
		ast_udp_body* ast_new_udp_combinatoral_body(ast_list*);
		ast_udp_combinatorial_entry* ast_new_udp_combinatoral_entry(ast_list*, ast_udp_next_state);
		ast_udp_sequential_entry* ast_new_udp_sequential_entry(ast_udp_seqential_entry_prefix, ast_list*, ast_level_symbol, ast_udp_next_state);
		ast_statement* ast_new_generate_item(ast_statement_type, void*);
		ast_generate_block* ast_new_generate_block(ast_identifier, ast_list*);
		ast_module_instantiation* ast_new_module_instantiation(ast_identifier, ast_list*, ast_list*);
		ast_module_instance* ast_new_module_instance(ast_identifier, ast_list*);
		ast_port_connection* ast_new_named_port_connection(ast_identifier, ast_expression*);
		ast_switch_gate* ast_new_switch_gate_d3(ast_switchtype, ast_delay3*);
		ast_switch_gate* ast_new_switch_gate_d2(ast_switchtype, ast_delay2*);
		ast_primitive_pull_strength* ast_new_primitive_pull_strength(ast_pull_direction, ast_primitive_strength, ast_primitive_strength);
		ast_pull_gate_instance* ast_new_pull_gate_instance(ast_identifier, ast_lvalue*);
		ast_pass_switch_instance* ast_new_pass_switch_instance(ast_identifier, ast_lvalue*, ast_lvalue*);
		ast_n_input_gate_instance* ast_new_n_input_gate_instance(ast_identifier, ast_list*, ast_lvalue*);
		ast_enable_gate_instance* ast_new_enable_gate_instance(ast_identifier, ast_lvalue*, ast_expression*, ast_expression*);
		ast_mos_switch_instance* ast_new_mos_switch_instance(ast_identifier, ast_lvalue*, ast_expression*, ast_expression*);
		ast_cmos_switch_instance* ast_new_cmos_switch_instance(ast_identifier, ast_lvalue*, ast_expression*, ast_expression*, ast_expression*);
		ast_pass_enable_switch* ast_new_pass_enable_switch(ast_identifier, ast_lvalue*, ast_lvalue*, ast_expression*);
		ast_pass_enable_switches* ast_new_pass_enable_switches(ast_pass_enable_switchtype, ast_delay2*, ast_list*);
		ast_n_input_gate_instances* ast_new_n_input_gate_instances(ast_gatetype_n_input, ast_delay3*, ast_drive_strength*, ast_list*);
		ast_enable_gate_instances* ast_new_enable_gate_instances(ast_gatetype_n_input, ast_delay3*, ast_drive_strength*, ast_list*);
		ast_n_output_gate_instance* ast_new_n_output_gate_instance(ast_identifier, ast_list*, ast_expression*);
		ast_n_output_gate_instances* ast_new_n_output_gate_instances(ast_n_output_gatetype, ast_delay2*, ast_drive_strength*, ast_list*);
		ast_switches * ast_new_switches(ast_switch_gate * type, ast_list * switches);
		ast_pull_strength* ast_new_pull_stregth(ast_primitive_strength, ast_primitive_strength);
		ast_gate_instantiation * ast_new_gate_instantiation(ast_gate_type type);
		ast_parameter_declarations* ast_new_parameter_declarations(ast_list*, bool, bool, ast_range*, ast_parameter_type);
		ast_port_declaration* ast_new_port_declaration(ast_port_direction, ast_net_type, bool, bool, bool, ast_range*, ast_list*);
		ast_type_declaration * ast_new_type_declaration(ast_declaration_type type);
		ast_list* ast_new_net_declaration(ast_type_declaration*);
		ast_list* ast_new_reg_declaration(ast_type_declaration*);
		ast_list* ast_new_var_declaration(ast_type_declaration*);
		ast_delay_value* ast_new_delay_value(ast_delay_value_type, void*);
		ast_delay3* ast_new_delay3(ast_delay_value*, ast_delay_value*, ast_delay_value*);
		ast_delay2* ast_new_delay2(ast_delay_value*, ast_delay_value*);
		ast_pulse_control_specparam* ast_new_pulse_control_specparam(ast_expression*, ast_expression*);
		ast_range* ast_new_range(ast_expression*, ast_expression*);
		ast_range_or_type * ast_new_range_or_type(bool is_range);
		ast_function_declaration* ast_new_function_declaration(bool, bool, bool, ast_range_or_type*, ast_identifier, ast_list*, ast_statement*);
		ast_function_item_declaration * ast_new_function_item_declaration();
		ast_task_port* ast_new_task_port(ast_port_direction, bool, bool, ast_range*, ast_task_port_type, ast_list*);
		ast_task_declaration* ast_new_task_declaration(bool, ast_identifier, ast_list*, ast_list*, ast_statement*);
		ast_block_reg_declaration* ast_new_block_reg_declaration(bool, ast_range*, ast_list*);
		ast_block_item_declaration* ast_new_block_item_declaration(ast_block_item_declaration_type, ast_node_attributes*);
		ast_module_item* ast_new_module_item(ast_node_attributes*, ast_module_item_type);
		ast_statement_block* ast_extract_statement_block(ast_statement_type, ast_statement*);
		ast_module_declaration* ast_new_module_declaration(ast_node_attributes*, ast_identifier, ast_list*, ast_list*, ast_list*);
		ast_source_item * ast_new_source_item(ast_source_item_type type);
		int ast_identifier_cmp(ast_identifier, ast_identifier);
		ast_identifier_t* ast_new_identifier(char*, unsigned int);
		ast_identifier_t* ast_new_system_identifier(char*, unsigned int);
		ast_identifier_t* ast_append_identifier(ast_identifier, ast_identifier);
		ast_config_rule_statement* ast_new_config_rule_statement(bool, ast_identifier, ast_identifier);
		ast_config_declaration* ast_new_config_declaration(ast_identifier, ast_identifier, ast_list*);
		ast_library_declaration* ast_new_library_declaration(ast_identifier, ast_list*, ast_list*);
		ast_library_descriptions* ast_new_library_description(ast_library_item_type);
		ast_number* ast_new_number(ast_number_base, ast_number_representation, char*);
		char* ast_number_tostring(ast_number*);
		verilog_source_tree * verilog_new_source_tree();
		void verilog_free_source_tree(verilog_source_tree*);
	};
}

#endif // VERILOGCODE_H
