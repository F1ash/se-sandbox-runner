#include "pgrep.h"

// EXIT_SUCCESS is 0
// EXIT_FAILURE is 1
#define EXIT_USAGE 2
#define EXIT_FATAL 3

static const char *progname = "pgrep";

union el {
    long	num;
    char *	str;
};

/* User supplied arguments */

static int opt_full = 0;
static int opt_long = 0;
static int opt_oldest = 0;
static int opt_newest = 0;
static int opt_negate = 0;
static int opt_exact = 0;
//static int opt_lock = 0;
static int opt_case = 0;

static const char *opt_delim = "\n";
static union el *opt_pgrp = NULL;
static union el *opt_rgid = NULL;
static union el *opt_pid = NULL;
static union el *opt_ppid = NULL;
static union el *opt_sid = NULL;
static union el *opt_term = NULL;
static union el *opt_euid = NULL;
static union el *opt_ruid = NULL;
static char *opt_pattern = NULL;
static char *opt_pidfile = NULL;

static union el *split_list (const char * str, int (*convert)(const char *, union el *))
{
    fprintf (stderr, "%s: 1_1_0\n",progname);
    fprintf (stderr, "%s: %s\n",progname,str);
    char *copy = strdup (str);
    fprintf (stderr, "%s: 1_1_0_1 %s\n",progname, copy);
    char *ptr = copy;
    char *sep_pos;
    int i = 0;
    int size = 0;
    union el *list = NULL;

    fprintf (stderr, "%s: 1_1_1\n",progname);

    do {
        if (i == size) {
            size = size * 5 / 4 + 4;
            // add 1 because slot zero is a count
            list = (el*) realloc (list, 1 + size * sizeof *list);
            if (list == NULL)
                exit (EXIT_FATAL);
        }
        fprintf (stderr, "%s: 1_1_1_1\n",progname);
        sep_pos = strchr (ptr, ',');
        fprintf (stderr, "%s: 1_1_1_2\n",progname);
        if (sep_pos)
            *sep_pos = 0;
        // Use ++i instead of i++ because slot zero is a count
        if (!convert (ptr, &list[++i]))
            exit (EXIT_USAGE);

        fprintf (stderr, "%s: 1_1_1_3\n",progname);
        if (sep_pos)
            ptr = sep_pos + 1;
    } while (sep_pos);

    fprintf (stderr, "%s: 1_1_2\n",progname);

    free (copy);

    fprintf (stderr, "%s: 1_1_3_(%d)\n",progname, i);
    if (!i) {
        free (list);
        list = NULL;
    } else {
        list[0].num = i;
    }

    fprintf (stderr, "%s: 1_1_4\n",progname);
    return list;
}

// strict_atol returns a Boolean: TRUE if the input string
// contains a plain number, FALSE if there are any non-digits.
static int strict_atol (const char * str, long * value)
{
    int res = 0;
    int sign = 1;

    if (*str == '+')
        ++str;
    else if (*str == '-') {
        ++str;
        sign = -1;
    }

    for ( ; *str; ++str) {
        if (! isdigit (*str))
            return (0);
        res *= 10;
        res += *str - '0';
    }
    *value = sign * res;
    return 1;
}

#include <sys/file.h>

// Seen non-BSD code do this:
//
//if (fcntl_lock(pid_fd, F_SETLK, F_WRLCK, SEEK_SET, 0, 0) == -1)
//                return -1;
int fcntl_lock(int fd, int cmd, int type, int whence, int start, int len)
{
        struct flock lock[1];

        lock->l_type = type;
        lock->l_whence = whence;
        lock->l_start = start;
        lock->l_len = len;

        return fcntl(fd, cmd, lock);
}

// We try a read lock. The daemon should have a write lock.
// Seen using flock: FreeBSD code
static int has_flock(int fd)
{
    return flock(fd, LOCK_SH|LOCK_NB)==-1 && errno==EWOULDBLOCK;
}

// We try a read lock. The daemon should have a write lock.
// Seen using fcntl: libslack
static int has_fcntl(int fd)
{
    struct flock f;  // seriously, struct flock is for a fnctl lock!
    f.l_type = F_RDLCK;
    f.l_whence = SEEK_SET;
    f.l_start = 0;
    f.l_len = 0;
    return fcntl(fd,F_SETLK,&f)==-1 && (errno==EACCES || errno==EAGAIN);
}

static union el *read_pidfile(void)
{
    char buf[12];
    int fd;
    struct stat sbuf;
    char *endp;
    int pid;
    union el *list = NULL;
    char *res;
    strcpy(res,"fail\n");

    fd = open(opt_pidfile, O_RDONLY|O_NOCTTY|O_NONBLOCK);
    fprintf(stderr, "g_0\n");
    if(fd<0)
        goto out;
    fprintf(stderr, "g_1\n");
    if(fstat(fd,&sbuf) || !S_ISREG(sbuf.st_mode) || sbuf.st_size<1)
        goto out;
    fprintf(stderr, "g_2\n");
    // type of lock, if any, is not standardized on Linux
    if(!has_flock(fd) && !has_fcntl(fd))
            goto out;
    fprintf(stderr, "g_3\n");
    memset(buf,'\0',sizeof buf);
    buf[read(fd,buf+1,sizeof buf-2)] = '\0';
    pid = strtoul(buf+1,&endp,10);
    if(endp<=buf+1 || pid<1 || pid>0x7fffffff)
        goto out;
    fprintf(stderr, "g_4\n");
    if(*endp && !isspace(*endp))
        goto out;
    fprintf(stderr, "g_5\n");
    list = (el*) malloc(2 * sizeof *list);
    list[0].num = 1;
    list[1].num = pid;
    strcpy(res,"success\n");
out:
    close(fd);
    fprintf(stderr, "%s: %s", progname, res);
    return list;
}

