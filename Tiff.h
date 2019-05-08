/*
$Log: Tiff.h,v $
Revision 1.8  2017/04/18 00:51:23  AV00287
*** empty log message ***

Revision 1.7  2007/07/12 08:42:19  hiran
no message

Revision 1.6  2005/11/11 03:08:16  hiran
test 6

*/

//test
// Tiff.h: interface for the CTiff class.
//
//////////////////////////////////////////////////////////////////////
#ifndef _TIFF_H_
#define _TIFF_H_

#define WINDOWS 
#ifdef WINDOWS
 #ifdef TIFFS_EXPORTS
 #define AV_EXPORT_API __declspec(dllexport)
 #include <fstream.h>
 #include <iostream>
 #include <string>
 #include <stdio.h>
 #include <vector>
 using namespace std;
 #else
 #define AV_EXPORT_API __declspec(dllimport)
 #endif //TIFFS_EXPORTS
#else//For Mac
 #include <stdio.h>
 #include <stdlib.h>
 #include <fstream.h>
 #include <string>
 #include <vector>
 #include	<MacTypes.h>
 typedef	Byte					BYTE;
 typedef	Byte* 					LPBYTE;
 typedef	unsigned short 			WORD;
 typedef	unsigned long 			DWORD;
 typedef	unsigned short*			LPWORD;
 typedef	unsigned long*			LPDWORD;
 typedef const char*				LPCTSTR;
#endif //WIDNOWS
#include <stdio.h>

