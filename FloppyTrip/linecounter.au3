#include <File.au3>
$i = 0

$a = _FileListToArray(@ScriptDir,"*.cpp")

for $x = 1 to $a[0] -1 ;
	$file = FileRead($a[$x])
	$ss = StringSplit($file,@CRLF)
$i += $ss[0]
Next


$a = _FileListToArray(@ScriptDir,"*.h")

for $x = 1 to $a[0] -1 ;
	$file = FileRead($a[$x])
	$ss = StringSplit($file,@CRLF)
$i += $ss[0]
Next

ConsoleWrite($i & @LF)