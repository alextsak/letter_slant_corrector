//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit4.h"
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm4 *Form4;
//---------------------------------------------------------------------------
__fastcall TForm4::TForm4(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TForm4::Button2Click(TObject *Sender)
{
   Form4->Close(); 
}
//---------------------------------------------------------------------------
void __fastcall TForm4::RadioButton1Click(TObject *Sender)
{
      if(RadioButton1->Checked)
      {
        stage = 1;
      }
}
//---------------------------------------------------------------------------

void __fastcall TForm4::Button1Click(TObject *Sender)
{
    /*if(stage==1).... show button next and then continue the process
    else if ... every other condition -> do the same but check which stage to
    to the user
    */
    /*if(RadioButton1->Checked)
    {
        stage = 1;
        Form1->Button1->Caption="Next 1";
    }
    else if(RadioButton1->Checked && RadioButton2->Checked){

         stage = 12;
         Form1->Button1->Caption="Next 12";
    } */

     Close();

}
//---------------------------------------------------------------------------

