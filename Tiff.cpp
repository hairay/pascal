// Tiff.cpp: implementation of the CTiff class.
//
//////////////////////////////////////////////////////////////////////
#include "windows.h"
#include "Tiff.h"

using namespace AV_Tiff;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

RationalNumber::RationalNumber()
{
	Fraction = 0;
	Denominator = 0;
}

RationalNumber::~RationalNumber()
{
}

int RationalNumber::SetRationalNumber(WORD fraction, WORD denominator)
{
	Fraction = fraction * 100;
	Denominator = denominator * 100;
	return 1;
}

TIFFTAG2::TIFFTAG2()
{
	tag = (WORD)0;
	type = (WORD)0;
	n = 0;
	value = 0;
}

TIFFTAG2::~TIFFTAG2()
{
}

const	TIFFTAG2& TIFFTAG2::operator = (const TIFFTAG& temp)
{
	this->tag = (WORD)temp.tag;
	this->type = (WORD)temp.type;
	this->n = temp.n;
#ifndef  _WINDOWS	
	if(n==1)
	{
		switch(temp.type)
		{
		case Byte:value = temp.value<<24;break;
		case Short:value = temp.value<<16;break;
		default:value = temp.value;break;
		}
	}
	else
		value = temp.value;
#else
	this->value = temp.value;
#endif //_WINDOWS
	
	return *this;
}

int	TIFFTAG2::TransTag(TIFFTAG& NewTag)
{
	NewTag.tag = (TiffTagSignature)tag;
	NewTag.type = (FieldType)type;
	NewTag.n = n;
#ifndef _WINDOWS	
	if(n==1)
	{
		switch(NewTag.type)
		{
		case Byte:NewTag.value = value>>24;break;
		case Short:NewTag.value = value>>16;break;
		default:NewTag.value = value;break;
		}
	}
	else
		NewTag.value = value;
#else
	NewTag.value = value;
#endif //_WINDOWS
	
	return 1;
}

CTiff::CTiff()
{
	m_lpTiffHeader = new(TIFFHEADER);
	if(m_lpTiffHeader != NULL)
		Initialheader();
	m_lpImageBuf = NULL;
}

CTiff::~CTiff()
{
	if(m_lpTiffHeader != NULL)
		delete []m_lpTiffHeader;
	if(m_lpImageBuf != NULL)
		delete []m_lpImageBuf;
}


