object FrmDllExportTester: TFrmDllExportTester
  Left = 298
  Top = 125
  Width = 522
  Height = 594
  Caption = 'DLL Export Tester'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 514
    Height = 57
    Align = alTop
    BevelOuter = bvNone
    TabOrder = 0
    DesignSize = (
      514
      57)
    object Label3: TLabel
      Left = 7
      Top = 5
      Width = 70
      Height = 13
      Caption = 'Library to Load'
    end
    object EdtDllPath: TEdit
      Left = 83
      Top = 2
      Width = 424
      Height = 21
      Anchors = [akLeft, akTop, akRight]
      TabOrder = 0
      OnKeyDown = EdtDllPathKeyDown
    end
    object Button1: TButton
      Left = 4
      Top = 32
      Width = 73
      Height = 21
      Anchors = [akTop, akRight]
      Caption = 'List Functions'
      TabOrder = 1
      OnClick = ShowDLLFuncs
    end
    object btnLoadLibrary: TButton
      Left = 83
      Top = 32
      Width = 75
      Height = 21
      Anchors = [akTop, akRight]
      Caption = 'LoadLibrary()'
      TabOrder = 2
      OnClick = btnLoadLibraryClick
    end
    object btnFreeLibrary: TButton
      Left = 167
      Top = 32
      Width = 75
      Height = 21
      Anchors = [akTop, akRight]
      Caption = 'FreeLibrary()'
      TabOrder = 3
      OnClick = btnFreeLibraryClick
    end
  end
  object Panel2: TPanel
    Left = 0
    Top = 57
    Width = 514
    Height = 281
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 1
    object LbxDllFuncs: TListBox
      Left = 0
      Top = 0
      Width = 514
      Height = 281
      Align = alClient
      ItemHeight = 13
      TabOrder = 0
      OnClick = LbxDllFuncsClick
    end
  end
  object Panel3: TPanel
    Left = 0
    Top = 338
    Width = 514
    Height = 229
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 2
    object GroupBox1: TGroupBox
      Left = 0
      Top = 0
      Width = 514
      Height = 229
      Align = alClient
      Caption = 'Call a Function (GetProcAddress)'
      TabOrder = 0
      DesignSize = (
        514
        229)
      object Label2: TLabel
        Left = 11
        Top = 67
        Width = 53
        Height = 13
        Caption = 'Arguments:'
      end
      object Label4: TLabel
        Left = 8
        Top = 44
        Width = 62
        Height = 13
        Caption = 'Return Type:'
      end
      object Label1: TLabel
        Left = 10
        Top = 20
        Width = 73
        Height = 13
        Caption = 'Function to Call'
      end
      object btnCallDllFunction: TButton
        Left = 344
        Top = 48
        Width = 72
        Height = 33
        Caption = 'Call Function'
        TabOrder = 0
        OnClick = btnCallDllFunctionClick
      end
      object EdtArg1: TEdit
        Left = 72
        Top = 60
        Width = 129
        Height = 21
        TabOrder = 1
      end
      object EdtArg2: TEdit
        Left = 208
        Top = 60
        Width = 129
        Height = 21
        TabOrder = 2
      end
      object rdoReturnCharPtr: TRadioButton
        Left = 80
        Top = 40
        Width = 49
        Height = 17
        Caption = 'char*'
        Checked = True
        TabOrder = 3
        TabStop = True
      end
      object rdoReturnInt: TRadioButton
        Left = 136
        Top = 40
        Width = 41
        Height = 17
        Caption = 'int'
        TabOrder = 4
      end
      object rdoReturnVoid: TRadioButton
        Left = 184
        Top = 40
        Width = 41
        Height = 17
        Caption = 'void'
        TabOrder = 5
      end
      object MmoFunc: TMemo
        Left = 91
        Top = 14
        Width = 415
        Height = 23
        Anchors = [akLeft, akTop, akRight]
        TabOrder = 6
      end
      object btnShowCppExample: TButton
        Left = 420
        Top = 48
        Width = 81
        Height = 33
        Caption = 'Code Example'
        TabOrder = 7
        OnClick = btnCallDllFunctionClick
      end
      object mmoCallExample: TCodeEdit
        Left = 2
        Top = 98
        Width = 510
        Height = 129
        Language = lsCPlusPlus
        Active = True
        Align = alBottom
        Lines.Strings = (
          '//Load the library dynamically'
          'HINSTANCE hDLL = LoadLibrary("mylibrary.dll");'
          '')
        PlainText = True
        ScrollBars = ssBoth
        TabOrder = 8
        WordWrap = False
      end
    end
  end
end
