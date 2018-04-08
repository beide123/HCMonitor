#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <unistd.h>
#include <signal.h>
#include <getopt.h>
#include <sys/types.h>
#include <string.h>
#include <sys/queue.h>
#include <stdarg.h>
#include <errno.h>
#include <sys/param.h>
#include <pthread.h>

#include <rte_eal.h>
#include <rte_ethdev.h>
#include <rte_cycles.h>
#include <rte_malloc.h>
#include <rte_debug.h>
#include <rte_distributor.h>
#include <rte_ip.h>
#include <rte_tcp.h>
#include <rte_udp.h>
#include <rte_string_fns.h>
#include <rte_lpm.h>
#include <rte_ring.h>
#include <rte_mempool.h>
#include <rte_mbuf.h>
#include <rte_ether.h>
#include <rte_ethdev.h>
#include <rte_fbk_hash.h>
#include <rte_hash.h>
#include <rte_jhash.h>

//#include <python2.7/Python.h>
#include <pthread.h>
#include "config.h"
#include "monitor.h"

/* For packet direction index */
#define REQU_USR 0
#define RESP_USR 1
#define REQU_APP 2
#define RESP_APP 3
#define POFFSET 6
#define UOFFSET 8

/* For packet metadata enqueue */
unsigned long max_size = BUFSIZE;

/* For packet metadata hash and delay buffered*/
#define DEFAULT_HASH_FUNC  rte_jhash

//params for initial hash table
/*struct rte_hash_parameters ipv4_req_hash_params = {
       .name = NULL,
       .entries = REQ_HASH_ENTRIES,
       //.bucket_entries = REQ_HASH_BUCKET,
       .key_len = sizeof(struct ipv4_2tuple),
       .hash_func = DEFAULT_HASH_FUNC,
       .hash_func_init_val = 0,
};*/

extern struct rte_hash_parameters ipv4_req_hash_params;

typedef struct rte_hash req_lookup_struct_t;

static req_lookup_struct_t *req_lookup_struct[2];
static struct timespec req_out_if[REQ_HASH_ENTRIES];
static uint32_t Maxseq[REQ_HASH_ENTRIES] = {0};
#ifdef TEST_X86
float *ack_time_x86;
static int req_count[REQ_HASH_ENTRIES] = {0};
#else
float *ack_time_hy;
static int req_hy_count[REQ_HASH_ENTRIES] = {0};
#endif

/* Save the ack_time online */
float *ack_time;
unsigned long idx = 0;
unsigned long idx_hy = 0;
unsigned long idx_x86 = 0;

/* For print cdf online */
#define MAX_BUFF 5000000
#define BLOCK 1000

#define AREA_1 0.1
#define AREA_2 0.2
#define AREA_3 0.3
#define AREA_4 0.5
#define AREA_5 0.7
#define AREA_6 0.9
#define AREA_7 0.95
#define AREA_8 0.99
#define AREA_9 0.999
#define AREA_10 0.9999
#define POINT 10

int lock_flag;

unsigned long thre = MAX_BUFF;
unsigned long thre_hy = MAX_BUFF;

#define DEBUG

#ifdef DEBUG
char wzlog[10] = "cdf.txt";
#endif

//#define CMD_BUF_SIZE 512



#if 0
struct timeval *t_buf;


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


long last;



unsigned int recount = 0;

unsigned int plot_flag = 0;
uint64_t prev_tsc = 0;
uint64_t timer_tsc = 0;
//static long last_ret = 0;

#define TIMER_MILLISECOND 2000000ULL /* around 1ms at 2 Ghz */
static int64_t timer_period = 1000 * TIMER_MILLISECOND; /* default period is 1 milliseconds */



void initializePlotting(void) 
{
    Py_Initialize();
    // load matplotlib for plotting
    PyRun_SimpleString("from matplotlib import pyplot as pp");
    PyRun_SimpleString("pp.ion()"); // use pp.draw() instead of pp.show()
}

void uninitializePlotting(void) 
{
	Py_Finalize();
}

