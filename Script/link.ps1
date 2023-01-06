
$source = "$PSScriptRoot".Replace('\', '/')
Get-ChildItem "$source" -recurse -Filter "*.json" | ForEach-Object {
	$item = $_.FullName.Remove(0, $source.Length).Replace('\', '/')
	if (-not ($item.Substring(0, 7) -eq "/.build") -and -not ($item -eq "/tsconfig.json")) {
		Write-Host "$item"
		New-Item -ItemType SymbolicLink -Path "$source/.build/$item" -Target "$source$item"
	}
}
