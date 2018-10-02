//---------------------------------------------------------------------------

#ifndef Unit6H
#define Unit6H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TForm6 : public TForm
{
__published:	// IDE-managed Components
    TButton *Button1;
    TLabel *Label1;
    TEdit *ThresholdL;
    TEdit *ThresholdH;
    TLabel *Label2;
    TButton *Cancel;
    TLabel *Label3;
    TLabel *Label4;
    void __fastcall ThresholdLChange(TObject *Sender);
    void __fastcall ThresholdLClick(TObject *Sender);
    void __fastcall ThresholdHClick(TObject *Sender);
    void __fastcall ThresholdHChange(TObject *Sender);
    void __fastcall Button1Click(TObject *Sender);
    void __fastcall CancelClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TForm6(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm6 *Form6;
//---------------------------------------------------------------------------
#endif
