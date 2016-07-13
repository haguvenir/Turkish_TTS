#include "TTSIncludes.h"
using namespace std;

// -- DEFINITIONS

#define TAGSTART		'['
#define TAGEND			']'
#define TAGCLOSE		'/'
#define TRUE			1
#define FALSE			0
#define MAX_TAG_LEN		400
#define DELIMITER_CHAR		'-'
#define DELIMITER_STRING	"-"
#define DEBUG_0				1
#define NO_OF_TAGS			16
#define HASH_TRIE_BUFFER	4

// -- ENGINE
#define THREAD_COUNT 2
#define QUEUE_SIZE 10000
#define PARSE_UNIT_LENGTH 30

#define SPLIT_BY_PHONEME 0

// -- TYPE DEFINITIONS
typedef enum {NONE, SAYI, DERECELISAYI, CUMLE, SEMBOL, INFO } WORD_TYPE;
typedef enum {ALPHA, NUMERIC, SYMBOLIC} CHAR_TYPE;
typedef enum {LEAF_NODE, INTERNAL_NODE} TRIE_NODE_TYPE;

typedef enum EMPH_LEVEL		{ EL_STRONG=1, EL_MODERATE, EL_NONE, EL_REDUCED };
typedef enum BREAK_LEVEL	{ BL_LARGE=1, BL_MEDIUM, BL_SMALL, BL_NONE };
typedef enum BREAK_TYPE		{ BT_QUEST=1, BT_DOT, BT_EXCL, BT_COMMA };
typedef enum PITCH_BASE		{ PB_HIGHEST=1, PB_HIGH, PB_MEDIUM, PB_LOW, PB_LOWEST, PB_DEFAULT };
typedef enum PITCH_MIDDLE	{ PM_HIGHEST=1, PM_HIGH, PM_MEDIUM, PM_LOW, PM_LOWEST, PM_DEFAULT };
typedef enum PITCH_RANGE	{ PR_HIGHEST=1, PR_HIGH, PR_MEDIUM, PR_LOW, PR_LOWEST, PR_DEFAULT };
typedef enum RATE_SPEED		{ RS_FASTEST=1, RS_FAST, RS_MEDIUM, RS_SLOW, RS_SLOWEST };
typedef enum VOLUME_LEVEL	{ VL_LOUDEST=1, VL_LOUD, VL_MEDIUM, VL_QUIET };
typedef enum ENGINE_ID		{ EI_SY=1, EI_PH };
typedef enum LANG_ID		{ LI_TR=1, LI_ENG };
typedef enum SPEAKER_GENDER	{ SG_MALE=1, SG_FEMALE };
typedef enum SPEAKER_AGE	{ SA_OLDER=1, SA_MIDDLE, SA_YOUNGER, SA_TEEN, SA_CHILD };
typedef enum SAYAS_MODE		{ SM_LITERAL=1, SM_DATE, SM_TIME, SM_PHONE, SM_NET, SM_POSTAL, SM_CURRENCY, SM_MATH, SM_FRACTION, SM_MEASURE, SM_NAME };
typedef enum SAYAS_MODETYPE	{ SMT_DMY=1, SMT_MDY, SMT_YMD, SMT_YM, SMT_MY, SMT_MD, SMT_HM, SMT_HMS, SMT_EMAIL, SMT_URL };
 
static string TAGS[NO_OF_TAGS] =
{
	"emph",		// 0
	"break",	// 1
	"pitch",	// 2
	"rate",		// 3
	"volume",	// 4
	"engine",	// 5
	"sable",	// 6
	"pron",		// 7
	"sayas",	// 8
	"lang",		// 9
	"speaker",	// 10

	"pronc",	// 11
	"audio",	// 12
	"marker",	// 13
	"div",		// 14
	"punct"		// 15
};
