object FrmPreferences: TFrmPreferences
  Left = 416
  Top = 189
  Width = 590
  Height = 370
  Caption = 'Preferences'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poOwnerFormCenter
  OnCreate = FormCreate
  OnPaint = OnLoad
  PixelsPerInch = 96
  TextHeight = 13
  object Splitter1: TSplitter
    Left = 89
    Top = 0
    Width = 3
    Height = 302
    Cursor = crHSplit
  end
  object PleaseChoosePanel: TPanel
    Left = 92
    Top = 0
    Width = 490
    Height = 302
    Align = alClient
    Caption = 'This option contains no preferences.'
    TabOrder = 2
  end
  object PnlPrefPlugins: TPanel
    Left = 92
    Top = 0
    Width = 490
    Height = 302
    Align = alClient
    TabOrder = 1
    DesignSize = (
      490
      302)
    object PluginsGrid: TNextGrid
      Left = 1
      Top = 1
      Width = 488
      Height = 160
      Align = alTop
      Caption = 'Plugins'
      Options = [goHeader, goRowMoving, goSelectFullRow]
      TabOrder = 0
      TabStop = True
      OnCellClick = PluginsGridCellClick
      object nxCbxEnabled: TNxCheckBoxColumn
        DefaultWidth = 118
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        Header.Caption = 'Enabled'
        Options = [coCanClick, coCanInput, coCanSort, coPublicUsing, coTextForCaption]
        Position = 0
        SortType = stBoolean
        Width = 118
      end
      object nxTxtPlugin: TNxTextColumn
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        Header.Caption = 'Plugin'
        Position = 1
        SortType = stAlphabetic
      end
      object nxTxtName: TNxTextColumn
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        Header.Caption = 'Name'
        Position = 2
        SortType = stAlphabetic
      end
      object nxTxtVersion: TNxTextColumn
        DefaultWidth = 128
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        Header.Caption = 'Version'
        Options = [coAutoSize, coCanClick, coCanInput, coCanSort, coPublicUsing, coShowTextFitHint]
        Position = 3
        SortType = stAlphabetic
        Width = 128
      end
      object nxTxtPluginFolder: TNxTextColumn
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        Header.Caption = 'Folder'
        Position = 4
        SortType = stAlphabetic
      end
    end
    object btnPluginsRefresh: TButton
      Left = 384
      Top = 168
      Width = 97
      Height = 25
      Anchors = [akTop, akRight]
      Caption = 'Refresh Plugins'
      TabOrder = 1
      OnClick = btnPluginsRefreshClick
    end
    object PluginsInfoMemo: TMemo
      Left = 8
      Top = 168
      Width = 369
      Height = 124
      Anchors = [akLeft, akTop, akRight, akBottom]
      Color = clScrollBar
      ReadOnly = True
      ScrollBars = ssVertical
      TabOrder = 2
    end
  end
  object PnlPrefBibles: TPanel
    Left = 92
    Top = 0
    Width = 490
    Height = 302
    Align = alClient
    TabOrder = 3
    DesignSize = (
      490
      302)
    object BiblesGrid: TNextGrid
      Left = 1
      Top = 1
      Width = 488
      Height = 160
      Align = alTop
      Caption = 'No Bibles Installed'
      Options = [goHeader, goRowMoving, goSelectFullRow]
      TabOrder = 0
      TabStop = True
      OnAfterRowMove = BiblesGridAfterRowMove
      OnCellClick = BiblesGridCellClick
      object nxCbxInstalled: TNxCheckBoxColumn
        DefaultWidth = 118
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        Header.Caption = 'Installed'
        Options = [coCanClick, coCanInput, coCanSort, coPublicUsing, coTextForCaption]
        Position = 0
        SortType = stBoolean
        Width = 118
      end
      object nxTxtAbbreviation: TNxTextColumn
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        Header.Caption = 'Abbreviation'
        Position = 1
        SortType = stAlphabetic
      end
      object nxTxtDescription: TNxTextColumn
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        Header.Caption = 'Description'
        Position = 2
        SortType = stAlphabetic
      end
      object nxTxtFormat: TNxTextColumn
        DefaultWidth = 128
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        Header.Caption = 'Format'
        Options = [coAutoSize, coCanClick, coCanInput, coCanSort, coPublicUsing, coShowTextFitHint]
        Position = 3
        SortType = stAlphabetic
        Width = 128
      end
      object nxTxtBibleFolder: TNxTextColumn
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        Header.Caption = 'Folder'
        Position = 4
        SortType = stAlphabetic
      end
    end
    object BiblesInfoMemo: TRichEdit
      Left = 4
      Top = 165
      Width = 481
      Height = 131
      Anchors = [akLeft, akTop, akRight, akBottom]
      Color = clScrollBar
      HideScrollBars = False
      ReadOnly = True
      ScrollBars = ssVertical
      TabOrder = 1
    end
  end
  object PnlPrefAdvanced: TPanel
    Left = 92
    Top = 0
    Width = 490
    Height = 302
    Align = alClient
    TabOrder = 6
    object AdvancedBtnClearCache: TButton
      Left = 8
      Top = 216
      Width = 81
      Height = 25
      Caption = 'Clear Cache'
      TabOrder = 0
      OnClick = AdvancedBtnClearCacheClick
    end
    object AdvancedCacheGrid: TNextGrid
      Left = 1
      Top = 1
      Width = 488
      Height = 209
      Align = alTop
      Caption = 'There are no cached searches'
      TabOrder = 1
      TabStop = True
      object NxTextColumn1: TNxTextColumn
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        Header.Caption = 'Searched'
        Position = 0
        SortType = stAlphabetic
      end
      object NxTextColumn2: TNxTextColumn
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        Header.Caption = 'Bible'
        Position = 1
        SortType = stAlphabetic
      end
      object NxTimeColumn1: TNxTimeColumn
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        Header.Caption = 'Seconds'
        Position = 2
        SortType = stDate
      end
      object NxMemoColumn1: TNxMemoColumn
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        Header.Caption = 'Results'
        Position = 3
        SortType = stAlphabetic
        ScrollBars = ssBoth
      end
    end
  end
  object PnlPrefAppearance: TPanel
    Left = 92
    Top = 0
    Width = 490
    Height = 302
    Align = alClient
    TabOrder = 5
    object AppearanceCbxStayOnTop: TCheckBox
      Left = 16
      Top = 8
      Width = 81
      Height = 17
      Caption = 'Stay on top'
      TabOrder = 0
      OnClick = AppearanceCbxStayOnTopClick
    end
    object cbxAppearanceShowSearchOptions: TCheckBox
      Left = 16
      Top = 32
      Width = 169
      Height = 17
      Caption = 'Show extra search options bar'
      TabOrder = 1
      OnClick = cbxAppearanceShowSearchOptionsClick
    end
  end
  object TreeView1: TTreeView
    Left = 0
    Top = 0
    Width = 89
    Height = 302
    Align = alLeft
    Indent = 19
    ReadOnly = True
    ShowRoot = False
    TabOrder = 0
    OnClick = TreeView1Click
    Items.Data = {
      040000001F0000000000000000000000FFFFFFFFFFFFFFFF0000000000000000
      064269626C6573200000000000000000000000FFFFFFFFFFFFFFFF0000000000
      00000007506C7567696E73230000000000000000000000FFFFFFFFFFFFFFFF00
      000000000000000A417070656172616E6365210000000000000000000000FFFF
      FFFFFFFFFFFF000000000000000008416476616E636564}
  end
  object Panel1: TPanel
    Left = 0
    Top = 302
    Width = 582
    Height = 41
    Align = alBottom
    TabOrder = 4
    DesignSize = (
      582
      41)
    object Button1: TButton
      Left = 498
      Top = 8
      Width = 75
      Height = 25
      Anchors = [akTop, akRight]
      Caption = 'Close'
      Default = True
      TabOrder = 0
      OnClick = Button1Click
    end
  end
end