int CTiff::Initialheader()
{
#ifdef _WINDOWS
	m_lpTiffHeader->Version.order = 0x4949;
#else
	m_lpTiffHeader->Version.order = 0x4D4D;
#endif //_WINDOWS

	m_lpTiffHeader->Version.version = 0x2A;
	m_lpTiffHeader->Version.offset = 8;

	m_lpTiffHeader->IFD.TagList[0].tag = NewSubfileType;
	m_lpTiffHeader->IFD.TagList[0].type = Byte;
	m_lpTiffHeader->IFD.TagList[0].n = 4;
	m_lpTiffHeader->IFD.TagList[0].value = 0;

	m_lpTiffHeader->IFD.TagList[1].tag = ImageWidth;
	m_lpTiffHeader->IFD.TagList[1].type = Long; //or Short
	m_lpTiffHeader->IFD.TagList[1].n = 1;
	m_lpTiffHeader->IFD.TagList[1].value = 0;

	m_lpTiffHeader->IFD.TagList[2].tag = ImageLength;
	m_lpTiffHeader->IFD.TagList[2].type = Long; //or Short
	m_lpTiffHeader->IFD.TagList[2].n = 1;
	m_lpTiffHeader->IFD.TagList[2].value = 0;

	m_lpTiffHeader->IFD.TagList[3].tag = BitsPerSample;
	m_lpTiffHeader->IFD.TagList[3].type = Short;
	m_lpTiffHeader->IFD.TagList[3].n = 1;
	m_lpTiffHeader->IFD.TagList[3].value = 0;
	//entryCount * sizeof(TIFFTAG);//8 8 8
	// You need to caculate the offset later.

	m_lpTiffHeader->IFD.TagList[4].tag = Compression;
	m_lpTiffHeader->IFD.TagList[4].type = Short; 
	m_lpTiffHeader->IFD.TagList[4].n = 1;
	m_lpTiffHeader->IFD.TagList[4].value = 1;

	m_lpTiffHeader->IFD.TagList[5].tag = PhotometricInterpretation;
	m_lpTiffHeader->IFD.TagList[5].type = Short; 
	m_lpTiffHeader->IFD.TagList[5].n = 1;
	m_lpTiffHeader->IFD.TagList[5].value = 2;
	//0:White is zero, 1:Black is Zero, 2:RGB color, 3:Palette color, 6:YCbCr

	m_lpTiffHeader->IFD.TagList[6].tag = StripOffsets;
	m_lpTiffHeader->IFD.TagList[6].type = Long; 
	m_lpTiffHeader->IFD.TagList[6].n = 1;
	m_lpTiffHeader->IFD.TagList[6].value = 0;
	//entryCount * sizeof(TIFFTAG) + 3 * sizeof(XYRESOLUTION);

	m_lpTiffHeader->IFD.TagList[7].tag = SamplesPerPixel;
	m_lpTiffHeader->IFD.TagList[7].type = Short; 
	m_lpTiffHeader->IFD.TagList[7].n = 1;
	m_lpTiffHeader->IFD.TagList[7].value = 3;

	m_lpTiffHeader->IFD.TagList[8].tag = RowsPerStrip;
	m_lpTiffHeader->IFD.TagList[8].type = Long; 
	m_lpTiffHeader->IFD.TagList[8].n = 1;
	m_lpTiffHeader->IFD.TagList[8].value = 0;

	m_lpTiffHeader->IFD.TagList[9].tag = StripByteCounts;
	m_lpTiffHeader->IFD.TagList[9].type = Long; 
	m_lpTiffHeader->IFD.TagList[9].n = 1;
	m_lpTiffHeader->IFD.TagList[9].value = 0;

	m_lpTiffHeader->IFD.TagList[10].tag = XResolution;
	m_lpTiffHeader->IFD.TagList[10].type = Rational; 
	m_lpTiffHeader->IFD.TagList[10].n = 1;
	m_lpTiffHeader->IFD.TagList[10].value = 0;

	m_lpTiffHeader->IFD.TagList[11].tag = YResolution;
	m_lpTiffHeader->IFD.TagList[11].type = Rational; 
	m_lpTiffHeader->IFD.TagList[11].n = 1;
	m_lpTiffHeader->IFD.TagList[11].value = 0;

	m_lpTiffHeader->IFD.TagList[12].tag = ResolutionUnit;
	m_lpTiffHeader->IFD.TagList[12].type = Short; 
	m_lpTiffHeader->IFD.TagList[12].n = 1;
	m_lpTiffHeader->IFD.TagList[12].value = 2;
	//2:inch, 3:centimeter

	m_lpTiffHeader->IFD.TagList[13].tag = PlanarConfiguration;
	m_lpTiffHeader->IFD.TagList[13].type = Short; 
	m_lpTiffHeader->IFD.TagList[13].n = 1;
	m_lpTiffHeader->IFD.TagList[13].value = 1;
	//1: Data is stored as RGBRGB..., 2:RRRR....,GGGG...., BBBB....

	m_lpTiffHeader->IFD.EntryCounts = 14;
	m_lpTiffHeader->IFD.NextIFD = 0; // for only one IFD
//	m_TiffHeaderParam.nextIFD = 8; // It point to the first IFD_Position. For multipage

	return 1;
}


int CTiff::GetTag(TiffTagSignature Signature,  TIFFTAG &TiffTag)
{
	int TagCount, TotalTag;
	
	TotalTag = m_lpTiffHeader->IFD.EntryCounts;
	TiffTag.tag = Null;
	TiffTag.type = Unknown;
	TiffTag.n = 0;
	TiffTag.value = 0;
	TagCount = 0;

	while(TagCount < TotalTag)
	{
		if(m_lpTiffHeader->IFD.TagList[TagCount].tag != Null)
		{
			if(m_lpTiffHeader->IFD.TagList[TagCount].tag == Signature)
			{
				TiffTag = m_lpTiffHeader->IFD.TagList[TagCount];
//				return TempTag;
				break;
			}
			else
				TagCount ++;
		}
	}
	return 1; //Fail, The data will be all 0.
}

