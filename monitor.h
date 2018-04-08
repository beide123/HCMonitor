/*************************************************************************
        > File Name: res_req.h
        > Author: 
        > Mail: 
        > Created Time: Jan 21 ,2016 16:05:25 PM PDT
 ************************************************************************/

#ifndef _RESREQ_H
#define _RESREQ_H

#include <sys/time.h>
#include "rx_queue.h"
#include "send_sql.h"
#include "http_parse.h"

#if 0



#define DEBUG
//#define ACKTEST 1
#define MIRROR 1
#define SAMPLE

#ifdef DEBUG
extern unsigned long occupy;
extern unsigned long deque;
#endif


uint64_t burst_pkts;
int *sample_rate;
unsigned int msec_count;
int bool_trans;
int bool_transed;
int bool_input;
int la;





//plot the cdf of time by python lib,just for no mysql 
void plotPoint2d(float *x, float *y);
void initializePlotting(void);
void uninitializePlotting(void);



unsigned long get_sample(uint64_t cur_tsc,uint64_t prev_tsc,uint64_t *timer_tsc);
#endif

/* dequeue the meta data and caculate response time */
unsigned int conn_active;
unsigned int conn_active_mid;
float avg_delay;
float avg_delay_hy;
float delay_99;//delay value @99% 
unsigned int conn;
unsigned int count;

void clear(void);
float avg(float a[],unsigned int n);
int compInc(const void *a, const void *b);

void lcore_online(void);
void cdf_acktime(float *ack_time_pro,unsigned int idx);


/* dequeue the meta data and caculate response time */

unsigned int conn_active_hy;
uint32_t pkt_num; //toatl pkts without ack
uint32_t pkt_req; //request pkts num 
uint32_t pkt_rep; //response pkts num
uint32_t miss_rep;//missed response 
unsigned int timeout_50;//pkts of response time over 50ms
unsigned int timeout_100;//pkts of response time over 100ms

int res_setup_hash(uint16_t socket_id);
int response_time_process(struct node_data *data,uint16_t nb_rx,uint16_t socket_id);


/* parse rhe packet and enqueue the meta data */
struct timespec ts;

int packet_process(struct ipv4_hdr *ip_hdr,struct timespec ts_now);
int key_extract(struct ipv4_hdr *ip_hdr,struct node_data *data,struct timespec ts_now);


#endif

