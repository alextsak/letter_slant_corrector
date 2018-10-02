//---------------------------------------------------------------------------

#ifndef Unit10H
#define Unit10H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "ImagXpr7_OCX.h"
#include <OleCtrls.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TForm10 : public TForm
{
__published:	// IDE-managed Components
    TImagXpress7_ *ImagXpress7_1;
        
    TLabel *Label1;
        void __fastcall ImagXpress7_1MouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y) ;
private:	// User declarations
public:		// User declarations
        __fastcall TForm10(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm10 *Form10;
//---------------------------------------------------------------------------
#endif
 
