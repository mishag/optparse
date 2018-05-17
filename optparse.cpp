#include <optparse.h>

#include <cassert>
#include <cstring>
#include <sstream>
#include <iostream>

namespace {

char isFlag(const char * s)
{
    // a flag is of the form '-x' where x is
    // a character in [a-zA-Z]

    // return 0 if not a flag, and
    // the char representing flag if it is.

    if (strlen(s) != 2) {
        return 0;
    }

    if ((s[0] == '-') &&
        (('a' <= s[1] && s[1] <= 'z') ||
         ('A' <= s[1] && s[1] <= 'Z'))) {
        return s[1];
    }

    return 0;
}

bool getIntValue(int& result, const char * s)
{
    std::istringstream ss(s);
    ss >> result;
    return ss ? true : false;
}

bool getDoubleValue(double& result, const char * s)
{
    std::istringstream ss(s);
    ss >> result;
    return ss ? true : false;
}


} // unnamed namespace


// =====================
// Option Implementation
// =====================

Option::Option(char flag, Type type)
    : d_flag(flag)
    , d_type(type)
    , d_value()
    , d_isSet(false)
{
    switch(d_type) {
        case BOOL:
            d_value.b = false;
            break;
        case INT:
            d_value.i = 0;
            break;
        case DOUBLE:
            d_value.d = 0;
            break;
        case STRING:
            d_value.s = "";
            break;
        default:
            assert(false);
    }
}


// ===========================
// OptionParser Implementation
// ===========================

OptionParser::OptionParser()
    : d_options()
{
}

void OptionParser::addOption(char flag, Option::Type type)
{
    Option opt(flag, type);

    d_options.insert(OptionsMap::value_type(flag,
                                            Option(flag, type)));
}

const Option& OptionParser::getOption(char flag) const
{
    auto it = d_options.find(flag);

    if (it == d_options.end()) {
        throw std::runtime_error("Unsupported flag.");
    }

    return it->second;
}

int OptionParser::parse(int argc, char * argv[])
{
    if (argc == 1) {
        d_argumentsStart = 1;
        return 0;
    }

    assert(argc > 1);

    int pos = 1;
    while (pos < argc) {

        const char * token = argv[pos];
        char flag = isFlag(token);

        // we either have a flag or
        // we've reached the start of tail arguments

        if (!flag) {
            d_argumentsStart = pos;
            return 0;
        }

        // we have a flag

        auto it = d_options.find(flag);

        if (it == d_options.end()) {
            d_argumentsStart = pos;
            return 0;
        }

        // got a valid flag

        Option& opt = it->second;

        if (opt.type() == Option::BOOL) {
            opt.d_value.b = true;
            opt.d_isSet = true;
            ++pos;
            continue;
        }

        ++pos;  // advance to the next token

        if (pos == argc) {
            std::cerr << "Expecting argument for flag: "
                      << flag << "\n";
            return -3;
        }

        token = argv[pos];

        switch (opt.type()) {
            case Option::BOOL:
                assert(false);
            case Option::INT:
                if (!getIntValue(opt.d_value.i, token)) {
                    std::cerr << "Expected integer for flag "
                              << flag << " got: " << token << "\n";
                    return -2;
                }
                break;
            case Option::DOUBLE:
                if (!getDoubleValue(opt.d_value.d, token)) {
                    std::cerr << "Expected double for flag "
                              << flag << " got: " << token << "\n";
                    return -2;
                }
                break;
            case Option::STRING:
                opt.d_value.s = token;
                break;
            default:
                assert(false);
        }

        opt.d_isSet = true;
        ++pos;
    }

    d_argumentsStart = argc;

    return 0;
}
