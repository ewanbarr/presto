/* Maximum number of samples (in time) to process at a time */
#define SPIGOT_MAXPTSPERBLOCK 1024
/* Maximum number of lags we can have for each sample */
#define SPIGOT_MAXLAGS 2048
/* Maximum data block length in bytes (the 4 is for up to 4 channels (i.e. full Stokes)) */
#define SPIGOT_MAXDATLEN SPIGOT_MAXPTSPERBLOCK*SPIGOT_MAXLAGS*4
/* Maximum block length in bytes for the raw lags */
/* The 2 comes from using 16bit lags              */
#define SPIGOT_MAXLAGLEN SPIGOT_MAXDATLEN*2

typedef struct SPIGOT_INFO{
  char instrument[40];    /* Device or program of origin */
  char software_vers[40]; /* Version of observing software */
  char telescope[40];	  /* Telescope used */
  char object[40];	  /* Source observed */
  char observer[40];	  /* Name(s) of observer(s) */
  char project_id[16];	  /* Project identifier */
  char obs_id[16];	  /* Observation identifier */
  char date_obs[16];	  /* Start of observation (YYYY-MM-DD) */
  char time_obs[16];	  /* Start of observation (HH:MM:SS) */
  char time_sys[16];	  /* Time scale specification */
  char coord_sys[16];	  /* Offset coordinate mode of GBT */
  char ra_str[16];	  /* RA  of observation (deg, J2000) */
  char dec_str[16];	  /* Dec of observation (deg, J2000) */
  char pol_type[8];	  /* Polarization recorded (L or C) */
  char corr_mode[8];	  /* Spigot correlator mode */
  double dt_us;		  /* Sampling time (us) */
  double freq_ctr;	  /* Sky center freq (MHz) for sampler 1 */
  double bandwidth;	  /* Bandwidth (MHz) for sampler 1 */
  double MJD_obs;         /* Start of observation (MJD) */
  double file_duration;   /* Duration of the observation (s) in this file (s) */
  double elapsed_time;    /* Total time elapsed (s) in obs at the start of this file */
  double start_block;     /* Starting block number in this file */
  double end_block;       /* Last block number in this file */
  int scan_number;	  /* Number of scan */
  int header_len;	  /* Size of header (bytes) */
  int sec_obs;		  /* Start time in sec since MJD=40587.0 */
  int tracking;		  /* Tracking (T) or drift scan (F) */
  int num_samplers;	  /* Number of samplers */
  int summed_pols;	  /* Are polarizations summed? */
  int upper_sideband;	  /* Upper sideband? */
  int bits_per_lag;	  /* Bits/lag */
  int lags_per_sample;	  /* Number of lags/sample */
  int samples_per_file;	  /* Number of samples in this file */
  int tot_num_samples;    /* Total (planned) number of samples */
  int num_blocks;         /* Number of working data blocks in this file */
  int padding_samples;    /* Number of padding samples to add after this file */
} SPIGOT_INFO;

/* spigot.c */

void set_SPIGOT_padvals(float *fpadvals, int good_padvals);
int read_SPIGOT_header(char *filename, SPIGOT_INFO *spigot);
void print_SPIGOT_header(SPIGOT_INFO *spigot);
void SPIGOT_INFO_to_inf(SPIGOT_INFO *spigot, infodata *idata);

int read_SPIGOT_rawblock(FILE *infiles[], int numfiles, unsigned char *data, 
			 int *padding, IFs ifs);
int read_SPIGOT_rawblocks(FILE *infiles[], int numfiles, unsigned char rawdata[], 
			  int numblocks, int *padding, IFs ifs);
int read_SPIGOT(FILE *infiles[], int numfiles, float *data, int numpts, 
		double *dispdelays, int *padding, int *maskchans, 
		int *nummasked, mask *obsmask, IFs ifs);
void get_SPIGOT_channel(int channum, float chandat[], 
			unsigned char rawdata[], int numblocks);
void get_SPIGOT_file_info(FILE *files[], SPIGOT_INFO *spigot_files, 
			  int numfiles, int usewindow,
			  float clipsig, long long *N, int *ptsperblock, 
			  int *numchan, double *dt, double *T, 
			  infodata *idata, int output);
int prep_SPIGOT_subbands(unsigned char *rawdata, float *data, 
			 double *dispdelays, int numsubbands, 
			 int transpose, int *maskchans, 
			 int *nummasked, mask *obsmask);
int read_SPIGOT_subbands(FILE *infiles[], int numfiles, float *data, 
			 double *dispdelays, int numsubbands, int transpose, 
			 int *padding, int *maskchans, int *nummasked, 
			 mask *obsmask, IFs ifs);
void convert_SPIGOT_point(void *rawdata, unsigned char *bytes, IFs ifs);
void SPIGOT_update_infodata(int numfiles, infodata *idata);
int skip_to_SPIGOT_rec(FILE *infiles[], int numfiles, int rec);
int check_SPIGOT_byteswap(char *hdr);