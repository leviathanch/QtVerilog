%option prefix="verilog"
%option yylineno
%option nodefault
%option noyywrap

%top {
#include "verilog.tab.hh"
#include "verilog_ast.h"
#include "verilog_preprocessor.h"
#include "asttree.h"

#define EMIT_TOKEN(x) return x;
}

/* Pre-processor definitions */
CD_DEFAULT_NETTYPE     "`default_nettype"
%x in_default_nettype

CD_LINE                "`line"
%x in_line_1
%x in_line_2
%x in_line_3
%x in_line_4

CD_CELLDEFINE          "`celldefine"
CD_DEFINE              "`define"
CD_RESETALL            "`resetall"
CD_ENDCELLDEFINE       "`endcelldefine"

CD_ELSE                "`else"
CD_ELSIF               "`elsif"
CD_ENDIF               "`endif"
CD_IFDEF               "`ifdef"
CD_IFNDEF              "`ifndef"

%x in_ifdef
%x in_ifndef
%x in_elseif

CD_UNDEF               "`undef"

%x in_undef

CD_NOUNCONNECTED_DRIVE "`nounconnected_drive"
CD_UNCONNECTED_DRIVE   "`unconnected_drive"

%x in_unconnected_drive

/* Include Directives */

CD_INCLUDE             "`include"

%x in_include

/* Times and compiler directives */

CD_TIMESCALE           "`timescale"
TIME_UNITS             "(s|ms|us|ns|ps|fs)"
%x in_ts_1
%x in_ts_2
%x in_ts_3

/* Keyword Token deffinitions. */
ALWAYS              "always"
AND                 "and"
ASSIGN              "assign"
AUTOMATIC           "automatic"
BEGIN               "begin"
BUF                 "buf"
BUFIF0              "bufif0"
BUFIF1              "bufif1"
CASE                "case"
CASEX               "casex"
CASEZ               "casez"
CELL                "cell"
CMOS                "cmos"
CONFIG              "config"
DEASSIGN            "deassign"
DEFAULT             "default"
DEFPARAM            "defparam"
DESIGN              "design"
DISABLE             "disable"
EDGE                "edge"
ELSE                "else"
END                 "end"
ENDCASE             "endcase"
ENDCONFIG           "endconfig"
ENDFUNCTION         "endfunction"
ENDGENERATE         "endgenerate"
ENDMODULE           "endmodule"
ENDPRIMITIVE        "endprimitive"
ENDSPECIFY          "endspecify"
ENDTABLE            "endtable"
ENDTASK             "endtask"
EVENT               "event"
FOR                 "for"
FORCE               "force"
FOREVER             "forever"
FORK                "fork"
FUNCTION            "function"
GENERATE            "generate"
GENVAR              "genvar"
HIGHZ0              "highz0"
HIGHZ1              "highz1"
IF                  "if"
IFNONE              "ifnone"
INCDIR              "-incdir"
INCLUDE             "include"
INITIAL             "initial"
INOUT               "inout"
INPUT               "input"
INSTANCE            "instance"
INTEGER             "integer"
JOIN                "join"
LARGE               "large"
LIBLIST             "liblist"
LIBRARY             "library"
LOCALPARAM          "localparam"
MACROMODULE         "macromodule"
MEDIUM              "medium"
MODULE              "module"
NAND                "nand"
NEGEDGE             "negedge"
NMOS                "nmos"
NOR                 "nor"
NOSHOWCANCELLED     "noshowcancelled"
NOT                 "not"
NOTIF0              "notif0"
NOTIF1              "notif1"
OR                  "or"
OUTPUT              "output"
PARAMETER           "parameter"
PMOS                "pmos"
POSEDGE             "posedge"
PRIMITIVE           "primitive"
PULL0               "pull0"
PULL1               "pull1"
PULLDOWN            "pulldown"
PULLUP              "pullup"
PULSESTYLE_ONEVENT  "pulsestyle_onevent"
PULSESTYLE_ONDETECT "pulsestyle_ondetect"
RCMOS               "rcmos"
REAL                "real"
REALTIME            "realtime"
REG                 "reg"
RELEASE             "release"
REPEAT              "repeat"
RNMOS               "rnmos"
RPMOS               "rpmos"
RTRAN               "rtran"
RTRANIF0            "rtranif0"
RTRANIF1            "rtranif1"
SCALARED            "scalared"
SHOWCANCELLED       "showcancelled"
SIGNED              "signed"
SMALL               "small"
SPECIFY             "specify"
SPECPARAM           "specparam"
STRONG0             "strong0"
STRONG1             "strong1"
SUPPLY0             "supply0"
SUPPLY1             "supply1"
TABLE               "table"
TASK                "task"
TIME                "time"
TRAN                "tran"
TRANIF0             "tranif0"
TRANIF1             "tranif1"
TRI0                "tri0"
TRI1                "tri1"
TRI                 "tri"
TRIAND              "triand"
TRIOR               "trior"
TRIREG              "trireg"
UNSIGNED            "unsigned"
USE                 "use"
VECTORED            "vectored"
WAIT                "wait"
WAND                "wand"
WEAK0               "weak0"
WEAK1               "weak1"
WHILE               "while"
WIRE                "wire"
WOR                 "wor"
XNOR                "xnor"
XOR                 "xor"

