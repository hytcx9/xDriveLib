/*
 NOTE!!
 
 the minicfg class does not make ANY changes to the config files.
 It just read all data of the file and parse into list for later use.
 So there are no save function in it.
 
 */
#ifndef		MINICFG_H
#define		MINICFG_H

#include	<stdio.h>
#include    	"util_win.h"

#define         CONTENT_BUF_SIZE    81920
#define         LINE_BUF_SIZE       1024
#define         LINE_BREAK_CHAR     "\n"

enum PARSE_RESULT {
    READ_ERROR  = -1,
    FILE_TOO_LONG = 1,
    LINE_TOO_LONG = 2,
    INVALID_FORMAT = 3
    };

enum LINE_TYPE {
    UNKNOWN = -1,
    COMMENT = 0,
    VALID_STR = 1
};

// lines node list 
struct LinesList{
    LINE_TYPE type;
    char lineBuf[LINE_BUF_SIZE];
    int  len;
    char* key;
    char* value;
    
    struct LinesList* next;
    
    LinesList():type(UNKNOWN), next(NULL), len(0), key(NULL), value(NULL)
    {
        memset(lineBuf, 0, LINE_BUF_SIZE);
    }
};

/*
 
 Class minicfg declaration
 
 */
class minicfg
{
private:
    char* filefullname;     // including path and name
    FILE* f;
    char contentBuf[CONTENT_BUF_SIZE+1];
    LinesList* lines_list;
    LinesList* lines_head;
public:
	minicfg(const char* filepath);
	~minicfg();

    int     parseFile();
    int     parseLine(LinesList* line);
	char*   getValue(const char* key);
	
    char*   trim(char* inStr);
};

#endif