int CTiff::SetTag(TIFFTAG NewTag)
{	
	int NullTagIndex, TagCount, TotalTag;
	
	TotalTag = m_lpTiffHeader->IFD.EntryCounts;
	TagCount = 0;
	NullTagIndex = 0;
	while(TagCount < TotalTag)
	{
		if(m_lpTiffHeader->IFD.TagList[TagCount].tag != Null)
		{
			if(m_lpTiffHeader->IFD.TagList[TagCount].tag == NewTag.tag)
			{
				m_lpTiffHeader->IFD.TagList[TagCount] = NewTag;
				return 1;
				break;
			}
			TagCount++;
		}
		else
		{
			NullTagIndex = TagCount;
		}
	}

	if(TagCount < MAXTAG) //The maximal size of TagList
	{
		if(NullTagIndex == 0)
			NullTagIndex = TagCount;
		if(NewTag.type != Undefine)
		{
			m_lpTiffHeader->IFD.TagList[NullTagIndex] = NewTag;
			m_lpTiffHeader->IFD.EntryCounts++;
		}
		return 1;
	}
	else
		return -1;//The TagList is not enough
}

int CTiff::CreateNew(int Width, int Length, int resX, int resY, int sampleperpixel, int bitspersample, int ColorOrder)
{
	TIFFTAG Temp;

	if(m_lpTiffHeader == NULL)
		return 0;

	Temp.tag = ImageWidth;
	Temp.type = Long;
	Temp.n = 1;
	Temp.value = Width;
	SetTag(Temp);

	Temp.tag = ImageLength;
	Temp.type = Long;
	Temp.n = 1;
	Temp.value = Length;
	SetTag(Temp);

	Temp.tag = RowsPerStrip;
	Temp.type = Long;
	Temp.n = 1;
	Temp.value = Length;
	SetTag(Temp);

	Temp.tag = XResolution;
	Temp.type = Rational;
	Temp.n = 1;
	Temp.value = resX;
	SetTag(Temp);
//	xResolution.SetRationalNumber(Resolution, 1);

	Temp.tag = YResolution;
	Temp.type = Rational;
	Temp.n = 1;
	Temp.value = resY;
	SetTag(Temp);
//	yResolution.SetRationalNumber(Resolution, 1);

	Temp.tag = 	SamplesPerPixel;
	Temp.type = Short;
	Temp.n = 1;
	Temp.value = sampleperpixel;
	SetTag(Temp);

	if(sampleperpixel == 4)
	{
		Temp.tag = PhotometricInterpretation;
		Temp.type = Short;
		Temp.n = 1;
		Temp.value = 5;
		SetTag(Temp);
	}

	Temp.tag =BitsPerSample;
	Temp.type = Short;
	Temp.n = sampleperpixel;
	Temp.value = bitspersample;
	SetTag(Temp);

	Temp.tag = StripByteCounts;
	Temp.type = Long;
	Temp.n = 1;
	switch(bitspersample)
	{
	case  1: // cschiu : 2003/1/14, fix bugs.
		Temp.value =  ((Width + 7) / 8 )  * Length * sampleperpixel ;
		break;
	case  8:
		Temp.value = Width * Length * sampleperpixel;
		break;
	case  16:
		Temp.value = Width * Length * sampleperpixel * 2;
		break;
	default:
		Temp.value = Width * Length * sampleperpixel;
		break;
	}

	SetTag(Temp);

	if(this->m_lpImageBuf != NULL)
		delete []m_lpImageBuf;
	m_lpImageBuf = new BYTE[Temp.value];

	if(m_lpImageBuf != NULL)
		return 1;
	else
		return 0;	
}

int CTiff::SaveFile(LPCTSTR FileName)
{
	size_t len = 0;
	
	TIFFTAG	TagStripByteCounts;
	m_FileName = FileName;
	fopen_s(&m_File, (const char*)FileName, "wb+" );
	if(m_File == NULL)
		goto RETURN;
	fseek(m_File,0,SEEK_SET);
	WriteHeader();
	GetTag(StripByteCounts, TagStripByteCounts);
	len = fwrite(m_lpImageBuf, 1, TagStripByteCounts.value, m_File);
	fclose(m_File);

RETURN:
	if(len == TagStripByteCounts.value)
		return 1;
	else
		return 0;
}


