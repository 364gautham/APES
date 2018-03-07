#include <stdio.h>
#include <stdlib.h>

#include "dll.h"

int main(void)
{
  const struct CMUnitTest tests[] = {

	cmocka_unit_test(test_insert_begin),
	cmocka_unit_test(test_insert_begin2),
	cmocka_unit_test(test_insert_end),
	cmocka_unit_test(test_insert_end2),
	cmocka_unit_test(test_insert_pos_last),
	cmocka_unit_test(test_insert_posn),
     	cmocka_unit_test(test_insert_pos0),
	cmocka_unit_test(test_del_begin),
	cmocka_unit_test(test_del_begin2),
	cmocka_unit_test(test_del_end),
	cmocka_unit_test(test_del_end2),
	cmocka_unit_test(test_del_pos_last),
	cmocka_unit_test(test_del_posn),
     	cmocka_unit_test(test_del_pos0),
	cmocka_unit_test(test_size),
	cmocka_unit_test(test_size1),
     	cmocka_unit_test(test_destroy_all),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
