// test.main

#include "kernel/interface/interface.hpp"
#include "kernel/test/test.hpp"

#define M_kernel_library_symbol &Twinning::Kernel::Interface::service
#define M_kernel_test_statement do { Twinning::Kernel::Test::run(); } while (false);

#include "shell/main.cpp"