int CTiff::WriteHeader()
{
	int		i, TagCount, OffsetValue, ChannelNumber, BitsPerSampleValue;
	RationalNumber xResolution, yResolution;
	TIFFTAG	TagSamplePerPixel, TagBitsPerSample, TagPhotometricInterpretation, Temp;
	TIFFTAG2	TagData;
	WORD	Channel[10];
	
	TagCount = 0;
	OffsetValue = 10 + m_lpTiffHeader->IFD.EntryCounts * 12 + 4;
//	OffsetValue = header + sizeof(taglist) + nextIFD;

	GetTag(SamplesPerPixel, TagSamplePerPixel);
	GetTag(BitsPerSample, TagBitsPerSample);
	ChannelNumber = (int)TagSamplePerPixel.value;
	if(ChannelNumber > 1)
	{
		for(i = 0; i < ChannelNumber; i++)
			Channel[i] = (WORD)TagBitsPerSample.value;
		BitsPerSampleValue = TagBitsPerSample.value;
		TagBitsPerSample.value = OffsetValue;
		OffsetValue += 2 * (int)ChannelNumber;
		SetTag(TagBitsPerSample);
	}
	else
	{
		GetTag(PhotometricInterpretation, TagPhotometricInterpretation);
		TagPhotometricInterpretation.value = 1;
		SetTag(TagPhotometricInterpretation);
		BitsPerSampleValue = TagBitsPerSample.value;
	}

	GetTag(XResolution, Temp);
	xResolution.SetRationalNumber((WORD)Temp.value, (WORD)1);
	Temp.value = OffsetValue;
	OffsetValue += 8 * 1;
	SetTag(Temp);

	GetTag(YResolution, Temp);
	yResolution.SetRationalNumber((WORD)Temp.value, (WORD)1);
	Temp.value = OffsetValue;
	OffsetValue += 8 * 1;
	SetTag(Temp);

	GetTag(StripOffsets, Temp);
	Temp.value = OffsetValue;
	SetTag(Temp);

	fwrite(&m_lpTiffHeader->Version, 1, 8, m_File);
	fwrite(&m_lpTiffHeader->IFD.EntryCounts, 1, 2, m_File);

	while(TagCount < MAXTAG)
	{
		if (m_lpTiffHeader->IFD.TagList[TagCount].tag != NULL)
		{
			TagData = m_lpTiffHeader->IFD.TagList[TagCount];
			fwrite(&TagData, 1, sizeof(TIFFTAG2), m_File);
			TagCount++;
		}
		if (TagCount >= (int)m_lpTiffHeader->IFD.EntryCounts)
			break;
	}
	fwrite(&m_lpTiffHeader->IFD.NextIFD, 1, 4, m_File);

	if(ChannelNumber > 1)
		fwrite(&Channel, 1, 2 * ChannelNumber, m_File);

	fwrite(&xResolution, 1, 8, m_File);
	fwrite(&yResolution, 1, 8, m_File);

	//Set BitsPerSample to real value;
	if(ChannelNumber > 1)
	{
		TagBitsPerSample.value = BitsPerSampleValue;
		SetTag(TagBitsPerSample);
	}
	return 1;
}


int CTiff::PutRow(LPBYTE lpBuf, int Line, int Bytes)
{
	int Width, Sample, BytesPerLine;
	TIFFTAG	TagImageWidth, TagBitsPerSample, TagSamplesPerPixel;
	LPBYTE lpPosition;
	lpPosition = (LPBYTE)m_lpImageBuf;

	GetTag(BitsPerSample, TagBitsPerSample);
	GetTag(ImageWidth, TagImageWidth);
	GetTag(SamplesPerPixel, TagSamplesPerPixel);
	Width = TagImageWidth.value;
	Sample = TagSamplesPerPixel.value;

	switch(TagBitsPerSample.value)
	{
	case 1 :BytesPerLine = Width * Sample / 8;break;
	case 8 :BytesPerLine = Width * Sample ;break;
	default:BytesPerLine = Width * Sample ;break;
	}
	
	lpPosition += BytesPerLine * Line;
	memcpy(lpPosition, lpBuf, Bytes);
	return Bytes;
}

int CTiff::PutRow(LPWORD lpBuf, int Line, int WORDs)
{
	int Width, Sample, BytesPerLine;
	TIFFTAG	TagImageWidth, TagBitsPerSample, TagSamplesPerPixel;
	LPWORD  lpPosition;
	lpPosition = (LPWORD)m_lpImageBuf;

	GetTag(BitsPerSample, TagBitsPerSample);
	GetTag(ImageWidth, TagImageWidth);
	GetTag(SamplesPerPixel, TagSamplesPerPixel);
	Width = TagImageWidth.value;
	Sample = TagSamplesPerPixel.value;

	switch(TagBitsPerSample.value)
	{
	case 1 :BytesPerLine = Width / 8;break;
	case 8 :BytesPerLine = Width * Sample;break;
	default:BytesPerLine = Width * Sample;break;
	}

	lpPosition += BytesPerLine * Line;
	memcpy(lpPosition, lpBuf, WORDs * 2);
	return 1;
}

