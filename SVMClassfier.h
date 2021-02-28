#include "svm.h"

using namespace cv;

typedef struct _SVM_MODEL
{
	svm_model* model;
	string path;
	int number_of_classes;
	int feature_vector_size;
    double *feature_max;
	double *feature_min;
	int    max_index;
	double  lower;
	double  upper;
}SVM_MODEL;
typedef struct _SVMModels
{
	SVM_MODEL model_letter;
	SVM_MODEL model__letter;
	SVM_MODEL model_letter_;
	SVM_MODEL model__letter_;
}SVMModels;
typedef struct MODEL_OCR_RESPONSE
{
   double *probability;
   int response;
};
void thining(Mat & inputarray, Mat & outputarray);
class SVMClassifier
{
public:
	SVMModels models;
	void LoadSvmModels(char*models_path[]);
	int FindSymbolIndex(string strSymbol);
	int CreateSVMFeatureFile(char * outFilePath, int classId, Mat features);
	void Feature_Char(Mat charImg,Mat &FeatureVector);
	MODEL_OCR_RESPONSE MODEL_OCR(Mat Feature, SVM_MODEL &model);
private:
	double MODEL_output(SVM_MODEL &model, int index, double value );
	void MODEL_Read_Range_File(SVM_MODEL &model, const char *restore_filename);
	double StrokeWidthMeasurement4DTime(Mat I, Mat &SW);
	Mat GetFitBW(Mat input, cv::Rect& originalBoundRect);
	void ErodeTines(Mat src, Mat &dst);
	void FitWindow(Mat src, Mat &dst);
	void FindMaxConcavity(Mat src, Mat &concavityMat, float strokeSize, int &crCount, float &dist, float &area, float &roudness, float &solidity);
	
	int feature_svm(char* inPath, int classId, char* outPath);
	double SVMClassifier::GetStrokeSize(Mat src);
};