void plotPoint2d(float *x, float *y) 
{
		// this buffer will be used later to handle the commands to python
	static char command[CMD_BUF_SIZE];
	static char element_x[CMD_BUF_SIZE];
	static char element_y[CMD_BUF_SIZE];
	int i;
	
	/*
	 * convert the x,y data to string format so that python can execute command
	 * */
	snprintf(element_x, CMD_BUF_SIZE, "%f,", x[0]);
	snprintf(element_y, CMD_BUF_SIZE, "%f,", y[0]);
	for(i  = 1;i < POINT + 1;i++){
			sprintf(element_x, "%s%f,",element_x,x[i]);
			sprintf(element_y, "%s%f,",element_y,y[i]);			
	}
	sprintf(element_x,"%s%f", element_x,x[POINT+1]);
	sprintf(element_y,"%s%f", element_y,y[POINT+1]);
	/*end convert*/
    
	//printf("x:%s,y:%s\n",element_x,element_y);
	snprintf(command, CMD_BUF_SIZE, "pp.plot([%s],[%s],'r-')\npp.draw()",element_x,element_y);
	if(plot_flag)
		PyRun_SimpleString("pp.clf()");//if ex figure exists,clear it
	plot_flag = 1;
		PyRun_SimpleString(command);//update plot figure
}

unsigned long get_sample(uint64_t cur_tsc,uint64_t prev_tsc,uint64_t *timer_tsc)
{
    uint64_t diff_tsc;             
    diff_tsc = cur_tsc - prev_tsc;
	LinkQueue *Qlist[2] = {KeyQue,QueBak};

	/* if timer is enabled */
	if (timer_period > 0) {

		/* advance the timer */
		*timer_tsc += diff_tsc;

		/* if timer has reached its timeout */
		if (unlikely(*timer_tsc >= (uint64_t) timer_period)) {			             
            /* reset the timer */
            if(msec_count){
				Qcur = Qlist[msec_count % 2];
				bool_trans = 1;
			}
            *timer_tsc = 0;
            return 1;	
				
	    }else
	        return 0;
	}else
		return 0;
}
#endif

int compInc(const void *a, const void *b)  
{  
	//return *(int *)a - *(int *)b; 
	const float *af = (const float *)a;
	const float *bf = (const float *)b;
	return *af > *bf ? 1 : -1;
} 	   

void clear(void)
{
#ifdef TEST_X86
    memset(req_count,0,sizeof(int)*REQ_HASH_ENTRIES);
#else
    memset(req_hy_count,0,sizeof(int)*REQ_HASH_ENTRIES);
#endif
}

float avg(float a[],unsigned int n)
{
    unsigned int i;
    float result;
    float sum = 0;
    for(i = 0;i < n;i++)
    sum += a[i];
    result = sum / n;
    return result;
}