namespace AV_Tiff{
typedef enum TiffTagSignatre{
	Null						= 0x0000L,
	NewSubfileType				= 0x00FEL,
	SubfileType					= 0x00FFL,
	ImageWidth					= 0x0100L,
	ImageLength					= 0x0101L,
	BitsPerSample				= 0x0102L,
	Compression					= 0x0103L,
	PhotometricInterpretation	= 0x0106L,
	Threshholding				= 0x0107L,
	CellWidth					= 0x0108L,
	CellLength					= 0x0109L,
	FillOrder					= 0x0110L,
	DocumentName				= 0x010DL,
	ImageDescription			= 0x010EL,
	Make						= 0x010FL,
	Model						= 0x0110L,
	StripOffsets				= 0x0111L,
	Orientation					= 0x0112L,
	SamplesPerPixel				= 0x0115L,
	RowsPerStrip				= 0x0116L,
	StripByteCounts				= 0x0117L,
	MinSampleValue				= 0x0118L,
	MaxSampleValue				= 0x0119L,
	XResolution					= 0x011AL,
	YResolution					= 0x011BL,
	PlanarConfiguration			= 0x011CL,
	PageName					= 0x011DL,
	XPosition					= 0x011EL,
	YPosition					= 0x011FL,
	FreeOffsets					= 0x0120L,
	FreeByteCounts				= 0x0121L,
	GrayResponseUnit			= 0x0122L,
	GrayResponsCurve			= 0x0123L,
	T4Options					= 0x0124L,
	T6Options					= 0x0125L,
	ResolutionUnit				= 0x0128L,
	PageNumber					= 0x0129L,
	TransferFunction			= 0x012DL,
	Software					= 0x0131L,
	DateTime					= 0x0132L,
	Artist						= 0x013BL,
	HostComputer				= 0x013CL,
	Predicator					= 0x013DL,
	WhitePoint					= 0x013EL,
	PrimaryChromaticities		= 0x013FL,
	ColorMap					= 0x0140L,
	HalftoneHints				= 0x0141L,
	TileWidth					= 0x0142L,
	TileLength					= 0x0143L,
	TileOffsets					= 0x0144L,
	TileByteCounts				= 0x0145L,
	InkSet						= 0x014CL,
	InkNames					= 0x014DL,
	NumberOfInks				= 0x014EL,
	DotRange					= 0x0150L,
	TargetPrinter				= 0x0151L,
	ExtraSamples				= 0x0152L,
	SampleFormat				= 0x0153L,
	SMinSampleValue				= 0x0154L,
	SMaxSampleValue				= 0x0155L,
	TransforRange				= 0x0156L,
	JPEGProc					= 0x0157L,
	JPEGInterchangeFormat		= 0x0201L,
	JPEGIngerchangeFormatLength = 0x0202L,
	JPEGRestartInterval			= 0x0203L,
	JPEGLosslessPredicators		= 0x0205L,
	JPEGPointTransforms			= 0x0206L,
	JPEGQTable					= 0x0207L,
	JPEGDCTable					= 0x0208L,
	JPEGACTable					= 0x0209L,
	YCbCrCoefficients			= 0x0211L,
	YCbCrSampling				= 0x0212L,
	YCbCrPositioning			= 0x0213L,
	ReferenceBlackWhite			= 0x0214L,
	CopyRight					= 0x8298L
	} TiffTagSignature;

typedef enum {
	Unknown						= 0x0000L,
	Byte						= 0x0001L,
	ASCII						= 0x0002L,
	Short						= 0x0003L,
	Long						= 0x0004L,
	Rational					= 0x0005L,
	SBYTE						= 0x0006L,
	Undefine					= 0x0007L,
	SShort						= 0x0008L,
	SLong						= 0x0009L,
	Float						= 0x000AL,
	Double						= 0x000BL
	}FieldType;

typedef struct TIFFTAG
{
	TiffTagSignature	tag;          
	FieldType			type;         
	DWORD				n;          
	DWORD				value;       
}TIFFTAG;

class TIFFTAG2
{
public:
	WORD				tag;          
	WORD				type;         
	DWORD				n;          
	DWORD				value;       

public:
	TIFFTAG2();
	~TIFFTAG2();
	const	TIFFTAG2&	operator=(const TIFFTAG& temp);
	int		TransTag(TIFFTAG& NewTag);
};

typedef struct  HEADER
{
	WORD	order;       // "II", 0x4949
	WORD	version;     // 0x2A
	DWORD	offset;      // 8, offset value of first Image File Directory,ie entry count position
}HEADER;

#define MAXTAG 30

typedef struct IFD_STRUCTURE
{
	WORD		EntryCounts;
	TIFFTAG		TagList[MAXTAG];
	DWORD		NextIFD;
}IFD_STRUCTURE;

typedef struct  stTIFFHEADER
{
	HEADER			Version;
	IFD_STRUCTURE	IFD;
}TIFFHEADER, *LPTIFFHEADER;

#ifdef WINDOWS
class  CTiff  
#else
class CTiff
#endif //WINDOWS
{
public:
	CTiff();
	virtual ~CTiff();

	int		Initialheader();
	int		GetTag(TiffTagSignatre Signature, TIFFTAG &TiffTag);
	int		SetTag(TIFFTAG data);
	int		CreateNew(int Width, int Length, int resX, int resY, int sampleperpixel, int bitspersample, int ColorOrder);
	int		WriteHeader();
	int		SaveFile(LPCTSTR FileName);
	int		ReadFile(LPCTSTR FileName);
//	int		DumpFile();
	int		PutRow(LPBYTE lpBuf, int Line, int Bytes);
	int		PutRow(LPWORD lpBuf, int Line, int WORDs);
	int		GetRow(LPBYTE lpBuf, int Line, int Bytes);
	int		GetRow(LPWORD lpBuf, int Line, int Words);
	int		GetRowColumn(unsigned char *lpBuf, int x, int y, int RecX, int RecY);
	void	GetChannelBuf(LPBYTE lpBuf_C, LPBYTE lpBuf_M, LPBYTE lpBuf_Y, LPBYTE lpBuf_K, int index);

	int		GetImage();
// Attributes
public:

	LPCTSTR			m_FileName;
	FILE			*m_File;

	LPTIFFHEADER	m_lpTiffHeader;
	LPBYTE			m_lpImageBuf;

};

class RationalNumber
{
public:
	DWORD Fraction;
	DWORD Denominator;
public:
	int SetRationalNumber(WORD Fraction, WORD Denominator);
	RationalNumber();
	~RationalNumber();
};
}
#endif //_TIFF_H_

