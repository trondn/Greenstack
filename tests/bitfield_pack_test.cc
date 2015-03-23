#include <iostream>

int main(void) {
    struct Flags {
        bool response : 1;
        bool flexHeaderPresent : 1;
        bool fence : 1;
        bool more : 1;
        bool quiet : 1;
        bool unassigned : 2;
        bool next : 1;
    };

    if (sizeof(struct Flags) != 1) {
        std::cerr << "FATAL: size of flags struct is " << sizeof(struct Flags) << " (and not 1)" << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}