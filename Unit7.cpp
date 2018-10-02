//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit7.h"
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ImagXpr7_OCX"
#pragma resource "*.dfm"
TForm7 *Form7;
//---------------------------------------------------------------------------
__fastcall TForm7::TForm7(TComponent* Owner)
    : TForm(Owner)
{
    ImagXpress7_1->ScrollBars=3;
    ImagXpress7_2->ScrollBars=3;

}
//---------------------------------------------------------------------------
void __fastcall TForm7::ImagXpress7_1Scroll(TObject *Sender, short Bar,
      short Action)
{
  ImagXpress7_2->ScrollX = ImagXpress7_1->ScrollX;
  ImagXpress7_2->ScrollY = ImagXpress7_1->ScrollY;
  ImagXpress7_2->Zoom(ImagXpress7_1->IPZoomF);

}
//---------------------------------------------------------------------------

void __fastcall TForm7::ImagXpress7_2Scroll(TObject *Sender, short Bar,
      short Action)
{
  ImagXpress7_1->ScrollX = ImagXpress7_2->ScrollX;
  ImagXpress7_1->ScrollY = ImagXpress7_2->ScrollY;
  ImagXpress7_1->Zoom(ImagXpress7_2->IPZoomF);
}


