#include<stdlib.h>
#include<string.h>
#include "config.h"

const char *cfg_file = "setting.cfg";

struct mconfig* initConfig()
{
  conf = (struct mconfig *)malloc(sizeof(struct mconfig));
  memset(conf, 0, sizeof(struct mconfig));
  conf->enable_http    = 1;
  conf->label_offset   = 6;
  conf->pri_offset     = 5;
  conf->pri_high_label = 1;
  return conf;
}

int getConfig(struct mconfig *conf)
{
    config_t cfg;
    config_setting_t *req_label, *resp_label;
    int n, label_count;

    conf = initConfig();

    config_init(&cfg);
    if (! config_read_file(&cfg, cfg_file)) {
        perror("config_read_file");
        config_destroy(&cfg);
        return -1;
    }

    config_lookup_int(&cfg, "enableHTTP", &(conf->enable_http));
    config_lookup_int(&cfg, "enableHY", &(conf->enable_hy));
    config_lookup_int(&cfg, "enablePri", &(conf->enable_pri));
    config_lookup_int(&cfg, "enablePython", &(conf->enable_python));
    config_lookup_int(&cfg, "enableSQL", &(conf->enable_sql));

    req_label = config_lookup(&cfg, "request_label");
    resp_label = config_lookup(&cfg, "response_label");
    label_count = config_setting_length(req_label);
    label_count = label_count >= MAXLABEL ? MAXLABEL : label_count;
    for (n = 0; n < label_count; n++) {
        conf->req_label[n] = config_setting_get_int_elem(req_label, n); 
        conf->resp_label[n] = config_setting_get_int_elem(resp_label, n); 
        conf->label_count += 1;
    }
    config_lookup_int(&cfg, "LabelOffset", &(conf->label_offset));
    config_lookup_int(&cfg, "PriOffset", &(conf->pri_offset));
    config_lookup_int(&cfg, "PriLabelHigh", &(conf->pri_high_label));
    config_lookup_int(&cfg, "PriLabelLow", &(conf->pri_low_label));

    printf("Enable HTTP: %d\n", conf->enable_http);
    config_destroy(&cfg);
    return 0;
}

