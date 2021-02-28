

#ifndef _MYSIGNAL_
#define _MYSIGNAL_

//Singal Shift Direction 
#define SSD_LEFT		0
#define SSD_RIGHT		1

class CMySignal
{
	private:
		inline int round(double x) 
		{
			x += x > 0.0 ? 0.5 : -0.5 ; 
			return int(x);
		}
	public:
		int Length;
		int* Bits;

	CMySignal::CMySignal(int length = 0, bool clear = false)
	{
		if (length > 0)
		{
			Bits = new int[length];
			Length = length;
			if (clear)
			{
				for(int i = 0; i < length; i++)
					Bits[i] = 0;
			}
		}
		else
		{
			Length = 0;			 
			Bits = NULL;
		}

	};	

	CMySignal::~CMySignal()
	{
		if (Length > 0)
			delete[] Bits;
		Bits = NULL;
		Length = 0;
	};


	int CMySignal::SGNAssign(CMySignal& source)
	{
		Length =  source.Length;
		if(Bits)
			delete[] Bits;
		Bits = new int[Length];
		memcpy(Bits, source.Bits, Length * sizeof(int));
		return 1;
	}


	int CMySignal::SGNFill(int value)
	{
		for (int i = 0 ; i < Length ; i++)
		{
			Bits[i] = value;
		}
		return 1;
	};
	int CMySignal::SGNResize(int iNewLength,bool bZeroBits = false)
	{
		if (iNewLength > 0)
		{
			if (Length > 0)
				delete[] Bits;
			Bits = new int[iNewLength];
			Length = iNewLength;
			if (bZeroBits)
				SGNFill(0);
			return 1;
		}
		else
			return 0;
	};

	int CMySignal::DSPBinary(int iTheshold ,int iGreatVal = 1, int iSmallVal = 0)
	{
		if(iGreatVal == 1 && iSmallVal == 0) 
		{
			for(int i = 0; i < Length; i++)
				Bits[i] = Bits[i] >= iTheshold;
		}
		else
		{
			for(int i = 0; i < Length; i++)
			{
				if(Bits[i] >= iTheshold)
					Bits[i] = iGreatVal;
				else
				    Bits[i] = iSmallVal;
			}
		}
		return 1;
	};

	int CMySignal::DSPSmooth(int iND ,bool Circular = false)
	{
		CMySignal sng(Length);
		int ret = DSPSmooth(iND, sng,	Circular);
		SGNAssign(sng);
		return ret;
	};
	int CMySignal::DSPSmooth(int iND ,CMySignal& sgnResult ,bool Circular = false)
	{
		int iCoreLength = iND * 2 + 1;
		if (Circular)
			for (int i = 0 ; i < Length ; i++)
			{
				int j = 0;
				for (int ii = -iND ; ii <= iND ; ii++)
					j += Bits[(Length + ii + i) % Length];
				sgnResult.Bits[i] = round(double(j) / iCoreLength);
			}
		else
			for (int i = iND ; i < Length -iND ; i++)
			{
				int j = 0;
				for (int ii = -iND ; ii <= iND ; ii++)
					j += Bits[ii + i];
				sgnResult.Bits[i] = round(double(j) / iCoreLength);
			}

		return 1;
	}


	int CMySignal::DSPMedian(int iND ,bool Circular = false)
	{
		CMySignal sng(Length);
		int ret = DSPMedian(iND, sng, Circular);
		SGNAssign(sng);
		return ret;
	}


	int CMySignal::DSPMedian(int iND, CMySignal& sgnResult, bool Circular = false)
	{
		
		int iCoreLength = iND * 2 + 1;
		int* buf = new int[iCoreLength];

			for(int i = iND; i < Length - iND; i++)
			{
				int j = 0;
				for(int ii = -iND; ii <= iND; ii++)
					buf[ii + iND] = Bits[ii + i];
				for(int j = 0; j < iCoreLength; j++)
				{
					for(int k = j + 1; k < iCoreLength; k++)
						if(buf[j] > buf[k])
						{
							int c = buf[j];
							buf[j] = buf[k];			   
							buf[k] = c;
						}
				}
				sgnResult.Bits[i] = buf[iND];
			};
		for(int i = 0; i < iND; i++)
		{
			sgnResult.Bits[i] = sgnResult.Bits[iND];
			sgnResult.Bits[Length - i - 1] = sgnResult.Bits[Length - iND - 1];
		}

		delete[] buf;	
		return 1;
	}

	int CMySignal::DSPSubtraction(CMySignal& sgnSubtrahend, CMySignal* sgnResult = NULL, int offset = 0)
	{
		if(sgnSubtrahend.Length != Length)
			return 0;

		int * res = sgnResult ? sgnResult->Bits : Bits;

		for(int i = 0; i < Length; i++)
			res[i] = offset	+ Bits[i] - sgnSubtrahend.Bits[i];

		return 1;
	}

	int CMySignal::GetMaximumIndex(int iStartPoint = 0, int iEndPoint = -1)
	{
		int resIndex = iStartPoint;
		int levelEnd = iStartPoint;
		if (iEndPoint == -1)
			iEndPoint = Length;
		for (int i = iStartPoint ; i < iEndPoint ; i++)
			if (Bits[resIndex] < Bits[i])
			{
				resIndex = i;
				levelEnd = i;
			}
			else if(Bits[resIndex] == Bits[i])
			{
				levelEnd = i;
			}
		return (resIndex + levelEnd) / 2;
	};

	int CMySignal::GetMinimumIndex(int iStartPoint = 0, int iEndPoint = -1)
	{
		int resIndex = iStartPoint;
		int levelEnd = iStartPoint;
		if (iEndPoint == -1)
			iEndPoint = Length;
		for (int i = iStartPoint ; i < iEndPoint ; i++)
			if (Bits[resIndex] > Bits[i])
			{
				resIndex = i;
				levelEnd = i;
			}
			else if(Bits[resIndex] == Bits[i])
			{
				levelEnd = i;
			}
		return (resIndex + levelEnd) / 2;
	};

	int CMySignal::DSPShift(UINT dir,int n,int spaceVal = 0, bool circular = false)
	{
		int* buf;
		int iBufSize; 
		if (Length <= 0 || n == 0 || n >= Length)
			return 0;

		if (n < 0)
		{
			if (dir == SSD_LEFT)
				dir = SSD_RIGHT;
			else
				dir = SSD_LEFT;	
			n = abs(n);
		}

		iBufSize = (Length - n) * sizeof(int);

		if (dir == SSD_LEFT)
		{
			buf =new int[iBufSize / sizeof(int)];
			memcpy(buf, Bits, iBufSize);
			memcpy(Bits + n, buf, iBufSize);
			for (int i = 0 ; i < n ; i++)
				Bits[i] = spaceVal;
			delete[] buf;
		}
		else if (dir == SSD_RIGHT)
		{
			buf = new int[iBufSize / sizeof(int)]; 
			memcpy(buf, Bits + n, iBufSize);
			memcpy(Bits, buf, iBufSize);
			for (int i = Length - n ; i < Length ; i++)
				Bits[i] = spaceVal;
			delete[] buf;
		}
		else
			return 0;

		return 1;
	};


	int CMySignal::DRAWLine(int x1,int x2,int color) 
	{
		if (x1 > x2)
		{
			int x = x1;
			x1 = x2;
			x2 = x;
		}
		x1 = max(0 ,x1);
		x2 = min(x2 ,Length - 1);
		for(int x = x1 ; x <= x2; x++)
			Bits[x] = color;
		return 1;
	};



};





#endif