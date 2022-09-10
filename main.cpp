#include "Cassy.h"

int main(int argc, char* argv[])
{
    Cassy *cassy;
    cassy = new Cassy();
    cassy->connect();
    return 0;
}