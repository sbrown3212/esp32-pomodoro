#include <unity.h>

void setUp() {}
void tearDown() {}

void test_runner_works() {
  TEST_ASSERT_EQUAL_INT(2, 1 + 1);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_runner_works);
  return UNITY_END();
}
