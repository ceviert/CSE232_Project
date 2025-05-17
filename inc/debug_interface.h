#ifndef __DEBUG_INTERFACE_H__
#define __DEBUG_INTERFACE_H__

int get_argument(char *com, int n, char type, void *ptr, int limit);
void dump_chain(int head, const char *label);
void dump_list(const char *label);

#endif // __DEBUG_INTERFACE_H__