//---------------------------------------------------------------------------

#include <vcl.h>
#include <iostream.h>
#include <fstream>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <list>
#pragma hdrstop

#include "Unit1.h"
#include "Unit4.h"
#include "Unit5.h"
#include "Unit6.h"
#include "Unit7.h"
#include "Unit8.h"
#include "Unit10.h"
#include "Unit11.h"
#include "Unit9.h"
using namespace std;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ImagXpr7_OCX"
#pragma link "Unit2"
#pragma link "Unit3"
#pragma link "CSPIN"
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
 ImagXpress7_1->ScrollBars=3;
 Estages = 0;
 Dstages = 0;
 TextStages = 0;
 fileSelected = 0;
 ending = 0;
 stageDisable=0;

 Form1->Stages1->Enabled = false;
 Form1->Run1->Enabled = false;
}
//---------------------------------------------------------------------------

   
void __fastcall TForm1::EnableStages1Click(TObject *Sender)
{
    Estages = 1;
    //ShowMessage("Stages Enabled");
    MenuItemsManager(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::AutoRunNoStages1Click(TObject *Sender)
{
    Dstages =1;
    //ShowMessage("Auto-Run");
    MenuItemsManager(Sender);
    CorrectSlope1Click(Sender);

}
//---------------------------------------------------------------------------

/* MenuItemManager enables and disable choices for the user */
void TForm1::MenuItemsManager(TObject *Sender)
{

    if(fileSelected == 1)
    {
       Form1->Run1->Enabled = true;
       ShowMessage("Opening respective Data File");
       OpenDataFile1Click(Sender);
       fileSelected = 0;

    }
    /* Check if it will run automatically or with stages */
    if(Estages == 1)
    {
      Form1->Stages1->Enabled = true;
      Form1->InitializeProccess1->Enabled = true;
      Form1->FirstHorizontalThreshold1->Enabled = false;
      Form1->N2ndHorizontalHeightThreshold1->Enabled = false;
      Form1->CreateWindows1->Enabled = false;
      Form1->CreateFinalImage1->Enabled = false;
      Estages = 0;
    }
    if(Dstages == 1)
    {
      Form1->Stages1->Enabled = false;
      Dstages = 0;
    }

    /* Check to disable a stage */
    if(stageDisable==1)
    {
      Form1->InitializeProccess1->Enabled = false;
      Form1->FirstHorizontalThreshold1->Enabled = true;
    }
    if(stageDisable == 2)
    {
        Form1->FirstHorizontalThreshold1->Enabled = false;
        Form1->N2ndHorizontalHeightThreshold1->Enabled = true;
    }
    if(stageDisable == 3)
    {
        Form1->N2ndHorizontalHeightThreshold1->Enabled = false;
        Form1->CreateWindows1->Enabled = true;
    }
    if(stageDisable == 4)
    {
        Form1->CreateWindows1->Enabled = false;
        Form1->CreateFinalImage1->Enabled = true;
    }
    if(stageDisable == 5)
    {
       Form1->CreateFinalImage1->Enabled = false;
    }
    /***********************************************/

    /* Check for finishing the application
    free the buffers */
    if(ending == 1)
    {

        if(!Words.empty())
        {
            for(int i=0;i<Words.size(); i++)
            {
                Words[i].erase(Words[i].begin(),Words[i].end());
            }
            Words.erase(Words.begin(),Words.end());
        }
        if(!WordsSlopes.empty())
        {
            for(int i=0;i<WordsSlopes.size(); i++)
            {
                WordsSlopes[i].erase(WordsSlopes[i].begin(),WordsSlopes[i].end());
            }
            WordsSlopes.erase(WordsSlopes.begin(),WordsSlopes.end());
        }
        Estages = 0;
        Dstages = 0;
        TextStages = 0;
        fileSelected = 0;
        stageDisable=0;
        ending = 0;
        ShowMessage("Ending Slant Estimation");
        Form6->ThresholdL->Text = "";
        Form6->ThresholdH->Text = "";
        Form1->Stages1->Enabled = false;
        Form1->Run1->Enabled = false;
    }

}




void __fastcall TForm1::Exit1Click(TObject *Sender)
{
 Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::OpenImage1Click(TObject *Sender)
{
    if (OpenDialog1->Execute())
    {
        File = OpenDialog1->FileName;
        if (FileExists(File))
        {
            ImagXpress7_1->FileName = File;
            Ix = ImagXpress7_1->IWidth;
            Iy = ImagXpress7_1->IHeight;
        }
        fileSelected = 1;
        MenuItemsManager(Sender);
    }
    else
    {
        ShowMessage("Image Problem");
        exit(-1);
    }
}


/* Open's the data file with the word id's */
void __fastcall TForm1::OpenDataFile1Click(TObject *Sender)
{
         int value; // store the value of the pixel
        unsigned int *IM_SegmResult; //Pointer to store raw data
        AnsiString SegmResultName = File; //File that contains the raw data
        SegmResultName = SegmResultName + ".dat";
        FILE *f1;

        IM_SegmResult = (unsigned int *) calloc (Ix*Iy,sizeof(int));
        f1 = fopen(SegmResultName.c_str(),"rb");
        if (f1==NULL)
        {
            fputs ("File error",stderr);
            ShowMessage("File cannot be opened");
            exit(1);
        }
        fread(IM_SegmResult,Ix*Iy,sizeof(int),f1);

        ImagXpress7_1->IWidth = Ix;
        ImagXpress7_1->IHeight = Iy;
        ImagXpress7_1->ColorDepth(8,1,0);

        ImagXpress7_1->SaveToBuffer = true;
        ImagXpress7_1->SaveFileType =  FT_TIFF;
        ImagXpress7_1->SaveFile ();

        /* Create a buffer for the image */
        unsigned char *IMAGE;
        HANDLE hIM = (HANDLE)ImagXpress7_1->SaveBufferHandle;
        IMAGE = (unsigned char *)GlobalLock(hIM);
        long ln = GlobalSize(hIM);
        long offs=ln-(long)Ix*Iy;
        GlobalUnlock(hIM);

        Form7->ImagXpress7_1->IWidth = Ix;
        Form7->ImagXpress7_1->IHeight = Iy;
        Form7->ImagXpress7_1->hDIB = ImagXpress7_1->CopyDIB();
        Form7->ImagXpress7_1->SaveToBuffer = true;
        Form7->ImagXpress7_1->SaveFileType =  FT_TIFF;
        Form7->ImagXpress7_1->SaveFile ();

        /* Show a progressbar */
        Form5->ProgressBar1->Max=Ix-1;
        Form5->Show();
        for (int x=0;x<Ix;x++)
        {
            Form5->ProgressBar1->Position = x;
            for (int y=0;y<Iy;y++)
            {
                value = *(IM_SegmResult+y*Ix+x);
                if(value == 0)
                {
                    value = 255;
                    *(IMAGE+y*Ix+x+offs) = value;
                }
                else
                {
                    *(IMAGE+y*Ix+x+offs) = value;
                }
            }
        }


        ImagXpress7_1->LoadBuffer((long)IMAGE);
        Form5->Close();
       // ShowMessage("Data file in ImagExpress!");

        free(IM_SegmResult);
        GlobalFree(IMAGE);
        fclose(f1);

}   // end of function
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
/* Initialize's the arrays for the coordinates of the words */
void __fastcall TForm1::InitializeProccess1Click(TObject *Sender)
{
    Form6->ShowModal();
    ImagXpress7_1->SaveToBuffer = true;
    ImagXpress7_1->SaveFileType =  FT_TIFF;
    ImagXpress7_1->SaveFile ();

    unsigned char *IMAGE;
    HANDLE hIM = (HANDLE)ImagXpress7_1->SaveBufferHandle;
    IMAGE = (unsigned char *)GlobalLock(hIM);
    long ln = GlobalSize(hIM);
    long offs=ln-(long)Ix*Iy;
    GlobalUnlock(hIM);

    idCount = CountID(IMAGE, offs);
    createVectors(idCount+1, 0);
    createVectors(idCount+1, 1);

    int position = -1;
    int pixelID=-1;


    for (int y=0;y<Iy;y++)
    {
        for (int x=0;x<Ix;x++)
        {
            pixelID = *(IMAGE+y*Ix+x+offs);
            if(pixelID == 255)
            {
                continue;
            }
            else
            {
                position = SearchID(pixelID);
                if(position == -1) //new id
                {
                    Words[pixelID][0] = pixelID;
                }
                if(x < Words[pixelID][1])
                {
                    Words[pixelID][1] = x;
                }
                if(x > Words[pixelID][2])
                {
                    Words[pixelID][2] = x;
                }
                int tempID;
                for(int i=y;i<Iy;i++) // height of word for
                {
                    tempID = *(IMAGE+i*Ix+x+offs);
                    if(tempID != pixelID) // if we found another word break
                    {
                        break;
                    }
                    else
                    {
                        if(i < Words[tempID][3])
                        {
                            Words[tempID][3] = i;
                        }
                        if(i > Words[tempID][4])
                        {
                            Words[tempID][4] = i;
                        }
                    }
                } // height for word for
            }

        } //inner for  x

    }  //outer for y

     /* try to box outside the word */
    ShowSegm(IMAGE, offs);
    GlobalFree(IMAGE);
    /* disable choice */
    stageDisable = 1;
    MenuItemsManager(Sender);
}

/* A listener for showing color and coordinates of the pixel */
void __fastcall TForm1::ImagXpress7_1MouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{

  if (!FileExists(File)) return;

  int x = ImagXpress7_1->DIBXPos;
  int y = ImagXpress7_1->DIBYPos;
  int c = ImagXpress7_1->DIBGetPixel(x,y);

  AnsiString S = "["; S=S+x;S=S+",";S=S+y;
  S=S+"] c:";S=S+c;

  Form1->Label1->Caption=S;

}

//---------------------------------------------------------------------------

 void __fastcall TForm1::FormShow(TObject *Sender)
{
  Form5->Show();
  Form5->Close();
}

//---------------------------------------------------------------------------


/* This is the Auto-run function for the algorithm */
void __fastcall TForm1::CorrectSlope1Click(TObject *Sender)
{
     Form6->ShowModal();
    // ImagXpress7_1->ColorDepth(8,1,0);
     ImagXpress7_1->SaveToBuffer = true;
     ImagXpress7_1->SaveFileType =  FT_TIFF;
     ImagXpress7_1->SaveFile ();

     unsigned char *IMAGE;
     HANDLE hIM = (HANDLE)ImagXpress7_1->SaveBufferHandle;
     IMAGE = (unsigned char *)GlobalLock(hIM);
     long ln = GlobalSize(hIM);
     long offs=ln-(long)Ix*Iy;
     GlobalUnlock(hIM);

     idCount = CountID(IMAGE, offs);
     createVectors(idCount+1, 0);
     createVectors(idCount+1, 1);

     int position = -1;
     int pixelID=-1;
     /* Store the coordinates of every word */
     for (int x=0;x<Ix;x++)
     {
       for (int y=0;y<Iy;y++)
       {
          pixelID = *(IMAGE+y*Ix+x+offs);

          if(pixelID == 255)
          {
            continue;
          }
          else
          {
              position = SearchID(pixelID);
              if(position == -1) //new id
              {
                Words[pixelID][0] = pixelID;
              }
              if(x < Words[pixelID][1])
              {
                Words[pixelID][1] = x;
              }
              if(x > Words[pixelID][2])
              {
                Words[pixelID][2] = x;
              }
              int tempID;
              for(int i=y;i<Iy;i++) // height of word for
              {
                tempID = *(IMAGE+i*Ix+x+offs);
                if(tempID != pixelID) // if we found another word break
                {
                   break;
                }
                else
                {
                    if(i < Words[tempID][3])
                    {
                        Words[tempID][3] = i;
                    }
                    if(i > Words[tempID][4])
                    {
                      Words[tempID][4] = i;
                    }
                }
              } // height for word for
          }

       } //inner for  x

     }  //outer for y
      /* show a box outside the word */
      ShowSegm(IMAGE, offs);

     /***** Start cutting lines that the thresholds indicate ******/


     Cut_Horizontal_WhiteRuns(IMAGE, offs);
     ImagXpress7_1->LoadBuffer((long)IMAGE);
     CutByHeight(IMAGE, offs);
     ImagXpress7_1->LoadBuffer((long)IMAGE);
     
     /*Form8->ImagXpress7_1->ColorDepth(8,1,0);
     Form8->ImagXpress7_1->hDIB = ImagXpress7_1->CopyDIB();
     Form8->ImagXpress7_1->SaveToBuffer = true;
     Form8->ImagXpress7_1->SaveFileType =  FT_TIFF;
     Form8->ImagXpress7_1->SaveFile ();


     Form8->ShowModal();
     */
     Form7->ImagXpress7_1->ColorDepth(8,1,0);

     unsigned char *IMAGE1;
     HANDLE hIM1 = (HANDLE)Form7->ImagXpress7_1->SaveBufferHandle;
     IMAGE1 = (unsigned char *)GlobalLock(hIM1);
     long ln1 = GlobalSize(hIM1);
     long offs1=ln1-(long)Ix*Iy;
     GlobalUnlock(hIM1);

     Form7->ImagXpress7_2->ColorDepth(8,1,0);
     Form7->ImagXpress7_2->hDIB = Form7->ImagXpress7_1->CopyDIB();
     Form7->ImagXpress7_2->SaveToBuffer = true;
     Form7->ImagXpress7_2->SaveFileType =  FT_TIFF;
     Form7->ImagXpress7_2->SaveFile ();
     Form7->ImagXpress7_2->IWidth = Ix;
     Form7->ImagXpress7_2->IHeight = Iy;

     /* Second buffer to store the final Result from the shear transform */
     unsigned char *IMAGE2;
     HANDLE hIM2 = (HANDLE)Form7->ImagXpress7_2->SaveBufferHandle;
     IMAGE2 = (unsigned char *)GlobalLock(hIM2);
     long ln2 = GlobalSize(hIM2);
     long offs2=ln2-(long)Ix*Iy;
     GlobalUnlock(hIM2);
     MakeWhite(IMAGE2, offs2);
    
     int startX,startY,endX,endY,point,wordID;
     int zones=0;

     for(int i=0;i<Words.size();i++)
     {
         if(Words[i][0] == -1)
         {
            continue;
         }

         wordID = Words[i][0];
         startX = Words[i][1];
         endX = Words[i][2];
         startY = Words[i][3];
         endY = Words[i][4];
         int col = (startX + endX)/2;
         /* return the nmber of zones of the word */
         zones = FindZones(col, startY, endY, IMAGE, offs);

         if(zones > 0)
         {
            vector<int> ZH((zones*2),-1);
            /* Store the coordinates of the zones */
            StoreZones(wordID, col, startY, endY, IMAGE, offs, ZH);
            
            // Store the slope of every window of the word
            vector<float> WindowsSlope;
            /* create the windows for the given zones */
            CreateWindows(ZH, WindowsSlope, wordID, startX, endX, IMAGE, offs);

            /* if there is no slope's in windows don't bother to get the mean slope */
            if(!WindowsSlope.empty())
            {
                float mean = getMean(WindowsSlope,wordID);
                WindowsSlope.clear();
                WordsSlopes[i][0] = wordID;
                WordsSlopes[i][1] = startX;
                WordsSlopes[i][2] = endX;
                WordsSlopes[i][3] = startY;
                WordsSlopes[i][4] = endY;
                WordsSlopes[i][5] = mean;
           }
           else // the word does not have slope
           {
                float mean = 0;
                WordsSlopes[i][0] = wordID;
                WordsSlopes[i][1] = startX;
                WordsSlopes[i][2] = endX;
                WordsSlopes[i][3] = startY;
                WordsSlopes[i][4] = endY;
                WordsSlopes[i][5] = mean;
           }
         }
         else {
             /* we have no zones so copy paste the old word */
             WordsSlopes[i][0] = wordID;
             WordsSlopes[i][1] = startX;
             WordsSlopes[i][2] = endX;
             WordsSlopes[i][3] = startY;
             WordsSlopes[i][4] = endY;
             WordsSlopes[i][5] = 0;
         }


     }// for, for Words

    ImagXpress7_1->LoadBuffer((long)IMAGE);

    ShowMessage("Creating final image");

    FinalImage(IMAGE2,offs2,IMAGE1,offs1);

    Form7->ImagXpress7_1->LoadBuffer((long)IMAGE1);
    Form7->ImagXpress7_2->LoadBuffer((long)IMAGE2);
    Form7->ShowModal();

    GlobalFree(IMAGE);
    GlobalFree(IMAGE1);
    GlobalFree(IMAGE2);
    ending = 1;
    MenuItemsManager(Sender);
}

// makes image white
void TForm1::MakeWhite(unsigned char* IMAGE2, long offs2)
{
    for(int x=0; x<Ix; x++)
    {
        for(int y=0; y<Iy; y++)
        {
           *(IMAGE2+y*Ix+x+offs2) = 255;
        }
    }
}

/* Create the final Image and deslant the words with shear transform */
void TForm1::FinalImage(unsigned char* IMAGE2, long offs2, unsigned char* IMAGE1, long offs1)
{
    int wordID,startX,endX,startY,endY;
    float slope;
    /* for every word... */
    for(int i=0;i<WordsSlopes.size();i++)
    {
         if(WordsSlopes[i][0] == -1)
         {
            continue;
         }
         wordID = WordsSlopes[i][0];
         startX = WordsSlopes[i][1];
         endX = WordsSlopes[i][2];
         startY = WordsSlopes[i][3];
         endY = WordsSlopes[i][4];
         slope = WordsSlopes[i][5];
         /* check if we have some slope */
         if(slope == 0)
         {
            for(int x=startX; x<=endX; x++)
            {
                for(int y=startY; y<=endY; y++)
                {
                     *(IMAGE2+ y*Ix+x+offs2) = *(IMAGE1+ y*Ix+x+offs1);
                }
            }
         }
         else
         {
            /* create a shear transform */

            int newX=0;
            int newY=0;

            int new_startX;
            int new_endX;

            float deg =  ( (float)atan (slope) ) * 180.0 / 3.14159265 ;
            
            for(int x=startX; x<=endX; x++){
                for(int y=startY; y<=endY; y++){
                
                    if(deg < 0)
                    {
                        newX = x + (y*tan ( deg * PI / 180.0 ));
                    }
                    else {
                       newX = x - (y*tan ( deg * PI / 180.0 ));
                    }
                    newY = y;

                    if( x == startX)
                        new_startX = newX;
                    if( x == startX)
                        new_endX = newX;
                }
            }

            int dStart = new_startX - startX;
            int dEnd = new_endX - endX;

            for(int x=startX; x<=endX; x++){
                for(int y=startY; y<=endY; y++){

                   if(deg < 0)
                   {
                        newX = x + (y*tan ( deg * PI / 180.0 )) - dStart;
                   }
                   else {
                        newX = x - (y*tan ( deg * PI / 180.0 )) - dStart;
                   }

                    newY = y;
                    *(IMAGE2+newY*Ix+newX+offs2) = *(IMAGE1+y*Ix+x+offs1);
                }
            }
        }//else
    }//for Words
}

/* Calculates the mean value from the slopes of the windows */
float TForm1::getMean(vector<float>& WindowsSlope, int wordID)
{
   float meanSlope = 0;
   float tempSlope = 0;
   float degrees=0;
   for(int i=0; i<WindowsSlope.size(); i++)
   {
        tempSlope = tempSlope + WindowsSlope[i];
   }
   if(WindowsSlope.size() != 0)
   {
        meanSlope = (tempSlope) / (WindowsSlope.size());
        degrees = atan (meanSlope) * 180 / PI;
   }

    /*AnsiString s1 = "MeanSlope for wordID: ";
    s1=s1+wordID;
    s1=s1+"\n";
    s1=s1+"and Slopes size: ";
    s1=s1+ WindowsSlope.size();
    s1=s1+"\n";
    s1=s1+"meanSlope: ";
    s1=s1+meanSlope;
    s1=s1+"\n";
    s1=s1+"degrees: ";
    s1=s1+degrees;
    ShowMessage(s1);*/           
    
   return meanSlope;
}

/* Stores the zones in ZH vector */
void TForm1::StoreZones(int wordID, int col, int startY, int endY, unsigned char* IMAGE, long offs, vector<int>& ZH)
{
    int current = startY;
    int a1;
    int times = 0;
    int pos = 0;
    int height = 0;
    int point;

    while(current <=endY)
    {
        point = *(IMAGE+current*Ix+col+offs);
        if(point != 0)
        {
           height++;
           if(height == 1)
           {
                ZH[pos] = current;
           }
        }
        if(point == 0 && height >0)
        {
            ZH[pos+1] = ZH[pos]+(height-1);
            pos+=2;
            height = 0;
        }
        if(current == endY && point!=0)
        {
             ZH[pos+1] = ZH[pos]+(height-1);
             pos+=2;
        }
        current++;

    }

}

/* calculates the height between two black lines and returns it */
int TForm1::Height_Between_Blacks(int wordID, int start, int end,int col, unsigned char* IMAGE, long offs)
{
        int len = 0;
        int pix;
        for(int i=start; i<=end; i++)
        {
            pix = *(IMAGE+i*Ix+col+offs);

            if(pix != 0)
            {
              len++;
            }
            else
            {
                break;
            }
        }
        return len;
   
}

/* find the number of zones a word has */
int TForm1::FindZones(int col, int startY, int endY, unsigned char* IMAGE, long offs)
{

     int current = startY;
     int a1, zones=0;
     int point;
     int height=0;

     while(current <=endY)
     {
        point = *(IMAGE+current*Ix+col+offs);
        if(point != 0)
        {
           height++;
           if(height == 1)
           {
                zones++;
           }
        }
        if(point == 0 && height >0)
        {
            height = 0;
        }
        if(height == (endY+1)-startY)  // if the heigth is same as the word's
        {
            zones = 0;  // zero the zones
        }
        current++;

     }
     
     return zones;
}

/* Creates windows from the given coordinates(ZH vector) and after that
it stores the slope for the window in WindowsSlope vector */
void TForm1::CreateWindows(vector<int>& ZH, vector<float>& WindowsSlope, int wordID, int startX, int endX, unsigned char* IMAGE, long offs)
{

  int i=0;
  while(i<ZH.size())
  {
    int a1 = ZH[i];
    int a2 = ZH[i+1];

    int prevCounter = 0;
    int curCounter = 0;
    int flag = 0;
    int startCol = 0;
    int endCol = 0;
    for(int col=startX; col<=endX; col++)
    {
        for(int j=a1; j<=a2; j++)
        {
            
            int point = *(IMAGE+j*Ix+col+offs);
            if(point != 255 && point != 0)
            {
              curCounter++;
            }

        } // for with j
        if(curCounter > 0 && prevCounter == 0)
        {
            startCol = col-1;
            DarkCol(a1, a2, startCol, IMAGE, offs);
            flag++;
        }
        if((curCounter == 0 && prevCounter > 0) || (curCounter > 0 && col == endX))
        {
            if(curCounter > 0 && col == endX)
            {
               endCol = col+1;
            }
            else
            {
                endCol = col;
            }
            DarkCol(a1, a2, endCol, IMAGE, offs);
            flag++;
            //second vertical line
        }
        if(flag == 2)
        {
          int x1 = startCol+1;
          int x2 = endCol-1;
          
          //count weights , find slope of windows
          float slope = WindowCentroid(x1, x2, a1, a2, wordID, IMAGE, offs);
          //add the slope of the current window
          WindowsSlope.push_back(slope);
          flag=0;
          startCol = endCol;
        }

        prevCounter = curCounter;
        curCounter = 0;
    } // for, with col
    i = i+2;
  } // while
}



/* Calculates the centroid of the upper and lower halves
and finds the slope value */
float TForm1::WindowCentroid(int x1, int x2,int a1, int a2, int wordID, unsigned char* IMAGE, long offs)
{
        int counterUpper = 0;
        int sumX =0,sumY =0;
        float cOfmU=0;
        float cOfmS=0;
        int counterY=0;
        int wcounter=0;
        float slope;
        int offset = (a2-a1)/2;

        for(int y=a1;y<=a1+offset;y++){
                for(int x=x1; x<=x2; x++){

                        int point = *(IMAGE+y*Ix+x+offs);

                        if(point == wordID){
                                sumX = sumX + x;
                                sumY = sumY + y;
                                counterUpper++;
                        }
                }
        }

        if(counterUpper == 0)
                return 0.0;

        float centroidX1 = sumX/counterUpper;
        float centroidY1 = sumY/counterUpper;


        counterUpper = 0;
        sumX = sumY =0;
        counterY = 0;

        for(int y=a1+offset+1;y<=a2;y++){
                for(int x=x1; x<=x2; x++){

                        int point = *(IMAGE+y*Ix+x+offs);

                        if(point == wordID){

                                sumX = sumX + x;
                                sumY = sumY + y;
                                counterUpper++;
                        }
                }
        }

        if(counterUpper == 0)
                return 0.0;

        float centroidX2 = sumX/counterUpper;
        float centroidY2 = sumY/counterUpper;

        if( (centroidY2-centroidY1) != 0 )
        {
                slope = (centroidX2-centroidX1) /  (centroidY2-centroidY1);
        }

        return slope;
}
/********************************/

/* Makes a given column black */
void TForm1::DarkCol(int start, int end, int col, unsigned char* IMAGE, long offs)
{
  for(int i=start; i<=end; i++)
  {
    *(IMAGE+i*Ix+col+offs) = 0;
  }

}

/*
int TForm1::WhiteCol(int start, int end, int col, unsigned char* IMAGE, long offs)
{
    int length = 0;
    for(int i=0; i<=end; i++)
    {
        if(*(IMAGE+i*Ix+col+offs) == 255)
        {
            length++;
        }
        else
        {
          break;
        }

    }
    return length;

}
*/

/* Discard lines due to the given threshold
Threshold value is percent of the height of the word
*/
void TForm1::Cut_Horizontal_WhiteRuns(unsigned char* IMAGE, long offs)
{

     int point, wordID;
     int len;
     int startX,startY,endX,endY,wordH=0,wordL=0;
     int percentH = 0;

     for(int i=0;i<Words.size();i++)
     {
        if(Words[i][0] == -1)
        {
            continue;
        }
        wordID = Words[i][0];
        startX = Words[i][1];
        endX = Words[i][2];
        startY = Words[i][3];
        endY = Words[i][4];
        wordH = endY - startY;
        //wordL = endX-startX;
        percentH = wordH * get_ThresholdL() / 100;
         //percentH = wordL * get_ThresholdL() / 100;

        for(int y=startY; y <= endY; y++)
        {
            len = 0;
            for(int x = startX; x <= endX; x++)
            {
                point = *(IMAGE+y*Ix+x+offs);
                if(point == wordID)
                {
                    len++;
                }
                else {
                 len=0;
                }

            }//for with x
            if(len > percentH)
            {
                //black the line
                BlackLine(startX, endX, y, IMAGE, offs);
            }
        }//for with y

     }// for, with Words.size()

}

/* Discard lines between blacks if the height is lower than a given threshold*/
void TForm1::CutByHeight(unsigned char* IMAGE, long offs)
{
     int point, wordID;
     int len;
     int startX,startY,endX,endY,wordH = 0;
       /* AnsiString s = "Threshold Vertical: ";
           s=s+get_ThresholdH();
           ShowMessage(s);
       */
     //float percentH;
     for(int i=0;i<Words.size();i++)
     {
        if(Words[i][0] == -1)
        {
            continue;
        }
        wordID = Words[i][0];
        startX = Words[i][1];
        endX = Words[i][2];
        startY = Words[i][3];
        endY = Words[i][4];
       // wordH = endY - startY;
       // percentH = wordH * (get_ThresholdH() / 100);

        int center = (startX + endX)/2;
        int current = startY;
        int a1;

        while(current <= endY)
        {
            point = *(IMAGE+current*Ix+center+offs);
            if(point == 0)
            {
                current++;
                continue;
            }
            else //start descending
            {
                a1 = current;
                int height = Height_Between_Blacks(wordID,a1, endY,center,IMAGE,offs);
                if(height < get_ThresholdH() && height > 0)
                {
                    for(int j=a1; j<=a1+height; j++)
                    {
                        for(int k=startX; k<=endX; k++)
                        {
                           *(IMAGE+j*Ix+k+offs) = 0;
                        }
                    }
                }
                current = a1+height;

            }//else
        }//while
     } //1st for
     
}



/* Blacks a whole line */
void TForm1::BlackLine(int startX, int endX, int startY, unsigned char* IMAGE, long offs)
{
        for(int i = startX; i <= endX; i++)
        {
            *(IMAGE+startY*Ix+i+offs) = 0;
        }

}
/* Count the number of ID's */
int TForm1::CountID(unsigned char *IMAGE,long offs)
{
     int count = 0;
     int maxID = -1;

     for (int x=0;x<Ix;x++)
     {
       for (int y=0;y<Iy;y++)
       {
          int value = *(IMAGE+y*Ix+x+offs);
          
          if ( value > maxID && value != 255)
          {
             maxID = value;

          }
       }
     }
  return maxID;

}

/* Create's a box outside the word */
void TForm1::ShowSegm(unsigned char *IMAGE,long offs)
{
    int startX;
    int endX;
    int startY;
    int endY;
    for(int i=0;i<Words.size();i++)
    {

        int startX = Words[i][1];
        int endX = Words[i][2];
        int startY = Words[i][3];
        int endY = Words[i][4];
        
        if(Words[i][0] == -1)
        {
              continue;
        }
        int it = startY-1;
        while(it <= endY+1)
        {
            *(IMAGE+it*Ix+(startX-1)+offs) = 0;
            it++;
        }
        it = startX-1;
        while(it <= endX+1)
        {
            *(IMAGE+(endY+1)*Ix+it+offs) = 0;
            it++;
        }
        it = endY+1;
        while(it >= startY-1)
        {
            *(IMAGE+it*Ix+(endX+1)+offs) = 0;
            it--;
        }
        it = endX+1;
        while(it >= startX-1)
        {
            *(IMAGE+(startY-1)*Ix+it+offs) = 0;
            it--;
        }
    }
    ImagXpress7_1->LoadBuffer((long)IMAGE);

}


/*search the first column of the vector to find the ID given*/
int TForm1::SearchID(int ID)
{
   int y=0;
   int pos = -1;
	while(y<Words.size())
	{
		if(Words[y][0] == ID)
		{
			return y;
		}
		y++;
	}

	return -1; // -1 if the id does not exists in vector
}


//---------------------------------------------------------------------------

void __fastcall TForm1::FirstHorizontalThreshold1Click(TObject *Sender)
{
    //ImagXpress7_1->ColorDepth(8,1,0);
    ImagXpress7_1->SaveToBuffer = true;
    ImagXpress7_1->SaveFileType =  FT_TIFF;
    ImagXpress7_1->SaveFile ();

    unsigned char *IMAGE;
    HANDLE hIM = (HANDLE)ImagXpress7_1->SaveBufferHandle;
    IMAGE = (unsigned char *)GlobalLock(hIM);
    long ln = GlobalSize(hIM);
    long offs=ln-(long)Ix*Iy;
    GlobalUnlock(hIM);

    Cut_Horizontal_WhiteRuns(IMAGE,offs);

    ImagXpress7_1->LoadBuffer((long)IMAGE);
    GlobalFree(IMAGE);
    stageDisable = 2;
    MenuItemsManager(Sender);

}
//---------------------------------------------------------------------------

void __fastcall TForm1::N2ndHorizontalHeightThreshold1Click(
      TObject *Sender)
{
   // ImagXpress7_1->ColorDepth(8,1,0);
    ImagXpress7_1->SaveToBuffer = true;
    ImagXpress7_1->SaveFileType =  FT_TIFF;
    ImagXpress7_1->SaveFile ();

    unsigned char *IMAGE;
    HANDLE hIM = (HANDLE)ImagXpress7_1->SaveBufferHandle;
    IMAGE = (unsigned char *)GlobalLock(hIM);
    long ln = GlobalSize(hIM);
    long offs=ln-(long)Ix*Iy;
    GlobalUnlock(hIM);

    CutByHeight(IMAGE,offs);
    ImagXpress7_1->LoadBuffer((long)IMAGE);
    GlobalFree(IMAGE);
    stageDisable = 3;
    MenuItemsManager(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::CreateWindows1Click(TObject *Sender)
{
   // ImagXpress7_1->ColorDepth(8,1,0);
    ImagXpress7_1->SaveToBuffer = true;
    ImagXpress7_1->SaveFileType =  FT_TIFF;
    ImagXpress7_1->SaveFile ();

    unsigned char *IMAGE;
    HANDLE hIM = (HANDLE)ImagXpress7_1->SaveBufferHandle;
    IMAGE = (unsigned char *)GlobalLock(hIM);
    long ln = GlobalSize(hIM);
    long offs=ln-(long)Ix*Iy;
    GlobalUnlock(hIM);
    
     int startX,startY,endX,endY,point,wordID;
     int zones=0;
    
     for(int i=0;i<Words.size();i++)
     {
         if(Words[i][0] == -1)
         {
            continue;
         }

         wordID = Words[i][0];
         startX = Words[i][1];
         endX = Words[i][2];
         startY = Words[i][3];
         endY = Words[i][4];
         int col = (startX + endX)/2;
         
         zones = FindZones(col, startY, endY, IMAGE, offs);

         if(zones > 0)
         {
            // vector of zones*2 because we want start and end
            vector<int> ZH((zones*2),-1);
            StoreZones(wordID, col, startY, endY, IMAGE, offs, ZH);

            // Store the slope of every window of the word
            vector<float> WindowsSlope;

            CreateWindows(ZH, WindowsSlope, wordID, startX, endX, IMAGE, offs);

            if(!WindowsSlope.empty())
            {
                float mean = getMean(WindowsSlope,wordID);
                WindowsSlope.clear();
                WordsSlopes[i][0] = wordID;
                WordsSlopes[i][1] = startX;
                WordsSlopes[i][2] = endX;
                WordsSlopes[i][3] = startY;
                WordsSlopes[i][4] = endY;
                WordsSlopes[i][5] = mean;
            }
            else // the word does not have slope
            {
                float mean = 0;
                WordsSlopes[i][0] = wordID;
                WordsSlopes[i][1] = startX;
                WordsSlopes[i][2] = endX;
                WordsSlopes[i][3] = startY;
                WordsSlopes[i][4] = endY;
                WordsSlopes[i][5] = mean;
            }
         }
         else // the word does not have zones
         {
                float mean = 0;
                WordsSlopes[i][0] = wordID;
                WordsSlopes[i][1] = startX;
                WordsSlopes[i][2] = endX;
                WordsSlopes[i][3] = startY;
                WordsSlopes[i][4] = endY;
                WordsSlopes[i][5] = mean;
            
         }


     }// for for Words

     ImagXpress7_1->LoadBuffer((long)IMAGE);

     GlobalFree(IMAGE);
     stageDisable = 4;
     MenuItemsManager(Sender);


}
//---------------------------------------------------------------------------

void __fastcall TForm1::CreateFinalImage1Click(TObject *Sender)
{

    Form7->ImagXpress7_1->ColorDepth(8,1,0);

    unsigned char *IMAGE1;
    HANDLE hIM1 = (HANDLE)Form7->ImagXpress7_1->SaveBufferHandle;
    IMAGE1 = (unsigned char *)GlobalLock(hIM1);
    long ln1 = GlobalSize(hIM1);
    long offs1=ln1-(long)Ix*Iy;
    GlobalUnlock(hIM1);

    Form7->ImagXpress7_2->ColorDepth(8,1,0);
    Form7->ImagXpress7_2->hDIB = Form7->ImagXpress7_1->CopyDIB();
    Form7->ImagXpress7_2->SaveToBuffer = true;
    Form7->ImagXpress7_2->SaveFileType =  FT_TIFF;
    Form7->ImagXpress7_2->SaveFile ();
    Form7->ImagXpress7_2->IWidth = Ix;
    Form7->ImagXpress7_2->IHeight = Iy;


    unsigned char *IMAGE2;
    HANDLE hIM2 = (HANDLE)Form7->ImagXpress7_2->SaveBufferHandle;
    IMAGE2 = (unsigned char *)GlobalLock(hIM2);
    long ln2 = GlobalSize(hIM2);
    long offs2=ln2-(long)Ix*Iy;
    GlobalUnlock(hIM2);
    
    ShowMessage("Creating final image");

    MakeWhite(IMAGE2, offs2);
    FinalImage(IMAGE2,offs2,IMAGE1,offs1);

    Form7->ImagXpress7_1->LoadBuffer((long)IMAGE1);
    Form7->ImagXpress7_2->LoadBuffer((long)IMAGE2);
    Form7->ShowModal();

    GlobalFree(IMAGE1);
    GlobalFree(IMAGE2);
    stageDisable = 5;
    ending = 1;
    MenuItemsManager(Sender);

}
//---------------------------------------------------------------------------

/************************ EXPANSION ****************************************/

/***************** Bozinovic - Shrihari for Estimation ******************************/
int TForm1::BozinovichEstimation(int wordID, unsigned char* wordBS)
{
         int degrees;

        /*
          For every word create a temp array and save the xmin,xmax,ymin,ymax
          in order to create an area for the processed word
        */
        int* tempCoords = (int*)malloc(4*sizeof(int));
        tempCoords[0] = Ix; // xmin
        tempCoords[1] = 0;  // xmax
        tempCoords[2] = Iy; // ymin
        tempCoords[3] = 0;  // ymax

        int pixelID;
        for (int x=0;x<Ix;x++)
        {
            for (int y=0;y<Iy;y++)
            {
                pixelID = *(wordBS+y*Ix+x);

                if(pixelID == wordID)
                {
                    if(x < tempCoords[0])
                    {
                        tempCoords[0] = x;
                    }
                    if(x > tempCoords[1])
                    {
                        tempCoords[1] = x;
                    }
                    int tempID;
                    for(int i=y;i<Iy;i++) // height of word for
                    {
                        tempID = *(wordBS+i*Ix+x);
                        if(tempID != pixelID) // if we found something else
                        {
                            break;
                        }
                        else
                        {
                            if(i < tempCoords[2])
                            {
                                tempCoords[2] = i;
                            }
                            if(i > tempCoords[3])
                            {
                                tempCoords[3] = i;
                            }
                        }
                    } // height for word for
                }
                else
                {
                   continue;
                }// else

            } //inner for  x

        }  //outer for y

        /* Show box for the word */
        Box(tempCoords, wordBS);


         /* Cut Horizontal and Vertical Strokes */
         HorizontalStrokes(tempCoords, wordID, wordBS);
         VerticalStrokes(tempCoords, wordID, wordBS);

         int col = (tempCoords[0] + tempCoords[1])/2;

         int zones = FindZones(col, tempCoords[2], tempCoords[3], wordBS, 0);
         
         if(zones > 0)
         {

            vector<int> ZH((zones*2),-1);
            StoreZones(wordID, col, tempCoords[2], tempCoords[3], wordBS, 0, ZH);

            // Store the slope of every window of the word
            vector<float> WindowsSlope;

            CreateWindows(ZH, WindowsSlope, wordID, tempCoords[0], tempCoords[1], wordBS, 0);

            if(!WindowsSlope.empty())
            {
                float mean = getMean(WindowsSlope,wordID);

                degrees = atan (mean) * 180 / PI;
                WindowsSlope.clear();
            }
         }
         else // the word does not have slope
         {
                
                float mean = 0;
                degrees = 0;
            
         }

        free(tempCoords);
        return degrees; // return degrees back for checking
}

/**** Create Box around the word ******/
void TForm1::Box(int* tempCoords, unsigned char* wordBS)
{


    Form10->ImagXpress7_1->SaveToBuffer = true;
    Form10->ImagXpress7_1->SaveFileType =  FT_TIFF;
    Form10->ImagXpress7_1->SaveFile ();

    unsigned char *image;
    HANDLE hIM = (HANDLE)Form10->ImagXpress7_1->SaveBufferHandle;
    image = (unsigned char *)GlobalLock(hIM);
    long ln = GlobalSize(hIM);
    long offs=ln-(long)Ix*Iy;
    GlobalUnlock(hIM);

        int startX = tempCoords[0];
        int endX = tempCoords[1];
        int startY = tempCoords[2];
        int endY = tempCoords[3];

        int it = startY-1;
        while(it <= endY+1)
        {
            *(wordBS+it*Ix+(startX-1)) = 0;
            it++;
        }
        it = startX-1;
        while(it <= endX+1)
        {
            *(wordBS+(endY+1)*Ix+it) = 0;
            it++;
        }
        it = endY+1;
        while(it >= startY-1)
        {
            *(wordBS+it*Ix+(endX+1)) = 0;
            it--;
        }
        it = endX+1;
        while(it >= startX-1)
        {
            *(wordBS+(startY-1)*Ix+it) = 0;
            it--;
        }


    Form10->ImagXpress7_1->LoadBuffer((long)image);

    GlobalFree(image);
    image = NULL;

}

void TForm1::HorizontalStrokes(int* tempCoords, int wordID, unsigned char* wordBS)
{
     int point;
     int len;
     int startX,startY,endX,endY,wordH=0,wordL=0;
     int percentH = 0;


     startX = tempCoords[0];
     endX = tempCoords[1];
     startY = tempCoords[2];
     endY = tempCoords[3];
     wordH = endY - startY;

     percentH = wordH * get_ThresholdL() / 100;
  
     for(int y=startY; y <= endY; y++)
     {
        len = 0;
        for(int x = startX; x <= endX; x++)
        {
            point = *(wordBS+y*Ix+x);
            if(point == wordID)
            {
                len++;
            }
            else
            {
                 len=0;
            }

        } // for with x

        if(len > percentH)
        {
            BlackLine(startX, endX, y, wordBS, 0);
        }
     }//for with y

}



void TForm1::VerticalStrokes(int* tempCoords, int wordID, unsigned char* wordBS)
{
    int point;
    int len;
    int startX,startY,endX,endY,wordH = 0;


    startX = tempCoords[0];
    endX = tempCoords[1];
    startY = tempCoords[2];
    endY = tempCoords[3];

    int center = (startX + endX)/2;
    int current = startY;
    int a1;

    while(current <= endY)
    {
        point = *(wordBS+current*Ix+center);
        if(point == 0)
        {
            current++;
            continue;
        }
        else //start descending
        {
            a1 = current;
            
            int height = Height_Between_Blacks(wordID,a1, endY,center,wordBS,0);
            if(height < get_ThresholdH() && height > 0)
            {
                for(int j=a1; j<=a1+height; j++)
                {
                    for(int k=startX; k<=endX; k++)
                    {
                        *(wordBS+j*Ix+k) = 0;
                    }
                }
            }
            current = a1+height;

        }//else
    }//while

}






/***************************************************************************/
/* Open image for expansion */
void TForm1::OpenImageExp(TObject *Sender)
{
  if (OpenDialog1->Execute())
  {
    File = OpenDialog1->FileName;
    if (FileExists(File))
    {
      ImagXpress7_1->FileName = File;
      Ix = ImagXpress7_1->IWidth;
      Iy = ImagXpress7_1->IHeight;
    }
    OpenCheckDataFile(Sender);
  }
  else
  {
    ShowMessage("Image Problem, Program will Terminate");
    return;
  }
}

/* Open's txt file and check's the word's with the .dat file
Keeps only the word's that exists in txt file */
void TForm1::OpenCheckDataFile(TObject *Sender)
{

    ShowMessage("Please Open txt file with zero slant words");
    if (OpenDialog1->Execute())
    {
        slantZeroW = OpenDialog1->FileName;
        AnsiString txtFile = slantZeroW;
        FILE *f;
        f = fopen(txtFile.c_str(),"rb");
        if (f==NULL)
        {
            fputs ("File error",stderr);
            exit (1);
        }
        for (int i = 0; i < 10; i++)
        {
            fscanf(f, "%d", &wordIDs[i]);
        }

        fclose(f);
    }
    ShowMessage("Loading respective data file");


        unsigned int *IM_SegmResult; //Pointer to store raw data
        AnsiString SegmResultName = File; //File that contains the raw data
        SegmResultName = SegmResultName + ".dat";
        FILE *f1;

        IM_SegmResult = (unsigned int *) calloc (Ix*Iy,sizeof(int));
        f1 = fopen(SegmResultName.c_str(),"rb");
        if (f1==NULL)
        {
            fputs ("File error",stderr);
            ShowMessage("Error Loading Data File");
            exit (1);
        }


       fread(IM_SegmResult,Ix*Iy,sizeof(int),f1);

        ImagXpress7_1->IWidth = Ix;
        ImagXpress7_1->IHeight = Iy;
        ImagXpress7_1->ColorDepth(8,1,0);

        ImagXpress7_1->SaveToBuffer = true;
        ImagXpress7_1->SaveFileType =  FT_TIFF;
        ImagXpress7_1->SaveFile ();

        unsigned char *IMAGE;
        HANDLE hIM = (HANDLE)ImagXpress7_1->SaveBufferHandle;
        IMAGE = (unsigned char *)GlobalLock(hIM);
        long ln = GlobalSize(hIM);
        long offs=ln-(long)Ix*Iy;
        GlobalUnlock(hIM);
        int value;
        unsigned char* temp = (unsigned char*)malloc(Ix*Iy*sizeof(unsigned char));
        for (int x=0;x<Ix;x++)
        {
           
            for (int y=0;y<Iy;y++)
            {
                value = *(IM_SegmResult+y*Ix+x);
                if(value == 0)
                {
                    value = 255;
                    *(IMAGE+y*Ix+x+offs) = value;
                }
                else
                {
                    *(IMAGE+y*Ix+x+offs) = value;
                }
            }
        }
        
        /* Clean temp buffer */
        for(int y=0;y<Iy; y++)
        {
            for(int x=0; x<Ix; x++)
            {
                *(temp+y*Ix+x) = 255;
            }
        }
        /* Check every Pixel */
        for(int i=0; i<10; i++)
        {
            checkPixel(temp,IMAGE,offs,wordIDs[i]);
        }
        /* clean image */
        for(int y=0;y<Iy; y++)
        {
            for(int x=0; x<Ix; x++)
            {
                *(IMAGE+y*Ix+x+offs) = 255;
            }
        }
        /* write to image */
        for(int y=0;y<Iy; y++)
        {
            for(int x=0; x<Ix; x++)
            {
                *(IMAGE+y*Ix+x+offs) = *(temp+y*Ix+x);
            }
        }
        ImagXpress7_1->LoadBuffer((long)IMAGE);
       
        free(temp);
        free(IM_SegmResult);
        GlobalFree(IMAGE);
        fclose(f1);

}

/* Check's if the pixel given is contained in words array */
void TForm1::checkPixel(unsigned char* temp, unsigned char* words, long offs, int wordID)
{     
        for(int y=0;y<Iy; y++)
        {
            for(int x=0; x<Ix; x++)
            {
                int pixelID = *(words+y*Ix+x+offs);
                if(pixelID == wordID)
                {
                   *(temp+y*Ix+x) = wordID;
                }
                
            }
        }

}


/* Function for the etsimation */
void __fastcall TForm1::Estimation1Click(TObject *Sender)
{
        int correct_wordsVL;
        int correct_wordsBS;
        unsigned char* wordBS;
        unsigned char* wordVL;
        /* Opening from start the tif image and the txt file
        for the 10 words with slant 0 */
        OpenImageExp(Sender);

        ShowMessage("Give parameters for Bozinovich Estimation");
        Form6->ShowModal();
        /* array for storing the results */
        float results[2][4];

        im = (unsigned char*)malloc(Ix*Iy*sizeof(unsigned char));
        wordVL = (unsigned char*)malloc(Ix*Iy*sizeof(unsigned char));
        wordBS = (unsigned char*)malloc(Ix*Iy*sizeof(unsigned char));
        int counter = 0;
        //Form5->Label1->Caption = "adaf";
        for(double a=4.0;a<=10.0;a+=2.0){
                angleStr = "";
                angleStr = angleStr + a;

                correct_wordsVL = 0;
                correct_wordsBS = 0;
                Form5->ProgressBar1->Max=10-1;
                Form5->Show();

                for(int i=0;i<10;i++){

                    Form5->ProgressBar1->Position = i;
                    
                    /*initialize word in array initializedWord for BS*/
                    initializeWordID(wordIDs[i], wordBS);
                    rotateInitializedWordID(wordIDs[i], a, wordBS);
                    /*bozinovich estimation*/
                    int angleBS = BozinovichEstimation(wordIDs[i], wordBS);
                    /* check the angle */
                    if( abs(abs(angleBS)-abs(a)) <= 2 )
                        correct_wordsBS++;

                    /* initialize initializedWord array now for Vinciarelli*/
                    initializeWordID(wordIDs[i],wordVL);
                    rotateInitializedWordID(wordIDs[i],a,wordVL);
                    /*vinciarelli estimation for every word*/
                    int angleVL =  vinciarelli_luettin(wordIDs[i], wordVL);
                    /* check the angle */
                    if( abs(abs(angleVL)-abs(a)) <= 2 )
                        correct_wordsVL++;


                } // for 10 words
                
                Form5->Close();
                float percentVL = (correct_wordsVL*100)/10;
                float percentBS = (correct_wordsBS*100)/10;
                /*AnsiString s = "degrees: ";
                s=s+a;
                s=s+"\n";
                s+="correctWordsVL: ";
                s+= correct_wordsVL;
                s = s + "\n";
                s = s + "percentVL: ";
                s = s + percentVL;
                s = s + "\n";
                s = s + "correct_wordsBS: ";
                s = s + correct_wordsBS;
                s = s + "\n";
                s = s + "percentBS: ";
                s = s + percentBS;
                s = s + "\n";
                ShowMessage(s);*/
                
                /* Add the results from the methods to the Result Array */
                results[0][counter] = percentBS;
                results[1][counter] = percentVL;
                counter++;
                
       }// for, shift angle

       /* Add Result to the StringGrid */
        Form11->FormCreate(results);
        Form11->ShowModal();
        /* free resources */
        free(im);
        free(wordVL);
        free(wordBS);
}

/* rotate's the word with shear transform */
void TForm1::rotateInitializedWordID(int ID,double th, unsigned char* initializedWord){

        int pixelID;

        unsigned char* rotate = (unsigned char*)malloc(Ix*Iy*sizeof(unsigned char));
        for (int y=0;y<Iy;y++)
        for (int x=0;x<Ix;x++)
                *(rotate+y*Ix+x) = 255;

        for (int y=0;y<Iy;y++){
                for (int x=0;x<Ix;x++){
                        pixelID = *(initializedWord+y*Ix+x);
                        *(initializedWord+y*Ix+x) = 255;
                        if( pixelID == ID   ){
                                int newX = x + y*tan(th*PI/180.0);
                                int newY = y;

                                if( newX > Ix )
                                {
                                    newX-=Ix;
                                }
                                *(rotate+newY*Ix+newX) = pixelID;
                        }
                }
        }
        for (int y=0;y<Iy;y++)
        for (int x=0;x<Ix;x++)
                *(initializedWord+y*Ix+x) = *(rotate+y*Ix+x);

        free(rotate);
}

/* vinciarelli - luettin calls this function to rotate the word */
void TForm1::rotateWordID(int ID,double th){

        int pixelID;
        unsigned char* rotate = (unsigned char*)malloc(Ix*Iy*sizeof(unsigned char));
        for (int y=0;y<Iy;y++)
        for (int x=0;x<Ix;x++)
                *(rotate+y*Ix+x) = 255;

        for (int y=0;y<Iy;y++){
                for (int x=0;x<Ix;x++){
                        pixelID = *(im+y*Ix+x);
                        if( pixelID == ID ){
                                int newX = x + y*tan(th*PI/180.0);
                                int newY = y;

                                if( newX > Ix )
                                        newX-=Ix;
                                        
                                *(rotate+newY*Ix+newX) = pixelID;
                        }
                }
        }
        for (int y=0;y<Iy;y++)
        for (int x=0;x<Ix;x++)
                *(im+y*Ix+x) = *(rotate+y*Ix+x);
        free(rotate);
}

/* Stores the word with the given ID in a buffer */
void TForm1::initializeWordID(int ID, unsigned char* initializedWord){

        int pixelID;
        ImagXpress7_1->SaveToBuffer = true;
        ImagXpress7_1->SaveFileType =  FT_TIFF;
        ImagXpress7_1->SaveFile ();

        unsigned char *words;
        HANDLE hIM = (HANDLE)ImagXpress7_1->SaveBufferHandle;
        words = (unsigned char *)GlobalLock(hIM);
        long ln = GlobalSize(hIM);
        long offs=ln-(long)Ix*Iy;
        GlobalUnlock(hIM);

        for (int y=0;y<Iy;y++){
                for (int x=0;x<Ix;x++){
                        pixelID = *(words+y*Ix+x+offs);
                        if( pixelID != ID ){
                                *(initializedWord+y*Ix+x) = 255;
                                *(words+y*Ix+x+offs) = 255;
                        }else{
                                *(initializedWord+y*Ix+x) = ID;
                                *(words+y*Ix+x+offs) = ID;
                        }
                }
        }
        GlobalFree(words);
}


/*
for word with id = pixelID do the following:
        1.rotate from -20 - +20.
        2.find slant using vinciarelli - luettin method
*/
int TForm1::vinciarelli_luettin(int pixelID, unsigned char* wordVL){

        float d = 0;
        float S;
        float maxS = -1;

        for(double th=-20.0;th<=20.0;th+=0.3){
                /*each time we reinitialize word to reduce noise*/
                for (int y=0;y<Iy;y++)
                for (int x=0;x<Ix;x++)
                     *(im+y*Ix+x) = *(wordVL+y*Ix+x);
                /*rotate th degrees so as to find highest S*/
                rotateWordID(pixelID,th);
                S = slantEstimation(pixelID);
                if( S >= maxS ){
                        maxS = S;
                        d = th;
                }
        }
        /*AnsiString A = "PixelID: ";
        A+=pixelID;
        A+="/ Degrees: ";
        A+=d;
       // ShowMessage(A);
          */
        return d;
}

float TForm1::slantEstimation(int ID){
        int pixelID;
        float S = 0;
        int maxS = -1;
        bool start_found;
        int start_pos;
        int end_pos;
        double h;
        double Dy;
        double H;
        int x,y;

        for ( x=0;x<Ix;x++)
        {

                start_found = false;
                start_pos = -1;
                end_pos = -1;
                h = 0.0;
                Dy = 0.0;
                H=0.0;
               /*collumn x*/
               /*row y*/
                for ( y=0;y<Iy;y++)
                {
                        pixelID = *(im+y*Ix+x);
                        if(pixelID == 255)
                              continue;

                        if( pixelID == ID){
                                h+=1.0;
                                if( !start_found){
                                        start_found = true;
                                        start_pos = y;
                                }
                                end_pos = y;
                        }
                }
                Dy = end_pos-start_pos;
                if( Dy != 0 ){
                        /*AnsiString A="";
                        A+="Collumn: ";
                        A+=x;
                        A+="/start: ";
                        A+=start_pos;
                        A+="/end: ";
                        A+=end_pos;
                        A+="/ h: ";
                        A+=h;
                       // ShowMessage(A);
                          */

                        H = (double)(h-1.0)/(double)Dy;
                      //  ShowMessage(H);
                        if(H == 1.0)
                                S+=h*h;
                }
        }
        return S;

}

