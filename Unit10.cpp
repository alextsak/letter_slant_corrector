//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit10.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ImagXpr7_OCX"
#pragma resource "*.dfm"
TForm10 *Form10;
//---------------------------------------------------------------------------
__fastcall TForm10::TForm10(TComponent* Owner)
        : TForm(Owner)
{
 ImagXpress7_1->ScrollBars=3;
}
//---------------------------------------------------------------------------
void __fastcall TForm10::ImagXpress7_1MouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
  int x = ImagXpress7_1->DIBXPos;
  int y = ImagXpress7_1->DIBYPos;
  int c = ImagXpress7_1->DIBGetPixel(x,y);

  AnsiString S = "["; S=S+x;S=S+",";S=S+y;
  S=S+"] c:";S=S+c;

  Label1->Caption=S;
}
//---------------------------------------------------------------------------




 
