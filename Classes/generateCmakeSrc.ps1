Write-Output "list(APPEND GAME_SOURCE" | Out-File .\CMakeSrc.txt -Encoding UTF8
Get-ChildItem -Recurse -Filter *.cpp | % {Write-Output $($_.FullName.Replace((Resolve-Path ..\).Path + '\', '').Replace('\', '/'))} | Out-File .\CMakeSrc.txt -Append -Encoding UTF8
Write-Output ")`nlist(APPEND GAME_HEADER" | Out-File .\CMakeSrc.txt -Append -Encoding UTF8
Get-ChildItem -Recurse -Filter *.h | % {Write-Output $($_.FullName.Replace((Resolve-Path ..\).Path + '\', '').Replace('\', '/'))} | Out-File .\CMakeSrc.txt -Append -Encoding UTF8
Write-Output ")" | Out-File .\CMakeSrc.txt -Append -Encoding UTF8