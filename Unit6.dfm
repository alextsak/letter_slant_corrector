object Form6: TForm6
  Left = 438
  Top = 199
  Width = 564
  Height = 295
  Caption = 'Set Thresholds'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 200
    Top = 48
    Width = 56
    Height = 13
    Caption = 'ThresholdL:'
  end
  object Label2: TLabel
    Left = 200
    Top = 88
    Width = 58
    Height = 13
    Caption = 'ThresholdH:'
  end
  object Label3: TLabel
    Left = 416
    Top = 40
    Width = 112
    Height = 13
    Caption = 'Percent of Word Height'
  end
  object Label4: TLabel
    Left = 16
    Top = 40
    Width = 113
    Height = 73
    Caption = 'Label4'
  end
  object Button1: TButton
    Left = 208
    Top = 136
    Width = 75
    Height = 25
    Caption = 'OK'
    TabOrder = 0
    OnClick = Button1Click
  end
  object ThresholdL: TEdit
    Left = 272
    Top = 40
    Width = 121
    Height = 21
    TabOrder = 1
    Text = 'ThresholdL'
    OnChange = ThresholdLChange
    OnClick = ThresholdLClick
  end
  object ThresholdH: TEdit
    Left = 272
    Top = 80
    Width = 121
    Height = 21
    TabOrder = 2
    Text = 'ThresholdH'
    OnChange = ThresholdHChange
    OnClick = ThresholdHClick
  end
  object Cancel: TButton
    Left = 312
    Top = 136
    Width = 75
    Height = 25
    Caption = 'Cancel'
    TabOrder = 3
    OnClick = CancelClick
  end
end
