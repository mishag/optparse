#include <map>


// ============
// class Option
// ============

class Option {

friend class OptionParser;

public:
    enum Type {
        BOOL,
        INT,
        DOUBLE,
        STRING
    };

    Option(char flag, Type type);

    bool        asBool()   const { return d_value.b; }
    int         asInt()    const { return d_value.i; }
    double      asDouble() const { return d_value.d; }
    const char* asString() const { return d_value.s; }

    Type type()  const { return d_type; }
    bool isSet() const { return d_isSet; }

private:

    union Value {
        bool        b;
        int         i;
        double      d;
        const char *s;
    };

    char  d_flag;
    Type  d_type;
    Value d_value;
    bool  d_isSet;
};


// ==================
// class OptionParser
// ==================

class OptionParser {
public:
    OptionParser();
    ~OptionParser() {}

    OptionParser(const OptionParser&) = delete;
    OptionParser& operator=(const OptionParser&) = delete;

    void addOption(char flag, Option::Type type);
    int  parse(int argc, char * argv[]);

    const Option& getOption(char flag) const;
    size_t argumentsStart() const { return d_argumentsStart; }

private:
    typedef std::map<char, Option> OptionsMap;

    OptionsMap   d_options;
    size_t       d_argumentsStart;
};
