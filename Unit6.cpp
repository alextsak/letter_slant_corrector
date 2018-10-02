//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit6.h"
#include "Unit1.h"
#include "Unit4.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm6 *Form6;
//---------------------------------------------------------------------------
__fastcall TForm6::TForm6(TComponent* Owner)
    : TForm(Owner)
{

    AnsiString str = "Default Thresholds";
    str = str + "\n";
    str = str + "ThresholdL: 1";
    str = str + "\n";
    str = str + "ThresholdH: 10";
    Form6->Label4->Caption = str;


}
//---------------------------------------------------------------------------

void __fastcall TForm6::ThresholdLClick(TObject *Sender)
{
    Form6->ThresholdL->Text = "";
}

void __fastcall TForm6::ThresholdHClick(TObject *Sender)
{
    Form6->ThresholdH->Text = "";
}

void __fastcall TForm6::ThresholdLChange(TObject *Sender)
{
   if(atoi(ThresholdL->Text.c_str()) > 100)
   {
      ShowMessage("It's percent, only lower than 100");
      //Form6->ThresholdL->Text = "";
   }
   else
   {
        Form1->set_ThresholdL(atoi(ThresholdL->Text.c_str()));
   }

}
//---------------------------------------------------------------------------

void __fastcall TForm6::ThresholdHChange(TObject *Sender)
{

      if(atoi(ThresholdH->Text.c_str()) > 100)
      {
         ShowMessage("It's percent, only lower than 100");
         Form6->ThresholdH->Text = "";
      }
      else
      {
        Form1->set_ThresholdH(atoi(ThresholdH->Text.c_str()));
      }

}
//---------------------------------------------------------------------------

void __fastcall TForm6::Button1Click(TObject *Sender)
{
     if(strcmp(ThresholdH->Text.c_str(),"ThresholdH") == 0 || strcmp(ThresholdH->Text.c_str(),"") == 0
     || strcmp(ThresholdL->Text.c_str(),"ThresholdL") == 0 || strcmp(ThresholdL->Text.c_str(),"") == 0)
    {
        AnsiString msg = "Setting Default Parameters";
        msg = msg + "\n";
        Form1->set_ThresholdL(1);
        Form1->set_ThresholdH(10);
        msg = msg + "ThresholdL: ";
        msg = msg + Form1->get_ThresholdL();
        msg = msg + "\n";
        msg = msg + "ThresholdH: ";
        msg = msg + Form1->get_ThresholdH();
        ShowMessage(msg);
    }
    else
    {
        AnsiString msg = "ThresholdL: ";
        msg = msg + ThresholdL->Text;
        msg = msg + "\n";
        msg = msg + "ThresholdH: ";
        msg = msg + ThresholdH->Text;
        ShowMessage(msg);
    }
    Form6->Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm6::CancelClick(TObject *Sender)
{
    ShowMessage("Setting Default Parameters");
    Form1->set_ThresholdL(1);
    Form1->set_ThresholdH(10);
    Form6->Close();
}
//---------------------------------------------------------------------------