/* Single character tokens */

NEWLINE             "\n"|"\r\n"
SPACE               " "
TAB                 "\t"

AT                  "@"
COMMA               ","
HASH                "#"
DOT                 "."
EQ                  "="
COLON               ":"
IDX_PRT_SEL         "+:"|"-:"
SEMICOLON           ";"
OPEN_BRACKET        "\("
CLOSE_BRACKET       "\)"
OPEN_SQ_BRACKET     "\["
CLOSE_SQ_BRACKET    "\]"
OPEN_SQ_BRACE       "{"
CLOSE_SQ_BRACE      "}"


/* Tokens related to numbers */

EXP                 "e"|"E"
UNDERSCORE          "_"
SIGN                {PLUS}|{MINUS}
X                   "x"|"X"
Z                   "z"|"Z"|"?"

DIGIT_DECIMAL       [0-9]
DIGIT_DECMIAL_NZ    [1-9]
DIGIT_BINARY        [0-1]|{X}|{Z}
DIGIT_OCTAL         [0-7]|{X}|{Z}
DIGIT_HEX           [0-9a-fA-F]|{X}|{Z}

BASE_DECIMAL        '[sS]?[dD]
BASE_BINARY         '[sS]?[bB]
BASE_OCTAL          '[sS]?[oO]
BASE_HEX            '[sS]?[hH]

NUM_REAL_EXP        {NUM_UNSIGNED}({DOT}{NUM_UNSIGNED})?{EXP}({SIGN})?{NUM_UNSIGNED}

BIN_VALUE           {DIGIT_BINARY}({UNDERSCORE}|{DIGIT_BINARY})*
OCT_VALUE           {DIGIT_OCTAL}({UNDERSCORE}|{DIGIT_OCTAL})*
HEX_VALUE           {DIGIT_HEX}({UNDERSCORE}|{DIGIT_HEX})*

%x in_hex_val
%x in_oct_val
%x in_bin_val
%s in_number

NUM_REAL            {NUM_UNSIGNED}{DOT}{NUM_UNSIGNED}|{NUM_REAL_EXP}
NUM_UNSIGNED        {DIGIT_DECIMAL}({UNDERSCORE}|{DIGIT_DECIMAL})*

/* Identifiers */

