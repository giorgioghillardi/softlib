#ifndef _CONFIGURATION_SCRIPT_H
#define _CONFIGURATION_SCRIPT_H

#include <vector>
#include <string>

#include <softlib/Configuration.h>

/************************************************
 * INTERNAL OBJECTS (for lexing/interpretation) *
 ************************************************/
typedef enum {
	CTKN_INVALID,
	CTKN_NAME,
	CTKN_VALUE,
	CTKN_EQUALS,		/* Equals sign (=) */
	CTKN_BLOCKTYPE,
	CTKN_BLOCKNAME,
    CTKN_LPAR,          // (
    CTKN_RPAR,          // )
	CTKN_BLOCK_START,	// {
	CTKN_BLOCK_END,		// }
	CTKN_END_STATEMENT,	// ; (or \n)
	CTKN_EOF
} conftoken_t;

/* Token generated by the lexer and interpreted by the interpreter */
class ConfigToken {
	conftoken_t type;
	std::string value;
	ConfigToken *parent;
	ConfigToken *next;
    size_t line, charpos;
    std::string filename;
public:
	ConfigToken();
    ConfigToken(const ConfigToken&);
	ConfigToken(conftoken_t, const std::string&, ConfigToken*, size_t, size_t, const std::string&);
	~ConfigToken();
    size_t GetCharpos() const { return this->charpos; }
    std::string GetFilename() const { return this->filename; }
    size_t GetLine() const { return this->line; }
	conftoken_t GetType() const { return this->type; }
	std::string GetValue() const { return this->value; }
	ConfigToken *Next() const { return this->next; }
	ConfigToken *Previous() const { return this->parent; }
	void SetNext(ConfigToken *ct) { this->next = ct; }
};

class ConfigurationScript : public Configuration {
public:
    ConfigurationScript() : Configuration() {}
    ConfigurationScript(const Configuration& c) : Configuration(c) {}
    virtual ~ConfigurationScript() {}

    virtual void FromFile(const std::string&) override;

	void FromStdin();
	ConfigBlock& FromString(const std::string&, const std::string& src="<unknown>");
private:
	/* Lexer properties/methods */
	size_t cnfbufferpos, cnfbufferlength,
		   linecounter,		/* Line number */
		   charpos;			/* Index of character on line */
	std::string cnfbuffer;
    std::string file;       /* Name of file currently being read */
	bool instring,			/* Currently in string (so all characters matter) */
         ininclude,         /* Currently between '<' and '>', meaning the text contains the name
                               of a file to be "included". */
		 prevws;			/* Previous character was whitespace, so any ws can be safely ignored */

	std::vector<ConfigToken*> Lex(const std::string&);

	void initbuffer(const std::string&);
	char nextc();
	ConfigToken *gettkn(std::string&, ConfigToken*);
	void pushbuffer(std::string&, std::vector<ConfigToken*>&);

	/* Interpreter properties/methods */
	std::vector<ConfigToken*>::iterator tknstream;
	ConfigBlock Interpret(std::vector<ConfigToken*>&);

	bool advance();
	bool advanceTo(std::initializer_list<conftoken_t>);
    size_t currcharpos();
    std::string currfile();
    size_t currline();
	void expect(conftoken_t);
    std::vector<ConfigToken*> include_file(const std::string&);
	ConfigToken *token();
	conftoken_t tokent();
	conftoken_t tokent_n();
	const char *ttos(conftoken_t);
};

#endif/*_CONFIGURATION_SCRIPT_H*/
