object frmQuickBible: TfrmQuickBible
  Left = 372
  Top = 185
  Width = 332
  Height = 258
  Caption = 'Quick Bible'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PopupMenu = PopupMenu1
  Position = poScreenCenter
  Visible = True
  OnClose = FormClose
  OnDestroy = FormDestroy
  OnKeyPress = rchVerseResultsKeyPress
  OnPaint = FormPaint
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 23
    Width = 324
    Height = 208
    Align = alClient
    Caption = 'Panel1'
    TabOrder = 0
    DesignSize = (
      324
      208)
    object pnlSearchOptions: TPanel
      Left = 1
      Top = 1
      Width = 322
      Height = 18
      Align = alTop
      BevelOuter = bvNone
      TabOrder = 4
      Visible = False
      object rdoSearchAll: TRadioButton
        Left = 4
        Top = 1
        Width = 47
        Height = 17
        Hint = 'Old and New Testaments'
        Caption = 'Both'
        Checked = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -8
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        ParentShowHint = False
        ShowHint = True
        TabOrder = 0
        TabStop = True
        OnClick = rdoSearchOTNTBothClick
      end
      object rdoSearchOT: TRadioButton
        Left = 52
        Top = 1
        Width = 57
        Height = 17
        Hint = 'Old Testament'
        Caption = 'Old Tst.'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -8
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        ParentShowHint = False
        ShowHint = True
        TabOrder = 2
        OnClick = rdoSearchOTNTBothClick
      end
      object cbxIgnoreCase: TCheckBox
        Left = 237
        Top = 0
        Width = 80
        Height = 17
        Hint = 'Non case-sensitive searches'
        Caption = 'Ignore case'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -8
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        ParentShowHint = False
        ShowHint = True
        TabOrder = 3
        OnClick = cbxIgnoreCaseClick
      end
      object rdoSearchNT: TRadioButton
        Left = 112
        Top = 1
        Width = 65
        Height = 17
        Hint = 'New Testament'
        Caption = 'New  Tst.'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -8
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        ParentShowHint = False
        ShowHint = True
        TabOrder = 1
        OnClick = rdoSearchOTNTBothClick
      end
    end
    object LbxAutoComplete: TfnHtmlListBox
      Left = 1
      Top = 2
      Width = 185
      Height = 105
      Style = lbOwnerDrawVariable
      BevelKind = bkSoft
      Color = clCream
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ItemHeight = 16
      ParentFont = False
      TabOrder = 2
      Visible = False
      OnDblClick = ChooseAutoCompleteItem
      OnKeyDown = LbxAutoCompleteKeyDown
    end
    object rchVerseResults: TRichEdit
      Left = 1
      Top = 19
      Width = 322
      Height = 171
      Align = alClient
      Anchors = [akLeft, akTop, akBottom]
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Arial'
      Font.Style = []
      HideSelection = False
      Lines.Strings = (
        'Quick Bible'
        'C++Builder Version')
      ParentFont = False
      ReadOnly = True
      ScrollBars = ssVertical
      TabOrder = 0
      OnKeyPress = rchVerseResultsKeyPress
      OnMouseDown = rchVerseResultsMouseDown
      OnMouseMove = rchVerseResultsMouseMove
    end
    object StatusBar1: TStatusBar
      Left = 1
      Top = 190
      Width = 322
      Height = 17
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBtnText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      Panels = <>
      SimplePanel = True
      UseSystemFont = False
    end
    object gagProgress: TProgressBar
      Left = 241
      Top = 192
      Width = 81
      Height = 15
      Anchors = [akRight, akBottom]
      Min = 0
      Max = 31102
      Smooth = True
      Step = 1
      TabOrder = 1
      Visible = False
    end
  end
  object Panel2: TPanel
    Left = 0
    Top = 0
    Width = 324
    Height = 23
    Align = alTop
    Caption = 'Panel2'
    TabOrder = 1
    DesignSize = (
      324
      23)
    object edtLookup: TNxButtonEdit
      Left = 1
      Top = 1
      Width = 262
      Height = 21
      Anchors = [akLeft, akTop, akRight]
      TabOrder = 0
      Text = 'Jesus*truth'
      WantTabs = True
      OnChange = edtLookupChange
      OnKeyDown = edtLookupKeyDown
      OnButtonClick = edtLookupButtonClick
      ButtonCaption = 'Go'
      TransparentColor = clNone
    end
    object cmbBibleVersions: TComboBox
      Left = 264
      Top = 1
      Width = 57
      Height = 21
      Style = csDropDownList
      Anchors = [akTop, akRight]
      ItemHeight = 13
      TabOrder = 1
      OnChange = edtLookupButtonClick
    end
    object pnlFind: TPanel
      Left = 2
      Top = 1
      Width = 71
      Height = 22
      BevelOuter = bvNone
      Color = clBlack
      TabOrder = 2
      Visible = False
      DesignSize = (
        71
        22)
      object btnFind: TSpeedButton
        Left = 307
        Top = 0
        Width = 38
        Height = 21
        Caption = 'Find '
        Visible = False
        OnClick = btnFindClick
      end
      object edtFind: TEdit
        Left = 0
        Top = 0
        Width = 69
        Height = 21
        Anchors = [akLeft, akTop, akRight]
        Color = 12581887
        TabOrder = 0
        OnExit = edtFindExit
        OnKeyDown = edtFindKeyDown
        OnKeyPress = edtFindKeyPress
        OnKeyUp = edtFindKeyUp
      end
      object cbxFindWholeWord: TCheckBox
        Left = 136
        Top = 0
        Width = 89
        Height = 17
        Caption = 'Whole word'
        TabOrder = 1
        Visible = False
      end
      object cbxMatchCase: TCheckBox
        Left = 216
        Top = 0
        Width = 89
        Height = 17
        Caption = 'Match case'
        TabOrder = 2
        Visible = False
      end
    end
  end
  object PopupMenu1: TPopupMenu
    Left = 120
    Top = 72
    object Copy1: TMenuItem
      Caption = 'Copy'
      ShortCut = 16451
      OnClick = Copy1Click
    end
    object N4: TMenuItem
      Caption = '-'
    end
    object SearchOptions1: TMenuItem
      Caption = 'Search Options'
      object IgnoreCase1: TMenuItem
        AutoCheck = True
        Caption = 'Ignore Case'
        OnClick = cbxIgnoreCaseClick
      end
      object N3: TMenuItem
        Caption = '-'
      end
      object OldTestament1: TMenuItem
        AutoCheck = True
        Caption = 'Old Testament'
        Checked = True
        OnClick = OldNewTestament1Click
      end
      object NewTestament1: TMenuItem
        AutoCheck = True
        Caption = 'New Testament'
        Checked = True
        OnClick = OldNewTestament1Click
      end
    end
    object ShowHideSearchBar1: TMenuItem
      Caption = 'Show/Hide Search Bar'
      OnClick = ShowHideSearchBar1Click
    end
    object N1: TMenuItem
      Caption = '-'
    end
    object Find1: TMenuItem
      Caption = 'Find'
      ShortCut = 16454
      OnClick = Find1Click
    end
    object mniRefresh: TMenuItem
      Caption = 'Refresh'
      ShortCut = 116
      OnClick = mniRefreshClick
    end
    object mniPreferences: TMenuItem
      Caption = 'Preferences'
      ShortCut = 16464
      OnClick = mniPreferencesClick
    end
    object mniAbout: TMenuItem
      Caption = 'About'
      OnClick = mniAboutClick
    end
    object N2: TMenuItem
      Caption = '-'
    end
  end
end
