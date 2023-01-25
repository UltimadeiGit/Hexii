Write-Output "list(APPEND GAME_SOURCE" | Out-File .\CMakeSrc.txt -Encoding UTF8
ls | where -Property Name -Like "*.cpp" | select -ExpandProperty Name | % {Write-Output Classes/$_} | Out-File .\CMakeSrc.txt -Append -Encoding UTF8
Write-Output ")`nlist(APPEND GAME_HEADER" | Out-File .\CMakeSrc.txt -Append -Encoding UTF8
ls | where -Property Name -Like "*.h" | select -ExpandProperty Name | % {Write-Output Classes/$_} | Out-File .\CMakeSrc.txt -Append -Encoding UTF8
Write-Output ")" | Out-File .\CMakeSrc.txt -Append -Encoding UTF8