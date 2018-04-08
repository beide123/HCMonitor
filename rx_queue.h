#ifndef _KEY_QUEUE_H
#define _KEY_QUEUE_H

#include <sys/time.h>

#define BUFSIZE 20000000

struct ipv4_2tuple {
	uint32_t ip_src;
	uint16_t port_src;	
};

struct http_tuple {
	uint32_t ip_src;
	uint16_t port_src;
	uint8_t  type;
};

/*typedef struct node_data{
    struct ipv4_2tuple key;
    struct timespec ts;
    uint32_t sent_seq;
    uint16_t total_len;
    int type;
}QType;*/

typedef struct node_data{
    struct http_tuple key;
    struct timespec ts;
    uint32_t sent_seq;
    uint16_t total_len;
    int type;
	int status;
}QType;


typedef struct key_node
{
    struct node_data data;
    struct key_node* next;
}*KeyNode;

typedef struct Queue
{
  QType* base; 
  unsigned long front,rear;	
  int occupy;
}LinkQueue;

LinkQueue *KeyQue;//(struct Queue*)malloc(sizeof(struct Queue));
LinkQueue *QueBak;
LinkQueue *Qcur;
LinkQueue *Qnext;
KeyNode Qbuf;

QType *RxQue;

unsigned long occupy;
unsigned long deque;


int InitQueue(void);
int EnQueue(LinkQueue *Q,struct node_data e);
int GetHead(LinkQueue *Q,struct node_data *e);
int DeQueue(LinkQueue *Q);
int QueueEmpty(LinkQueue *Q);
int FullQueue(LinkQueue *Q);
long QueueLength(LinkQueue *Q);


int send_key(LinkQueue *Q,struct node_data *data);

#endif