void cdf_acktime(float *ack_time_pro,unsigned int idx)
{	
	float w[POINT + 2],w_95,w_999,w_9999;
	float u[POINT + 2],u_95,u_999,u_9999;
	unsigned int i = 0;
	
	qsort(ack_time_pro,idx,sizeof(float), compInc);

	//printf("ack_time_min:%f\n",ack_time_pro[0]);
#ifdef TEST_X86
	avg_delay = avg(ack_time_pro,(int)(0.99*idx));
	
#else
	avg_delay_hy = avg(ack_time_pro,(int)(0.99*idx));
#endif

#ifdef OP_MYSQL
	//open the AUTOCOMMIT
	unsigned int t = 0;
 	t = mysql_real_query(&g_conn,"SET AUTOCOMMIT =0",(unsigned int)strlen("SET AUTOCOMMIT =0"));
 	if(t)
    {
     	printf("fail to open commit\n");
 	}
#endif

#ifdef DEBUG 
	FILE *fp = fopen(wzlog, "a");

	fprintf(fp, "=====================\n%d\n", idx);
#endif

		
	for(i = 0;i < POINT;i++) //zhangwl@20160715
	{
		u[i] = (float)(i)/POINT;
		
		w[i] = ack_time_pro[(int)(u[i]*idx)];
		
#ifdef DEBUG 
		fprintf(fp, "%.2f: %.2f\n; ", w[i], u[i]);
#endif

#ifdef OP_MYSQL 
#ifdef TEST_X86
		if(insert_mysql(w[i],u[i],i)) 
#else
		if(insert_mysql_hy(w[i],u[i],i))
#endif
		    printf("Error when update row =%d\n",i);
		else
		    insert_tag = 1;
#endif
	}
	
	u_95 = AREA_7;
	w_95 = ack_time_pro[(int)(AREA_7*idx)];

	u[POINT] = AREA_8;
	
	w[POINT] = ack_time_pro[(int)(AREA_8*idx)];
	
	delay_99 = w[POINT];
	
	u_999 = AREA_9;
    w_999 = ack_time_pro[(int)(AREA_9*idx)];
    u_9999 = AREA_10;
    w_9999 = ack_time_pro[(int)(AREA_10*idx)];

#ifdef OP_MYSQL 

#ifdef TEST_X86
	if(insert_mysql(w[POINT],u[POINT],POINT))
#else
	if(insert_mysql_hy(w[POINT],u[POINT],POINT))
		printf("Error when update row =%d\n",POINT);
#endif

#endif

#ifdef DEBUG 
		fprintf(fp, "%.2f: %.2f\n; ", w_95, u_95);
		fprintf(fp, "%.2f: %.2f\n", w[POINT], u[POINT]);
		fprintf(fp, "%.3f: %.3f\n; ", w_999, u_999);
        fprintf(fp, "%.4f: %.4f\n", w_9999, u_9999);
		fclose(fp);
#endif
		//u[POINT+1] = 1.0;
		u[POINT+1] = 1.0;
        	//w[POINT+1] = w[POINT] + 2.0;
		w[POINT+1] = w[POINT] + 2.0;

#ifdef OP_MYSQL 

#ifdef TEST_X86
		if(insert_mysql(w[POINT+1],u[POINT+1],POINT+1))
#else
		if(insert_mysql_hy(w[POINT+1],u[POINT+1],POINT+1))
#endif
		printf("Error when update row =%d\n",POINT+1);
#endif

#ifdef OP_MYSQL              
        mysql_real_query(&g_conn,"COMMIT;",(unsigned int)strlen("COMMIT;"));
#endif

		if(idx){   //zhangwl@20160715
			printf("<%.3f:%.1f,<%.3f:%.1f,<%.3f:%.1f,<%.3f:%.1f,<%.3f:%.2f,<%.3f:%.2f,<%.3f:%.2f,<%.3f:%.2f\n",w[0],u[0],w[1],u[1],w[2],u[2],w[5],u[5],w[7],u[7],w[9],u[9],w_95,u_95,w[POINT],u[POINT]);
#ifdef PY_PLOT
			plotPoint2d(w,u);
#endif
		}else{
			printf("not receive data yet!\n");
		}
}

void lcore_online(void)
{ 

#ifdef OP_MYSQL
	int t = 0,i = 0;
    printf("start to insert init");
    for(i = 20;i > 0;i--)
	{
        t = mysql_real_query(&g_conn_init,"SET AUTOCOMMIT =0",(unsigned int)strlen("SET AUTOCOMMIT =0"));
        if(t){
        	printf("fail to open commit\n");
        }else{
            if(insert_mysql_init(i))
            	printf("insert init error!\n");
            mysql_real_query(&g_conn_init,"COMMIT;",(unsigned int)strlen("COMMIT;"));
         }
 	}
#endif

	if(lock_flag < BLOCK)
		lock_flag ++;
	
	while(1){
		//wait for several sec
		sleep(60);
#ifdef TEST_X86
		conn_active_mid = conn_active;
		conn_active = 0;
#else
		conn_active_mid = conn_active_hy;
		conn_active_hy = 0;
#endif

		traffic_mid = traffic;
        traffic = 0;

		clear();
		
		if(lock_flag && (idx_x86 || idx_hy)){
#ifdef TEST_X86
			printf("receive %lu response\n",idx_x86);
			if(idx_x86 < thre)
				memcpy(ack_time_x86,ack_time,sizeof(float)*(idx_x86-1));
 			else{
                float *ack_time_new = (float*)malloc(sizeof(float)*idx_x86 * 2);
				printf("new alloc!\n");
                free(ack_time_x86);
				printf("free old!\n");
				ack_time_x86 = ack_time_new;
				printf("new copy!\n");
				memcpy(ack_time_x86,ack_time,sizeof(float)*(idx_x86-1));
				printf("new copy done!\n");
				thre = idx_x86 * 2;
			}
			idx = idx_x86 - 1;
			idx_x86 = 0;
			cdf_acktime(ack_time_x86,idx);
#else
			//printf("idx_hy:%ld\n",idx_hy);
			if(idx_hy < thre_hy)
				memcpy(ack_time_hy,ack_time,sizeof(float)*(idx_hy-1));
			else{
				thre_hy = idx_hy * 2;
				ack_time_hy	= (float*)realloc(ack_time_hy,thre_hy*sizeof(float));
				memcpy(ack_time_hy,ack_time,sizeof(float)*(idx_hy-1));
 			}			
			idx = idx_hy - 1;
			idx_hy = 0;
			cdf_acktime(ack_time_hy,idx);
#endif
		}else{
			lock_flag = 1;			
 		}
		
 	}
}