SYSTEM_ID           \$[a-zA-Z0-9_\$]+
SIMPLE_ID           [a-zA-Z_][a-zA-Z0-9_$]*
ESCAPED_ID          \\{SIMPLE_ID}
MACRO_IDENTIFIER    `{SIMPLE_ID}

MACRO_TEXT          .*\n

%x in_define
%x in_define_t

/* Attributes */

ATTRIBUTE_START     \(\*
ATTRIBUTE_END       \*\)

/* Comments */
COMMENT_LINE        "//".*\n

COMMENT_BEGIN       "/*"
COMMENT_END         "*/"

%x in_comment

/* Strings */

STRING              \".*\"

/* Operators */

STAR                "\*"
PLUS                "+"
MINUS               "-"
ASL                 "<<<"
ASR                 ">>>"
LSL                 "<<"
LSR                 ">>"
DIV                 "/"
POW                 "**"
MOD                 "%"
GTE                 ">="
LTE                 "<="
GT                  ">"
LT                  "<"
L_NEG               "!"
L_AND               "&&"
L_OR                "||"
C_EQ                "==="
L_EQ                "=="
C_NEQ               "!=="
L_NEQ               "!="
B_NEG               "~"
B_AND               "&"
B_OR                "|"
B_XOR               "^"
B_EQU               "^~"|"~^"
B_NAND              "~&"
B_NOR               "~|"
TERNARY             "?"

%%
{ATTRIBUTE_START}      {EMIT_TOKEN(verilog::parser::token::ATTRIBUTE_START);}
{ATTRIBUTE_END}        {EMIT_TOKEN(verilog::parser::token::ATTRIBUTE_END);}

{COMMENT_LINE}         {/*EMIT_TOKEN(verilog::parser::token::COMMENT_LINE); IGNORE */}
{COMMENT_BEGIN}        {BEGIN(in_comment);                    ;}

<in_comment>.|\n       {/* IGNORE                            */}
<in_comment>{COMMENT_END} {BEGIN(INITIAL);                     }

{CD_CELLDEFINE}          {verilog_preproc_enter_cell_define();}
{CD_ENDCELLDEFINE}       {verilog_preproc_exit_cell_define();}

{CD_DEFAULT_NETTYPE}     {BEGIN(in_default_nettype);}
<in_default_nettype>{TRIAND}  {
    BEGIN(INITIAL);
    verilog_preproc_default_net(yy_preproc -> token_count,
        yylineno, NET_TYPE_TRIAND );
    }
<in_default_nettype>{TRIOR}   {
    BEGIN(INITIAL);
    verilog_preproc_default_net(yy_preproc -> token_count,
        yylineno, NET_TYPE_TRIOR  );
    }
<in_default_nettype>{TRIREG}     {
    BEGIN(INITIAL);
    verilog_preproc_default_net(yy_preproc -> token_count,
        yylineno, NET_TYPE_TRIREG );
    }
<in_default_nettype>{TRI0}     {
    BEGIN(INITIAL);
    verilog_preproc_default_net(yy_preproc -> token_count,
        yylineno, NET_TYPE_TRI    );
    }
<in_default_nettype>{TRI}     {
    BEGIN(INITIAL);
    verilog_preproc_default_net(yy_preproc -> token_count,
        yylineno, NET_TYPE_TRI    );
    }
<in_default_nettype>{WIRE}    {
    BEGIN(INITIAL);
    verilog_preproc_default_net(yy_preproc -> token_count,
        yylineno, NET_TYPE_WIRE   );
    }
<in_default_nettype>{WAND}    {
    BEGIN(INITIAL);
    verilog_preproc_default_net(yy_preproc -> token_count,
        yylineno, NET_TYPE_WAND   );
    }
<in_default_nettype>{WOR}     {
    BEGIN(INITIAL);
    verilog_preproc_default_net(yy_preproc -> token_count,
        yylineno, NET_TYPE_WOR    );
    }

{CD_TIMESCALE}           {
    BEGIN(in_ts_1);
}
<in_ts_1>{NUM_UNSIGNED}      {
	yy_preproc -> timescale.scale = yylval.string;
}
<in_ts_1>{SIMPLE_ID}         {
    BEGIN(in_ts_2);
}
<in_ts_2>{DIV}               {
    BEGIN(in_ts_3);
}
<in_ts_3>{NUM_UNSIGNED}      {
	yy_preproc -> timescale.precision= yylval.string;
}
<in_ts_3>{SIMPLE_ID}         {
    BEGIN(INITIAL);
}

{CD_RESETALL}            {
    verilog_preprocessor_resetall();
}

{CD_IFDEF}               {
    BEGIN(in_ifdef);
}
<in_ifdef>{SIMPLE_ID}    {
	verilog_preprocessor_ifdef(yytext,yylineno,false);
    BEGIN(INITIAL);
}

{CD_IFNDEF}              {
    BEGIN(in_ifndef);
}
<in_ifndef>{SIMPLE_ID}   {
	verilog_preprocessor_ifdef(yytext,yylineno,true);
    BEGIN(INITIAL);
}

{CD_ELSIF}               {
    BEGIN(in_elseif);
}
<in_elseif>{SIMPLE_ID}   {
    verilog_preprocessor_elseif(yytext, yylineno);
    BEGIN(INITIAL);
}

{CD_ELSE}                {
    verilog_preprocessor_else(yylineno);
}

{CD_ENDIF}               {
    verilog_preprocessor_endif(yylineno);
}

{CD_INCLUDE}             {
    BEGIN(in_include);
}
<in_include>{STRING}     {
    YY_BUFFER_STATE cur = YY_CURRENT_BUFFER;

    verilog_include_directive * id =
        verilog_preprocessor_include(yytext,yylineno);

    // Now, we need to look for the file, open it as a buffer, and then
    // switch to it.

	if(id -> file_found == true)
    {
        FILE * file = fopen(id -> filename, "r");

        YY_BUFFER_STATE n   = yy_create_buffer(file, YY_BUF_SIZE);

	cur -> yy_bs_lineno = yylineno;
	yy_switch_to_buffer(cur);
	yypush_buffer_state(n);
	BEGIN(INITIAL);
    }
    else
    {
        printf("ERROR - Could not find include file %s on line %d\n",
            id -> filename, id-> lineNumber);
        printf("\tExpect stuff to break now.\n");
    }

    BEGIN(INITIAL);
}

{CD_LINE}                 {BEGIN(in_line_1);}
<in_line_1>{INTEGER}      {BEGIN(in_line_2);}
<in_line_1>{NUM_UNSIGNED} {BEGIN(in_line_2);}
<in_line_2>{STRING}       {BEGIN(in_line_3);}
<in_line_3>{INTEGER}      {BEGIN(INITIAL);}
<in_line_3>{NUM_UNSIGNED} {BEGIN(INITIAL);}

{CD_NOUNCONNECTED_DRIVE} {
    verilog_preprocessor_nounconnected_drive(STRENGTH_NONE);
}
{CD_UNCONNECTED_DRIVE}   {
    BEGIN(in_unconnected_drive);
}
<in_unconnected_drive>{PULL0} {
    verilog_preprocessor_nounconnected_drive(STRENGTH_PULL0);
    BEGIN(INITIAL);
}
<in_unconnected_drive>{PULL1} {
    verilog_preprocessor_nounconnected_drive(STRENGTH_PULL1);
    BEGIN(INITIAL);
}

{CD_DEFINE}              {
    BEGIN(in_define);
}

<in_define>{SIMPLE_ID}   {
    yy_preproc -> scratch = ast_strdup(yytext);
    BEGIN(in_define_t);
}

<in_define_t>{MACRO_TEXT} {
    if(yyleng == 1)
    {
        // Macro has no value, and is just a newline character.
        verilog_preprocessor_macro_define(
            yylineno-1,
            yy_preproc -> scratch,
            NULL,
            0); // -1 to avoid including the newline.
    }
    else
    {
        // Macro has a proper value.
        verilog_preprocessor_macro_define(
            yylineno-1,
            yy_preproc -> scratch,
            yytext+1,
            yyleng-2); // -1 to avoid including the newline.
    }
    BEGIN(INITIAL);
}

{CD_UNDEF}             {
    BEGIN(in_undef);
}

<in_undef>{SIMPLE_ID}  {
    verilog_preprocessor_macro_undefine(
        yytext
    );
    BEGIN(INITIAL);
}

{MACRO_IDENTIFIER}     {

    // Look for the macro entry.
    verilog_macro_directive * macro = NULL;
    char * macroName = (yytext)+1;
    ast_hashtable_result r = ast_hashtable_get(yy_preproc -> macrodefines,
                                               macroName,
                                               (void**)&macro);

    if(r == HASH_SUCCESS)
    {
        // Switch buffers to expand the macro.

	YY_BUFFER_STATE cur = YY_CURRENT_BUFFER;
	YY_BUFFER_STATE n   = yy_scan_string(macro -> macro_value);

	yy_switch_to_buffer(cur);
	yypush_buffer_state(n);
    }
    else
    {
        // Undefined macro - PANIC!
        //printf("ERROR: Undefined macro '%s' on line %d\n", yytext, yylineno);
        //printf("\tIt's probably all going to fall apart now...\n\n");
    }
}

{AT}                   {EMIT_TOKEN(verilog::parser::token::AT);}
{COMMA}                {EMIT_TOKEN(verilog::parser::token::COMMA);}
{HASH}                 {EMIT_TOKEN(verilog::parser::token::HASH);}
{DOT}                  {EMIT_TOKEN(verilog::parser::token::DOT);}
{EQ}                   {yylval.operator = OPERATOR_L_EQ; EMIT_TOKEN(verilog::parser::token::EQ);}
{COLON}                {EMIT_TOKEN(verilog::parser::token::COLON);}
{IDX_PRT_SEL}          {EMIT_TOKEN(verilog::parser::token::IDX_PRT_SEL);}
{SEMICOLON}            {EMIT_TOKEN(verilog::parser::token::SEMICOLON);}
{OPEN_BRACKET}         {EMIT_TOKEN(verilog::parser::token::OPEN_BRACKET);}
{CLOSE_BRACKET}        {EMIT_TOKEN(verilog::parser::token::CLOSE_BRACKET);}
{OPEN_SQ_BRACKET}      {EMIT_TOKEN(verilog::parser::token::OPEN_SQ_BRACKET);}
{CLOSE_SQ_BRACKET}     {EMIT_TOKEN(verilog::parser::token::CLOSE_SQ_BRACKET);}
{OPEN_SQ_BRACE}        {EMIT_TOKEN(verilog::parser::token::OPEN_SQ_BRACE);}
{CLOSE_SQ_BRACE}       {EMIT_TOKEN(verilog::parser::token::CLOSE_SQ_BRACE);}
{STAR}                 {yylval.operator=OPERATOR_STAR   ; EMIT_TOKEN(verilog::parser::token::STAR);}
{PLUS}                 {yylval.operator=OPERATOR_PLUS   ; EMIT_TOKEN(verilog::parser::token::PLUS);}
{MINUS}                {yylval.operator=OPERATOR_MINUS  ; EMIT_TOKEN(verilog::parser::token::MINUS);}
{ASL}                  {yylval.operator=OPERATOR_ASL    ; EMIT_TOKEN(verilog::parser::token::ASL);}
{ASR}                  {yylval.operator=OPERATOR_ASR    ; EMIT_TOKEN(verilog::parser::token::ASR);}
{LSL}                  {yylval.operator=OPERATOR_LSL    ; EMIT_TOKEN(verilog::parser::token::LSL);}
{LSR}                  {yylval.operator=OPERATOR_LSR    ; EMIT_TOKEN(verilog::parser::token::LSR);}
{DIV}                  {yylval.operator=OPERATOR_DIV    ; EMIT_TOKEN(verilog::parser::token::DIV);}
{POW}                  {yylval.operator=OPERATOR_POW    ; EMIT_TOKEN(verilog::parser::token::POW);}
{MOD}                  {yylval.operator=OPERATOR_MOD    ; EMIT_TOKEN(verilog::parser::token::MOD);}
{GTE}                  {yylval.operator=OPERATOR_GTE    ; EMIT_TOKEN(verilog::parser::token::GTE);}
{LTE}                  {yylval.operator=OPERATOR_LTE    ; EMIT_TOKEN(verilog::parser::token::LTE);}
{GT}                   {yylval.operator=OPERATOR_GT     ; EMIT_TOKEN(verilog::parser::token::GT);}
{LT}                   {yylval.operator=OPERATOR_LT     ; EMIT_TOKEN(verilog::parser::token::LT);}
{L_NEG}                {yylval.operator=OPERATOR_L_NEG  ; EMIT_TOKEN(verilog::parser::token::L_NEG);}
{L_AND}                {yylval.operator=OPERATOR_L_AND  ; EMIT_TOKEN(verilog::parser::token::L_AND);}
{L_OR}                 {yylval.operator=OPERATOR_L_OR   ; EMIT_TOKEN(verilog::parser::token::L_OR);}
{C_EQ}                 {yylval.operator=OPERATOR_C_EQ   ; EMIT_TOKEN(verilog::parser::token::C_EQ);}
{L_EQ}                 {yylval.operator=OPERATOR_L_EQ   ; EMIT_TOKEN(verilog::parser::token::L_EQ);}
{C_NEQ}                {yylval.operator=OPERATOR_C_NEQ  ; EMIT_TOKEN(verilog::parser::token::C_NEQ);}
{L_NEQ}                {yylval.operator=OPERATOR_L_NEQ  ; EMIT_TOKEN(verilog::parser::token::L_NEQ);}
{B_NEG}                {yylval.operator=OPERATOR_B_NEG  ; EMIT_TOKEN(verilog::parser::token::B_NEG);}
{B_AND}                {yylval.operator=OPERATOR_B_AND  ; EMIT_TOKEN(verilog::parser::token::B_AND);}
{B_OR}                 {yylval.operator=OPERATOR_B_OR   ; EMIT_TOKEN(verilog::parser::token::B_OR);}
{B_XOR}                {yylval.operator=OPERATOR_B_XOR  ; EMIT_TOKEN(verilog::parser::token::B_XOR);}
{B_EQU}                {yylval.operator=OPERATOR_B_EQU  ; EMIT_TOKEN(verilog::parser::token::B_EQU);}
{B_NAND}               {yylval.operator=OPERATOR_B_NAND ; EMIT_TOKEN(verilog::parser::token::B_NAND);}
{B_NOR}                {yylval.operator=OPERATOR_B_NOR  ; EMIT_TOKEN(verilog::parser::token::B_NOR);}
{TERNARY}              {yylval.operator=OPERATOR_TERNARY; EMIT_TOKEN(verilog::parser::token::TERNARY);}

{BASE_DECIMAL}         {EMIT_TOKEN(verilog::parser::token::DEC_BASE);}
{BASE_HEX}             {BEGIN(in_hex_val); EMIT_TOKEN(verilog::parser::token::HEX_BASE);}
{BASE_OCTAL}           {BEGIN(in_oct_val); EMIT_TOKEN(verilog::parser::token::OCT_BASE);}
{BASE_BINARY}          {BEGIN(in_bin_val); EMIT_TOKEN(verilog::parser::token::BIN_BASE);}

<in_bin_val>{BIN_VALUE} {BEGIN(INITIAL); yylval.string = yytext; EMIT_TOKEN(verilog::parser::token::BIN_VALUE);}
<in_oct_val>{OCT_VALUE} {BEGIN(INITIAL); yylval.string = yytext; EMIT_TOKEN(verilog::parser::token::OCT_VALUE);}
<in_hex_val>{HEX_VALUE} {BEGIN(INITIAL); yylval.string = yytext; EMIT_TOKEN(verilog::parser::token::HEX_VALUE);}

{NUM_REAL}             {yylval.string=yytext;EMIT_TOKEN(verilog::parser::token::NUM_REAL);}
{NUM_UNSIGNED}         {yylval.string=yytext;EMIT_TOKEN(verilog::parser::token::UNSIGNED_NUMBER);}

{ALWAYS}               {EMIT_TOKEN(verilog::parser::token::KW_ALWAYS);}
{AND}                  {EMIT_TOKEN(verilog::parser::token::KW_AND);}
{ASSIGN}               {EMIT_TOKEN(verilog::parser::token::KW_ASSIGN);}
{AUTOMATIC}            {EMIT_TOKEN(verilog::parser::token::KW_AUTOMATIC);}
{BEGIN}                {EMIT_TOKEN(verilog::parser::token::KW_BEGIN);}
{BUF}                  {EMIT_TOKEN(verilog::parser::token::KW_BUF);}
{BUFIF0}               {EMIT_TOKEN(verilog::parser::token::KW_BUFIF0);}
{BUFIF1}               {EMIT_TOKEN(verilog::parser::token::KW_BUFIF1);}
{CASE}                 {EMIT_TOKEN(verilog::parser::token::KW_CASE);}
{CASEX}                {EMIT_TOKEN(verilog::parser::token::KW_CASEX);}
{CASEZ}                {EMIT_TOKEN(verilog::parser::token::KW_CASEZ);}
{CELL}                 {EMIT_TOKEN(verilog::parser::token::KW_CELL);}
{CMOS}                 {EMIT_TOKEN(verilog::parser::token::KW_CMOS);}
{CONFIG}               {EMIT_TOKEN(verilog::parser::token::KW_CONFIG);}
{DEASSIGN}             {EMIT_TOKEN(verilog::parser::token::KW_DEASSIGN);}
{DEFAULT}              {EMIT_TOKEN(verilog::parser::token::KW_DEFAULT);}
{DEFPARAM}             {EMIT_TOKEN(verilog::parser::token::KW_DEFPARAM);}
{DESIGN}               {EMIT_TOKEN(verilog::parser::token::KW_DESIGN);}
{DISABLE}              {EMIT_TOKEN(verilog::parser::token::KW_DISABLE);}
{EDGE}                 {EMIT_TOKEN(verilog::parser::token::KW_EDGE);}
{ELSE}                 {EMIT_TOKEN(verilog::parser::token::KW_ELSE);}
{END}                  {EMIT_TOKEN(verilog::parser::token::KW_END);}
{ENDCASE}              {EMIT_TOKEN(verilog::parser::token::KW_ENDCASE);}
{ENDCONFIG}            {EMIT_TOKEN(verilog::parser::token::KW_ENDCONFIG);}
{ENDFUNCTION}          {EMIT_TOKEN(verilog::parser::token::KW_ENDFUNCTION);}
{ENDGENERATE}          {EMIT_TOKEN(verilog::parser::token::KW_ENDGENERATE);}
{ENDMODULE}            {EMIT_TOKEN(verilog::parser::token::KW_ENDMODULE);}
{ENDPRIMITIVE}         {EMIT_TOKEN(verilog::parser::token::KW_ENDPRIMITIVE);}
{ENDSPECIFY}           {EMIT_TOKEN(verilog::parser::token::KW_ENDSPECIFY);}
{ENDTABLE}             {EMIT_TOKEN(verilog::parser::token::KW_ENDTABLE);}
{ENDTASK}              {EMIT_TOKEN(verilog::parser::token::KW_ENDTASK);}
{EVENT}                {EMIT_TOKEN(verilog::parser::token::KW_EVENT);}
{FOR}                  {EMIT_TOKEN(verilog::parser::token::KW_FOR);}
{FORCE}                {EMIT_TOKEN(verilog::parser::token::KW_FORCE);}
{FOREVER}              {EMIT_TOKEN(verilog::parser::token::KW_FOREVER);}
{FORK}                 {EMIT_TOKEN(verilog::parser::token::KW_FORK);}
{FUNCTION}             {EMIT_TOKEN(verilog::parser::token::KW_FUNCTION);}
{GENERATE}             {EMIT_TOKEN(verilog::parser::token::KW_GENERATE);}
{GENVAR}               {EMIT_TOKEN(verilog::parser::token::KW_GENVAR);}
{HIGHZ0}               {EMIT_TOKEN(verilog::parser::token::KW_HIGHZ0);}
{HIGHZ1}               {EMIT_TOKEN(verilog::parser::token::KW_HIGHZ1);}
{IF}                   {EMIT_TOKEN(verilog::parser::token::KW_IF);}
{IFNONE}               {EMIT_TOKEN(verilog::parser::token::KW_IFNONE);}
{INCDIR}               {EMIT_TOKEN(verilog::parser::token::KW_INCDIR);}
{INCLUDE}              {EMIT_TOKEN(verilog::parser::token::KW_INCLUDE);}
{INITIAL}              {EMIT_TOKEN(verilog::parser::token::KW_INITIAL);}
{INOUT}                {EMIT_TOKEN(verilog::parser::token::KW_INOUT);}
{INPUT}                {EMIT_TOKEN(verilog::parser::token::KW_INPUT);}
{INSTANCE}             {EMIT_TOKEN(verilog::parser::token::KW_INSTANCE);}
{INTEGER}              {EMIT_TOKEN(verilog::parser::token::KW_INTEGER);}
{JOIN}                 {EMIT_TOKEN(verilog::parser::token::KW_JOIN);}
{LARGE}                {EMIT_TOKEN(verilog::parser::token::KW_LARGE);}
{LIBLIST}              {EMIT_TOKEN(verilog::parser::token::KW_LIBLIST);}
{LIBRARY}              {EMIT_TOKEN(verilog::parser::token::KW_LIBRARY);}
{LOCALPARAM}           {EMIT_TOKEN(verilog::parser::token::KW_LOCALPARAM);}
{MACROMODULE}          {EMIT_TOKEN(verilog::parser::token::KW_MACROMODULE);}
{MEDIUM}               {EMIT_TOKEN(verilog::parser::token::KW_MEDIUM);}
{MODULE}               {EMIT_TOKEN(verilog::parser::token::KW_MODULE);}
{NAND}                 {EMIT_TOKEN(verilog::parser::token::KW_NAND);}
{NEGEDGE}              {EMIT_TOKEN(verilog::parser::token::KW_NEGEDGE);}
{NMOS}                 {EMIT_TOKEN(verilog::parser::token::KW_NMOS);}
{NOR}                  {EMIT_TOKEN(verilog::parser::token::KW_NOR);}
{NOSHOWCANCELLED}      {EMIT_TOKEN(verilog::parser::token::KW_NOSHOWCANCELLED);}
{NOT}                  {EMIT_TOKEN(verilog::parser::token::KW_NOT);}
{NOTIF0}               {EMIT_TOKEN(verilog::parser::token::KW_NOTIF0);}
{NOTIF1}               {EMIT_TOKEN(verilog::parser::token::KW_NOTIF1);}
{OR}                   {EMIT_TOKEN(verilog::parser::token::KW_OR);}
{OUTPUT}               {EMIT_TOKEN(verilog::parser::token::KW_OUTPUT);}
{PARAMETER}            {EMIT_TOKEN(verilog::parser::token::KW_PARAMETER);}
{PMOS}                 {EMIT_TOKEN(verilog::parser::token::KW_PMOS);}
{POSEDGE}              {EMIT_TOKEN(verilog::parser::token::KW_POSEDGE);}
{PRIMITIVE}            {EMIT_TOKEN(verilog::parser::token::KW_PRIMITIVE);}
{PULL0}                {EMIT_TOKEN(verilog::parser::token::KW_PULL0);}
{PULL1}                {EMIT_TOKEN(verilog::parser::token::KW_PULL1);}
{PULLDOWN}             {EMIT_TOKEN(verilog::parser::token::KW_PULLDOWN);}
{PULLUP}               {EMIT_TOKEN(verilog::parser::token::KW_PULLUP);}
{PULSESTYLE_ONEVENT}   {EMIT_TOKEN(verilog::parser::token::KW_PULSESTYLE_ONEVENT);}
{PULSESTYLE_ONDETECT}  {EMIT_TOKEN(verilog::parser::token::KW_PULSESTYLE_ONDETECT);}
{RCMOS}                {EMIT_TOKEN(verilog::parser::token::KW_RCMOS);}
{REAL}                 {EMIT_TOKEN(verilog::parser::token::KW_REAL);}
{REALTIME}             {EMIT_TOKEN(verilog::parser::token::KW_REALTIME);}
{REG}                  {EMIT_TOKEN(verilog::parser::token::KW_REG);}
{RELEASE}              {EMIT_TOKEN(verilog::parser::token::KW_RELEASE);}
{REPEAT}               {EMIT_TOKEN(verilog::parser::token::KW_REPEAT);}
{RNMOS}                {EMIT_TOKEN(verilog::parser::token::KW_RNMOS);}
{RPMOS}                {EMIT_TOKEN(verilog::parser::token::KW_RPMOS);}
{RTRAN}                {EMIT_TOKEN(verilog::parser::token::KW_RTRAN);}
{RTRANIF0}             {EMIT_TOKEN(verilog::parser::token::KW_RTRANIF0);}
{RTRANIF1}             {EMIT_TOKEN(verilog::parser::token::KW_RTRANIF1);}
{SCALARED}             {EMIT_TOKEN(verilog::parser::token::KW_SCALARED);}
{SHOWCANCELLED}        {EMIT_TOKEN(verilog::parser::token::KW_SHOWCANCELLED);}
{SIGNED}               {EMIT_TOKEN(verilog::parser::token::KW_SIGNED);}
{SMALL}                {EMIT_TOKEN(verilog::parser::token::KW_SMALL);}
{SPECIFY}              {EMIT_TOKEN(verilog::parser::token::KW_SPECIFY);}
{SPECPARAM}            {EMIT_TOKEN(verilog::parser::token::KW_SPECPARAM);}
{STRONG0}              {EMIT_TOKEN(verilog::parser::token::KW_STRONG0);}
{STRONG1}              {EMIT_TOKEN(verilog::parser::token::KW_STRONG1);}
{SUPPLY0}              {EMIT_TOKEN(verilog::parser::token::KW_SUPPLY0);}
{SUPPLY1}              {EMIT_TOKEN(verilog::parser::token::KW_SUPPLY1);}
{TABLE}                {EMIT_TOKEN(verilog::parser::token::KW_TABLE);}
{TASK}                 {EMIT_TOKEN(verilog::parser::token::KW_TASK);}
{TIME}                 {EMIT_TOKEN(verilog::parser::token::KW_TIME);}
{TRAN}                 {EMIT_TOKEN(verilog::parser::token::KW_TRAN);}
{TRANIF0}              {EMIT_TOKEN(verilog::parser::token::KW_TRANIF0);}
{TRANIF1}              {EMIT_TOKEN(verilog::parser::token::KW_TRANIF1);}
{TRI0}                 {EMIT_TOKEN(verilog::parser::token::KW_TRI0);}
{TRI1}                 {EMIT_TOKEN(verilog::parser::token::KW_TRI1);}
{TRIAND}               {EMIT_TOKEN(verilog::parser::token::KW_TRIAND);}
{TRIOR}                {EMIT_TOKEN(verilog::parser::token::KW_TRIOR);}
{TRIREG}               {EMIT_TOKEN(verilog::parser::token::KW_TRIREG);}
{TRI}                  {EMIT_TOKEN(verilog::parser::token::KW_TRI);}
{UNSIGNED}             {EMIT_TOKEN(verilog::parser::token::KW_UNSIGNED);}
{USE}                  {EMIT_TOKEN(verilog::parser::token::KW_USE);}
{VECTORED}             {EMIT_TOKEN(verilog::parser::token::KW_VECTORED);}
{WAIT}                 {EMIT_TOKEN(verilog::parser::token::KW_WAIT);}
{WAND}                 {EMIT_TOKEN(verilog::parser::token::KW_WAND);}
{WEAK0}                {EMIT_TOKEN(verilog::parser::token::KW_WEAK0);}
{WEAK1}                {EMIT_TOKEN(verilog::parser::token::KW_WEAK1);}
{WHILE}                {EMIT_TOKEN(verilog::parser::token::KW_WHILE);}
{WIRE}                 {EMIT_TOKEN(verilog::parser::token::KW_WIRE);}
{WOR}                  {EMIT_TOKEN(verilog::parser::token::KW_WOR);}
{XNOR}                 {EMIT_TOKEN(verilog::parser::token::KW_XNOR);}
{XOR}                  {EMIT_TOKEN(verilog::parser::token::KW_XOR);}

{SYSTEM_ID}            {
	yylval.identifier = ast_new_identifier(yytext,yylineno);
	EMIT_TOKEN(verilog::parser::token::SYSTEM_ID);
}
{ESCAPED_ID}           {
	yylval.identifier = ast_new_identifier(yytext,yylineno);
	EMIT_TOKEN(verilog::parser::token::ESCAPED_ID);
}
{SIMPLE_ID}            {
	yylval.identifier = ast_new_identifier(yytext,yylineno);
	EMIT_TOKEN(verilog::parser::token::SIMPLE_ID);
}

{STRING}               {yylval.string= yytext;EMIT_TOKEN(verilog::parser::token::STRING);}

<*>{NEWLINE}              {/*EMIT_TOKEN(verilog::parser::token::NEWLINE); IGNORE */   }
<*>{SPACE}                {/*EMIT_TOKEN(verilog::parser::token::SPACE);   IGNORE */   }
<*>{TAB}                  {/*EMIT_TOKEN(verilog::parser::token::TAB);     IGNORE */   }

<<EOF>> {

	yypop_buffer_state();

	// We are exiting a file, so pop from the the preprocessor stack of files
	// being parsed.
	ast_stack_pop(yy_preproc -> current_file);


	if ( !YY_CURRENT_BUFFER )
	{
		yyterminate();
	}
	else
	{
		YY_BUFFER_STATE cur = YY_CURRENT_BUFFER;
		yylineno = cur -> yy_bs_lineno;
	}
}

  .                      {
	  EMIT_TOKEN(verilog::parser::token::ANY);
  }
%%
