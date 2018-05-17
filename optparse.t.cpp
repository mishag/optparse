#include <optparse.h>
#include <iostream>

int main(int argc, char * argv[])
{
    OptionParser parser;

    parser.addOption('v', Option::INT);
    parser.addOption('l', Option::STRING);
    parser.addOption('x', Option::DOUBLE);
    parser.addOption('b', Option::BOOL);

    int rc = parser.parse(argc, argv);

    if (0 != rc) {
        return rc;
    }

    size_t argstart = parser.argumentsStart();

    std::cout << "Parsed values are: \n"
              << "v: " << parser.getOption('v').asInt()
              << " isSet: " << parser.getOption('v').isSet() << "\n"
              << "l: " << parser.getOption('l').asString()
              << " isSet: " << parser.getOption('l').isSet() << "\n"
              << "x: " << parser.getOption('x').asDouble()
              << " isSet: " << parser.getOption('x').isSet() << "\n"
              << "b: " << parser.getOption('b').asBool()
              << " isSet: " << parser.getOption('b').isSet() << "\n"
              << "Arguments start at: " << argstart;

    if (argstart < argc) {
        std::cout << " token: \"" << argv[argstart] << "\"";
    }

    std::cout << "\n";

    return 0;
}