static int conv_num (const char * name, union el * e)
{
    if (! strict_atol (name, &e->num)) {
        fprintf (stderr, "%s: not a number: %s\n",
             progname, name);
        return 0;
    }
    return 1;
}

static int match_numlist (long value, const union el * list)
{
    int found = 0;
    if (list == NULL)
        found = 0;
    else {
        int i;
        for (i = list[0].num; i > 0; i--) {
            if (list[i].num == value)
                found = 1;
        }
    }
    return found;
}

static int match_strlist (const char * value, const union el * list)
{
    int found = 0;
    if (list == NULL)
        found = 0;
    else {
        int i;
        for (i = list[0].num; i > 0; i--) {
            if (! strcmp (list[i].str, value))
                found = 1;
        }
    }
    return found;
}

static char *output_numlist (const union el * list, int num)
{
    char *str = (char*) malloc(256);
    strcpy(str, "");
    for (int i = 0; i < num; i++)
      {
        char *buf =(char*) malloc(64);
        snprintf(buf, 64, "%ld%s", list[i].num, opt_delim);
        strcat(str, buf);
      };
    fprintf (stderr, "%s ======", str);
    return str;
}

static PROCTAB *do_openproc (void)
{
    PROCTAB *ptp;
    int flags = 0;

    if (opt_pattern || opt_full)
        flags |= PROC_FILLCOM;
    if (opt_ruid || opt_rgid)
        flags |= PROC_FILLSTATUS;
    if (opt_oldest || opt_newest || opt_pgrp || opt_sid || opt_term)
        flags |= PROC_FILLSTAT;
    if (!(flags & PROC_FILLSTAT))
        flags |= PROC_FILLSTATUS;  // FIXME: need one, and PROC_FILLANY broken
    if (opt_euid && !opt_negate) {
        int num = opt_euid[0].num;
        int i = num;
        uid_t *uids = (uid_t*) malloc (num * sizeof (uid_t));
        if (uids == NULL)
            exit (EXIT_FATAL);
        while (i-- > 0) {
            uids[i] = opt_euid[i+1].num;
        }
        flags |= PROC_UID;
        ptp = openproc (flags, uids, num);
    } else {
        ptp = openproc (flags);
    }
    return ptp;
}

static regex_t * do_regcomp (void)
{
    regex_t *preg = NULL;

    if (opt_pattern) {
        char *re;
        char errbuf[256];
        int re_err;

        preg = (regex_t*) malloc (sizeof (regex_t));
        if (preg == NULL)
            exit (EXIT_FATAL);
        if (opt_exact) {
            re = (char*) malloc (strlen (opt_pattern) + 5);
            if (re == NULL)
                exit (EXIT_FATAL);
            sprintf (re, "^(%s)$", opt_pattern);
        } else {
            re = opt_pattern;
        }

        re_err = regcomp (preg, re, REG_EXTENDED | REG_NOSUB | opt_case);
        if (re_err) {
            regerror (re_err, preg, errbuf, sizeof(errbuf));
            fputs(errbuf,stderr);
            exit (EXIT_USAGE);
        }
    }
    return preg;
}

static union el * select_procs (int *num, pid_t pid)
{
    PROCTAB *ptp;
    proc_t task;
    unsigned long long saved_start_time;      // for new/old support
    pid_t saved_pid = 0;                      // for new/old support
    int matches = 0;
    int size = 0;
    regex_t *preg;
    pid_t myself = getpid();
    union el *list = NULL;
    char cmd[4096];

    //ptp = do_openproc();
    int flags = 0;
    flags |= PROC_FILLSTATUS;
    flags |= PROC_FILLSTAT;
    ptp = openproc (flags);
    preg = do_regcomp();

    if (opt_newest) saved_start_time =  0ULL;
    if (opt_oldest) saved_start_time = ~0ULL;
    if (opt_newest) saved_pid = 0;
    if (opt_oldest) saved_pid = INT_MAX;

    memset(&task, 0, sizeof (task));
    while(readproc(ptp, &task)) {

        int match = 1;
        //fprintf(stderr, "Children %d <-- %d\n", task.ppid, task.tid);
        if (task.XXXID == pid){
            proc_t subtask;
                            memset(&subtask, 0, sizeof (subtask));
                            while (readtask(ptp, &task, &subtask)){
                                // don't add redundand tasks
                                fprintf(stderr, "%d %d Children %d <-- %d\n",myself, task.XXXID, subtask.ppid, subtask.XXXID);
                                if (task.XXXID == subtask.ppid)
                                    fprintf(stderr, "Children %d\n", subtask.XXXID);
                            }
    }

        memset (&task, 0, sizeof (task));
    }
    closeproc (ptp);

    *num = matches;
    return list;
}

static void parse_opts (char *argv)
{
    fprintf (stderr, "%s: 1_1\n",progname);
    opt_ppid = split_list (argv, conv_num);

    fprintf (stderr, "%s: 1_2 child: %ld\n",progname, opt_ppid[0].num);
    if (opt_ppid == NULL)
        fprintf (stderr, "%s:  args is uncorrect (2)\n",
                 progname);

    fprintf (stderr, "%s: 1_3\n",progname);
}

char *pgrep(char *argv)
{
    union el *procs;
    int num;
    fprintf (stderr, "%s: %s\n",progname, argv);
    parse_opts (argv);
    fprintf (stderr, "%s: 2 count: %d\n",progname, num);

    long int pid;
    strict_atol(argv, &pid);
    procs = select_procs (&num, pid);
    fprintf (stderr, "%s: 3 count: %d\n",progname, num);
    //output_numlist(procs,num);

    fprintf (stderr, "%s: 4\n",progname);
    return output_numlist(procs,num);
}
