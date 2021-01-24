//Setup file for Sierra tests, if your version of Sierra supports more than 8 tasks
//add more tasks and stacks here.

// TASK STACKS
#define STACK_SIZE 800
extern char idle_stack[];
extern char task_1_stack[];
extern char task_2_stack[];
extern char task_3_stack[];
extern char task_4_stack[];
extern char task_5_stack[];
extern char task_6_stack[];
extern char task_7_stack[];

// TASK, in Sierra
#define task_1 1
#define task_2 2
#define task_3 3
#define task_4 4
#define task_5 5
#define task_6 6
#define task_7 7
