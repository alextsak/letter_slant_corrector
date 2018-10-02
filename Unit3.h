//---------------------------------------------------------------------------


#ifndef Unit3H
#define Unit3H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TResults : public TForm
{
__published:	// IDE-managed Components
    TListView *ListView1;
private:	// User declarations
public:		// User declarations
  
    __fastcall TResults(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TResults *Results;
//---------------------------------------------------------------------------
#endif
