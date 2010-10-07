unit ListDLLFunctions;

interface

uses Windows, Classes, SysUtils, ImageHlp;

procedure PAS_ListDLLFunctions(DLLName: string; List: TStrings);

implementation

procedure PAS_ListDLLFunctions(DLLName: string; List: TStrings);
// by Dmitry Streblechenko
type
  chararr = array[0..$FFFFFF] of char;
var
  h: THandle;
  i, fc: integer;
  st: string;
  arr: pointer;
  ImageDebugInformation: PImageDebugInformation;
begin
  List.Clear;
  DLLName := ExpandFileName(DLLName);
  if FileExists(DLLName) then
    begin
      h := CreateFile(PChar(DLLName),
                      GENERIC_READ,
                      FILE_SHARE_READ or FILE_SHARE_WRITE,
                      nil,
                      OPEN_EXISTING,
                      FILE_ATTRIBUTE_NORMAL,
                      0);
      if h <> INVALID_HANDLE_VALUE then
        try
          ImageDebugInformation := MapDebugInformation(h, PChar(DLLName), nil, 0);
          if ImageDebugInformation <> nil then
            try
              arr := ImageDebugInformation^.ExportedNames;
              fc := 0;
              for i := 0 to ImageDebugInformation^.ExportedNamesSize-1 do
                if chararr(arr^)[i] = #0 then
                  begin
                    st := PChar(@chararr(arr^)[fc]);
                    if length(st)>0 then List.Add(st);
                    if (i>0) and (chararr(arr^)[i-1]=#0) then Break;
                    fc := i+1;
                  end;
            finally
              UnmapDebugInformation(ImageDebugInformation);
            end;
        finally
          CloseHandle(h);
        end;
    end;
end;


end.

