#include <check.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <linkedList.h>

START_TEST(list_insert_and_remove){
  struct list l;

  ll_initialize(&l);

  int ints[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

  struct list_node *mk;
  for(int i = 0; i < 10; i++){
    mk = ll_insert_val(&l, (void*)&ints[i]);
    ck_assert_ptr_eq(mk, ll_search_node(&l, (void*)&ints[i], sizeof(int)));
    ck_assert_int_eq(l.size, i+1);
  }

  int *val;
  for(int i = 9; i >= 0; i--){
    val = (int*)ll_remove_root(&l);

    ck_assert_int_eq(l.size, i);
    ck_assert_int_eq(*val, ints[i]);
  }
  
}
END_TEST


START_TEST(list_pq_basic){
  struct list l;

  ll_initialize(&l);

  int priorities[6] = {0, 1, 2, 3, 4, 5};

  struct list_node *mk;

  ll_pq_enqueue(&l, (void*)&priorities[0], priorities[0]);
  ck_assert_int_eq(l.size, 1);
  ll_pq_enqueue(&l, (void*)&priorities[2], priorities[2]);
  ck_assert_int_eq(l.size, 2);
  ll_pq_enqueue(&l, (void*)&priorities[1], priorities[1]);
  ck_assert_int_eq(l.size, 3);
  ll_pq_enqueue(&l, (void*)&priorities[0], priorities[0]);
  ck_assert_int_eq(l.size, 4);
  ll_pq_enqueue(&l, (void*)&priorities[3], priorities[3]);
  ck_assert_int_eq(l.size, 5);
  ll_pq_enqueue(&l, (void*)&priorities[3], priorities[3]);
  ck_assert_int_eq(l.size, 6);
  ll_pq_enqueue(&l, (void*)&priorities[1], priorities[1]);
  ck_assert_int_eq(l.size, 7);
  ll_pq_enqueue(&l, (void*)&priorities[2], priorities[2]);
  ck_assert_int_eq(l.size, 8);

  
  ck_assert_int_eq(0, *(int*)ll_pq_dequeue(&l));
  ck_assert_int_eq(l.size, 7);
  ck_assert_int_eq(0, *(int*)ll_pq_dequeue(&l));
  ck_assert_int_eq(l.size, 6);
  ck_assert_int_eq(1, *(int*)ll_pq_dequeue(&l));
  ck_assert_int_eq(l.size, 5);
  ck_assert_int_eq(1, *(int*)ll_pq_dequeue(&l));
  ck_assert_int_eq(l.size, 4);
  ck_assert_int_eq(2, *(int*)ll_pq_dequeue(&l));
  ck_assert_int_eq(l.size, 3);
  ck_assert_int_eq(2, *(int*)ll_pq_dequeue(&l));
  ck_assert_int_eq(l.size, 2);
  ck_assert_int_eq(3, *(int*)ll_pq_dequeue(&l));
  ck_assert_int_eq(l.size, 1);
  ck_assert_int_eq(3, *(int*)ll_pq_dequeue(&l));
  ck_assert_int_eq(l.size, 0);

  ck_assert_ptr_eq(NULL, ll_pq_dequeue(&l));

  
  ll_pq_enqueue(&l, (void*)&priorities[0], priorities[0]);
  ck_assert_int_eq(l.size, 1);
  ll_pq_enqueue(&l, (void*)&priorities[2], priorities[2]);
  ck_assert_int_eq(l.size, 2);
  ll_pq_enqueue(&l, (void*)&priorities[1], priorities[1]);
  ck_assert_int_eq(l.size, 3);
  ll_pq_enqueue(&l, (void*)&priorities[0], priorities[0]);
  ck_assert_int_eq(l.size, 4);
  ll_pq_enqueue(&l, (void*)&priorities[3], priorities[3]);
  ck_assert_int_eq(l.size, 5);
  ll_pq_enqueue(&l, (void*)&priorities[3], priorities[3]);
  ck_assert_int_eq(l.size, 6);
  ll_pq_enqueue(&l, (void*)&priorities[1], priorities[1]);
  ck_assert_int_eq(l.size, 7);
  ll_pq_enqueue(&l, (void*)&priorities[2], priorities[2]);
  ck_assert_int_eq(l.size, 8);

  
  ck_assert_int_eq(0, *(int*)ll_pq_dequeue(&l));
  ck_assert_int_eq(l.size, 7);
  ck_assert_int_eq(0, *(int*)ll_pq_dequeue(&l));
  ck_assert_int_eq(l.size, 6);
  ck_assert_int_eq(1, *(int*)ll_pq_dequeue(&l));
  ck_assert_int_eq(l.size, 5);
  ck_assert_int_eq(1, *(int*)ll_pq_dequeue(&l));
  ck_assert_int_eq(l.size, 4);
  ck_assert_int_eq(2, *(int*)ll_pq_dequeue(&l));
  ck_assert_int_eq(l.size, 3);
  ck_assert_int_eq(2, *(int*)ll_pq_dequeue(&l));
  ck_assert_int_eq(l.size, 2);
  ck_assert_int_eq(3, *(int*)ll_pq_dequeue(&l));
  ck_assert_int_eq(l.size, 1);
  ck_assert_int_eq(3, *(int*)ll_pq_dequeue(&l));
  ck_assert_int_eq(l.size, 0);

 

}
END_TEST


START_TEST(queue_usage){
  struct list l;

  ll_initialize(&l);

  int priorities[6] = {0, 1, 2, 3, 4, 5};
  
  ll_pq_enqueue(&l, (void*)&priorities[0], priorities[0]);
  ck_assert_int_eq(l.size, 1);
  ll_pq_enqueue(&l, (void*)&priorities[0], priorities[0]);
  ck_assert_int_eq(l.size, 2);
  

  ck_assert_int_eq(0, *(int*)ll_pq_dequeue(&l));
  ck_assert_int_eq(l.size, 1);
  
  ll_pq_enqueue(&l, (void*)&priorities[0], priorities[0]);
  ck_assert_int_eq(l.size, 2);

  ck_assert_int_eq(0, *(int*)ll_pq_dequeue(&l));
  ck_assert_int_eq(l.size, 1);

  ck_assert_int_eq(0, *(int*)ll_pq_dequeue(&l));
  ck_assert_int_eq(l.size, 0);

  ck_assert_ptr_eq(NULL, ll_pq_dequeue(&l));

}
END_TEST

Suite * list_suite(void){

  Suite *s;
  TCase *tc_core;

  s = suite_create("linked_list");


  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, list_insert_and_remove);
  tcase_add_test(tc_core, list_pq_basic);
  tcase_add_test(tc_core, queue_usage);
  suite_add_tcase(s, tc_core);

  return s;

}

int main(){

  int number_failed;
  Suite *s;
  SRunner *sr;
  
  s = list_suite();
  sr = srunner_create(s);
  
  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
  
}