int response_time_process(struct node_data *data,uint16_t nb_rx,uint16_t socket_id)
{
    long ret = 0;
    long ret_req = 0;
    
    if (data->type == REQ) // || data->type == REQU_APP)//enter the request process:
    {
        pkt_req++; //count the request pkts
        
        ret_req = rte_hash_add_key(req_lookup_struct[socket_id],(void *) &(data->key));

        if (ret < 0) {
            printf("ret error!\n");
    		rte_exit(EXIT_FAILURE, "Unable to add entry %u\n",nb_rx);
    	}	
    			
#ifdef TEST_X86
    	if(req_count[ret_req] == 0)
    	{
    		req_count[ret_req] = 1;
    		conn_active++;            
    	}
#else
    	if(req_hy_count[ret_req] == 0)
        {
            req_hy_count[ret_req] = 1;
            conn_active_hy++;
        }
#endif
        //traffic += data->total_len + 14; 

    	if(Maxseq[ret_req] < data->sent_seq)
    	{
    		Maxseq[ret_req] = data->sent_seq;
    		req_out_if[ret_req].tv_sec = data->ts.tv_sec;
    		req_out_if[ret_req].tv_nsec = data->ts.tv_nsec;
    		
    	}else{
    		//recount++;//count the retransmission pkts
    	}

    	return 1;

    }else if(data->type == REP )//RESP_APP || data->type == RESP_USR)
    {
        float ack = 0.0;
        struct timespec t_req;		
		/*FILE *fp;
		fp = fopen("response.txt","a+");*/
 		
		pkt_rep++; //count the response pkts
        ret = rte_hash_lookup(req_lookup_struct[socket_id],(const void *)&(data->key));


		if ((data->sent_seq - Maxseq[ret] > data->total_len) || ret < 0) {
            return -2;
        } 

		//fprintf(fp,"recv_seq:%d,sent_seq:%d,pkt_len:%d\n",data->sent_seq,Maxseq[ret],data->sent_seq - Maxseq[ret]);



 	    t_req.tv_sec = req_out_if[ret].tv_sec;//-2 presents not find req
        t_req.tv_nsec = req_out_if[ret].tv_nsec;
		        
		//fclose(fp);
	
		if(!(nb_rx % 1)) 
		{
			ack = (data->ts.tv_sec - t_req.tv_sec)*1000 + ((float)(data->ts.tv_nsec - t_req.tv_nsec)/1000000);
	
			if(ack > 0 && ack < 200)
			{
#ifdef TEST_X86
				if(idx_x86 > thre)
                {
                    float *ack_time_new = (float*)malloc(sizeof(float)*idx_x86 * 2);
				    memcpy(ack_time_new,ack_time,sizeof(float)*(idx_x86-1));
                    free(ack_time);
                    ack_time = ack_time_new;
                    thre = idx_x86 * 2;
                }
					
				ack_time[idx_x86] = ack;
				idx_x86++;
				if (ack > 50)
				{
					timeout_50++;			
				}
				
#else
				if(idx_hy > thre)
                {
                    thre = idx_hy * 2;
                    ack_time = (float*)realloc(ack_time,thre*sizeof(float));
                }
				ack_time[idx_hy] = ack;
				idx_hy++;
				if (ack > 50)
                    timeout_50++;
			
#endif
			}					
				
					
        }
   
        return 1;
    }else
		return -1;      
}

