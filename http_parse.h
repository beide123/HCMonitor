#ifndef http_parser_h
#define http_parser_h

#include "rx_queue.h"

/* Request Methods */
#define GET 1
#define POST 2
#define HEAD 3
#define PUT 4
#define DELETE 5
#define TRACE 6

/* Response Methods */
#define OK 200

#define REQ 7
#define REP 8

#define REQ_HASH_ENTRIES 1024*1024*16
#define REQ_HASH_BUCKET  16

uint8_t http_parse(struct ipv4_hdr *ip_hdr,struct node_data *data,struct timespec ts_now);


#endif
