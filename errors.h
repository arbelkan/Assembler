typedef enum
{
    MACRO_NAME_IS_AN_INSTRUCTION = 1,
    END_OF_MACRO_LINE_INCLUDES_EXTERNAL_CHARACTERS,
    LINE_LONGER_THAN_80_CHARACTERS,
    EXTERNAL_COMMA,
    COMMA_NOT_IN_PLACE,
    SAME_DECLERATION_FOR_BOTH_ENTRY_AND_EXTERNAL_SYMBOL,
    ILLEGAL_SYMBOL_NAME,
    SYMBOL_NAME_LONGER_THAN_31_CHARACTERS,
    COLONS_NOT_ADJACEN_TO_SYMBOL,
    SYMBOL_ALREADY_DEFINED,
    DECLERATION_BOTH_FOR_MACRO_AND_SYMBOL,
    SYMBOL_NOT_DEFINED,
    ILLEGAL_NUMBER,
    UNKOWN_INSTRUCTION,
    UNKNOWN_REGISTER,
    TOO_MANY_OPERANDS,
    NOT_ENOUGH_OPERANDS,
    OPERAND_NOT_SUITABLE_FOR_INSTRUCTION,
    ERR_MAX
} ErrorCodes;

/**
 * printing error and its line using error code
 * @param errorCode - the enum error code
 * @param lineNumber- the line of the error
 * @return line number of the error and prints the relevant error
 **/
unsigned int printError(unsigned int errorCode, unsigned int lineNumber);