int res_setup_hash(uint16_t socket_id)
{
    char s[64];
    /* create ipv4 hash */
    snprintf(s, sizeof(s), "ipv4_hash_%d", socket_id);
    ipv4_req_hash_params.name = s;
    ipv4_req_hash_params.socket_id = socket_id;
    req_lookup_struct[socket_id] = rte_hash_create(&ipv4_req_hash_params);
    if (req_lookup_struct[socket_id] == NULL)
        rte_exit(EXIT_FAILURE, "Unable to create the request hash on "
                            "socket %d\n", socket_id);
    else
     printf("Success creat request hash on socket %d\n",socket_id);
	/*creat delay time store buffer*/
	ack_time = (float*)malloc(sizeof(float)*MAX_BUFF);
#ifdef TEST_X86
	ack_time_x86 = (float*)malloc(sizeof(float)*MAX_BUFF);
#else
	ack_time_hy = (float*)malloc(sizeof(float)*MAX_BUFF);
#endif
	return 1;
}


int key_extract(struct ipv4_hdr *ip_hdr,struct node_data *data,struct timespec ts_now)
{
    struct tcp_hdr  *tcp;

    tcp = (struct tcp_hdr *)((unsigned char *) ip_hdr + sizeof(struct ipv4_hdr));
    unsigned char *req_bit = (unsigned char *) tcp + (tcp->data_off >> 2); 

    if (likely(req_bit[POFFSET] == REQU_USR || req_bit[POFFSET] == REQU_APP))
    { 
        data->key.ip_src = rte_be_to_cpu_32(ip_hdr->src_addr);
                
        data->key.port_src = rte_be_to_cpu_16(tcp->src_port);

        data->type = req_bit[POFFSET];

        data->total_len = rte_be_to_cpu_16(ip_hdr->total_length);

        data->sent_seq = rte_be_to_cpu_32(tcp->sent_seq);

        data->ts.tv_sec = ts_now.tv_sec;

        data->ts.tv_nsec = ts_now.tv_nsec;

        return 1;
    }else if(req_bit[POFFSET] == RESP_APP)//RESP_USR || req_bit[0] == RESP_APP)
    {
        
        data->key.ip_src = rte_be_to_cpu_32(ip_hdr->dst_addr);
            
        data->key.port_src = rte_be_to_cpu_16(tcp->dst_port);

        data->type = req_bit[POFFSET];

        data->total_len = rte_be_to_cpu_16(ip_hdr->total_length);

        data->sent_seq = rte_be_to_cpu_32(tcp->recv_ack);

        data->ts.tv_sec = ts_now.tv_sec;

        data->ts.tv_nsec = ts_now.tv_nsec;

        return 1;
    }else{
		printf("req_bit:%d\n",req_bit[0]);
        return 0;
	}
}

int packet_process(struct ipv4_hdr *ip_hdr,struct timespec ts_now)
{	
	if(likely(deque != (occupy + 1) % max_size)){    
    	//int la_key = key_extract(ip_hdr,RxQue + occupy,ts_now); 
    	uint8_t la_key = http_parse(ip_hdr,RxQue + occupy,ts_now);
		if(likely(la_key > 0))
    	{       
			occupy = (occupy + 1) % max_size;
        	return 1;			
    	}else{
    		printf("Enqueue failed\n");
            return la_key;		
		}		
	}else{
		RxQue = (QType*)realloc(RxQue,(2 * BUFSIZE)*sizeof(struct node_data));
		max_size = 2 * BUFSIZE;
		int la_key = http_parse(ip_hdr,RxQue + occupy,ts_now); 
		if(la_key > 0)
    	{       
			occupy = (occupy + 1) % max_size;
        	return 1;		
    	}else{		
			printf("Enqueue failed\n");
            return la_key;
		}	
	}
    		
}