int SymbolCount = 222;
extern std::wstring arrSymbols[172];
extern string arrModelSymbols[172];
bool IsContained(string str, string str2)
{
	string str1 = "";
	for(int i = 0 ; i < str.size(); i++)
	{
		if(str[i] < 48 || str[i] > 57)
			str1 += str[i];
	}
	if(str1.size() == str2.size())
	{
		for(int i = 0 ; i < str2.size(); i++)
		{
			if(str1[i] != str2[i])
				return false;
		}
	}
	else		
		return false;
	return true;
}
extern int arrHashCodes[172];;
void CreateHashArray();
void AddLogicalSpaces2(std::vector<string> &words1)
  {
	  
	  int hashCode = 0;
	  bool prevIsSpace = false;
	  std::vector<string> words;
	  for(int i = words1.size() - 1; i >= 0; i--)
	  {
		  string str1 = "";
		  for(int j = 0 ; j < words1[i].size(); j++)
		  {
			  if(words1[i][j] < 48 || words1[i][j] > 57)
				  str1 += words1[i][j];
		  }
		  words.push_back(str1);
	  }
	  for(int i = 0; i < words.size(); i++)
	  {
		  hashCode = 0;
		  for(int j = 0 ; j < words[i].size() ;j++)
				hashCode |= (words[i][j] << j * 8);
		  if(!prevIsSpace &&
			  (hashCode == arrHashCodes[18] || hashCode == arrHashCodes[19] || hashCode == arrHashCodes[55] || 
			 hashCode == arrHashCodes[56] || hashCode == arrHashCodes[59] || hashCode == arrHashCodes[60] ||   
			 hashCode == arrHashCodes[61] || hashCode == arrHashCodes[62] || hashCode == arrHashCodes[63] || 
			 hashCode == arrHashCodes[64] || hashCode == arrHashCodes[65] || hashCode == arrHashCodes[66] || 
			 hashCode == arrHashCodes[67] || hashCode == arrHashCodes[68] || hashCode == arrHashCodes[69] ||
			 hashCode == arrHashCodes[70] || hashCode == arrHashCodes[71] || hashCode == arrHashCodes[72] || 
			 hashCode == arrHashCodes[74] || hashCode == arrHashCodes[75] || hashCode == arrHashCodes[78] ||   
			 hashCode == arrHashCodes[79] || hashCode == arrHashCodes[80] || hashCode == arrHashCodes[81] || 
			 hashCode == arrHashCodes[82] || hashCode == arrHashCodes[96] || hashCode == arrHashCodes[97] || 
			 hashCode == arrHashCodes[98] || hashCode == arrHashCodes[99] || hashCode == arrHashCodes[100] ||
			 hashCode == arrHashCodes[101] || hashCode == arrHashCodes[102] || hashCode == arrHashCodes[103] || 
			 hashCode == arrHashCodes[104] || hashCode == arrHashCodes[105] || hashCode == arrHashCodes[106] ||   
			 hashCode == arrHashCodes[107] || hashCode == arrHashCodes[108] || hashCode == arrHashCodes[109] || 
			 hashCode == arrHashCodes[110] || hashCode == arrHashCodes[111] || hashCode == arrHashCodes[112] || 
			 hashCode == arrHashCodes[113] || hashCode == arrHashCodes[114] || hashCode == arrHashCodes[115] || 
			 hashCode == arrHashCodes[116] || hashCode == arrHashCodes[117] || hashCode == arrHashCodes[118] ||   
			 hashCode == arrHashCodes[119] || hashCode == arrHashCodes[120] || hashCode == arrHashCodes[121] || 
			 hashCode == arrHashCodes[122] || hashCode == arrHashCodes[123] || hashCode == arrHashCodes[124] || 
			 hashCode == arrHashCodes[129] || hashCode == arrHashCodes[130] || hashCode == arrHashCodes[131] || 
			 hashCode == arrHashCodes[132]
			 )
			)
		  {
			  words.insert(words.begin() + i, "nimspace");
			  i++;
		  }
		if(hashCode == arrHashCodes[1])
		{
			prevIsSpace = true;
		}
		else
		{
			prevIsSpace = false;
		}
	  }
	  
	  for(int i = 1; i < words.size() - 1; i++)
	  {
		  if(words.size() >= 2 && words[i - 1] == "space" && words[i] ==  "space")
		  {
			  words.erase(words.begin() + i);
		  }

	  }
	  /*for(int i = 1; i < words.size() - 1; i++)
	  {
		  if(words.size() >= 2 && words[i - 1][words[i - 1].size() - 1] == '_' && words[i] ==  "gume_")
		  {
			  words.insert(words.begin() + i,"_t");
			  i++;
		  }

	  }*/
	  if(words.size() >= 2 && words[words.size() - 1] == "a" && words[words.size() - 2] ==  "space")
	  {
			  words[words.size() - 1] = "aa";
			  words.erase(words.end() - 2);
	  }
	  for(int i = 1; i < words.size() - 1; i++)
	  {
		   /*if((words[i - 1] != "space") && (words[i] == "aa") && (words[i + 1] != "space") )
		   {
			   words[i] = "a";
			   continue;
		   }*/
		   if((words[i - 1] == "y" || words[i - 1] == "i") && (words[i] == "h"))
		   {
			   words.insert(words.begin() + i, "space");
			   i++;
			   continue;
		   }
		   if((words[i - 1] == "space") && (words[i] == "aa")  )
		   {
			  words.erase(words.begin() + i - 1);i--;
			   continue;
		   }
		   /*if((words[i - 1] == "n") && (words[i] == "a")  && (words[i + 1] == "space") )
		   {
			   words[i] = "aa";
			   continue;
		   }*/
		  if((words[i - 1] == "space") && (words[i] == "a") && (words[i + 1] == "space") )
		  {
			  words[i] = "aa";
			  words.erase(words.begin() + i - 1);i--;
			  continue;
		  }
		  if(words[i - 1] != "space" )
		  {
			  if( words[i] == "simi" || words[i] == "dot" ||
				  words[i] == "tdot" || words[i] == "kama" || words[i] == "vir" || words[i] == "que")
			  {
 				words.insert(words.begin() + i, "space");
				continue;
			  }
		  }
		  if(words[i + 1] == "space" )
		  {
			  if(words[i] == "akc" || words[i] == "brc" || words[i] == "parc" || words[i] == "simi" || words[i] == "dot" ||
				  words[i] == "tdot" || words[i] == "kama" || words[i] == "vir" || words[i] == "que" || words[i] == "wonder" )
			  {
				words.erase(words.begin() + i + 1);i--;
				continue;
			  }
		  }
		  if(words[i - 1] == "space")
		  {
			  if(words[i] == "ako" || words[i] == "bro" || words[i] == "paro" )
			  {
				words.erase(words.begin() + i - 1);i--;
				continue;
			  }
		  }            
			            
		   if((words[i - 1] == "space") && (words[i + 1] == "space") )
		   {
			   if(words[i] == "hh" || words[i] == "atan" ||words[i] == "zad" ||words[i] == "sin" ||words[i] == "za" ||words[i] == "h" ||words[i] == "ta" ||words[i] == "qaf" ||
				  words[i] == "sh" ||words[i] == "sad" ||words[i] == "allah" ||words[i] == "t" ||words[i] == "y" ||words[i] == "ch" ||words[i] == "i" ||words[i] == "m" ||
				  words[i] == "se" ||words[i] == "p" ||words[i] == "k" ||words[i] == "f" ||words[i] == "b" ||words[i] == "gs" ||words[i] == "he" ||words[i] == "j" ||
				  words[i] == "g"  ||words[i] == "l"||words[i] == "n"||words[i] == "q"||words[i] == "x")
			   {
				   
				words.erase(words.begin() + i + 1);i--;
				continue;
			   }
		   }
	  }
	  for(int i = 1; i < words.size() - 1; i++)
	  {
		  
		  if((words[i] == "zero" || words[i] == "one"  || words[i] == "two"||  words[i] == "three" ||
			  words[i] == "four" || words[i] == "five" || words[i] == "siz"||  words[i] == "seven" || 
			  words[i] == "eight"|| words[i] == "nine"))
		  {
			  if(words[i - 1] != "space" )
			  {
				  
				words.insert(words.begin() + i, "space");
				i++;
			  }
			  if(words[i + 1] != "space" )
			  {
				  
				words.insert(words.begin() + i + 1, "space");
				i++;
			  }
			  continue;
		  }
	  }
	  //می
	  for(int i = 2; i < words.size() - 2; i++)
	  {
		  if(words.size() >= 4 && words[i - 2] == "space" && words[i - 1] == "y_" && words[i] ==  "_m" && words[i + 1] == "space")
		  {
			   words[i - 2] == "nimspace" ;
			   continue;
		  }

	  }
	  words1.clear();
	  for(int i = words.size() - 1; i >= 0; i--)
	  {
		  words1.push_back(words[i]);
	  }
  }
 wstring ConvertToFarsi(vector<string>& text)
 {
	 
	 AddLogicalSpaces2(text);
	 std::wstring retStr = L"";
	 int hashCode = 0;
	 bool bBeFound = false;
	 for(int index = 0; index < text.size(); index++)
	 {
		 string str = text[index];
		 if(IsContained(str, "tine") || IsContained(str, "_tine") ||  IsContained(str, "_tine_"))
		 {
			 string str2 = text[index + 1];
			 if(IsContained(str2, "tine") || IsContained(str2, "_tine") ||  IsContained(str2, "_tine_"))
			 {
				 string str3 = text[index + 2];
				if(IsContained(str3, "tine") || IsContained(str3, "_tine") ||  IsContained(str3, "_tine_"))
				{
					retStr += L"س";
					index += 2;
					continue;
				}
				else
				{
					if(IsContained(str3, "sintail_"))
					{
						retStr += L"س‌";
						index += 2;
						continue;
					}
				}
			 }
			 else
			 {

				 string str3 = text[index + 2];
				 if(IsContained(str3, "tine") || IsContained(str3, "_tine") ||  IsContained(str3, "_tine_"))
				 {
					 retStr += L"ش";
					 index += 2;
					 continue;
				 }
				 else
				 {
					 if(IsContained(str3, "sintail_"))
					 {
						 retStr += L"ش‌";
						 index += 2;
						 continue;
					 }
				 }

			 }
		 }
		 if(index <  text.size() - 2)
		 if((!IsContained(text[index], "_tine") && IsContained(text[index + 1], "_tine_") &&  (IsContained(text[index + 2], "_tine_") || IsContained(text[index + 2], "sintail_"))) ||
			(IsContained(text[index], "_tine") && !IsContained(text[index + 1], "_tine_") &&  (IsContained(text[index + 2], "_tine_") || IsContained(text[index + 2], "sintail_"))) ||
			(IsContained(text[index + 2], "sintail_") && !IsContained(text[index + 1], "_sinDefected") ) 
			)
		 {
			 retStr += L"ش";
			 index += 2;
			 continue;
		 }
		 if(IsContained(str, "_sinDefected") || IsContained(str, "_sinDefected_"))
		 {
			 string str2 = text[index + 1];
			 if(IsContained(str2, "tine"))
			 {
				 retStr += L"س";
				 index += 1;
				 continue;
			 }
			 else
			 {
				 if(IsContained(str2, "sintail_"))
				 {
					 retStr += L"س‌";
					 index += 1;
					 continue;
				 }
			 }
		 }
		 if(IsContained(str, "_shDefected")|| IsContained(str, "_shDefected_"))
		 {
			 string str2 = text[index + 1];
			 if(IsContained(str2, "tine"))
			 {
				 retStr += L"س";
				 index += 1;
				 continue;
			 }
			 else
			 {
				 if(IsContained(str2, "sintail_"))
				 {
					 retStr += L"س‌";
					 index += 1;
					 continue;
				 }
			 }
		 }
		 for(int i = 0 ; i < 172 ;i++)
		 {
			 if(IsContained( str, arrModelSymbols[i]))
			 {
				 retStr += arrSymbols[i];
				 break;
			 }
		 }

		 if(!bBeFound)
		 {
		 }
	 }
	 for(int i = 0 ; i < retStr.size() - 1; i++)
	 {
		 if(retStr[i] == 1589 && retStr[i + 1] == 1587)
		 {
			 retStr.erase(retStr.begin() + i + 1);
			 i--;
		 }
	 }
	 return retStr;
 }
