/* 
 * thread library function prototypes
 */
int t_create(void(*fct)(int), int id, int pri);
void t_yield(void);
void t_init(void);
void t_terminate(void);

