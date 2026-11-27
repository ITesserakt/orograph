#include <cassert>

#include "launcher/launcher.h"
#include "lib.h"

TEST(i_work_correctly) {}

TEST(i_am_failing_but_expect_that, .should_panic = true) { throw "BOOM"; }

TEST(i_am_failing_with_cpp_exception, .should_panic = true) { throw std::runtime_error{"BOOM"}; }

TEST(i_am_failing_with_expected_panic, .should_panic = "BOOM") { throw std::runtime_error{"BOOM"}; }

TEST(i_am_ignored, .ignored = true) { throw "BOOM"; }

TEST(i_am_both, .ignored = true, .should_panic = true) { throw std::runtime_error{"BOOM"}; }

TEST(i_am_ignored_with_reason, .ignored = "test purposes") {}

TEST(the_great_question) { assert(42 == add(20, 22)); }
