/*** Server configuration ****************************************************/

/* Server to stream onto */
static Server server            = TwitchPrimary;
/* Put your stream key here */
static char *streamkey          = "";

/*** Streamer configuration **************************************************/

/* Video bitrate */
static char *videoBitrate       = "4500k";
/* Video codec */
static char *videoCodec         = "libx264";

/* Audio bitrate */
static char *audioBitrate       = "128k";
/* Audio codec */
static char *audioCodec         = "aac";
/* Audio sampling frequency */
static char *samplingFreq       = "44100";

/* Constant Rate Factor (for x264 - 0: best, 23: default, 51: worst) */
static char *crf                = "23";
/* Preset (for x264 - ultrafast, superfast, veryfast, faster, fast, medium, slow, slower, veryslow) */
static char *preset             = "medium";
/* Threads count (0 means optimal) */
static char *threads            = "0";

/*****************************************************************************/

/*
"ffmpeg "
	"$SBS_input_flags "
	"-pix_fmt yuvj420p "
	"-x264-params keyint=48:min-keyint=48:scenecut=-1 "
	"-b:v \"$SBS_videoBitrate\" "
	"-vcodec \"$SBS_videoBitrate\" "
	"-b:a \"$SBS_audioBitrate\" "
	"-acodec \"$SBS_audioBitrate\" "
	"-ar \"$SBS_samplingFreq\" "
	"-crf \"$SBS_crf\" "
	"-preset \"$SBS_preset\" "
	"-threads \"$SBS_threads\" "
	"-f flv "
	"$SBS_stream"
*/
