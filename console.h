#define SBUFSIZE 16
typedef struct 
{
	volatile unsigned head;
	volatile unsigned tail;
	volatile unsigned count;
	char buffer[SBUFSIZE];
} SerialBuffer;

void putBuf(SerialBuffer *sbuf, char c);
char getBuf(SerialBuffer *sbuf);
int rx_count();
char egetc(void);
void eputc(const char c);
void eputs(const char *s);
void printString(const char *s);
void printHex(unsigned int Number);
void flushinput();
