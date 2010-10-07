object FrmAutoComplete: TFrmAutoComplete
  Left = 396
  Top = 207
  BorderStyle = bsNone
  Caption = 'Auto Complete Plugin'
  ClientHeight = 214
  ClientWidth = 318
  Color = clCream
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnCloseQuery = FormCloseQuery
  OnKeyDown = FormKeyDown
  DesignSize = (
    318
    214)
  PixelsPerInch = 96
  TextHeight = 13
  object cmbDropDown: TComboBox
    Left = -2
    Top = -20
    Width = 319
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    Color = clCream
    ItemHeight = 13
    TabOrder = 0
    OnCloseUp = cmbDropDownCloseUp
    OnExit = cmbDropDownExit
  end
  object fnHtmlListBox1: TfnHtmlListBox
    Left = 0
    Top = 0
    Width = 318
    Height = 214
    Style = lbOwnerDrawVariable
    Align = alClient
    Color = clCream
    ItemHeight = 16
    TabOrder = 1
  end
end
