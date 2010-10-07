[Setup]
AppId={{E93F6010-5A7E-446B-8914-E11D900BE259}
AppName=QuickBible
AppVerName=QuickBible 0.915
AppPublisher=QuickBible
AppPublisherURL=http://tearsfornations.hostcell.net/software/quickbible/
AppSupportURL=http://tearsfornations.hostcell.net/software/quickbible/
AppUpdatesURL=http://tearsfornations.hostcell.net/software/quickbible/
DefaultDirName={pf}\QuickBible
DefaultGroupName=QuickBible
AllowNoIcons=yes
UninstallDisplayIcon={app}\QuickBible.exe
Compression=lzma
SolidCompression=yes
LicenseFile=C:\Utility\cpp\bcb\QuickBible\installation\nsis\license.txt
OutputDir=.
OutputBaseFilename=QuickBible0915Setup
[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "QuickBible.exe"; DestDir: "{app}"
Source: "QuickBible.ini"; DestDir: "{app}"
Source: "Bibles\kjvr.bible.sqlite3"; DestDir: "{app}\Bibles"
Source: "Bibles\asv.bible.sqlite3"; DestDir: "{app}\Bibles"
Source: "Bibles\web.bible.sqlite3"; DestDir: "{app}\Bibles"
Source: "Plugins\RtfCleaner.dll"; DestDir: "{app}\Plugins"
Source: "Plugins\HtmlCleaner.dll"; DestDir: "{app}\Plugins"
[Icons]
Name: "{group}\QuickBible"; Filename: "{app}\QuickBible.exe"
Name: "{group}\{cm:UninstallProgram,QuickBible}"; Filename: "{uninstallexe}"
Name: "{commondesktop}\QuickBible"; Filename: "{app}\QuickBible.exe"; Tasks: desktopicon
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\QuickBible"; Filename: "{app}\QuickBible.exe"; Tasks: quicklaunchicon

[Run]
Filename: "{app}\QuickBible.exe"; Description: "{cm:LaunchProgram,QuickBible}"; Flags: nowait postinstall skipifsilent

