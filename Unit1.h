//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>

#include <Menus.hpp>
#include <Dialogs.hpp>
#include "ImagXpr7_OCX.h"
#include <OleCtrls.hpp>
#include <ExtCtrls.hpp>
#include "Unit2.h"
#include <CustomizeDlg.hpp>
#include "Unit3.h"
#include <vector.h>
#include <math.h>
#include <ComCtrls.hpp>
#include "CSPIN.h"
#define PI 3.14159265
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
    TMainMenu *MainMenu1;
    TMenuItem *Stages1;
    TMenuItem *Run1;
    TOpenDialog *OpenDialog1;
    TImagXpress7_ *ImagXpress7_1;
    TMenuItem *Info1;
    TLabel *Label1;
    TMenuItem *FirstHorizontalThreshold1;
    TMenuItem *N2ndHorizontalHeightThreshold1;
    TMenuItem *CreateWindows1;
    TMenuItem *CreateFinalImage1;
    TMenuItem *EnableStages1;
    TMenuItem *AutoRunNoStages1;
    TMenuItem *InitializeProccess1;
    TMenuItem *Estimation1;
    TMenuItem *Exit1;
    TMenuItem *File1;
    TMenuItem *OpenImage1;
        
    
    void __fastcall ImagXpress7_1MouseMove(TObject *Sender,
          TShiftState Shift, int X, int Y);
    void __fastcall FormShow(TObject *Sender);
  


    void __fastcall CorrectSlope1Click(TObject *Sender);
    void __fastcall EnableStages1Click(TObject *Sender);
    void __fastcall AutoRunNoStages1Click(TObject *Sender);
    void __fastcall InitializeProccess1Click(TObject *Sender);
    void __fastcall FirstHorizontalThreshold1Click(TObject *Sender);
    void __fastcall N2ndHorizontalHeightThreshold1Click(TObject *Sender);
    void __fastcall CreateWindows1Click(TObject *Sender);
    void __fastcall CreateFinalImage1Click(TObject *Sender);
    void __fastcall OpenDataFile1Click(TObject *Sender);
    void __fastcall Estimation1Click(TObject *Sender);
    void __fastcall Exit1Click(TObject *Sender);
    void __fastcall OpenImage1Click(TObject *Sender);


private:	// User declarations
        int Ix,Iy;
        AnsiString A;
        AnsiString File;
        AnsiString datFile;

        int Estages, Dstages, TextStages, fileSelected, ending;
        int stageDisable;
        int thresholdL, thresholdH;
        int idCount;
        std::vector < vector< int > > Words; // vector of vectors for the words
        std::vector < vector< float > > WordsSlopes;


 
 void TForm1::MenuItemsManager(TObject *Sender);

 int TForm1::CountID(unsigned char *IMAGE,long offs);

 void TForm1::BlackLine(int startX, int endX, int startY, unsigned char* IMAGE, long offs);

 int TForm1::SearchID(int ID);

 void TForm1::ShowSegm(unsigned char *IMAGE,long offs);

 void TForm1::Cut_Horizontal_WhiteRuns(unsigned char* IMAGE, long offs);

 void TForm1::CutByHeight(unsigned char* IMAGE, long offs);

 int TForm1::Height_Between_Blacks(int wordID, int start, int end,int col, unsigned char* IMAGE, long offs);

 void TForm1::CreateWindows(vector<int>& ZH, vector<float>& WindowsSlope,int wordID,int startX, int endX, unsigned char* IMAGE, long offs);

 int TForm1::FindZones(int col, int startY, int endY, unsigned char* IMAGE, long offs);

 //int TForm1::WhiteCol(int start, int end, int col, unsigned char* IMAGE, long offs);

 void TForm1::DarkCol(int start, int end, int col, unsigned char* IMAGE, long offs);

 void TForm1::StoreZones(int wordID, int col, int startY, int endY, unsigned char* IMAGE, long offs, vector<int>& ZH);

 float TForm1::getMean(vector<float>& WindowsSlope, int wordID);

 

 void TForm1::FinalImage(unsigned char* IMAGE2, long offs2, unsigned char* IMAGE1, long offs1);

 void TForm1::MakeWhite(unsigned char* IMAGE2, long offs2);
 
 float TForm1::WindowCentroid(int x1, int x2,int a1, int a2, int wordID, unsigned char* IMAGE, long offs);

 /******************* EXPANSION *************************/
 
 int wordIDs[10];
 unsigned char* im;
 unsigned char* rotatedIm;
 unsigned char* initializedWord;
 AnsiString slantZeroW;


 void TForm1::OpenImageExp(TObject *Sender);
 void TForm1::OpenCheckDataFile(TObject *Sender);
 void TForm1::checkPixel(unsigned char* temp, unsigned char* words, long offs, int wordID);
 int TForm1::BozinovichEstimation(int wordID, unsigned char* wordBS);
 void TForm1::Box(int* tempCoords, unsigned char* wordBS);
 void TForm1::HorizontalStrokes(int* tempCoords, int wordID, unsigned char* wordBS);
 void TForm1::VerticalStrokes(int* tempCoords, int wordID, unsigned char* wordBS);


 int TForm1::vinciarelli_luettin(int ID, unsigned char* wordVL);
 float TForm1::slantEstimation(int ID);
 void TForm1::rotateWordID(int ID,double th);
 void TForm1::initializeWordID(int ID, unsigned char* initializedWord);
 void TForm1::rotateInitializedWordID(int ID,double th, unsigned char* initializedWord);





public:		// User declarations
        __fastcall TForm1(TComponent* Owner);
         AnsiString angleStr;
        /* Create and initialize the vectors */
        void createVectors(int sizeV, int type)
        {
            if(type == 0)
            {
               /* AnsiString s;
                s = s+"size: ";
                s = s+sizeV;
                ShowMessage(s);
                */ 
               for(int i=0; i < sizeV; i++)
               {
                   vector<int> row(5);
                   Words.push_back(row);
               }

               for(int i=0;i<sizeV;i++)
               {
                    Words[i][0] = -1;
                    Words[i][1] = Ix;
                    Words[i][2] = 0;
                    Words[i][3] = Iy;
                    Words[i][4] = 0;
               }
            }
            if(type == 1)
            {
                for(int i=0; i<sizeV; i++)
                {
                  vector<float> rowS(6);
                  WordsSlopes.push_back(rowS);
                }
                for(int i=0;i<sizeV;i++)
                {
                    WordsSlopes[i][0] = -1;
                    WordsSlopes[i][1] = -1;
                    WordsSlopes[i][2] = -1;
                    WordsSlopes[i][3] = -1;
                    WordsSlopes[i][4] = -1;
                    WordsSlopes[i][5] = -1;
                }

            }
        }
        /* Setter and Getters for the Vectors */
        vector< vector<int> >& getWords() 
        {
            return Words;
        }
        vector< vector<float> >& getWordsSlopes()
        {
            return WordsSlopes;
        }
        /* Setter and Getters for the Thresholds */
        void set_ThresholdL(int th)
        {
            thresholdL = th;
        }
        void set_ThresholdH(int h_th)
        {
            thresholdH = h_th;
        }
        int get_ThresholdL() const
        {
             return thresholdL;
        }
        int get_ThresholdH() const
        {
             return thresholdH;
        }
        
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
