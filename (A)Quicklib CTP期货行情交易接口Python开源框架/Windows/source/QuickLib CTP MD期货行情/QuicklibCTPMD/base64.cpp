#include "stdafx.h"
#include "base64.h"

CString szOutput;

int CStringEncodeBase64(const CString &szInput, CString &szOutput)
{
	int nRet = 0;
	int nLeng = 0;
	nLeng= (szInput.GetLength() + 1)*sizeof(TCHAR);
    char *pInput = new char[nLeng];	
    ZeroMemory(pInput, nLeng);	
    char *pOutput = new char[nLeng];
    ZeroMemory(pOutput, nLeng);
    //USES_CONVERSION;
    strcpy_s(pInput, nLeng, CT2A(szInput));
    nRet = base64_encode((void *)pInput, strlen(pInput), &pOutput);
    szOutput = CA2T(pOutput);
    delete []pInput;
    delete []pOutput;
    return nRet;
}


int CStringDecodeBase64(const CString &szInput, CString &szOutput)
{
	int nRet = 0;
	int nLeng = 0;
	nLeng= (szInput.GetLength() + 1)*sizeof(TCHAR);
    char *pInput = new char[nLeng];
    ZeroMemory(pInput, nLeng);
    char *pOutput = new char[nLeng];
    ZeroMemory(pOutput, nLeng);
   //USES_CONVERSION;
    strcpy_s(pInput, nLeng, CT2A(szInput));
    nRet = base64_decode(pInput, pOutput);
    szOutput = CA2T(pOutput);
    delete [] pInput;
    delete [] pOutput;
    return nRet;
}


int base64_encode(const void *data, const int size, char **str)
{
	char *s,*p;
	int i;
	int c;
	const unsigned char *q;
    p=s=(char *)malloc(size * 4/3+4);
	if(p==NULL){return -1;}
	q=(const unsigned char *)data;
	i=0;
	for(i=0;i<size;)
	{
	 c=q[i++];
	 c *= 256;
	 if (i < size){c += q[i];}
	 i++;
	 c *= 256;
	 if (i < size){c += q[i];}
	 i++;
	 p[0] = base64_chars[(c & 0x00fc0000) >> 18];
	 p[1] = base64_chars[(c & 0x0003f000) >> 12];
	 p[2] = base64_chars[(c & 0x00000fc0) >> 6];
	 p[3] = base64_chars[(c & 0x0000003f) >> 0];
	 if (i > size){p[3] = '=';}
	 if (i > size + 1){p[2] = '=';}
	 p += 4;
	 }

	*p = 0;
	*str = s;
	return strlen(s);
}



static int pos(const char c)
{
	char *p;
	for (p = base64_chars; *p; p++)
	{
		if (*p == c){return (p-base64_chars); }
	}
	return -1;
}


static unsigned int token_decode(const char *token)
{
int i;
unsigned int val = 0;
int marker = 0;
if(strlen(token)<4){return DECODE_ERROR;}
for(i=0;i<4;i++)
{
	val *= 64;
	if (token[i] == '='){marker++;}
	else
	{
		if (marker > 0){return DECODE_ERROR;}
		else{val += pos(token[i]);}
	  }
}

if (marker > 2){return DECODE_ERROR;}
return (marker << 24) | val;
}


int base64_decode(const char *str, char *data)
{
	const char *p;
	unsigned char *q;
	q = (unsigned char *)data;
	for (p = str; *p && (*p == '=' || strchr(base64_chars, *p)); p += 4)
	{
		unsigned int val = token_decode(p);
		 unsigned int marker = (val >> 24) & 0xff;
		 if (val == DECODE_ERROR){return -1; }
		 *q++ = (val >> 16) & 0xff;
		 if (marker < 2){*q++ = (val >> 8) & 0xff;}
		 if (marker < 1){*q++ = val & 0xff;}
	 }

	int dd=q-(unsigned char *)data;
return dd;
}