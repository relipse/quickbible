object FrmAutoCompleteQuickBiblePluginAbout: TFrmAutoCompleteQuickBiblePluginAbout
  Left = 272
  Top = 191
  Width = 318
  Height = 186
  BorderStyle = bsSizeToolWin
  Caption = 'About QuickBible Auto-Complete Plugin'
  Color = clCream
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poOwnerFormCenter
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 24
    Top = 16
    Width = 205
    Height = 24
    Caption = 'Auto-Complete Plugin'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label2: TLabel
    Left = 32
    Top = 48
    Width = 226
    Height = 13
    Caption = 'The first plugin to add a VCL form to it (this one) '
  end
  object Label3: TLabel
    Left = 8
    Top = 72
    Width = 291
    Height = 13
    Caption = 'and hook OnLookupBoxChange and OnLookupBoxKeyDown'
  end
  object Label4: TLabel
    Left = 64
    Top = 104
    Width = 162
    Height = 13
    Cursor = crHandPoint
    Caption = 'http://tearsfornations.hostcell.net/'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlue
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsUnderline]
    ParentFont = False
    OnClick = Label4Click
  end
  object Button1: TButton
    Left = 8
    Top = 128
    Width = 297
    Height = 25
    Caption = 'Close'
    ModalResult = 1
    TabOrder = 0
  end
end
