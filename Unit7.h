//---------------------------------------------------------------------------

#ifndef Unit7H
#define Unit7H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "ImagXpr7_OCX.h"
#include <OleCtrls.hpp>
//---------------------------------------------------------------------------
class TForm7 : public TForm
{
__published:	// IDE-managed Components
    TImagXpress7_ *ImagXpress7_1;
    TImagXpress7_ *ImagXpress7_2;
    void __fastcall ImagXpress7_1Scroll(TObject *Sender, short Bar,
          short Action);
        void __fastcall ImagXpress7_2Scroll(TObject *Sender, short Bar,
          short Action);
private:	// User declarations
public:		// User declarations
    __fastcall TForm7(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm7 *Form7;
//---------------------------------------------------------------------------
#endif