string arrModelSymbols2[] = {
"a",
"a1",
"aa_",
"ahb",
"ahb_",
"aht",
"aht_",
"akc",
"ako",
"allah",
"atan",
"atan_",
"a_",
"b",
"b_",
"ch",
"ch_",
"d",
"darsad1",
"darsad2",
"d_",
"eight",
"eq",
"f",
"five1",
"five2",
"four1",
"four2",
"f_",
"g1",
"g1_",
"g2",
"g2_",
"g3_",
"gs",
"gs1_",
"gs2_",
"gume1_",
"gume2_",
"h",
"hamze",
"he",
"he_",
"hh",
"hh2_",
"hh_",
"h_",
"i1",
"i1_",
"i2",
"i2_",
"i3_",
"j",
"j_",
"k1",
"k1_",
"k2",
"k2_",
"k3_",
"kama",
"l",
"la1",
"la1_",
"la2",
"la2_",
"la_",
"l_",
"m1",
"m1_",
"m2",
"m2_",
"m3_",
"m4_",
"m5_",
"n",
"nine",
"n_",
"one",
"p",
"parc",
"paro",
"plus",
"p_",
"q",
"q1_",
"q2_",
"q3_",
"qaf",
"qaf_",
"que1",
"que2",
"r",
"r_",
"sad1",
"sad1_",
"sad2",
"sad2_",
"se",
"seven",
"se_",
"sh",
"sh_",
"simi",
"sin",
"sintail_",
"sin_",
"six1",
"six2",
"slash",
"star",
"taqsim",
"ta_",
"tdot",
"three",
"two1",
"two2",
"two3",
"v",
"vh",
"vh_",
"vir",
"v_",
"wonder",
"x",
"x1_",
"x2_",
"y1",
"y1_",
"y2",
"y2_",
"y3_",
"za",
"zad1",
"zad1_",
"zad2",
"zad2_",
"zal",
"zal_",
"zarb",
"za_",
"ze",
"zero",
"ze_",
"zh",
"zh_",
"_allah_",
"_b",
"_b_",
"_Ch",
"_Ch1_",
"_Ch2_",
"_f",
"_f_",
"_g",
"_g1_",
"_g2_",
"_gs",
"_gs1_",
"_gs2_",
"_gume1",
"_gume2",
"_h",
"_h1_",
"_h2_",
"_h3_",
"_h4_",
"_he",
"_he1_",
"_he2_",
"_h_",
"_i",
"_i_",
"_j",
"_j1_",
"_j2_",
"_k",
"_k1_",
"_k2_",
"_l",
"_l_",
"_m",
"_m_",
"_n",
"_n_",
"_p",
"_p_",
"_q",
"_q1_",
"_q2_",
"_qaf",
"_qaf_",
"_sad1",
"_sad1_",
"_sad2",
"_sad2_",
"_se",
"_se_",
"_sh",
"_sh_",
"_sin",
"_sinDefected",
"_sinDefected_",
"_shDefected",
"_shDefected_",
"_sin_",
"_t",
"_ta",
"_ta_",
"_tine",
"_tine_",
"_t_",
"_x",
"_x1_",
"_x2_",
"_y",
"_y_",
"_za",
"_zad1",
"_zad1_",
"_zad2",
"_zad2_",
"_za_"
};