int CTiff::ReadFile(LPCTSTR FileName)
{
	TIFFTAG2		TempTag2;
	TIFFTAG			TempTag;
	DWORD			TiffVersion, TiffOffset;
	WORD			TagCount, BitsPerSampleValue;
	int				i; 
	RationalNumber  Resolution;

	m_FileName = FileName;
	//m_File = fopen(FileName, "rb");
	fopen_s(&m_File, (const char*)FileName, "rb");
	fseek(m_File, 0, SEEK_SET);
	fread(&TiffVersion, 1, 4,m_File);
#ifdef _WINDOWS	
	if(TiffVersion != 0x2A4949)
	{
		return -1;	
	}
#else	
	if(TiffVersion != 0x4D4D002A)
	{//Mac
		return -1;
	}
#endif //_WINDOWS

	fread(&TiffOffset, 1, 4, m_File);
	fseek(m_File, TiffOffset, SEEK_SET);
	fread(&TagCount, 1, 2, m_File);
	for(i = 0; i < TagCount; i++)
	{
		fread(&TempTag2, 1, 12, m_File);
		TempTag2.TransTag(TempTag);
		SetTag(TempTag);
	}
	//Read BitsPerSample data
	fread(&m_lpTiffHeader->IFD.NextIFD, 1, 4, m_File);
	GetTag(BitsPerSample, TempTag);
	//Book2:PC image process tech. page 142
	if(TempTag.type == Short && TempTag.n >2)	
	{
		fseek(m_File, TempTag.value, SEEK_SET);
		fread(&BitsPerSampleValue, 1, 2, m_File);
		TempTag.value = BitsPerSampleValue;
	}
	SetTag(TempTag);

	//Read Resolution Data
	GetTag(XResolution, TempTag);
	fseek(m_File, TempTag.value, SEEK_SET);
	fread(&Resolution, 1, sizeof(RationalNumber), m_File);
	TempTag.value = Resolution.Fraction / Resolution.Denominator;
	SetTag(TempTag);

	GetTag(YResolution, TempTag);
	fseek(m_File, TempTag.value, SEEK_SET);
	fread(&Resolution, 1, sizeof(RationalNumber), m_File);
	TempTag.value = Resolution.Fraction / Resolution.Denominator;
	SetTag(TempTag);

	//Read Data
	GetTag(StripOffsets, TempTag);
	if(TempTag.n == 1)
	{
		fseek(m_File, TempTag.value, SEEK_SET);
		GetTag(StripByteCounts, TempTag);
		if(m_lpImageBuf != NULL)
			delete []m_lpImageBuf;
		m_lpImageBuf = new BYTE[TempTag.value];
		fread(m_lpImageBuf, 1, TempTag.value, m_File);
		int ret = ferror(m_File); 
	}
	else
	{
		TIFFTAG	TagStripByteCounts;
		GetTag(StripByteCounts, TagStripByteCounts);
		if(TagStripByteCounts.n != TempTag.n) return -1;

		int Strip = TempTag.n;
		int stripByteCounts = 0; 

		LPDWORD lpStripOffsets = new DWORD[Strip];
		LPDWORD lpStripByteCounts = new DWORD[Strip];

		fseek(m_File, TempTag.value, SEEK_SET);//StripOffsets
		fread(lpStripOffsets, 4, TempTag.n, m_File);
		fseek(m_File, TagStripByteCounts.value, SEEK_SET);
		fread(lpStripByteCounts, 4, TagStripByteCounts.n, m_File);

		for(i = 0; i < Strip; i++)
			stripByteCounts += lpStripByteCounts[i];

		if(m_lpImageBuf != NULL)
			delete []m_lpImageBuf;
		m_lpImageBuf = new BYTE[stripByteCounts];

		LPBYTE lpTemp = m_lpImageBuf;
		for(i = 0; i < Strip; i++)
		{
			fseek(m_File, lpStripOffsets[i], SEEK_SET);
			fread(lpTemp, 1, lpStripByteCounts[i], m_File);
			lpTemp += lpStripByteCounts[i];
		}

		//Reset Tag Data "StripOffset, StripByteCounts, RowPerStrip"
		TIFFTAG	TagImageLength, TagRowsPerStrip;

		TagStripByteCounts.n = 1;
		TagStripByteCounts.value = stripByteCounts;
		SetTag(TagStripByteCounts);

		GetTag(ImageLength, TagImageLength);
		GetTag(RowsPerStrip, TagRowsPerStrip);
		TagRowsPerStrip.value = TagImageLength.value;
		SetTag(TagRowsPerStrip);
		TempTag.n = 1;//StripOffset
		//Don't worry about the StripOffset.value, It will be recount in writefile.
		SetTag(TempTag);


		delete []lpStripOffsets;
		delete []lpStripByteCounts;
	}
	fclose(m_File);
	return 1;
}


