#include "app.h"

int usage(char *appname);
int onexit();


void sig_handler(int signum)
{
    fprintf(stderr, "is going to exit!");
    onexit();
    exit(0);
}

void init_sig_handler()
{
    if (signal(SIGINT, sig_handler) == SIG_IGN)
        signal(SIGINT, SIG_IGN);
    if (signal(SIGHUP, sig_handler) == SIG_IGN)
        signal(SIGHUP, SIG_IGN);
    if (signal(SIGTERM, sig_handler) == SIG_IGN)
        signal(SIGTERM, SIG_IGN);
}


int init_app(int argc, char **argv, char *appname)
{
    init_sig_handler();
    //char *wrkdir;
    int ch;
    //uint8_t runmode;
    int rundaemon, logundefined;
    struct rlimit rls;

    char cfgfile[999];
    sprintf(cfgfile, "etc/%s.cfg", appname);
    rundaemon = 1;
    //runmode = RM_RESTART;
    logundefined = 1;           //TODO
    //appname = argv[0];

    while ((ch = getopt(argc, argv, "vduc:h?")) != -1) {
        switch (ch) {
        case 'd':
            rundaemon = 0;
            break;
        case 'c':
            strncpy(cfgfile, optarg, sizeof(cfgfile));
            break;
        case 'u':
            logundefined = 1;
            break;
        case 'v':
            printf("version: %s\n", APP_VERSION);
            exit(0);
        default:
            usage(appname);
            exit(0);
        }
    }
    fprintf(stderr, "take %s as config file: ", cfgfile);

    if (cfg_load(cfgfile, logundefined) == 0) {
        fprintf(stderr, "can't load config file: %s - using defaults\n",
                cfgfile);
    }
    cfg_add("CFG_FILE", cfgfile);

    argc -= optind;
    argv += optind;

    char *bname = basename(cfgfile);
    char logfile[999];
    sprintf(logfile, "log/%s.log", bname);
    log_init(logfile);
    //if (argc==1) {
    //    if (strcasecmp(argv[0],"start")==0) {
    //        //runmode = RM_START;
    //    } else if (strcasecmp(argv[0],"stop")==0) {
    //        //runmode = RM_STOP;
    //    } else if (strcasecmp(argv[0],"restart")==0) {
    //        //runmode = RM_RESTART;
    //    } else if (strcasecmp(argv[0],"reload")==0) {
    //        //runmode = RM_RELOAD;
    //    } else {
    //        usage(appname);
    //        exit(1);
    //    }
    //} else if (argc!=0) {
    //    usage(appname);
    //    exit(1);
    //}

    rls.rlim_cur = MAX_FILES;
    rls.rlim_max = MAX_FILES;

    if (setrlimit(RLIMIT_NOFILE, &rls) < 0) {

        fprintf(stderr, "can't change open files limit to %u\n", MAX_FILES);
    }
    /*

       wrkdir = cfg_getstr("DATA_PATH",DATA_PATH);
       fprintf(stderr,"working directory: %s\n",wrkdir);

       if (chdir(wrkdir)<0) {
       fprintf(stderr,"can't set working directory to %s\n",wrkdir);
       syslog(LOG_ERR,"can't set working directory to %s",wrkdir);
       return 1;
       }
       free(wrkdir);

       if ((runmode==RM_START || runmode==RM_RESTART) && rundaemon) {
       msgfd = makedaemon();
       } else {
       if (runmode==RM_START || runmode==RM_RESTART) {
       set_signal_handlers();
       }
       msgfd = fdopen(dup(STDERR_FILENO),"w");
       setvbuf(msgfd,(char *)NULL,_IOLBF,0);
       }

       umask(027);
     */
    return 0;
}