wstring arrSymbols2[222] ={
L"a",
L"a1",
L"aa",
L"ahb",
L"ahb_",
L"aht",
L"aht_",
L"akc",
L"ako",
L"allah",
L"atan",
L"atan_",
L"a_",
L"b",
L"b_",
L"ch",
L"ch_",
L"d",
L"darsad1",
L"darsad2",
L"d_",
L"eight",
L"eq",
L"f",
L"five1",
L"five2",
L"four1",
L"four2",
L"f_",
L"g1",
L"g1_",
L"g2",
L"g2_",
L"g3_",
L"gs",
L"gs1_",
L"gs2_",
L"gume1_",
L"gume2_",
L"h",
L"hamze",
L"he",
L"he_",
L"hh",
L"hh2_",
L"hh_",
L"h_",
L"i1",
L"i1_",
L"i2",
L"i2_",
L"i3_",
L"j",
L"j_",
L"k1",
L"k1_",
L"k2",
L"k2_",
L"k3_",
L"kama",
L"l",
L"la1",
L"la1_",
L"la2",
L"la2_",
L"la_",
L"l_",
L"m1",
L"m1_",
L"m2",
L"m2_",
L"m3_",
L"m4_",
L"m5_",
L"n",
L"nine",
L"n_",
L"one",
L"p",
L"parc",
L"paro",
L"plus",
L"p_",
L"q",
L"q1_",
L"q2_",
L"q3_",
L"qaf",
L"qaf_",
L"que1",
L"que2",
L"r",
L"r_",
L"sad1",
L"sad1_",
L"sad2",
L"sad2_",
L"se",
L"seven",
L"se_",
L"sh",
L"sh_",
L"simi",
L"sin",
L"sintail_",
L"sin_",
L"six1",
L"six2",
L"slash",
L"star",
L"taqsim",
L"ta_",
L"tdot",
L"three",
L"two1",
L"two2",
L"two3",
L"v",
L"vh",
L"vh_",
L"vir",
L"v_",
L"wonder",
L"x",
L"x1_",
L"x2_",
L"y1",
L"y1_",
L"y2",
L"y2_",
L"y3_",
L"za",
L"zad1",
L"zad1_",
L"zad2",
L"zad2_",
L"zal",
L"zal_",
L"zarb",
L"za_",
L"ze",
L"zero",
L"ze_",
L"zh",
L"zh_",
L"_allah_",
L"_b",
L"_b_",
L"_Ch",
L"_Ch1_",
L"_Ch2_",
L"_f",
L"_f_",
L"_g",
L"_g1_",
L"_g2_",
L"_gs",
L"_gs1_",
L"_gs2_",
L"_gume1",
L"_gume2",
L"_h",
L"_h1_",
L"_h2_",
L"_h3_",
L"_h4_",
L"_he",
L"_he1_",
L"_he2_",
L"_h_",
L"_i",
L"_i_",
L"_j",
L"_j1_",
L"_j2_",
L"_k",
L"_k1_",
L"_k2_",
L"_l",
L"_l_",
L"_m",
L"_m_",
L"_n",
L"_n_",
L"_p",
L"_p_",
L"_q",
L"_q1_",
L"_q2_",
L"_qaf",
L"_qaf_",
L"_sad1",
L"_sad1_",
L"_sad2",
L"_sad2_",
L"_se",
L"_se_",
L"_sh",
L"_sh_",
L"_sin",
L"_sinDefected",
L"_sinDefected_",
L"_shDefected",
L"_shDefected_",
L"_sin_",
L"_t",
L"_ta",
L"_ta_",
L"_tine",
L"_tine_",
L"_t_",
L"_x",
L"_x1_",
L"_x2_",
L"_y",
L"_y_",
L"_za",
L"_zad1",
L"_zad1_",
L"_zad2",
L"_zad2_",
L"_za_"
};