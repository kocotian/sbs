#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arg.h>
#include <util.h>

/* Macros */
#define VAR2ENV(N) setenv("SBS_" #N, (N), 1)
#define ENV2STR_NOESCAPE(N) "$SBS_" #N
#define ENV2STR(N) "\"" ENV2STR_NOESCAPE(N) "\""

/* Types */
enum Server {
	TwitchPrimary, TwitchPoland, TwitchAustria, TwitchCzechRepublic,
	TwitchGermanyBerlin, TwitchDenmark, TwitchSweden, TwitchFinland,
	TwitchGermanyDusseldorf, TwitchItaly, TwitchNetherlands,

	YouTubePrimary,
};

typedef enum Server Server;

struct Input {
	int (*function)(int argc, char *argv[]);
	char *name;
	char *description;
};

typedef struct Input Input;

/* Prototypes */
static int streamFile(int argc, char *argv[]);
static int usage();

/* Config */
#include "config.h"

/* Global Vars */
static char *servers[] = {
	[TwitchPrimary] = "rtmp://iad05.contribute.live-video.net/app/%s",
	[TwitchPoland] = "rtmp://waw.contribute.live-video.net/app/%s",
	[TwitchAustria] = "rtmp://vie02.contribute.live-video.net/app/%s",
	[TwitchCzechRepublic] = "rtmp://prg.contribute.live-video.net/app/%s",
	[TwitchGermanyBerlin] = "rtmp://ber.contribute.live-video.net/app/%s",
	[TwitchDenmark] = "rtmp://cph.contribute.live-video.net/app/%s",
	[TwitchSweden] = "rtmp://arn.contribute.live-video.net/app/%s",
	[TwitchFinland] = "rtmp://hel.contribute.live-video.net/app/%s",
	[TwitchGermanyDusseldorf] = "rtmp://dus01.contribute.live-video.net/app/%s",
	[TwitchItaly] = "rtmp://mil02.contribute.live-video.net/app/%s",
	[TwitchNetherlands] = "rtmp://ams03.contribute.live-video.net/app/%s",

	[YouTubePrimary] = "rtmp://a.rtmp.youtube.com/live2/%s",
};

char *argv0;

static char *serverfmt = NULL;
static char serverurl[512] = {'\0'};

static Input inputs[] = {
	/* function     name        description */
	{ streamFile,   "file",     "stream a file content" },
	{ usage,        "help",     "prints this help message" },
};

/* Function definitions */
static int
setenv_config(void)
{
	VAR2ENV(videoBitrate);
	VAR2ENV(videoCodec);

	VAR2ENV(audioBitrate);
	VAR2ENV(audioCodec);
	VAR2ENV(samplingFreq);

	VAR2ENV(crf);
	VAR2ENV(preset);
	VAR2ENV(threads);
}

static int
streamFile(int argc, char *argv[])
{
	char inputFlags[1024], *command;

	if (argc != 2)
		die("usage: %s [...] %s FILENAME", argv0, *argv);

	snprintf(inputFlags, LEN(inputFlags), "%s", argv[1]);

	VAR2ENV(inputFlags);
	VAR2ENV(serverurl);

	command = "ffmpeg -re -i "
		ENV2STR_NOESCAPE(inputFlags) " "
		"-pix_fmt yuvj420p "
		"-x264-params keyint=48:min-keyint=48:scenecut=-1 "
		"-b:v " ENV2STR(videoBitrate) " "
		"-vcodec " ENV2STR(videoCodec) " "
		"-b:a " ENV2STR(audioBitrate) " "
		"-acodec " ENV2STR(audioCodec) " "
		"-ar " ENV2STR(samplingFreq) " "
		"-crf " ENV2STR(crf) " "
		"-preset " ENV2STR(preset) " "
		"-threads " ENV2STR(threads) " "
		"-f flv "
		ENV2STR(serverurl);

	puts(command);
	system(command);
}

static int
usage()
{
	size_t i;
	fprintf(stderr, "usage: %s [-hv] [-s SERVER] [-k KEY] INPUT [INPUT_ARGS]\n", argv0);
	fprintf(stderr, "\nINPUT can be:\n");
	for (i = 0; i < LEN(inputs); ++i) {
		fprintf(stderr, "- %-14s - %s\n", inputs[i].name, inputs[i].description);
	}
	exit(1);
}

/* Main */
int
main(int argc, char *argv[])
{
	size_t i = 0;

	ARGBEGIN {
	case 's':
		serverfmt = ARGF();
		break;
	case 'k':
		streamkey = ARGF();
		break;
	case 'v':
		die("sbs-%s", VERSION);
		break;
	case 'h': default: /* FALLTHROUGH */
		usage();
		break;
	} ARGEND

	if (serverfmt == NULL)
		serverfmt = servers[server];

	if (serverfmt == NULL || !strlen(serverfmt))
		die("server url is empty or unset");

	snprintf(serverurl, 512, serverfmt, streamkey);

	if (system("ffmpeg --help > /dev/null 2>&1") != 0)
		die("Failed to execute ffmpeg");

	if (!argc)
		usage();

	setenv_config();

	for (i = 0; i < LEN(inputs); ++i) {
		if (!strcmp(inputs[i].name, *argv))
			(inputs[i].function)(argc, argv);
	}

	if (i == LEN(inputs))
		usage();

	return 0;
}