int CTiff::GetImage()
{
	TIFFTAG			TempTag;

	//m_File = fopen(m_FileName, "r+");
	fopen_s(&m_File, (const char*)m_FileName, "r+");
//	m_File.Open(m_FileName, CFile::modeRead, &e );
	fseek(m_File, 0, SEEK_SET);
//	m_File.SeekToBegin();

	GetTag(StripOffsets, TempTag);
	fseek(m_File, TempTag.value, SEEK_SET);

	//Just for one Strip.
	GetTag(StripByteCounts, TempTag);

	fread(m_lpImageBuf, 1, TempTag.value, m_File);
	fclose(m_File);

	return TempTag.value;	
}

int CTiff::GetRow(LPBYTE lpBuf, int Line, int Bytes)
{
	TIFFTAG TagImageWidth, TagBitsPerSample, TagSamplesPerPixel;
	LPBYTE	lpIndex;
	int		BytesPerLine;


	GetTag(ImageWidth, TagImageWidth);
	GetTag(BitsPerSample, TagBitsPerSample);
	GetTag(SamplesPerPixel, TagSamplesPerPixel);
	BytesPerLine = (TagImageWidth.value * TagBitsPerSample.value * TagSamplesPerPixel.value) / 8;

	lpIndex = (LPBYTE)m_lpImageBuf + BytesPerLine * Line;
	memcpy(lpBuf, lpIndex, Bytes);

	return Bytes;
}


int CTiff::GetRow(LPWORD lpBuf, int Line, int Words)
{
	TIFFTAG TagImageWidth, TagBitsPerSample, TagSamplesPerPixel;
	LPBYTE	lpIndex;
	int		BytesPerLine;


	GetTag(ImageWidth, TagImageWidth);
	GetTag(BitsPerSample, TagBitsPerSample);
	GetTag(SamplesPerPixel, TagSamplesPerPixel);
	BytesPerLine = (TagImageWidth.value * TagBitsPerSample.value * TagSamplesPerPixel.value) / 8;

	lpIndex = (LPBYTE)m_lpImageBuf + BytesPerLine * Line;
	memcpy((LPWORD)lpBuf, lpIndex, Words * 2);

	return Words;
}

int CTiff::GetRowColumn(unsigned char *lpBuf, int x, int y, int RecX, int RecY)
{
	int i, BytesPerLine;
	TIFFTAG	TagImageWidth, TagBitsPerSample, TagSamplesPerPixel;
	LPBYTE  lpPosition;
	lpPosition = (LPBYTE)m_lpImageBuf;

	GetTag(ImageWidth, TagImageWidth);
	GetTag(BitsPerSample, TagBitsPerSample);
    GetTag(SamplesPerPixel, TagSamplesPerPixel);
	BytesPerLine = (TagImageWidth.value * TagBitsPerSample.value * TagSamplesPerPixel.value) / 8;

	LPBYTE lpWidthBuf = new BYTE[BytesPerLine];
	LPBYTE lpCurrent = lpBuf;
	int StartX = x; int EndX = x + RecX;
	int StartY = y; int EndY = y + RecY;

	for(i = StartY; i < EndY; i++)
	{
		GetRow(lpWidthBuf, i, BytesPerLine);
		memcpy(lpCurrent, lpWidthBuf + StartX * 3, RecX * 3);
		lpCurrent += RecX * 3;
	}

	delete []lpWidthBuf;	
	return 1;
}

//For CMYK Tiff File
void CTiff::GetChannelBuf(LPBYTE lpBuf_C, LPBYTE lpBuf_M, LPBYTE lpBuf_Y, LPBYTE lpBuf_K, int index)
{
	int i, Width;
	TIFFTAG TagImageWidth;
	GetTag(ImageWidth, TagImageWidth);
	Width = TagImageWidth.value;

	LPBYTE lpBuf = new BYTE[Width * 4];

	GetRow(lpBuf, index, Width * 4);
	LPBYTE lpTemp = lpBuf;
	for(i = 0; i < Width; i++)
	{//?????
		*lpBuf_C++ = *(lpTemp++);
		*lpBuf_M++ = *(lpTemp++);
		*lpBuf_Y++ = *(lpTemp++);
		*lpBuf_K++ = *(lpTemp++);
	}
	
	delete[] lpBuf;
}
